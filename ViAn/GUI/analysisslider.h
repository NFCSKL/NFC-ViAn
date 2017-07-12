#ifndef ANALYSISSLIDER_H
#define ANALYSISSLIDER_H

#include <QWidget>
#include <QSlider>
#include <vector>
#include <set>
#include "Project/Analysis/analysis.h"


class AnalysisSlider : public QSlider {
    Q_OBJECT
    bool m_blocked = false;
    bool m_was_paused = false;
    bool m_show_pois = false;
    bool m_show_tags = false;

public:
    explicit AnalysisSlider(Qt::Orientation orientation, QWidget *parent = 0);

    int last_poi_end = -1;

    void set_blocked(bool value);
    bool is_blocked();
    bool get_was_paused();
    void set_was_paused(bool value);
    void add_slider_interval(int start_frame, int end_frame);
    int get_next_poi_start(int curr_frame);
    int get_next_poi_end(int frame);
    int get_prev_poi_start(int curr_frame);
    bool is_in_POI(int frame);
protected:
    void paintEvent(QPaintEvent *ev);
public slots:
    void set_analysis(Analysis* analysis);
    void set_show_pois(bool);
    void set_show_tags(bool);
    void clear_slider();

private:
    std::set<int> frames;
    std::vector<std::pair<int, int>> rects;
};

#endif // ANALYSISSLIDER_H
