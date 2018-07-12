#include "imageimporter.h"
#include <QCoreApplication>
#include <QDebug>

ImageImporter::ImageImporter(const QStringList& images, const QString& dest, QObject *parent) :
    m_images(images),
    m_dest(QString::fromStdString(Utility::add_serial_number(dest.toStdString(), ""))),
    QObject(parent) {}

/**
 * @brief ImageImporter::import_images
 * Copies all images given from the m_images paths to the m_dest destination.
 * Each copied image is named with a continous zero padded index so it can be played by
 * OpenCV.
 *
 * The following signals are emitted during work:
 * update_progress:    single int indicating the progress of the thread
 * imported_sequence:  all original image paths and the destination path
 * finished:           empty signal upon completion
 */
void ImageImporter::import_images() {
    int num_images = m_images.size();
    int num_digits = Utility::number_of_digits(num_images);

    QStringList checksums;

    if (QDir().mkpath(m_dest)) {
        for (int i = 0; i < m_images.size(); ++i) {
            QFileInfo file_info(m_images[i]);
            QString padded_num = QString::fromStdString(Utility::zfill(std::to_string(i), num_digits));
            QString new_path = m_dest + "/" + padded_num;

            bool copied = QFile().copy(m_images[i], new_path);
            if (!copied)
                m_images.erase(m_images.begin() + i);
            else
                checksums.append(Utility::checksum(new_path));

            emit update_progress(i + 1);
            QCoreApplication::processEvents(); // Process thread event loop. Needed for abort flag to update
            if (m_abort) {
                break;
            }
        }
    } else {
        qWarning() << "Could not create destination path";
        m_abort = true;
    }

    if (!m_abort)
        emit imported_sequence(m_images, checksums, m_dest.toStdString());
    emit update_progress(m_images.size());
    emit finished();
}

void ImageImporter::abort() {
    m_abort = true;
}
