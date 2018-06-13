#include "imageimporter.h"
#include <QDebug>

ImageImporter::ImageImporter(const QStringList& images, const QString& dest, QObject *parent) :
    m_images(images), m_dest(dest), QObject(parent) {
}

void ImageImporter::import_images() {
    int num_images = m_images.size();
    int num_digits = Utility::number_of_digits(num_images);
    QDir().mkpath(m_dest);

    for (int i = 0; i < m_images.size(); ++i) {
        QFileInfo file_info(m_images[i]);
        QString padded_num = QString::fromStdString(Utility::zfill(std::to_string(i), num_digits));
        qDebug() << m_images[i] << " -> " << m_dest + "/" + padded_num;
        QFile().copy(m_images[i], m_dest + "/" + padded_num);

        emit update_progress(i + 1);
        if (m_abort) {
            break;
        }
    }

    if (!m_abort) {
        emit imported_sequence(m_images, m_dest.toStdString());
    }
    emit finished();
}

void ImageImporter::abort() {
    m_abort = true;
}
