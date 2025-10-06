#ifndef TESTGRADIENT_H
#define TESTGRADIENT_H

#include <QImage>

class TestGradient
{
public:
    TestGradient();
    QColor getColor(int idx);

private:
    QImage image;

    void make_gradient();
};

#endif // TESTGRADIENT_H
