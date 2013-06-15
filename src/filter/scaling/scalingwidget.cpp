#include "scalingwidget.h"
#include "ui_scalingwidget.h"
#include <QDebug>
#include <QString>

ScalingWidget::ScalingWidget(QWidget *parent) :
    AbstractFilterWidget(parent),
    ui(new Ui::ScalingWidget)
{
    ui->setupUi(this);

    doubleValidator = new QDoubleValidator;
    doubleValidator->setBottom(0);
    doubleValidator->setDecimals(3);
    doubleValidator->setNotation(QDoubleValidator::StandardNotation);
    ui->imageHeight->setValidator(doubleValidator);
    ui->imageWidth->setValidator(doubleValidator);
    ui->dpi->setValidator(doubleValidator);
    ui->leftMargin->setValidator(doubleValidator);
    ui->rightMargin->setValidator(doubleValidator);
    ui->bottomMargin->setValidator(doubleValidator);
    ui->topMargin->setValidator(doubleValidator);
    ui->pageWidth->setValidator(doubleValidator);
    ui->pageHeight->setValidator(doubleValidator);
}

ScalingWidget::~ScalingWidget()
{
    delete ui;
    delete doubleValidator;
}

void ScalingWidget::setPixmap(QPixmap pixmap)
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

void ScalingWidget::setPreview(QPixmap pixmap)
{
    previewPixmap = pixmap;
    if (preview())
        ui->view->setPixmap(pixmap);
}

bool ScalingWidget::preview()
{
    return ui->preview->isChecked();
}

double ScalingWidget::imagePixelHeight()
{
    return ui->pixelImageHeight->text().toDouble();
}

double ScalingWidget::imagePixelWidth()
{
    return ui->pixelImageWidth->text().toDouble();
}

QMap<QString, QVariant> ScalingWidget::getSettings()
{
    QMap<QString, QVariant> settings;

    settings["imageWidth"] = ui->imageWidth->text().toDouble();
    settings["imageHeight"] = ui->imageHeight->text().toDouble();
    settings["DPI"] = ui->dpi->currentText().toDouble();
    settings["unit"] = ui->unit->currentIndex();

    return settings;

}

void ScalingWidget::setSettings(QMap<QString, QVariant> settings)
{
    if (settings.contains("imageWidth")
            && settings["imageWidth"].canConvert(QVariant::Double)
            && settings["imageWidth"].toDouble() != 0) {
        ui->imageWidth->setText(settings["imageWidth"].toString());
    } else {
        ui->imageWidth->clear();
    }
    if (settings.contains("imageHeight")
            && settings["imageHeight"].canConvert(QVariant::Double)
            && settings["imageHeight"].toDouble() != 0) {
        ui->imageHeight->setText(settings["imageHeight"].toString());
    } else {
        ui->imageHeight->clear();
    }
    if (settings.contains("DPI")
            && settings["DPI"].canConvert(QVariant::Double)) {
        QString DPIValue = settings["DPI"].toString();
        int index = ui->dpi->findText(DPIValue);
        if (index == -1) {          // This is no default value
            ui->dpi->addItem(DPIValue);
            index = ui->dpi->count();
        }
        ui->dpi->setCurrentIndex(index);
    } else {
        ui->dpi->setCurrentIndex(1);
    }
    if (settings.contains("unit")
            && settings["unit"].canConvert(QVariant::Int)) {
        lastUnitIndex = settings["unit"].toInt();
        ui->unit->setCurrentIndex(settings["unit"].toInt());
    } else {
        lastUnitIndex = 1;
        ui->unit->setCurrentIndex(1);
    }

    slotPropertyChanged();
}

void ScalingWidget::setBackgroundColor(QColor color)
{
    ui->view->setBackgroundBrush(QBrush(color));
}

void ScalingWidget::on_preview_toggled(bool checked)
{
    if (checked)
        ui->view->setPixmap(previewPixmap);
    else
        ui->view->setPixmap(inputPixmap);
}

/** \brief Slot to call when a parameter of the filter changes

 This is an agregator for all changed signals. It has multiple functions:
    - display calculated width and height of the resulting image.
    - emit a signel signalPropertyChanged, wich is used by the Scaling class to
      recalculate the resulting image.
 */
void ScalingWidget::slotPropertyChanged()
{
    qreal factor;
    const qreal millimeterPerInch = 25.4;     // 1 inch = 25,4 mm

    qreal dpi = ui->dpi->currentText().toDouble();
    if (dpi > 0) {
        // calculate the factor into pixel
        switch (ui->unit->currentIndex()) {
            case 0:      // pixel -> do nothing
                factor = 1;
                break;
            case 1:    // millimeter
                factor = 1 / millimeterPerInch * dpi;
                break;
            case 2:     // inches
                factor = dpi;
                break;
            default:
            // This step is never reached.
            Q_ASSERT (false);
        }

        // Sizes in Pixel
        qreal imageWidth = ui->imageWidth->text().toDouble() * factor;
        qreal imageHeight = ui->imageHeight->text().toDouble() * factor;
        qreal leftMargin = ui->leftMargin->text().toDouble() * factor;
        qreal rightMargin = ui->rightMargin->text().toDouble() * factor;
        qreal bottomMargin = ui->bottomMargin->text().toDouble() * factor;
        qreal topMargin = ui->topMargin->text().toDouble() * factor;
        qreal specifiedPageWidth = ui->pageWidth->text().toDouble() * factor;
        qreal specifiedPageHeight = ui->pageHeight->text().toDouble() * factor;

        qreal pageWidth, pageHeight;
        if (ui->noMarginLayout->isChecked()) {
            pageWidth = imageWidth;
            pageHeight = imageHeight;
        }
        if (ui->marginLayout->isChecked()) {
            pageWidth = imageWidth + leftMargin + rightMargin;
            pageHeight = imageHeight + topMargin + bottomMargin;
        }
        if (ui->pageLayout->isChecked()) {
            pageWidth = specifiedPageWidth;
            pageHeight = specifiedPageHeight;
        }

        //image
        ui->pixelImageWidth->setText(QString::number(imageWidth, 'f', 1));
        ui->pixelImageHeight->setText(QString::number(imageHeight, 'f', 1));
        ui->inchImageWidth->setText(QString::number(imageWidth / dpi, 'f', 1));
        ui->inchImageHeight->setText(QString::number(imageHeight / dpi, 'f', 1));
        ui->millimeterImageWidth->setText(QString::number(imageWidth / dpi * millimeterPerInch, 'f', 1));
        ui->millimeterImageHeight->setText(QString::number(imageHeight / dpi * millimeterPerInch, 'f', 1));
        //page
        ui->pixelPageWidth->setText(QString::number(pageWidth, 'f', 1));
        ui->pixelPageHeight->setText(QString::number(pageHeight, 'f', 1));
        ui->inchPageWidth->setText(QString::number(pageWidth / dpi, 'f', 1));
        ui->inchPageHeight->setText(QString::number(pageHeight / dpi, 'f', 1));
        ui->millimeterPageWidth->setText(QString::number(pageWidth / dpi * millimeterPerInch, 'f', 1));
        ui->millimeterPageHeight->setText(QString::number(pageHeight / dpi * millimeterPerInch, 'f', 1));
    } else {
        ui->pixelImageWidth->clear();
        ui->pixelImageHeight->clear();
        ui->inchImageWidth->clear();
        ui->inchImageHeight->clear();
        ui->millimeterImageWidth->clear();
        ui->millimeterImageHeight->clear();
        ui->leftMargin->clear();
        ui->rightMargin->clear();
        ui->bottomMargin->clear();
        ui->topMargin->clear();
        ui->pageWidth->clear();
        ui->pageHeight->clear();
    }

    emit signalPropertyChanged();
}

/** \brief Update the size displayed when the Unit changes
*/
void ScalingWidget::on_unit_currentIndexChanged(int index)
{
    qreal dpi = ui->dpi->currentText().toDouble();
    const qreal millimeterPerInch = 25.4;     // 1 inch = 25,4 mm

    if (lastUnitIndex == index || dpi == 0) {
        return;
    }

    qreal factor;

    /* First calculate the factor to convert into pixel */
    switch (lastUnitIndex) {
        case 0:      // pixel -> do nothing
            factor = 1;
            break;
        case 1:    // millimeter
            factor = 1 / millimeterPerInch * dpi;
            break;
        case 2:     // inches
            factor = dpi;
            break;
        default:
        // This step is never reached.
        Q_ASSERT (false);
    }


    /* Then update the factor to convert to the new Unit */
    switch (index) {
        case 0:      // pixel -> do nothing
            // factor = factor * 1;
            break;
        case 1:    // millimeter
            factor = factor / dpi * millimeterPerInch;
            break;
        case 2:     // inches
            factor = factor / dpi;
            break;
        default:
        // This step is never reached.
        Q_ASSERT (false);
    }

    QString calcWidth, calcHeight;
    qreal width = ui->imageWidth->text().toDouble();
    qreal height = ui->imageHeight->text().toDouble();
    calcWidth = QString::number(width * factor);
    calcHeight = QString::number(height * factor);
    ui->imageWidth->setText(calcWidth);
    ui->imageHeight->setText(calcHeight);

    lastUnitIndex = index;

}
