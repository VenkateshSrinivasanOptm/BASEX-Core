#include "rfidreader.h"
#include <QDebug>
#include <unistd.h> // for sleep function
#include <winscard.h>

RFIDReader::RFIDReader(QObject *parent) : QThread(parent)
{
}

RFIDReader::~RFIDReader()
{
}

QString byteArrayToHexString(const BYTE* data, DWORD length)
{
    QString hexString;
    for (DWORD i = 0; i < length; ++i) {
        hexString.append(QString("%1 ").arg(data[i], 2, 16, QChar('0')).toUpper());
    }
    return hexString.trimmed();
}
void RFIDReader::run()
{
    SCARDCONTEXT hContext;
    SCARDHANDLE hCard;
    DWORD dwActiveProtocol;
    LONG lResult;

    // Establish context
    lResult = SCardEstablishContext(SCARD_SCOPE_SYSTEM, NULL, NULL, &hContext);
    if (lResult != SCARD_S_SUCCESS) {
        qDebug() << "Failed to establish context:"<< pcsc_stringify_error(lResult);
        emit statusMessage(tr("Failed to establish context: %s\n", pcsc_stringify_error(lResult)));
        return;
    }

    // List available readers
    LPTSTR mszReaders;
    DWORD dwReaders;
    lResult = SCardListReaders(hContext, NULL, NULL, &dwReaders);
    if (lResult != SCARD_S_SUCCESS) {
        qDebug() << "Failed to list readers:"<< pcsc_stringify_error(lResult);
        SCardReleaseContext(hContext);
        emit statusMessage(tr("Failed to list readers: %s\n", pcsc_stringify_error(lResult)));
        return;
    }

    mszReaders = (LPTSTR)malloc(sizeof(char) * dwReaders); // Cast malloc return value to LPTSTR
    if (mszReaders == NULL) {
        qDebug() << "Failed to allocate memory for readers list";
        SCardReleaseContext(hContext);
        emit statusMessage(tr("Failed to allocate memory for readers list"));
        return;
    }

    lResult = SCardListReaders(hContext, NULL, mszReaders, &dwReaders);
    if (lResult != SCARD_S_SUCCESS) {    
        qDebug() << "Failed to list readers:"<< pcsc_stringify_error(lResult);
        free(mszReaders);
        SCardReleaseContext(hContext);
        emit statusMessage(tr("Failed to list readers: %s\n", pcsc_stringify_error(lResult)));
        return;
    }
    
    // List all readers
    qDebug() << "Available readers:";     
    LPTSTR pReader = mszReaders;
    while (*pReader != '\0') {
        qDebug() << pReader;
        pReader += strlen(pReader) + 1;
    }

    // Use the first reader in the list
    LPTSTR readerName = mszReaders;
    if (*readerName == '\0') {
        qDebug() << "No readers found";
        free(mszReaders);
        SCardReleaseContext(hContext);
        return;
    }
    qDebug() << "Using reader:" << readerName;

    // Main loop to continuously check for card presence
    while (true) {
        SCARD_READERSTATE readerState;
        readerState.szReader = readerName;
        readerState.dwCurrentState = SCARD_STATE_UNAWARE;

        lResult = SCardGetStatusChange(hContext, INFINITE, &readerState, 1);
        if (lResult != SCARD_S_SUCCESS) {
            qDebug() << "Failed to get status change::" << pcsc_stringify_error(lResult);
            emit statusMessage(tr("Failed to get status change: %s\n", pcsc_stringify_error(lResult)));
            break;
        }

        if (readerState.dwEventState & SCARD_STATE_PRESENT) {
            // Connect to the reader
            lResult = SCardConnect(hContext, readerName, SCARD_SHARE_SHARED,
                                   SCARD_PROTOCOL_T0 | SCARD_PROTOCOL_T1, &hCard, &dwActiveProtocol);
            if (lResult != SCARD_S_SUCCESS) {
                qDebug() << "Failed to connect to the reader:" << pcsc_stringify_error(lResult);
                emit statusMessage(tr("Failed to connect to the reader: %s\n", pcsc_stringify_error(lResult)));
                continue;
            }

            // RFID reading logic
            BYTE pbSendBuffer[] = { 0xFF, 0xCA, 0x00, 0x00, 0x00 }; // APDU command to get UID
            BYTE pbRecvBuffer[256];
            DWORD dwRecvLength = sizeof(pbRecvBuffer);
            lResult = SCardTransmit(hCard, SCARD_PCI_T1, pbSendBuffer, sizeof(pbSendBuffer), NULL, pbRecvBuffer, &dwRecvLength);
            if (lResult != SCARD_S_SUCCESS) {
                qDebug() << "Failed to transmit APDU:" << pcsc_stringify_error(lResult);
                emit statusMessage(tr("Failed to transmit APDU: %s\n", pcsc_stringify_error(lResult)));

            } else {
                if (dwRecvLength > 2) { // Ensure there's a response other than SW1 and SW2

                    QString uid = byteArrayToHexString(pbRecvBuffer, dwRecvLength - 2);
                    qDebug() << "Card detected with UID:" << uid;
                    emit cardDetected(uid);
                } else {
                    qDebug() << "No Card detected";                  
                    emit statusMessage(tr("No card detected"));
                }
            }

            // Disconnect from the reader
            SCardDisconnect(hCard, SCARD_UNPOWER_CARD);
        } else {
            qDebug() << "No card present";
            emit statusMessage(tr("No card present"));
        }

        sleep(1); // Delay for 1 second before the next read attempt
    }

    // Release context
    SCardReleaseContext(hContext);

    free(mszReaders);
}
