QT += quick serialbus
CONFIG -= rtti
OBJECTS_DIR = obj
MOC_DIR = moc

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
#DEFINES += SIMULATION

DESTDIR = ../interface/bin

INCLUDEPATH += ../../libs/canserviceDelegate/interface/include \
        ../../libs/canservice/interface/include \
        ../../libs/communicationlib/interface/include \
        ../../libs/canserviceDelegate/impl/src \
        ../../libs/canlib/interface/include \
        ../../libs/canlib/impl/src \
        ../../libs/canservice/interface/include \
        ../../libs/wifiservice/interface/include \
        ../../libs/gsmservice/interface/include \
        ../../libs/connectionmanagerservice/interface/include \
        ../../libs/btservice/interface/include

LIBS += -L../../libs/canlib/interface/lib -lcanlib
LIBS += -L../../libs/communicationlib/interface/lib -lcommunicationlib
LIBS += -L../../libs/canserviceDelegate/interface/lib -lcanserviceDelegate
LIBS += -L../../libs/canservice/interface/lib -l canservice


SOURCES += \
        EngineManager.cpp \
        TranslationManager.cpp \
        main.cpp
HEADERS += \
        ../../libs/canserviceDelegate/interface/include/CanserviceDelegate.h \
        EngineManager.h \
        MessageHandler.h \
        TranslationManager.h \
        Utility.h

RESOURCES += qml.qrc

TRANSLATIONS += \
        translations/BaseX_Hmi_Eng.ts \
        translations/BaseX_Hmi_Hin.ts

# Default rules for deployment
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# Ensure the translations directory exists in DESTDIR
translations.files = translations/*.qm
translations.path = $$DESTDIR/translations

# Debugging step: Print out the destination path
message("Copying translation files to: $$translations.path")

# Install the translations files
INSTALLS += translations

# If you want to embed translations as well:
TRANSLATIONS += \
                translations/BaseX_Hmi_Eng.ts \
                translations/BaseX_Hmi_Hin.ts

CONFIG += lrelease
CONFIG += embed_translations

# # Define source and destination folders
# SOURCE_FOLDER =  $$PWD/../resources
# DESTINATION_FOLDER = $$DESTDIR/../../

# # Define a custom target to copy the folder
# copy_folder.target = copy_folder
# copy_folder.commands = $$QMAKE_COPY_DIR $$SOURCE_FOLDER $$DESTINATION_FOLDER
# copy_folder.CONFIG += no_link

# # Add the custom target to the list of extra targets
# QMAKE_EXTRA_TARGETS += copy_folder

# # Add a dependency to the "all" target to ensure it runs as part of the build process
# QMAKE_POST_LINK += $$copy_folder.commands



