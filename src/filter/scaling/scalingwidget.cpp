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

double ScalingWidget::imageHeight()
{
    return ui->imageHeight->text().toDouble();
}

double ScalingWidget::imageWidth()
{
    return ui->imageWidth->text().toDouble();
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
        ui->unit->setCurrentIndex(settings["unit"].toInt());
    } else {
        ui->dpi->setCurrentIndex(1);
    }
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

 This is an agregator for all changed signals. It emits signalPropertyChanged, wich is used
 The Scaling class to recalculate the resulting image.
 */
void ScalingWidget::slotPropertyChanged()
{
    emit signalPropertyChanged();
}
