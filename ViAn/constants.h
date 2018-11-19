#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <cmath>

class Singleton {
private:
    Singleton();

public:
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    static Singleton* get_instance();

    // Constants
    int LINE_THICKNESS = 10;
};

namespace Constants {
    const double DEGREE_TO_RADIAN_FACTOR = M_PI / 180;
}


#endif // CONSTANTS_H
