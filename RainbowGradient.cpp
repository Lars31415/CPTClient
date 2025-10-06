#include "RainbowGradient.hpp"

#include <QPainter>
#include <cmath>

QColor gradientColor(double hue)
{
    // Convert hue to RGB color using HSV (Hue-Saturation-Value) model
    hue = std::min(1.0, std::max(hue, 0.0));
    hue *= 300 / 360.0;
    return QColor::fromHsvF(hue, 1.0, 1.0);
}
