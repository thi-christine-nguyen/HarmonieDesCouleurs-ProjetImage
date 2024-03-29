QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
LIBS += C:\opencv\build\install\x64\mingw\bin\libopencv_ml490.dll
LIBS += C:\opencv\build\install\x64\mingw\bin\libopencv_objdetect490.dll
LIBS += C:\opencv\build\install\x64\mingw\bin\libopencv_photo490.dll
LIBS += C:\opencv\build\install\x64\mingw\bin\libopencv_stitching490.dll
LIBS += C:\opencv\build\install\x64\mingw\bin\libopencv_video490.dll
LIBS += C:\opencv\build\install\x64\mingw\bin\libopencv_videoio490.dll
LIBS += C:\opencv\build\install\x64\mingw\bin\libopencv_calib3d490.dll
LIBS += C:\opencv\build\install\x64\mingw\bin\libopencv_core490.dll
LIBS += C:\opencv\build\install\x64\mingw\bin\libopencv_dnn490.dll
LIBS += C:\opencv\build\install\x64\mingw\bin\libopencv_features2d490.dll
LIBS += C:\opencv\build\install\x64\mingw\bin\libopencv_flann490.dll
LIBS += C:\opencv\build\install\x64\mingw\bin\libopencv_gapi490.dll
LIBS += C:\opencv\build\install\x64\mingw\bin\libopencv_highgui490.dll
LIBS += C:\opencv\build\install\x64\mingw\bin\libopencv_imgcodecs490.dll
LIBS += C:\opencv\build\install\x64\mingw\bin\libopencv_imgproc490.dll

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += C:\opencv\build\include
DEPENDPATH += C:\opencv\build\include


SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    convertisseur.h \
    file_type.h \
    image_ppm.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    images/cheval.ppm
