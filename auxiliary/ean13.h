#ifndef EAN13_H
#define EAN13_H

#include <QStringList>
#include <QPixmap>

/********************************************************************************/
class EAN13 : public QObject
{
    Q_OBJECT
public:
        EAN13(QObject *parent = nullptr);
        bool makePattern(const QString &code);
        void draw(const QRectF &rect, QPainter &painter, const QString &code);
        void draw(const QRectF &rect, QPainter &painter);
        void EAN13ToImage(QPixmap &image, const QString &code);
        QString generateBarcode(const QString &code, int _checksum = -1);

private:
        QRectF      *codeRect;				// bounds of this device

        QStringList LCode;
        QStringList GCode;
        QStringList RCode;
        QStringList Parity;
        QString     quietZone;
        QString     leadTail;
        QString     separator;
        QString     checksumDigit;
        qreal       nominalWidth;
        qreal       nominalHeight;
        qreal       nominalFontSize;
        int         checksum ;

        QString     barcode;
        QString     parity ;
        QString     pattern ;
        QString     countryCode, leftCode, rightCode ;

        QString     convertToDigitPattern(QString number, QStringList &pattern);
};

/********************************************************************************/

#endif // EAN13_H
