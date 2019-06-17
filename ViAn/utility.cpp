#include "utility.h"

#include "constants.h"

#include <QDir>
#include <QDebug>
#include <QDirIterator>

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

/**
 * @brief Utility::min_size_ratio
 * @param s1
 * @param s2
 * @return min size ratio
 */
double Utility::min_size_ratio(QSize s1, QSize s2) {
    std::pair<double, double> ratios = size_ratio(s1, s2);
    return std::min(ratios.first, ratios.second);
}

/**
 * @brief Utility::number_of_digits
 * @param n
 * @return number of digits in number
 */
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
QString Utility::zfill(QString number, int length) {
    length -= number.length();
    if (length){
        return QString(length, '0').append(number);
    } else {
        return number;
    }
}

QString Utility::zfill(const int& number, int length){
    length -= QString::number(number).length();
    if (length) {
        return QString(length, '0').append(QString::number(number));
    } else {
        return QString::number(number);
    }
}

/**
 * @brief Utility::name_from_path
 * @param full_path
 * @return with path "C:/Somepath/somefile.ending" returns "somefile.ending"
 */
std::string Utility::name_from_path(const std::string full_path) {
    std::string res = full_path.substr(full_path.find_last_of("/")+1, full_path.size());
    return res;
}

QString Utility::name_from_path(const QString full_path) {
    int name_length = full_path.size() - (full_path.lastIndexOf("/")+1);
    QString res = full_path.right(name_length);
    return res;
}

/**
 * @brief Utility::add_serial_number
 * @param name
 * @param file_end
 * @return
 * Appends (i) to file if i-1 images with same name exist
 */
std::string Utility::add_serial_number(std::string name, std::string file_end) {
    QString qend = QString::fromStdString(file_end);
    QString qname = QString::fromStdString(name);
    QString res = qname+qend;
    int i = 0;
    if(QFile::exists(res)){
        res = qname + QString("(%1)"+ qend).arg(i);
    }
    while(QFile::exists(res) && ++i){
        res = qname + QString("(%1)"+ qend).arg(i);
    }
    return res.toStdString();
}

QString Utility::add_serial_number(QString name, QString file_end) {
    QString res = name + file_end;
    int i = 0;
    if(QFile::exists(res)){
        res = name + QString("(%1)"+ file_end).arg(i);
    }
    while(QFile::exists(res) && ++i){
        res = name + QString("(%1)"+ file_end).arg(i);
    }
    return res;
}

std::string Utility::remove_serial_number(std::string file) {
    std::string file_type = file.substr(file.find_last_of("."), file.size());
    std::string img_name = file.substr(0, file.size() - file_type.size());
    std::string res = img_name;
    if (img_name.back() == ')') {
        res = img_name.substr(0, img_name.find_last_of("("));
    }
    return res + file_type;
}

/**
 * @brief Utility::scale_rect
 * @param rect
 * @param scale_factor
 * @param anchor
 * @return scaled cv::Rect
 */
cv::Rect Utility::scale_rect(cv::Rect rect, double scale_factor, cv::Point anchor) {
    cv::Point start(rect.tl().x/scale_factor - anchor.x/scale_factor, rect.tl().y/scale_factor - anchor.y/scale_factor);
    cv::Point end(rect.br().x/scale_factor - anchor.x/scale_factor, rect.br().y/scale_factor - anchor.y/scale_factor);
    cv::Rect res = cv::Rect(start, end);
    return res;
}

/**
 * @brief Utility::scale_rect
 * @param rect
 * @param scale_factor
 * @param anchor
 * @return scaled qrect
 */
QRect Utility::scale_rect(QRect rect, double scale_factor, QPoint anchor){
    return from_cvrect(scale_rect(from_qrect(rect), scale_factor, from_qpoint(anchor)));
}

/**
 * @brief Utility::from_cvpoint
 * @param point
 * @return QPoint from cv::Point
 */
QPoint Utility::from_cvpoint(cv::Point point) {
    return QPoint(point.x,point.y);
}

/**
 * @brief Utility::from_qpoint
 * @param point
 * @return cv::Point from Qpoint
 */
cv::Point Utility::from_qpoint(QPoint point) {
    return cv::Point(point.x(),point.y());
}

/**
 * @brief Utility::from_qrect
 * @param rect
 * @return cv::Rect from QRect
 */
cv::Rect Utility::from_qrect(QRect rect) {
    return cv::Rect(from_qpoint(rect.topLeft()), from_qpoint(rect.bottomRight()));
}

/**
 * @brief Utility::from_cvrect
 * @param rect
 * @return QRect from cv::Rect
 */
QRect Utility::from_cvrect(cv::Rect rect) {
    return QRect(from_cvpoint(rect.tl()), from_cvpoint(rect.br()));
}

QByteArray Utility::checksum(const QString &file, QCryptographicHash::Algorithm hash_algorithm) {
    QFile f(file);
    if (f.open(QFile::ReadOnly)) {
        QCryptographicHash hash(hash_algorithm);
        if (hash.addData(&f)) {
            return hash.result().toHex();
        }
        f.close();
    }
    return QByteArray().toHex();
}

/**
 * @brief Utility::remove_checksum_file
 * Iterates through the given directory and hashes each file.
 * When a hash is found that matched checksum the corresponding file is removed and the iteration aborted.
 * @param parent_folder :   Directory/path to search in
 * @param checksum      :   Hash to search for
 * @return
 */
bool Utility::remove_checksum_files(const QString& parent_folder, const QStringList& hashes) {
    bool removed{false};
    QDirIterator it(parent_folder);
    while (it.hasNext()) {
        auto path = it.next();
        auto current_hash = checksum(path);
        for (auto hash : hashes) {
            if (current_hash == hash) {
                QFile(path).remove();
                break;
            }
        }
//        if (checksum(path) == hash) {
//            QFile f(path);
//            removed = f.remove();
//            break;
//        }
    }
    return removed;
}

/**
 * @brief Utility::rotate
 * Rotate the given point by the given rotation
 * @param pos           : Point to be rotated
 * @param rotation      : Wanted rotation
 * @param width         : Width of frame
 * @param height        : Height of frame
 * @return              : Rotated point
 */
QPoint Utility::rotate(QPoint pos, int rotation, int width, int height) {
    double angle = rotation*Constants::DEGREE_TO_RADIAN_FACTOR;

    // Translate by negative pivot of old frame size
    double translated_x{pos.x() - static_cast<double>(width) / 2};
    double translated_y{pos.y() - static_cast<double>(height) / 2};

    // Rotate around pivot
    double rotated_x{translated_x * std::cos(angle) - translated_y * std::sin(angle)};
    double rotated_y{translated_x * std::sin(angle) + translated_y * std::cos(angle)};

    if (rotation == 90 || rotation == 270) {
        std::swap(width,height);
    }
    // Translate back using the new frame size
    translated_x = rotated_x + width / 2;
    translated_y = rotated_y + height / 2;

    return QPoint(translated_x, translated_y);
}

QPoint Utility::flip(QPoint pos, bool flip_h, bool flip_v, int width, int height) {
    // 0 - x horizontal, 1 - y, -1 - both
    if (flip_h) {
        pos = QPoint(pos.x(), height - pos.y());
    }
    if (flip_v) {
        pos = QPoint(width - pos.x(), pos.y());
    }
    return pos;
}
