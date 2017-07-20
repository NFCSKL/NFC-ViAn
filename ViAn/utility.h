#ifndef UTILITY_H
#define UTILITY_H

#include <QSize>


class Utility
{
public:
    Utility();
    static std::pair<double, double> size_ratio(QSize s1, QSize s2);
    static double min_size_ratio(QSize s1, QSize s2);
    static int number_of_digits(int n);
    static std::string zfill(std::string number, int length);  
};

#endif // UTILITY_H

