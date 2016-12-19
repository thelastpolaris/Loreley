#include "mainwindow.h"
#include <QApplication>
#include <QSettings>
#include <QDebug>
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
    db.setDatabaseName(settings.value("database/dbname", "loreley").toString());
    db.setUserName(settings.value("database/username", "loreley").toString());
    db.setPassword(settings.value("database/password", "redwood32").toString());
    Q_ASSERT_X(db.open(), "MainWindow::MainWindow", "Couldn't connect to database");

    //Initialize data - TO DO: Move to another class
    ProductsData::Create();

    MainWindow* win = MainWindow::Create();
    win->loadLanguage(QLocale::system().bcp47Name());

    win->openHome();
    win->show();

    return a.exec();
}
