#ifndef SCALEWIDGET_H
#define SCALEWIDGET_H

#include <QWidget>
#include "abstractfilterwidget.h"
#include <QDoubleValidator>


namespace Ui {
class ScaleWidget;
}

class ScaleWidget : public AbstractFilterWidget
{
    Q_OBJECT
    
public:
    explicit ScaleWidget(QWidget *parent = 0);
    ~ScaleWidget();
    
    void setPixmap(QPixmap pixmap);
    void setPreview(QPixmap pixmap);
    bool preview();
    double imagePixelHeight();
    double imagePixelWidth();

    QMap<QString, QVariant> getSettings();
    void setSettings(QMap <QString, QVariant> settings);
    void setDisplayUnit(QString unit);


public slots:
    void setBackgroundColor(QColor color);
    void setDPI(int newDpi);

private slots:
    void on_preview_toggled(bool checked);
    void on_imageWidth_editingFinished();
    void on_imageHeight_editingFinished();
    void on_imageWidth_textEdited(const QString &strValue);
    void on_imageHeight_textEdited(const QString &strValue);

private:
    void updateResultingSizes();

    Ui::ScaleWidget *ui;
    QDoubleValidator *doubleValidator;
    int lastUnitIndex = 0;
    QString displayUnit;
    qreal factorPixeltoDisplayUnit = 1;

    qreal pxImageWidth = 0;
    qreal pxImageHeight = 0;
    qreal dpi = 1;
};

#endif // SCALEWIDGET_H
