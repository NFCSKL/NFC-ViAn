#ifndef ANALYSISSLIDER_H
#define ANALYSISSLIDER_H

#include <QSlider>

class Analysis;
class AnalysisProxy;
class BasicAnalysis;
class Interval;
class Tag;

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
    bool m_show_interval_areas = false;
    bool show_on_slider = true;
    bool show_interval = true;
    bool show_ana_interval = false;
    bool details_checked = false;

public:
    explicit AnalysisSlider(Qt::Orientation orientation, QWidget *parent = nullptr);

    Analysis* m_analysis = nullptr;
    Tag* m_tag = nullptr;
    Interval* m_interval_areas = nullptr;

    std::pair<int, int> m_interval = std::make_pair(-1, -1);
    std::pair<int, int> m_ana_interval = std::make_pair(-1, -1);

    // Control functions
    void set_blocked(bool value);
    bool is_blocked();
    bool get_was_paused();
    void set_was_paused(bool value);
    bool get_show_tags();

    // Interval settings
    int set_interval_first();
    int set_interval_second();
    void clear_interval(void);

    // Drawing interval functions
    void draw_interval(std::pair<int, int> interval, QRect groove, double frame_width);
    bool valid_interval(std::pair<int, int> interval);

    // POI functions
    bool is_in_POI(int frame);
    int get_next_poi_start(int curr_frame);
    bool is_poi_start(int frame);
    bool is_poi_end(int frame);
    int get_prev_poi_start(int curr_frame);
    int get_prev_poi_end(int curr_frame);
    int get_closest_poi(int curr_frame);
    int get_last_poi_end();

protected:
    void paintEvent(QPaintEvent *ev);
public slots:

    // Set analysis to be used when displaying slider pois
    void set_basic_analysis(BasicAnalysis *analysis);
    void set_analysis_proxy(AnalysisProxy* analysis);

    // Wrapped repaint
    void update();

    // Control slots
    void set_show_pois(bool);
    void set_show_tags(bool);
    void set_show_interval_areas(bool);
    void set_show_on_slider(bool);
    void set_show_interval(bool);
    void set_show_ana_interval(bool);
    void set_details_checked(bool);

    // Remove all displayed intervals of interest
    void clear_slider();

private:
    std::vector<std::pair<int, int>> rects; // Analysis poi
    std::vector<QRect> interval_rects;  // Intervals
};

#endif // ANALYSISSLIDER_H
