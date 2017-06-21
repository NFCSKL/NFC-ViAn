#ifndef ANALYSISSLIDER_H
#define ANALYSISSLIDER_H

#include <QWidget>
#include <QSlider>
#include <vector>


class AnalysisSlider : public QSlider {

public:
    explicit AnalysisSlider(Qt::Orientation orientation, QWidget *parent = 0);
    void add_slider_interval(int start_frame, int end_frame);
    void clear_rects();

protected:
    void paintEvent(QPaintEvent *ev);

private:
    std::vector<std::pair<double, double>> rects;
};

#endif // ANALYSISSLIDER_H
