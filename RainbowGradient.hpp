#ifndef RAINBOWGRADIENT_HPP
#define RAINBOWGRADIENT_HPP

#include <QPixmap>
#include <QColor>

QColor gradientColor(double hue);
QPixmap createRainbowGradient(int width, int height);

#endif // RAINBOWGRADIENT_HPP
