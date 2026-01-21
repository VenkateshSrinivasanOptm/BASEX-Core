// #ifndef RFIDREADER_H
// #define RFIDREADER_H

#include <QObject>
#include <QThread>
#include <QString>

class RFIDReader : public QThread
{
    Q_OBJECT

public:
    explicit RFIDReader(QObject *parent = nullptr);
    ~RFIDReader();

signals:
    void cardDetected(const QString &uid);
    void statusMessage(const QString &uid);

protected:
    void run() override;

};
//  9F64 C116 0C7F 0000
// 2B9BBA76FF7F0000
// #endif // RFIDREADER_H*/
// #include <QThread>
// #include <QByteArray>
// #include <QDebug>
// #include <pcsclite.h>
// #include <QString>
// #include <winscard.h>

// class RFIDReader : public QThread
// {
//     Q_OBJECT

// public:
//     RFIDReader() : mContext(0), mCard(0), mReaderName(nullptr), mCardInserted(false) {}
//     ~RFIDReader() {
//         if (mCard) {
//             SCardDisconnect(mCard, SCARD_LEAVE_CARD);
//         }
//         if (mContext) {
//             SCardReleaseContext(mContext);
//         }
//     }

//     void run() override {
//         // Initialize context for smart card communication
//         LONG result = SCardEstablishContext(SCARD_SCOPE_SYSTEM, nullptr, nullptr, &mContext);
//         if (result != SCARD_S_SUCCESS) {
//             qDebug() << "Failed to establish context.";
//             return;
//         }

//         // List available readers
//         DWORD dwReaders = 0;
//         result = SCardListReaders(mContext, nullptr, nullptr, &dwReaders);
//         if (result != SCARD_S_SUCCESS || dwReaders == 0) {
//             qDebug() << "No readers found.";
//             return;
//         }

//         char *mszReaders = (char *)malloc(dwReaders);
//         result = SCardListReaders(mContext, nullptr, mszReaders, &dwReaders);
//         if (result != SCARD_S_SUCCESS) {
//             qDebug() << "Failed to list readers.";
//             free(mszReaders);
//             return;
//         }

//         mReaderName = mszReaders;
//         qDebug() << "Using reader:" << mReaderName;

//         // Continuous RFID scanning loop
//         while (true) {
//             SCARDHANDLE hCard;
//             DWORD dwActiveProtocol;

//             // Try to connect to the card reader if not already connected
//             result = SCardConnect(mContext, mReaderName, SCARD_SHARE_SHARED, SCARD_PROTOCOL_T1, &hCard, &dwActiveProtocol);
//             if (result != SCARD_S_SUCCESS) {
//                 qDebug() << "Failed to connect to the reader:" << result;
//                 QThread::msleep(1000);  // Sleep before retrying
//                 continue;  // If failed to connect, try again
//             }

//             DWORD dwState;
//             DWORD dwProtocol;
//             BYTE pbRecvBuffer[262];
//             DWORD dwRecvLength = sizeof(pbRecvBuffer);
//             BYTE CardID [8];

//             // Check if a card is inserted
//             result = SCardStatus(hCard, nullptr, nullptr, &dwState, &dwProtocol, pbRecvBuffer, &dwRecvLength);
//             if (result != SCARD_S_SUCCESS) {
//                 qDebug() << "Failed to check card status:" << result;
//                 SCardDisconnect(hCard, SCARD_LEAVE_CARD);
//                 continue;  // Retry after disconnecting
//             }

//             // If card is inserted and it wasn't already detected
//             if ((dwState & SCARD_STATE_PRESENT) /*&& !mCardInserted*/) {
//                 mCardInserted = true; // Mark card as inserted

//                 // Send APDU command to get UID (ISO 14443 Type A card)
//                 BYTE pbSendBuffer[] = { 0xFF, 0xCA, 0x00, 0x00, 0x00 };  // APDU command to get UID
//                 result = SCardTransmit(hCard, SCARD_PCI_T1, pbSendBuffer, sizeof(pbSendBuffer), nullptr, pbRecvBuffer, &dwRecvLength);
//                 if (result != SCARD_S_SUCCESS) {
//                     qDebug() << "Failed to transmit APDU command:" << result;
//                 } else {
//                     if (dwRecvLength > 2) {
//                         int uidLength = pbRecvBuffer[0];
//                         if (uidLength > 0) {
//                             for (DWORD i=0;i < dwRecvLength-2;i++)
//                             {
//                                 CardID [i] = pbRecvBuffer[i];
//                             }

//                             auto uid = byteArrayToHexString(CardID);
//                             qDebug() << "Card detected with UID:" << uid;
//                             emit cardDetected(uid);  // Emit signal when card is detected
//                         } else {
//                             qDebug() << "No UID detected.";
//                         }
//                     } else {
//                         qDebug() << "Received invalid response length.";
//                     }
//                 }
//             }

//             // If card is removed
//             if (!(dwState & SCARD_STATE_PRESENT) && mCardInserted) {
//                 mCardInserted = false; // Mark card as removed
//                 qDebug() << "Card removed. Waiting for a new card...";
//             }

//             // Clean up before trying again
//             SCardDisconnect(hCard, SCARD_LEAVE_CARD);
//             QThread::msleep(500);  // Sleep for 500 ms before checking again
//         }

//         free(mszReaders);  // Free the memory used for reader names
//     }

// signals:
//     void cardDetected(const QString &uid);  // Signal to notify when card is detected

// private:
//     SCARDCONTEXT mContext;
//     SCARDHANDLE mCard;
//     const char *mReaderName;
//     bool mCardInserted;

//     QString byteArrayToHexString(const BYTE cardID[8]) {
//         QByteArray byteArray(reinterpret_cast<const char*>(cardID), 8);  // Convert BYTE array to QByteArray
//         return byteArray.toHex().toUpper();  // Convert to hex string and make it uppercase
//     }
// };
