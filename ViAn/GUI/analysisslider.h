#ifndef ANALYSISSLIDER_H
#define ANALYSISSLIDER_H

#include <QWidget>
#include <QSlider>
#include <vector>


class AnalysisSlider : public QSlider {
    bool m_blocked = false;
    bool m_was_paused = false;
public:
    explicit AnalysisSlider(Qt::Orientation orientation, QWidget *parent = 0);
    void add_slider_interval(int start_frame, int end_frame);
    void clear_slider();
    void set_blocked(bool value);
    bool is_blocked();
    bool get_was_paused();
    void set_was_paused(bool value);

protected:
    void paintEvent(QPaintEvent *ev);

private:
    std::vector<std::pair<double, double>> rects;
};

#endif // ANALYSISSLIDER_H
