#ifndef TEMPERATUREGRADIENT_H
#define TEMPERATUREGRADIENT_H

#include <QColor>
#include <vector>

class TemperatureGradient
{
public:
    TemperatureGradient(double temp = 5800);

    QColor getColor(int idx);

private:
    double temperature;
    double maxIntensity;
    std::vector<double> intensities;

    void make_intensity_scale();
};

QColor coldWarm(double v);

#endif // TEMPERATUREGRADIENT_H
