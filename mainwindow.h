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

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    static MainWindow* Create();
    static MainWindow* Instance() { return pinstance; }

protected:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    // this event is called, when a new translator is loaded or the system language is changed
    void changeEvent(QEvent*);

signals:
    void categoriesTriggered(bool state);
    void colorsTriggered(bool state);
    void brandsTriggered(bool state);
    void sizeTriggered(bool state);

protected slots:
    // this slot is called by the language menu actions
    void slotLanguageChanged(QAction* action);

private slots:
    void openProducts();

private:
    Ui::MainWindow *ui;
    ProductsData* productsData;

    static MainWindow* pinstance;

    void createActions();

    // loads a language by the given language shortcur (e.g. de, en)
    void loadLanguage(const QString& rLanguage);

    // creates the language menu dynamically from the content of m_langPath
    void createLanguageMenu(void);

    QTranslator m_translator; // contains the translations for this application
    QTranslator m_translatorQt; // contains the translations for qt
    QString m_currLang; // contains the currently loaded language
    QString m_langPath; // Path of language files. This is always fixed to /languages.
};

#endif // MAINWINDOW_H
