#include "colorcorrectionwidget.h"
#include "ui_colorcorrectionwidget.h"
#include <QDebug>

ColorCorrectionWidget::ColorCorrectionWidget(QWidget *parent) :
    AbstractFilterWidget(parent),
    ui(new Ui::ColorCorrectionWidget)
{
    ui->setupUi(this);

    // Only allow values between 0 and 255 for RGB colors
    intValidator = new QIntValidator(0,255);
    ui->whiteRedValue->setValidator(intValidator);
    ui->whiteGreenValue->setValidator(intValidator);
    ui->whiteBlueValue->setValidator(intValidator);
    ui->blackRedValue->setValidator(intValidator);
    ui->blackGreenValue->setValidator(intValidator);
    ui->blackBlueValue->setValidator(intValidator);

    connect(ui->view, SIGNAL(pixmapClicked(QColor)),
            this, SLOT(imageClicked(QColor)));
}

ColorCorrectionWidget::~ColorCorrectionWidget()
{
    delete ui;
    delete intValidator;
}


void ColorCorrectionWidget::setPixmap(QPixmap pixmap)
{
    inputPixmap = pixmap;
    if (!preview()) {
        ui->view->setPixmap(pixmap);
    }
}

void ColorCorrectionWidget::setPreview(QPixmap pixmap)
{
    previewPixmap = pixmap;
    if (preview())
        ui->view->setPixmap(pixmap);
}

bool ColorCorrectionWidget::preview()
{
    return ui->preview->isChecked();
}

QColor ColorCorrectionWidget::whitePoint()
{
    QColor color;

    color.setRed(ui->whiteRedValue->text().toInt());
    color.setGreen(ui->whiteGreenValue->text().toInt());
    color.setBlue(ui->whiteBlueValue->text().toInt());

    return color;
}

QColor ColorCorrectionWidget::blackPoint()
{
    QColor color;

    color.setRed(ui->blackRedValue->text().toInt());
    color.setGreen(ui->blackGreenValue->text().toInt());
    color.setBlue(ui->blackBlueValue->text().toInt());

    return color;
}

void ColorCorrectionWidget::on_whiteReset_clicked()
{
    setWhitePoint(QColor("white"));
}

void ColorCorrectionWidget::on_blackReset_clicked()
{
    setBlackPoint(QColor("black"));
}


void ColorCorrectionWidget::setWhitePoint(QColor white)
{
    ui->whiteRedValue->setText(QString::number(white.red()));
    ui->whiteGreenValue->setText(QString::number(white.green()));
    ui->whiteBlueValue->setText(QString::number(white.blue()));
    emit parameterChanged();
}

void ColorCorrectionWidget::setBlackPoint(QColor black)
{
    ui->blackRedValue->setText(QString::number(black.red()));
    ui->blackGreenValue->setText(QString::number(black.green()));
    ui->blackBlueValue->setText(QString::number(black.blue()));
    emit parameterChanged();
}

void ColorCorrectionWidget::enableFilter(bool enable)
{
    ui->enable->setChecked(enable);
}

void ColorCorrectionWidget::on_preview_toggled(bool checked)
{
    if (checked) {
        // This does recalculate the output image if necessary and sets the preview Image.
        emit previewChecked();
    } else {
        ui->view->setPixmap(inputPixmap);
    }
}

void ColorCorrectionWidget::imageClicked(QColor color)
{
    if (setWhitePointClicked) {
        setWhitePoint(color);
    }

    if (setBlackPointClicked) {
        setBlackPoint(color);
    }
}

void ColorCorrectionWidget::setBackgroundColor(QColor color)
{
    ui->view->setBackgroundBrush(QBrush(color));
}

// While Setting a point, the text of the button changes.
void ColorCorrectionWidget::on_whiteSetPoint_clicked()
{
    if (setWhitePointClicked) {
        ui->whiteSetPoint->setText(tr("Set"));
        setWhitePointClicked = false;
    } else {
        ui->whiteSetPoint->setText(tr("Done"));
        setWhitePointClicked = true;
    }

    // deaktivate setting black point if necessery
    if (setBlackPointClicked) {
        ui->blackSetPoint->setText(tr("Set"));
        setBlackPointClicked = false;
    }
}



void ColorCorrectionWidget::on_blackSetPoint_clicked()
{
    if (setBlackPointClicked) {
        ui->blackSetPoint->setText(tr("Set"));
        setBlackPointClicked = false;
    } else {
        ui->blackSetPoint->setText(tr("Done"));
        setBlackPointClicked = true;
    }

    // deaktivate setting black point if necessery
    if (setWhitePointClicked) {
        ui->whiteSetPoint->setText(tr("Set"));
        setWhitePointClicked = false;
    }
}

void ColorCorrectionWidget::on_enable_toggled(bool checked)
{
    emit enableFilterToggled(checked);
}
