#include <QDebug>
#include <QString>
#include <QtCore/qmath.h>

#include "scalewidget.h"
#include "ui_scalewidget.h"
#include "constants.h"

ScaleWidget::ScaleWidget(QWidget *parent) :
    AbstractFilterWidget(parent),
    ui(new Ui::ScaleWidget)
{
    ui->setupUi(this);


    // default value
    displayUnit = "pixel";

    doubleValidator = new QDoubleValidator;
    doubleValidator->setBottom(0.001);
    doubleValidator->setDecimals(3);
    doubleValidator->setNotation(QDoubleValidator::StandardNotation);
    ui->imageHeight->setValidator(doubleValidator);
    ui->imageWidth->setValidator(doubleValidator);
    ui->dpi->setValidator(doubleValidator);

    connect(ui->dpi->lineEdit(), SIGNAL(editingFinished()),
            this, SLOT(dpiFormChanged()));

    ui->dpi->insertItems(0, Constants::dpiList);
    setDpi(300);
}

ScaleWidget::~ScaleWidget()
{
    delete ui;
    delete doubleValidator;
}

void ScaleWidget::setPixmap(QPixmap pixmap)
{
    inputPixmap = pixmap;
    if (!pixmap.isNull()
            && ui->imageWidth->text().length() == 0
            && ui->imageHeight->text().length() == 0) {
        ui->imageWidth->setText(QString::number(pixmap.width()));
        ui->imageHeight->setText(QString::number(pixmap.height()));
    }
    if (!preview()) {
        ui->view->setPixmap(pixmap);
    }
}

void ScaleWidget::setPreview(QPixmap pixmap)
{
    previewPixmap = pixmap;
    if (preview())
        ui->view->setPixmap(pixmap);
}

bool ScaleWidget::preview()
{
    return ui->preview->isChecked();
}

double ScaleWidget::imagePixelHeight()
{
    return pxImageHeight;
}

double ScaleWidget::imagePixelWidth()
{
    return pxImageWidth;
}

QMap<QString, QVariant> ScaleWidget::getSettings()
{
    QMap<QString, QVariant> settings;

    settings["pxImageWidth"] = pxImageWidth;
    settings["pxImageHeight"] = pxImageHeight;
    settings["dpi"] = dpi;

    return settings;
}

void ScaleWidget::setSettings(QMap<QString, QVariant> settings)
{
    pxImageWidth = settings["pxImageWidth"].toDouble();
    pxImageHeight = settings["pxImageHeight"].toDouble();
    setDpi(settings["dpi"].toDouble());

    // Update form input
    setDisplayUnit(displayUnit);
    // Update rest of form
    updateResultingSizes();

}

void ScaleWidget::setDisplayUnit(QString unit)
{
    switch (Constants::displayUnits.indexOf(unit)) {
        case 0:     // pixel
            factorPixeltoDisplayUnit = 1;
            break;
        case 1:     // millimeter
            factorPixeltoDisplayUnit = Constants::milimeterPerInch / dpi;
            break;
        case 2:     // inches
            factorPixeltoDisplayUnit = 1 / dpi;
            break;
        default:
        // The unit provided is not valid. Do nothing.
        return;
    }
    displayUnit = unit;
    ui->lbUnitHeight->setText(unit);
    ui->lbUnitWidth->setText(unit);

    QString newImageWidth, newImageHeight;
    newImageWidth = QString::number(pxImageWidth * factorPixeltoDisplayUnit);
    newImageHeight = QString::number(pxImageHeight * factorPixeltoDisplayUnit);

    ui->imageWidth->setText(newImageWidth);
    ui->imageHeight->setText(newImageHeight);
}

void ScaleWidget::setBackgroundColor(QColor color)
{
    ui->view->setBackgroundBrush(QBrush(color));
}

void ScaleWidget::on_preview_toggled(bool checked)
{
    if (checked)
        ui->view->setPixmap(previewPixmap);
    else
        ui->view->setPixmap(inputPixmap);
}

// FIXME: this slot should disapear
/** \brief Slot to call when a parameter of the filter changes

 This is an agregator for all changed signals. It has multiple functions:
    - display calculated width and height of the resulting image.
    - emit a signel signalPropertyChanged, wich is used by the Scale class to
      recalculate the resulting image.
 */
void ScaleWidget::updateResultingSizes()
{
    ui->pixelImageWidth->setText(QString::number(pxImageWidth, 'f', 3));
    ui->pixelImageHeight->setText(QString::number(pxImageHeight, 'f', 3));
    ui->inchImageWidth->setText(QString::number(pxImageWidth / dpi, 'f', 3));
    ui->inchImageHeight->setText(QString::number(pxImageHeight / dpi, 'f', 3));
    ui->millimeterImageWidth->setText(
            QString::number(pxImageWidth / dpi * Constants::milimeterPerInch, 'f', 3));
    ui->millimeterImageHeight->setText(
                QString::number(pxImageHeight / dpi * Constants::milimeterPerInch, 'f', 3));
}

void ScaleWidget::setDpi(qreal newDpi)
{
    if (newDpi <= 0) // we only accept positive DPI
        return;

    dpi = newDpi;

    // we check if dpi is an integer. If so, we display it with precision = 0
    int precision = 3;
    qreal rest = newDpi - qFloor(newDpi);
    if (rest == 0) {
        precision = 0;
    }

    QString dpiString;
    dpiString = QString::number(dpi, 'f', precision);
    if (Constants::dpiList.contains(dpiString)) {
        ui->dpi->setCurrentIndex(Constants::dpiList.indexOf(dpiString));
    } else {
        ui->dpi->setEditText(dpiString);
    }
}

void ScaleWidget::on_imageWidth_editingFinished()
{
    pxImageWidth = ui->imageWidth->text().toDouble() / factorPixeltoDisplayUnit;

    updateResultingSizes();
    emit parameterChanged();
}

void ScaleWidget::on_imageHeight_editingFinished()
{
    pxImageHeight = ui->imageHeight->text().toDouble() / factorPixeltoDisplayUnit;

    updateResultingSizes();
    emit parameterChanged();
}

void ScaleWidget::dpiFormChanged()
{

    qreal newDPI = ui->dpi->currentText().toDouble();

    if (dpi == newDPI) // do nothing when nothing changes.
        return;

    if (newDPI <= 0) {  // We don't accept DPI <= 0, but this should never happen (Validator on Form)
        setDpi(dpi);    // Reset to the old value
    } else {
        dpi = newDPI;
    }

    switch (Constants::displayUnits.indexOf(displayUnit)) {
        case 0:     // pixel
            factorPixeltoDisplayUnit = 1;
            break;
        case 1:     // millimeter
            factorPixeltoDisplayUnit = Constants::milimeterPerInch / dpi;
            break;
        case 2:     // inches
            factorPixeltoDisplayUnit = 1 / dpi;
            break;
        // default:
        // Default will not happen, we check the unit before setting displayUnit
    }

    updateResultingSizes();
    emit parameterChanged();
}

void ScaleWidget::on_dpi_textChanged(const QString &stringDPI)
{
    qreal dpi = stringDPI.toDouble();

    if (dpi <= 0.001) {
        ui->dpi->setStyleSheet("background-color: rgb(255, 0, 0);");
    } else {
        ui->dpi->setStyleSheet("");
    }
}

void ScaleWidget::on_imageWidth_textEdited(const QString &strValue)
{
    qreal width = strValue.toDouble();

    if (width <= 0.001) {
        ui->imageWidth->setStyleSheet("background-color: rgb(255, 0, 0);");
    } else {
        ui->imageWidth->setStyleSheet("");
    }
}

void ScaleWidget::on_imageHeight_textEdited(const QString &strValue)
{
    qreal height = strValue.toDouble();

    if (height <= 0.001) {
        ui->imageHeight->setStyleSheet("background-color: rgb(255, 0, 0);");
    } else {
        ui->imageHeight->setStyleSheet("");
    }}
