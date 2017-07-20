#ifndef ANALYSISSLIDER_H
#define ANALYSISSLIDER_H

#include <QWidget>
#include <QSlider>
#include <vector>
#include <set>
#include "Project/Analysis/basicanalysis.h"
#include "Project/Analysis/tag.h"


class AnalysisSlider : public QSlider {
    Q_OBJECT
    bool m_blocked = false;
    bool m_was_paused = false;
    bool m_show_pois = false;
    bool m_show_tags = false;
    bool show_on_slider = true;

    //Change this to set how many frames the POI buttons should ignore
    const int JUMP_INTERVAL = 0;

public:
    explicit AnalysisSlider(Qt::Orientation orientation, QWidget *parent = 0);

    int last_poi_end = -1;
    int interval = -1;

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
    void set_basic_analysis(BasicAnalysis *analysis);

    void update();

    void set_interval(int frame);
    void set_show_pois(bool);
    void set_show_tags(bool);
    void set_show_on_slider(bool);
    void clear_slider();

private:
    std::vector<int> frames;
    std::vector<std::pair<int, int>> rects;
};

#endif // ANALYSISSLIDER_H
