#-------------------------------------------------
#
# Project created by QtCreator 2016-11-06T12:53:48
#
#-------------------------------------------------

QT       += core gui sql printsupport multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Loreley
TEMPLATE = app
TRANSLATIONS = languages/Loreley_ru.ts \
                languages/Loreley_en.ts
CONFIG += c++11

SOURCES += main.cpp\
        mainwindow.cpp \
#    auxiliary/
    widgets/delegates/comboboxitemdelegate.cpp \
    widgets/delegates/datepickeritemdelegate.cpp \
    widgets/delegates/textfielditemdelegate.cpp \
    widgets/products/addproductdialog.cpp \
    widgets/products/addsubproductdialog.cpp \
    widgets/products/products.cpp \
    data/productsdata.cpp \
    data/productsmodel.cpp \
    auxiliary/ean13.cpp \
    widgets/products/editproperty.cpp \
    auxiliary/propertiesmodel.cpp \
    data/subproductsmodel.cpp \
    widgets/products/reducesubproductdialog.cpp \
    widgets/home.cpp \
    widgets/sell/sell.cpp \
    widgets/products/searchproductdialog.cpp

HEADERS  += mainwindow.h \
    widgets/delegates/comboboxitemdelegate.h \
    widgets/products/products.h \
    widgets/products/addproductdialog.h \
    widgets/products/addsubproductdialog.h \
    widgets/delegates/datepickeritemdelegate.h \
    widgets/delegates/textfielditemdelegate.h \
    data/productsmodel.h \
    data/productsdata.h \
    auxiliary/ean13.h \
    widgets/products/editproperty.h \
    auxiliary/propertiesmodel.h \
    data/subproductsmodel.h \
    widgets/products/reducesubproductdialog.h \
    widgets/home.h \
    widgets/sell/sell.h \
    widgets/products/searchproductdialog.h

FORMS    += mainwindow.ui \
    widgets/products/products.ui \
    widgets/products/addproductdialog.ui \
    widgets/products/addsubproductdialog.ui \
    widgets/products/editproperty.ui \
    widgets/products/reducesubproductdialog.ui \
    widgets/home.ui \
    widgets/sell/sell.ui \
    widgets/products/searchproductdialog.ui

RESOURCES += \
    resources.qrc

win32 {
message("Using win32 configuration")

OPENCV_PATH = C:/devel/OpenCV/opencv-2.4.6.0


#LIBS_PATH = "$$OPENCV_PATH/build/x86/mingw/lib" #project compiled using MINGW
LIBS_PATH = "$$OPENCV_PATH/build/x86/vc10/lib" #project compiled using Visual C++ 2010 32bit compiler

    CONFIG(debug, debug|release) {
    LIBS     += -L$$LIBS_PATH \
                -lopencv_core246d \
                -lopencv_highgui246d
    }

    CONFIG(release, debug|release) {
    LIBS     += -L$$LIBS_PATH \
                -lopencv_core246 \
                -lopencv_highgui246
    }

}

unix {
message("Using unix configuration")

OPENCV_PATH = /usr/local/opencv2/

LIBS     += -L$$LIBS_PATH \
            -lopencv_core \
            -lopencv_highgui
}

INCLUDEPATH += $$OPENCV_PATH/modules/core/include/ \ #core module
    $$OPENCV_PATH/modules/highgui/include/ #highgui modul

message("OpenCV path: $$OPENCV_PATH")
message("Includes path: $$INCLUDEPATH")
message("Libraries: $$LIBS")
