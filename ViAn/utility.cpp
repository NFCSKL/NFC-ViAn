#include "utility.h"

Utility::Utility(){
}

/**
 * @brief Utility::size_ratio
 * calculates and returns the height and width ratios between the two QSize objects
 * @param s1
 * @param s2
 * @return
 */
std::pair<double, double> Utility::size_ratio(QSize s1, QSize s2) {
    return std::make_pair(double(s1.width()) / s2.width(), double(s1.height()) / s2.height());
}

double Utility::min_size_ratio(QSize s1, QSize s2) {
    std::pair<double, double> ratios = size_ratio(s1, s2);
    return std::min(ratios.first, ratios.second);
}

int Utility::number_of_digits(int n) {
    int number_of_digits = 0;
    do {
         ++number_of_digits;
         n /= 10;
    } while (n);
    return number_of_digits;
}

/**
 * @brief Utility::zfill
 * Fills the given number string with zeros
 * @param number    :    number to be zero padded
 * @param length    :   specifies how long the return string should be
 * @return
 */
std::string Utility::zfill(std::string number, int length){
    length -= number.length();
    if (length)
        return std::string(length, '0').append(number);
    else
        return number;
}
