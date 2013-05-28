#include "scalingwidget.h"
#include "ui_scalingwidget.h"

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
