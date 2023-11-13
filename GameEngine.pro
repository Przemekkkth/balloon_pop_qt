greaterThan(QT_MAJOR_VERSION, 4): QT += widgets multimedia

SOURCES += \
    src/balloon.cpp \
    src/fontmanager.cpp \
    src/gamescene.cpp \
    src/main.cpp \
    src/pixmapmanager.cpp \
    src/view.cpp \
    src/wall.cpp

HEADERS += \
    src/balloon.h \
    src/resourceholder.h \
    src/fontmanager.h \
    src/gamescene.h \
    src/pixmapmanager.h \
    src/utils.h \
    src/view.h \
    src/wall.h

RESOURCES += \
    resource.qrc

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/vendor/box2d/lib/release/ -lbox2d
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/vendor/box2d/lib/debug/ -lbox2d
else:unix: LIBS += -L$$PWD/vendor/box2d/lib/ -lbox2d

INCLUDEPATH += $$PWD/vendor/box2d/include
DEPENDPATH += $$PWD/vendor/box2d/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/vendor/box2d/lib/release/libbox2d.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/vendor/box2d/lib/debug/libbox2d.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/vendor/box2d/lib/release/box2d.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/vendor/box2d/lib/debug/box2d.lib
else:unix: PRE_TARGETDEPS += $$PWD/vendor/box2d/lib/libbox2d.a
