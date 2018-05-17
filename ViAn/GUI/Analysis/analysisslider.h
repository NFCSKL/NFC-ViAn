#ifndef ANALYSISSLIDER_H
#define ANALYSISSLIDER_H

#include <QWidget>
#include <QSlider>
#include <vector>
#include <set>
#include "Project/Analysis/analysisproxy.h"
#include "Project/Analysis/tag.h"
/**
 * @brief The AnalysisSlider class
 * This class represents a conventional slider
 * With additions for displaying intervals of interest
 * and chossing intervals
 */
class AnalysisSlider : public QSlider {
    Q_OBJECT
    // Control variables
    bool m_blocked = false;
    bool m_was_paused = false;
    bool m_show_pois = false;
    bool m_show_tags = false;
    bool show_on_slider = true;
    bool show_interval = true;
    bool show_ana_interval = false;

    //Change this to set how many frames the POI buttons should ignore
    const int JUMP_INTERVAL = 0;

public:
    explicit AnalysisSlider(Qt::Orientation orientation, QWidget *parent = 0);

    int last_poi_end = -1;

    Analysis* m_analysis = nullptr;

    std::pair<int, int> m_interval = std::make_pair(-1, -1);
    std::pair<int, int> m_ana_interval = std::make_pair(-1, -1);

    // Control functions
    void set_blocked(bool value);
    bool is_blocked();
    bool get_was_paused();
    void set_was_paused(bool value);

    // Interval settings
    void add_slider_interval(int start_frame, int end_frame);
    int set_interval_first();
    int set_interval_second();

    // Drawing interval functions
    void draw_interval(std::pair<int, int> interval, QRect groove, double frame_width);
    bool valid_interval(std::pair<int, int> interval);

    // POI functions
    bool is_in_POI(int frame);
    int get_next_poi_start(int curr_frame);
    int get_next_poi_end(int frame);
    int get_prev_poi_start(int curr_frame);



protected:
    void paintEvent(QPaintEvent *ev);
public slots:

    // Set analysis to be used when displaying slider pois
    void set_basic_analysis(BasicAnalysis *analysis);

    // Wrapped repaint
    void update();

    // Interval slots
    void set_interval(int start, int end);
    void clear_interval(void);

    // Control slots
    void set_show_pois(bool);
    void set_show_tags(bool);
    void set_show_on_slider(bool);
    void set_show_interval(bool);
    void set_show_ana_interval(bool);

    // Remove all displayed intervals of interest
    void clear_slider();

private:
    std::vector<int> frames;
    std::vector<std::pair<int, int>> rects;
    std::vector<QRect> interval_rects;
};

#endif // ANALYSISSLIDER_H
