INCLUDEPATH+=PlayCore/libvlc_include

SOURCES +=$$PWD/myMediaList.cpp\
        $$PWD/FFmpegPlayer.cpp\

HEADERS +=$$PWD/myMediaList.h\
        $$PWD/FFmpegPlayer.h\


LIBS += -L$$PWD/lib/ -lavcodec\
        -L$$PWD/lib/ -lavdevice \
        -L$$PWD/lib/ -lavfilter \
        -L$$PWD/lib/ -lavutil \
        -L$$PWD/lib/ -lavformat \
        -L$$PWD/lib/ -lpostproc \
        -L$$PWD/lib/ -lswresample \
        -L$$PWD/lib/ -lSDL2main  \
        -L$$PWD/lib/ -lswscale \
        -L$$PWD/lib/ -lSDL2

INCLUDEPATH +=$$PWD/include/

