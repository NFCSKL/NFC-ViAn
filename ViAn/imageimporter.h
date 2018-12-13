#ifndef IMAGEIMPORTER_H
#define IMAGEIMPORTER_H

#include <QObject>

class ImageImporter : public QObject
{
    Q_OBJECT
    QStringList m_images{};
    QString m_dest{};
    int m_type;
    bool m_abort{false};
public:
    explicit ImageImporter(const QStringList& images, const QString& dest, const int &sequence_type, QObject *parent = nullptr);

public slots:
    void import_images();
    void abort();
signals:
    void finished();
    void imported_sequence(QStringList images, QStringList checksums, QString path, int seq_type);
    void update_progress(int progress);
};

#endif // IMAGEIMPORTER_H
