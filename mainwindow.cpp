#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "widgets/sell/sell.h"
#include "widgets/reports/reports.h"
#include "widgets/products/products.h"
#include "widgets/home.h"
#include <QDir>
#include <QDebug>
#include <QSettings>
#include <QKeyEvent>

MainWindow* MainWindow::pinstance = 0;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow), p_products(NULL), p_home(NULL), p_sell(NULL),
    p_reports(NULL)
{
    ui->setupUi(this);
    ui->mainToolBar->hide();
    setCentralWidget(p_home);

    createActions();
    createLanguageMenu();

    connect(ui->actionCategories, SIGNAL(triggered(bool)), SIGNAL(categoriesTriggered(bool)));
    connect(ui->actionColors, SIGNAL(triggered(bool)), SIGNAL(colorsTriggered(bool)));
    connect(ui->actionBrands, SIGNAL(triggered(bool)), SIGNAL(brandsTriggered(bool)));
    connect(ui->actionSize, SIGNAL(triggered(bool)), SIGNAL(sizeTriggered(bool)));
    connect(ui->actionImportExcel, SIGNAL(triggered(bool)), SIGNAL(importExcelTriggered(bool)));
}

MainWindow* MainWindow::Create() {
    if(pinstance) delete pinstance;
    pinstance = new MainWindow;
    return pinstance;
}

void MainWindow::Initialize() {
    QWidget *prevWidget = centralWidget();
    if(p_products) delete p_products;
    p_products = new Products;

    if(p_home) delete p_home;
    p_home = new Home;

    if(p_sell) delete p_sell;
    p_sell = new Sell;

    if(p_reports) delete p_reports;
    p_reports = new Reports;

    if(prevWidget) setCentralWidget(prevWidget);
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

    QDir dir(":/languages");
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
            break;
        }
        default:
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
        switchTranslator(m_translator, QString(":languages/Loreley_%1.qm").arg(rLanguage));
        switchTranslator(m_translatorQt, QString("qt_%1.qm").arg(rLanguage));
        ui->statusBar->showMessage(tr("Current Language changed to %1").arg(languageName));
        productsData = ProductsData::Create();
        Initialize();
    }
}

void initializeWidgets() {

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::prepareCentralWidget() {
    if(centralWidget()) {
        centralWidget()->setParent(0);
    }
}

void MainWindow::openProducts() {
    prepareCentralWidget();
    p_products->prepareProducts();
    setCentralWidget(p_products);
}

void MainWindow::openHome() {
    prepareCentralWidget();
    p_sell->prepareSell();
    setCentralWidget(p_home);
}

void MainWindow::openSell() {
    prepareCentralWidget();
    setCentralWidget(p_sell);
}

void MainWindow::openReports() {
    prepareCentralWidget();
    setCentralWidget(p_reports);
}

void MainWindow::createActions() {
    connect(ui->actionHome, &QAction::triggered, this, &MainWindow::openHome);
    /******* Separator ********/
    connect(ui->actionProducts, &QAction::triggered, this, &MainWindow::openProducts);
    connect(ui->actionSell, &QAction::triggered, this, &MainWindow::openSell);
    connect(ui->actionReports, &QAction::triggered, this, &MainWindow::openReports);
}
