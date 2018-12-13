#ifndef IMAGEEXPORTER_H
#define IMAGEEXPORTER_H

#include <QObject>

class ImageExporter : public QObject {
    Q_OBJECT
    // If either first or second is -1
    // it means that the entered interval is bad.
    std::pair<int, int> m_export_interval;
    QString m_file_path, m_export_path;
    bool m_aborted = false;
public:
    ImageExporter(const std::pair<int, int>& interval, const QString& file_path, const QString& export_path);
    ImageExporter();

    void set_interval(const std::pair<int, int>& interval);
    void set_file_path(const QString& file_path);
    void set_export_path(const QString& export_path);

    std::pair<int, int> get_interval();
public slots:
    void export_images();
    void abort();
signals:
    void finished();
    void finished_msg(QString msg);
    void update_progress(int progress);
};

#endif // IMAGEEXPORTER_H
