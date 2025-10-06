#include "gradientdialog.h"
#include "RainbowGradient.hpp"
#include "ui_gradientdialog.h"

#include <QPainter>
#include <QTimer>
#include <QtMath>

GradientDialog::GradientDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::GradientDialog)
{
    ui->setupUi(this);
    QTimer::singleShot(100, this, &GradientDialog::timer_1);
}

GradientDialog::~GradientDialog()
{
    delete ui;
}

void GradientDialog::timer_1()
{
    QPixmap map1(ui->label_rainbow->size());
    map1.fill(QColor(200, 200, 200));
    QPixmap map2(ui->label_temp->size());
    map2.fill(QColor(200, 200, 200));
    QPixmap map3(ui->label_cw->size());
    map3.fill(QColor(200, 200, 200));
    QPixmap map4(ui->label_test->size());
    map4.fill(QColor(200, 200, 200));

    QPainter painter;
    if (painter.begin(&map1)) {
        for (int i = 0; i < ui->label_rainbow->width(); ++i) {
            QColor c = gradientColor(int(double(i) / ui->label_rainbow->width() * 255) / 255.0);
            painter.setPen(QPen(c));
            painter.drawLine(i, 0, i, ui->label_rainbow->height());
        }
        painter.end();
    }
    ui->label_rainbow->setPixmap(map1);

    if (painter.begin(&map2)) {
        for (int i = 0; i < ui->label_rainbow->width(); ++i) {
            QColor c = tgradient.getColor(255 - int(double(i) / ui->label_rainbow->width() * 255));
            painter.setPen(QPen(c));
            painter.drawLine(i, 0, i, ui->label_rainbow->height());
        }
        painter.end();
    }
    ui->label_temp->setPixmap(map2);

    if (painter.begin(&map3)) {
        for (int i = 0; i < ui->label_cw->width(); ++i) {
            double v = double(i) / ui->label_cw->width();
            painter.setPen(QPen(coldWarm(v)));
            painter.drawLine(i, 0, i, ui->label_cw->height());
        }
        painter.end();
    }
    ui->label_cw->setPixmap(map3);

    if (painter.begin(&map4)) {
        for (int i = 0; i < ui->label_test->width(); ++i) {
            int v = int(double(i) / ui->label_test->width() * 255);
            painter.setPen(QPen(tstgradient.getColor(v)));
            painter.drawLine(i, 0, i, ui->label_test->height());
        }
        painter.end();
    }
    ui->label_test->setPixmap(map4);

    // QLinearGradient grad;
    // grad.setColorAt(0, QColor(0, 0, 0));
    // grad.setColorAt(0.25, QColor(0, 0, 255));
    // grad.setColorAt(0.5, QColor(255, 0, 255));
    // grad.setColorAt(0.75, QColor(255, 0, 0));
    // grad.setColorAt(1, QColor(255, 255, 255));
    // grad.setStart(0, 0);
    // grad.setFinalStop(ui->label_test->width(), ui->label_test->height());
    // if (painter.begin(&map4)) {
    //     QBrush brush(grad);
    //     painter.setBrush(brush);
    //     QPen pen;
    //     pen.setStyle(Qt::NoPen);
    //     painter.setPen(pen);
    //     painter.drawRect(0, 0, ui->label_test->width(), ui->label_test->height());
    //     painter.end();
    // }
    // ui->label_test->setPixmap(map4);
}

void GradientDialog::setPalet(int idx)
{
    switch (idx) {
    case 1:
        ui->radioButtonTemp->setChecked(true);
        break;
    case 2:
        ui->radioButtonCw->setChecked(true);
        break;
    case 3:
        ui->radioButtonTest->setChecked(true);
        break;
    default:
        ui->radioButtonRainbow->setChecked(true);
    }
}

int GradientDialog::palet() const
{
    if (ui->radioButtonTemp->isChecked())
        return 1;
    if (ui->radioButtonCw->isChecked())
        return 2;
    if (ui->radioButtonTest->isChecked())
        return 3;
    return 0;
}
