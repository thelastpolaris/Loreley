#-------------------------------------------------
#
# Project created by QtCreator 2016-11-06T12:53:48
#
#-------------------------------------------------

QT       += core gui sql printsupport
include(qtxlsx/qtxlsx.pri)

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Loreley
TEMPLATE = app
TRANSLATIONS = languages/Loreley_ru.ts \
                languages/Loreley_en.ts
CONFIG += c++11
INCLUDEPATH += "/usr/include/c++/6/"

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
    data/models/productsmodel.cpp \
    auxiliary/ean13.cpp \
    widgets/products/editproperty.cpp \
    auxiliary/propertiesmodel.cpp \
    data/models/subproductsmodel.cpp \
    widgets/products/reducesubproductdialog.cpp \
    widgets/home.cpp \
    widgets/sell/sell.cpp \
    widgets/products/searchproductdialog.cpp \
    data/selldata.cpp \
    data/models/cartmodel.cpp \
    widgets/sell/scanbarcodedialog.cpp

HEADERS  += mainwindow.h \
    widgets/delegates/comboboxitemdelegate.h \
    widgets/products/products.h \
    widgets/products/addproductdialog.h \
    widgets/products/addsubproductdialog.h \
    widgets/delegates/datepickeritemdelegate.h \
    widgets/delegates/textfielditemdelegate.h \
    data/models/productsmodel.h \
    data/productsdata.h \
    auxiliary/ean13.h \
    widgets/products/editproperty.h \
    auxiliary/propertiesmodel.h \
    data/models/subproductsmodel.h \
    widgets/products/reducesubproductdialog.h \
    widgets/home.h \
    widgets/sell/sell.h \
    widgets/products/searchproductdialog.h \
    data/selldata.h \
    data/models/cartmodel.h \
    widgets/sell/scanbarcodedialog.h \
    data/helpers.h

FORMS    += mainwindow.ui \
    widgets/products/products.ui \
    widgets/products/addproductdialog.ui \
    widgets/products/addsubproductdialog.ui \
    widgets/products/editproperty.ui \
    widgets/products/reducesubproductdialog.ui \
    widgets/home.ui \
    widgets/sell/sell.ui \
    widgets/products/searchproductdialog.ui \
    widgets/sell/scanbarcodedialog.ui

RESOURCES += \
    resources.qrc
