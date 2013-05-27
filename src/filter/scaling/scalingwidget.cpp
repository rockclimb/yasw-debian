#include "scalingwidget.h"
#include "ui_scalingwidget.h"

ScalingWidget::ScalingWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ScalingWidget)
{
    ui->setupUi(this);
}

ScalingWidget::~ScalingWidget()
{
    delete ui;
}
