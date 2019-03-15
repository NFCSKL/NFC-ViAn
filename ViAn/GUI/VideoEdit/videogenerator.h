#ifndef VIDEOGENERATOR_H
#define VIDEOGENERATOR_H

#include <QObject>
#include <QSize>

class QListWidget;
class VideoGenerator : public QObject {
    Q_OBJECT

    QListWidget* m_list = nullptr;

    bool m_aborted = false;
    QString m_name;
    QSize m_size;
    int m_fps;
    bool m_keep_size;
public:
    VideoGenerator(QListWidget* list, QString name, QSize size, int fps, bool keep_size);

public slots:
    void generate_video();
    void abort();

signals:
    void update_progress(int);
    void update_text(QString);
    void update_range(int, int);
    void finished();
};

#endif // VIDEOGENERATOR_H
