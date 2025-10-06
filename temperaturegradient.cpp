#include "temperaturegradient.h"

#include <cmath>

TemperatureGradient::TemperatureGradient(double temp)
    : temperature(temp)
{
    make_intensity_scale();
}

void TemperatureGradient::make_intensity_scale()
{
    const double h = 6.626e-34;
    const double c = 3.0e8;
    const double k = 1.381e-23;

    maxIntensity = 0;
    for (int i = 0; i < 256; ++i) {
        double wavelength = 380e-9 + i / 255.0 * (780e-9 - 380e-9);
        double numerator = 2.0 * h * c * c;
        double denominator = std::pow(wavelength, 5)
                             * (std::exp((h * c) / (wavelength * k * temperature)) - 1.0);
        double intensity = numerator / denominator;
        intensities.push_back(intensity);
        maxIntensity = std::max(intensity, maxIntensity);
    }
}

QColor TemperatureGradient::getColor(int idx)
{
    double gamma = 0.8;
    double factor;
    double R, G, B;
    double wavelength = 380 + idx / 255.0 * (780 - 380);

    if ((wavelength >= 380) && (wavelength < 440)) {
        R = -(wavelength - 440) / (440 - 380);
        G = 0.0;
        B = 1.0;
    } else if ((wavelength >= 440) && (wavelength < 490)) {
        R = 0.0;
        G = (wavelength - 440) / (490 - 440);
        B = 1.0;
    } else if ((wavelength >= 490) && (wavelength < 510)) {
        R = 0.0;
        G = 1.0;
        B = -(wavelength - 510) / (510 - 490);
    } else if ((wavelength >= 510) && (wavelength < 580)) {
        R = (wavelength - 510) / (580 - 510);
        G = 1.0;
        B = 0.0;
    } else if ((wavelength >= 580) && (wavelength < 645)) {
        R = 1.0;
        G = -(wavelength - 645) / (645 - 580);
        B = 0.0;
    } else if ((wavelength >= 645) && (wavelength <= 780)) {
        R = 1.0;
        G = 0.0;
        B = 0.0;
    } else {
        R = 0.0;
        G = 0.0;
        B = 0.0;
    }

    // Intensity adjustment
    if ((wavelength >= 380) && (wavelength < 420)) {
        factor = 0.3 + 0.7 * (wavelength - 380) / (420 - 380);
    } else if ((wavelength >= 420) && (wavelength < 645)) {
        factor = 1.0;
    } else if ((wavelength >= 645) && (wavelength <= 780)) {
        factor = 0.3 + 0.7 * (780 - wavelength) / (780 - 645);
    } else {
        factor = 0.0;
    }

    R = pow(R * factor, gamma) * 255;
    G = pow(G * factor, gamma) * 255;
    B = pow(B * factor, gamma) * 255;

    // Scale intensity based on max intensity
    double intensityRatio = intensities[idx] / maxIntensity;
    R *= intensityRatio;
    G *= intensityRatio;
    B *= intensityRatio;

    return QColor(static_cast<int>(R), static_cast<int>(G), static_cast<int>(B));
}

QColor coldWarm(double v)
{
    double r = 255 * sin(M_PI * v / 2);
    double b = 255 * std::cos(M_PI * v / 2);
    return QColor(r, 0, b);
}
