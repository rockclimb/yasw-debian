#ifndef SCALINGWIDGET_H
#define SCALINGWIDGET_H

#include <QWidget>

namespace Ui {
class ScalingWidget;
}

class ScalingWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit ScalingWidget(QWidget *parent = 0);
    ~ScalingWidget();
    
private:
    Ui::ScalingWidget *ui;
};

#endif // SCALINGWIDGET_H
