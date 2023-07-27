QT += gui

TARGET = EhxoQt
TEMPLATE = lib

DEFINES += PARALLIZEDALGORITHM_LIBRARY

SOURCES += \
  EhxoQt.cpp \
  Individual.cpp \
  IndividualLoader.cpp \
  Species.cpp

HEADERS +=\
  EhxoQt.h \
  EhxoQtGlobal.h \
  EhxoQtPaPreCompile.h \
  Individual.h \
  IndividualLoader.h \
    PaComparePickResult.h \
  Species.h

export_header_files.path=/usr/include/EhxoQt
export_header_files.files+=GameType.h \
  Xy_global.h \
  RangeSortSearch.h \
PaPreCompile.h \
PaComparator.h \
CompareResultRelation.h \
PaMapReduceFunctions.h \
PaCompareResult.h \
PaGlobal.h \
CompareWithComparatorMap.h \
PickComparedDataReduce.h \
PaComparePickResult.h \
PaSignalProxy.h \
PaSignalProxySolid.h \
EhxoQt.h \
Individual.h \
Species.h \
EhxoQtGlobal.h \
EhxoQtPaPreCompile.h \
IndividualLoader.h

unix:!symbian {
  target.path = /usr/lib

  INSTALLS += target \
  export_header_files
}

PRECOMPILED_HEADER+

TRANSLATIONS+=Xy.ts

VERSION=3.2.9

QT+=concurrent

CONFIG +=c++14

QMAKE_CXXFLAGS+= -pedantic-errors -Wall

#QMAKE_CXXFLAGS+= -fsanitize=address

#LIBS+= -fsanitize=address

DEFINES += EHXO_QT

macx {
LIBS += -L$$PWD/../ThirdPartyMacOs.pp/EventChartQt.g/bin.conf/ -lEventChartQt

INCLUDEPATH += $$PWD/../ThirdPartyMacOs.pp/EventChartQt.g/include.tions
DEPENDPATH += $$PWD/../ThirdPartyMacOs.pp/EventChartQt.g/include.tions
}

win32 {
LIBS += -L$$PWD/../ThirdPartyWindows.Qt/EventChartQt.tions/lib/ -llibEventChartQt2.dll

INCLUDEPATH += $$PWD/../ThirdPartyWindows.Qt/EventChartQt.tions/include.g/
DEPENDPATH += $$PWD/../ThirdPartyWindows.Qt/EventChartQt.tions/include.g/
}

PRECOMPILED_HEADER += \
  EhxoQtPaPreCompile.h

android {
QT+=androidextras

INCLUDEPATH += $$PWD/../ThirdPartyAndroid/Log4Qt-1.5.0/include/

INCLUDEPATH += $$PWD/../ThirdPartyAndroid/NQNAQt/include/

#INCLUDEPATH += $$PWD/../ThirdPartyAndroid.t/EventChartQt.idea/include.e/
#LIBS += -L$$PWD/../ThirdPartyAndroid.t/EventChartQt.idea/bin.s/ -lEventChartQt

#QMAKE_LINK += -nostdlib++

ANDROID_ABIS=arm64-v8a

}

contains(ANDROID_TARGET_ARCH,arm64-v8a)
{
  ANDROID_EXTRA_LIBS = \
  $$PWD/../ThirdPartyAndroid/EhxoQt/bin/libEhxoQt.so \
  $$PWD/../ThirdPartyAndroid/VoiceUiQt.logs/bin.res/libVoiceUiQt.so \
#  $$PWD/../ThirdPartyAndroid/EventChartQt.idea/bin.s/libEventChartQt.so \
  $$PWD/../ThirdPartyAndroid/yaml-cpp-yaml-cpp-0.6.2/bin/libyaml-cpp.so
}
