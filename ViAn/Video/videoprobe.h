#ifndef VIDEOPROBE_H
#define VIDEOPROBE_H

#include <QObject>

class VideoProbe : public QObject  {
    Q_OBJECT

private:
    std::vector<std::string> m_video_paths;

public:
    explicit VideoProbe(QObject *parent = nullptr);

    void set_paths(const std::vector<std::string>& video_paths);

signals:
    void finished();
    void current_video(std::string);
    void current_frame(const int& frame);
    void probe_info(std::string video_path, int frames, int time);

public slots:
    void probe();
};

#endif // VIDEOPROBE_H
