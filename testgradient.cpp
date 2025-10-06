#include "testgradient.h"

#include <QLinearGradient>
#include <QPainter>

TestGradient::TestGradient()
    : image(256, 1, QImage::Format_RGB32)
{
    make_gradient();
}

QColor TestGradient::getColor(int idx)
{
    return image.pixelColor(idx, 0);
}

void TestGradient::make_gradient()
{
    QLinearGradient grad;
    grad.setColorAt(0, QColor(0, 0, 0));
    grad.setColorAt(0.25, QColor(0, 0, 255));
    grad.setColorAt(0.5, QColor(255, 0, 255));
    grad.setColorAt(0.75, QColor(255, 0, 0));
    grad.setColorAt(1, QColor(255, 255, 255));
    grad.setStart(0, 0);
    grad.setFinalStop(image.width(), 0);
    QPainter painter;
    if (painter.begin(&image)) {
        QBrush brush(grad);
        painter.setBrush(brush);
        QPen pen;
        pen.setStyle(Qt::NoPen);
        painter.setPen(pen);
        painter.drawRect(0, 0, image.width(), image.height());
        painter.end();
    }
}
