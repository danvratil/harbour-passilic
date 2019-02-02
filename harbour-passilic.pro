TARGET = harbour-passilic

CONFIG += sailfishapp

INCLUDEPATH += 3rdparty/kitemmodels/

SOURCES += \
    src/main.cpp \
    src/abbreviations.cpp \
    src/passwordfiltermodel.cpp \
    src/passwordprovider.cpp \
    src/passwordsmodel.cpp \
    src/passwordsortproxymodel.cpp \
    3rdparty/kitemmodels/kdescendantsproxymodel.cpp


HEADERS += \
    src/abbreviations.h \
    src/passwordfiltermodel.h \
    src/passwordprovider.h \
    src/passwordsmodel.h \
    src/passwordsortproxymodel.h \
    3rdparty/kitemmodels/kdescendantsproxymodel.h

DISTFILES += \
    qml/harbour-passilic.qml \
    qml/cover/CoverPage.qml \
    qml/pages/PasswordListPage.qml \
    rpm/harbour-passilic.changes.in \
    rpm/harbour-passilic.changes.run.in \
    rpm/harbour-passilic.spec \
    rpm/harbour-passilic.yaml \
    translations/*.ts \
    harbour-passilic.desktop

SAILFISHAPP_ICONS = 86x86 108x108 128x128 172x172

# to disable building translations every time, comment out the
# following CONFIG line
#CONFIG += sailfishapp_i18n

#TRANSLATIONS += \
#    translations/harbour-passilic-de.ts
