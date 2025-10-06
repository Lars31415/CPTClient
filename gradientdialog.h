#ifndef GRADIENTDIALOG_H
#define GRADIENTDIALOG_H

#include <QDialog>
#include "temperaturegradient.h"
#include "testgradient.h"

namespace Ui {
class GradientDialog;
}

class GradientDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GradientDialog(QWidget *parent = nullptr);
    ~GradientDialog();

    void setPalet(int idx);
    int palet() const;

private:
    Ui::GradientDialog *ui;
    TemperatureGradient tgradient;
    TestGradient tstgradient;

    void timer_1();
};

#endif // GRADIENTDIALOG_H
