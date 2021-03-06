#include "Utils.h"
#include <random>

namespace Utils
{
    QColor generateRandomColorForChart()
    {
        std::random_device randomDevice;
        std::mt19937 engine{ randomDevice() };
        std::uniform_real_distribution<double> hueDist{ 0.0, 1.0 };
        const double hue{ hueDist(engine) };
        std::uniform_real_distribution<double> saturationDist{ 0.5, 0.9 };
        const double saturation{ saturationDist(engine) };
        std::uniform_real_distribution<double> lightnessDist{ 0.5, 0.6 };
        const double lightness{ lightnessDist(engine) };
        return QColor::fromHslF(hue, saturation, lightness);
    }

    QString getTimeAsString(int seconds)
    {
        const int hours{ seconds / 3600 };
        seconds /= 3600;
        const int minutes{ seconds / 60 };
        const QString hoursText{ QString::number(hours) + "h" };
        const QString minutesText{ QString::number(minutes) + "m" };
        return hoursText + " " + minutesText;
    }
}
