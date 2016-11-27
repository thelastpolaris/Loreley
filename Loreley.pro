#-------------------------------------------------
#
# Project created by QtCreator 2016-11-06T12:53:48
#
#-------------------------------------------------

QT       += core gui sql printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Loreley
TEMPLATE = app
TRANSLATIONS = languages/Loreley_ru.ts \
                languages/Loreley_en.ts

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
    data/subproductsmodel.cpp

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
    data/subproductsmodel.h

FORMS    += mainwindow.ui \
    widgets/products/products.ui \
    widgets/products/addproductdialog.ui \
    widgets/products/addsubproductdialog.ui \
    widgets/products/editproperty.ui

RESOURCES += \
    resources.qrc
