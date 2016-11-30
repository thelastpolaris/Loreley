#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "widgets/products/products.h"

#include <QDir>
#include <QDebug>
#include <QSettings>

MainWindow* MainWindow::pinstance = 0;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    createActions();
    createLanguageMenu();

    connect(ui->actionCategories, SIGNAL(triggered(bool)), SIGNAL(categoriesTriggered(bool)));
    connect(ui->actionColors, SIGNAL(triggered(bool)), SIGNAL(colorsTriggered(bool)));
    connect(ui->actionBrands, SIGNAL(triggered(bool)), SIGNAL(brandsTriggered(bool)));
    connect(ui->actionSize, SIGNAL(triggered(bool)), SIGNAL(sizeTriggered(bool)));

    loadLanguage(QLocale::system().bcp47Name());
}

MainWindow* MainWindow::Create() {
    if(pinstance) delete pinstance;
    pinstance = new MainWindow;
    Products::Create();
    return pinstance;
}

// we create the menu entries dynamically, dependent on the existing translations.
void MainWindow::createLanguageMenu(void)
{
    QActionGroup* langGroup = new QActionGroup(ui->menuLanguage);
    langGroup->setExclusive(true);

    connect(langGroup, SIGNAL (triggered(QAction *)), this, SLOT (slotLanguageChanged(QAction *)));

    // format systems language
    QString defaultLocale = QLocale::system().name(); // e.g. "de_DE"
    defaultLocale.truncate(defaultLocale.lastIndexOf('_')); // e.g. "de"

    m_langPath = QApplication::applicationDirPath();
    m_langPath.append("/languages");
    QDir dir(m_langPath);
    QStringList fileNames = dir.entryList(QStringList("Loreley_*.qm"));

    for (int i = 0; i < fileNames.size(); ++i) {
        // get locale extracted by filename
        QString locale;
        locale = fileNames[i]; // "Loreley_ru.qm"
        locale.truncate(locale.lastIndexOf('.')); // "Loreley_ru"
        locale.remove(0, locale.indexOf('_') + 1); // "ru"

        QString lang = QLocale::languageToString(QLocale(locale).language());
        //        QIcon ico(QString("%1/%2.png").arg(m_langPath).arg(locale));

        QAction *action = new QAction(lang, this);
        action->setCheckable(true);
        action->setData(locale);

        ui->menuLanguage->addAction(action);
        langGroup->addAction(action);

        // set default translators and language checked
        if (defaultLocale == locale)
        {
            action->setChecked(true);
        }
    }
}

void MainWindow::changeEvent(QEvent* event)
{
    if(0 != event) {
        switch(event->type()) {
        // this event is send if a translator is loaded
        case QEvent::LanguageChange:
            ui->retranslateUi(this);
            break;

            // this event is send, if the system, language changes
        case QEvent::LocaleChange:
        {
            QString locale = QLocale::system().name();
            locale.truncate(locale.lastIndexOf('_'));
            loadLanguage(locale);
        }
            break;
        }
    }
    QMainWindow::changeEvent(event);
}

// Called every time, when a menu entry of the language menu is called
void MainWindow::slotLanguageChanged(QAction* action)
{
    if(0 != action) {
        // load the language dependant on the action content
        loadLanguage(action->data().toString());
        //        setWindowIcon(action->icon());
    }
}

void switchTranslator(QTranslator& translator, const QString& filename)
{
    // remove the old translator
    qApp->removeTranslator(&translator);

    // load the new translator
    if(translator.load(filename))
        qApp->installTranslator(&translator);
}

void MainWindow::loadLanguage(const QString& rLanguage)
{
    if(m_currLang != rLanguage) {
        m_currLang = rLanguage;
        QLocale locale = QLocale(m_currLang);
        QLocale::setDefault(locale);
        QString languageName = QLocale::languageToString(locale.language());
        switchTranslator(m_translator, QString("languages/Loreley_%1.qm").arg(rLanguage));
        switchTranslator(m_translatorQt, QString("qt_%1.qm").arg(rLanguage));
        ui->statusBar->showMessage(tr("Current Language changed to %1").arg(languageName));
        productsData = ProductsData::Create();
        Products::Create();
    }
}

void initializeWidgets() {

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openProducts() {
    ui->mainToolBar->hide();
    setCentralWidget(Products::Instance());

    //    qDeleteAll(ui->mainToolBar->actions());
    //    QAction *addProd = ui->mainToolBar->addAction(QIcon(":/images/icons/add.png"), tr("Add Product"));
    //    connect(addProd,SIGNAL(triggered(bool)), products, SLOT(startAddingProduct()));
    //    QAction *delProd = ui->mainToolBar->addAction(QIcon(":/images/icons/remove.png"), tr("Remove Product"));
    //    connect(delProd,SIGNAL(triggered(bool)), products, SLOT(startDeleteProduct()));
}

void MainWindow::createActions() {
    connect(ui->actionProducts, &QAction::triggered, this, &MainWindow::openProducts);
}
