#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include "data/productsdata.h"
#include <QTranslator>

namespace Ui {
    class MainWindow;
}

class Products;
class Home;
class Sell;
class Reports;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    static MainWindow* Create();
    static MainWindow* Instance() { return pinstance; }
    /**
     * @brief loadLanguage - loads a language by the given language shortcur (e.g. de, en)
     * Use this function to initialize widgets
     * @param rLanguage - BCP47 Name of language
     */
    void loadLanguage(const QString& rLanguage);

    Sell* getSellWidget() { return p_sell; }
    Products* getProdWidget() { return p_products; }

protected:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    // this event is called, when a new translator is loaded or the system language is changed
    void changeEvent(QEvent*);

    /**
     * @brief prepareCentralWidget - changes the ownership of central widget
     */
    void prepareCentralWidget();

signals:
    void categoriesTriggered(bool state);
    void colorsTriggered(bool state);
    void brandsTriggered(bool state);
    void sizeTriggered(bool state);
    void importExcelTriggered(bool state);

protected slots:
    // this slot is called by the language menu actions
    void slotLanguageChanged(QAction* action);

public slots:
    void openProducts();
    void openHome();
    void openSell();
    void openReports();

private:
    Ui::MainWindow *ui;
    ProductsData* productsData;

    Products *p_products;
    Home *p_home;
    Sell *p_sell;
    Reports *p_reports;

    static MainWindow* pinstance;

    void createActions();

    void Initialize();

    // creates the language menu dynamically from the content of m_langPath
    void createLanguageMenu(void);

    QTranslator m_translator; // contains the translations for this application
    QTranslator m_translatorQt; // contains the translations for qt
    QString m_currLang; // contains the currently loaded language
    QString m_langPath; // Path of language files. This is always fixed to /languages.
};

#endif // MAINWINDOW_H
