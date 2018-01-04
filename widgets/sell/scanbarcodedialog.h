#ifndef SCANBARCODEDIALOG_H
#define SCANBARCODEDIALOG_H

#include <QDialog>

namespace Ui {
class ScanBarcodeDialog;
}

class ScanBarcodeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ScanBarcodeDialog(QWidget *parent = 0);
    ~ScanBarcodeDialog();
    void keyPressEvent(QKeyEvent *);

signals:
    void newBarcode(QString _barCode);

private:
    QString barCode;
    Ui::ScanBarcodeDialog *ui;
};

#endif // SCANBARCODEDIALOG_H
