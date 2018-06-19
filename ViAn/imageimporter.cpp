#include "imageimporter.h"
#include <QDebug>

ImageImporter::ImageImporter(const QStringList& images, const QString& dest, QObject *parent) :
    m_images(images), m_dest(dest), QObject(parent) {
}

void ImageImporter::import_images() {
    int num_images = m_images.size();
    int num_digits = Utility::number_of_digits(num_images);

    if (QDir().mkpath(m_dest)) {
        for (int i = 0; i < m_images.size(); ++i) {
            QFileInfo file_info(m_images[i]);
            QString padded_num = QString::fromStdString(Utility::zfill(std::to_string(i), num_digits));

            bool copied = QFile().copy(m_images[i], m_dest + "/" + padded_num);
            if (!copied)
                m_images.erase(m_images.begin() + i);

            emit update_progress(i + 1);
            if (m_abort) {
                break;
            }
        }
    } else {
        qWarning() << "Could not create destination path";
        m_abort = true;
    }

    if (!m_abort)
        emit imported_sequence(m_images, m_dest.toStdString());
    emit finished();
}

void ImageImporter::abort() {
    m_abort = true;
}
