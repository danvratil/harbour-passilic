TARGET = harbour-passilic

CONFIG += sailfishapp

DEFINES += \
    QT_NO_CAST_FROM_ASCII \
    QT_NO_CAST_TO_ASCII \
    QT_STRICT_ITERATORS \
    QT_USE_QSTRINGBUILDER \
    QT_NO_URL_CAST_FROM_STRING \
    QT_NO_CAST_FROM_BYTEARRAY \
    QT_NO_SIGNALS_SLOTS_KEYWORDS


INCLUDEPATH += 3rdparty/kitemmodels/

SOURCES += \
    src/main.cpp \
    src/abbreviations.cpp \
    src/imageprovider.cpp \
    src/passwordfiltermodel.cpp \
    src/passwordprovider.cpp \
    src/passwordsmodel.cpp \
    src/passwordsortproxymodel.cpp \
    3rdparty/kitemmodels/kdescendantsproxymodel.cpp \
    src/settings.cpp \
    src/passwordgenerator.cpp


HEADERS += \
    src/abbreviations.h \
    src/imageprovider.h \
    src/passwordfiltermodel.h \
    src/passwordprovider.h \
    src/passwordsmodel.h \
    src/passwordsortproxymodel.h \
    3rdparty/kitemmodels/kdescendantsproxymodel.h \
    src/settings.h \
    src/scopeguard.h \
    src/passwordgenerator.h

DISTFILES += \
    qml/harbour-passilic.qml \
    qml/cover/CoverPage.qml \
    qml/pages/AboutPage.qml \
    qml/pages/PasswordListPage.qml \
    qml/pages/PassphraseRequester.qml \
    qml/components/GlobalPullDownMenu.qml \
    rpm/harbour-passilic.changes.in \
    rpm/harbour-passilic.changes.run.in \
    rpm/harbour-passilic.spec \
    rpm/harbour-passilic.yaml \
    translations/*.ts \
    harbour-passilic.desktop \
    qml/pages/SearchPage.qml \
    qml/components/PasswordDelegate.qml \
    qml/pages/SettingsPage.qml \
    qml/pages/NewPasswordDialog.qml \
    qml/pages/GeneratePasswordDialog.qml

OTHER_FILES += \
    README.md

SAILFISHAPP_ICONS = 86x86 108x108 128x128 172x172

# to disable building translations every time, comment out the
# following CONFIG line
CONFIG += sailfishapp_i18n

TRANSLATIONS += \
    translations/harbour-passilic.ts \
    translations/harbour-passilic-cs.ts \
    translations/harbour-passilic-fr.ts \
    translations/harbour-passilic-hr.ts \
    translations/harbour-passilic-sv.ts \
    translations/harbour-passilic-zh.ts
