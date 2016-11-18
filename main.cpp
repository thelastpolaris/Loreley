#include "mainwindow.h"
#include <QApplication>
#include <QSettings>
#include "widgets/products/products.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setOrganizationName("Silmaril");
    QApplication::setOrganizationDomain("silmaril.com");
    QApplication::setApplicationName("Loreley");

    QSettings settings;

    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");

    db.setHostName(settings.value("database/hostname", "localhost").toString());
    db.setDatabaseName(settings.value("database/dbname", "saturnPOS").toString());
    db.setUserName(settings.value("database/username", "saturnPOS").toString());
    db.setPassword(settings.value("database/password", "redwood32").toString());
    Q_ASSERT_X(db.open(),"MainWindow::MainWindow", "Couldn't connect to database");

    //Initialize data - TO DO: Move to another class
    ProductsData::Create();

    MainWindow* win = MainWindow::Create();
    win->setCentralWidget(Products::Instance());
    win->show();

    return a.exec();
}
