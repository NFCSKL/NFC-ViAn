#include "analysisslider.h"
#include <QSlider>
#include <QBrush>
#include <QPaintEvent>
#include <QStylePainter>
#include <QStyleOptionSlider>
#include <QDebug>


AnalysisSlider::AnalysisSlider(Qt::Orientation orientation, QWidget * parent) : QSlider(parent) {
    setOrientation(orientation);
    setPageStep(10);
}

/**
 * @brief AnalysisSlider::paintEvent
 * New paint event for the slider that will also
 * draw the rectangle areas on the slider for showing POIs.
 * @param ev
 */
void AnalysisSlider::paintEvent(QPaintEvent *ev) {
    QStyleOptionSlider option;
    initStyleOption(&option);
    QStylePainter painter(this);
    QSlider::paintEvent(ev);

    option.subControls = QStyle::SC_SliderGroove;
    painter.drawComplexControl(QStyle::CC_Slider, option);
    QRect groove_rect = style()->subControlRect(QStyle::CC_Slider, &option, QStyle::SC_SliderGroove, this);

    //Get one frame's width of the slider
    double c = (double)(groove_rect.right()-groove_rect.left())/maximum();

    QBrush brush = Qt::yellow;

    // Draws the detections on slider and the tags
    if ((m_show_pois||m_show_tags)&& show_on_slider) {
        if(m_show_tags) brush = Qt::red;

        for (auto it = rects.begin(); it != rects.end(); ++it) {
            double first_frame = (double)(*it).first;
            double second_frame = (double)(*it).second;

            //Walk first/second_frame number of frames in on the slider
            double first = (groove_rect.left()+first_frame*c);
            double second = (groove_rect.left()+second_frame*c);

            //Draw the rects, +1 so it's not too small
            //QRect(int x, int y, int width, int height)
            QRect rect(first, groove_rect.top(), 1+second-first, groove_rect.height());
            painter.fillRect(rect, brush);
        }
    }

    // Draws the analysis interval
    if (show_ana_interval) {
        brush = Qt::darkMagenta;
        draw_interval(m_ana_interval, groove_rect, c);
        for (auto rect : interval_rects) {
            painter.fillRect(rect, brush);
        }
    }
    // Draws the interval
    if (show_interval) {
        brush = Qt::black;
        draw_interval(m_interval, groove_rect, c);
        for (auto rect : interval_rects) {
            painter.fillRect(rect, brush);
        }
    }
    option.subControls = QStyle::SC_SliderHandle;
    painter.drawComplexControl(QStyle::CC_Slider, option);
}

/**
 * @brief AnalysisSlider::draw_interval
 * Draw
 * @param interval
 */
void AnalysisSlider::draw_interval(std::pair<int, int> interval, QRect groove, double frame_width) {
    double first = (groove.left()+(double)interval.first*frame_width);
    double second = (groove.left()+(double)interval.second*frame_width);
    interval_rects.clear();

    // Draw the interval
    if (valid_interval(interval)) {
        QRect rect(first, groove.top()+groove.height()/3, 1+second-first, groove.height()/3);
        interval_rects.push_back(rect);
    }
    // Draw the interval start marker
    if (interval.first != -1) {
        QRect rect(first, groove.top(), 1, groove.height());
        interval_rects.push_back(rect);
    }
    // Draw the interval end marker
    if (interval.second != -1) {
        QRect rect(second, groove.top(), 1, groove.height());
        interval_rects.push_back(rect);
    }
}

/**
 * @brief AnalysisSlider::valid_interval
 * @param interval
 * @return true if interval is a valid interval
 */
bool AnalysisSlider::valid_interval(std::pair<int, int> interval) {
    // Checks if both ends of the interval is set and the end is larger than the start
    return interval.first != -1 && interval.second != -1 && interval.first <= interval.second;
}

void AnalysisSlider::update(){
    repaint();
}

/**
 * @brief AnalysisSlider::set_basic_analysis
 * Set tag
 * Adds all POIs to the slider
 * @param analysis
 */
void AnalysisSlider::set_basic_analysis(BasicAnalysis* analysis) {
    rects.clear();
    if (analysis != nullptr) {
        qDebug() << "set b_analysis" << analysis->get_ana_interval();
        m_ana_interval = std::make_pair(analysis->get_ana_interval().first, analysis->get_ana_interval().second);

//        for (auto p : analysis->get_intervals()) {
//            add_slider_interval(p->get_start(), p->get_end());
//        }

        if (analysis->get_type() == TAG || analysis->get_type() == DRAWING_TAG) {
            for (auto p : analysis->get_intervals()) {
                add_slider_interval(p->get_start(), p->get_end());
            }
        } else {
            qDebug() << "Rects size" << analysis->m_slider_interval.size();
            rects = analysis->m_slider_interval;
        }
        for (auto rect : rects) {
            qDebug() << rect;
        }
    }
    repaint();
}

/**
 * @brief AnalysisSlider::set_show_ana_interval
 * @param show
 */
void AnalysisSlider::set_show_ana_interval(bool show) {
    show_ana_interval = show;
    repaint();
}

/**
 * @brief AnalysisSlider::set_interval
 * @param start
 * @param end
 */
void AnalysisSlider::set_interval(int start, int end) {
    m_interval = std::make_pair(start, end);
}

/**
 * @brief AnalysisSlider::clear_interval
 */
void AnalysisSlider::clear_interval() {
    m_interval.first = -1;
    m_interval.second = -1;
}

/**
 * @brief AnalysisSlider::set_interval_first
 * @return
 */
int AnalysisSlider::set_interval_first() {
    m_interval.first = value();
    return m_interval.first;
}

/**
 * @brief AnalysisSlider::set_interval_second
 * @return
 */
int AnalysisSlider::set_interval_second() {
    m_interval.second = value();
    return m_interval.second;
}

/**
 * @brief AnalysisSlider::add_slider_rect
 * Adds a pair of doubles to a pair vector. This pair represents
 * a certain area of the slider from start to end.
 * @param start frame
 * @param end frame
 */
void AnalysisSlider::add_slider_interval(int start_frame, int end_frame) {
    std::pair<int, int> pair;
    if (start_frame > end_frame) {
        pair = std::make_pair(end_frame, start_frame);
    } else {
        pair = std::make_pair(start_frame, end_frame);
    }
    rects.push_back(pair);
    qDebug() << "pair" << pair;
    last_poi_end = end_frame;
}

/**
 * @brief AnalysisSlider::get_next_poi_start
 * Return the start frame of the POI after frame
 * @param frame     : current frame
 * @return
 */
int AnalysisSlider::get_next_poi_start(int curr_frame) {
    if (!rects.empty()) {
        for (std::pair<int, int> rect : rects) {
            if ( rect.first > curr_frame) {
                return rect.first;
            }
        }
    } else if (!frames.empty()) {
        int edge_frame = curr_frame;
        for (int frame : frames) {
            if (frame > edge_frame+JUMP_INTERVAL+1) {
                return frame;
            } else if (frame > edge_frame+JUMP_INTERVAL){
                edge_frame = frame;
            }
        }
    }
    return curr_frame;
}

/**
 * @brief AnalysisSlider::get_next_poi_end
 * Return the end frame of the POI after frame
 * @param frame     : current frame
 * @return
 */
int AnalysisSlider::get_next_poi_end(int frame) {
    for (std::pair<int, int> rect : rects) {
        if ( rect.first > frame) {
            return rect.second;
        }
    }
    return frame;
}

/**
 * @brief AnalysisSlider::get_prev_poi_start
 * Return the start frame of the POI before frame
 * @param frame     : current frame
 * @return
 */
int AnalysisSlider::get_prev_poi_start(int curr_frame) {
    int new_frame = curr_frame;
    if (!rects.empty()) {
        for (std::pair<int, int> rect : rects) {
            if ( rect.second >= curr_frame) {
                break;
            } else new_frame = rect.first;
        }
    } else if (!frames.empty()) {
        int edge_frame = curr_frame;
        for (int i = frames.size()-1; i >= 0; i--) {
            int frame = frames[i];
            if (frame < edge_frame-JUMP_INTERVAL-1) {
                return frame;
            } else if (frame < edge_frame-JUMP_INTERVAL){
                edge_frame = frame;
            }
        }
    }
    return new_frame;
}

/**
 * @brief AnalysisSlider::is_in_POI
 * Checks to see if frame is in a POI.
 * @param frame
 * @return
 */
bool AnalysisSlider::is_in_POI(int frame) {
    for (std::pair<int, int> rect : rects) {
        if (frame >= rect.first && frame < rect.second) {
            return true;
        }
    }return false;
}

/**
 * @brief AnalysisSlider::set_show_pois
 * @param show_pois
 */
void AnalysisSlider::set_show_pois(bool show_pois) {
    m_show_pois = show_pois;
}

/**
 * @brief AnalysisSlider::set_show_tags
 * @param show_tags
 */
void AnalysisSlider::set_show_tags(bool show_tags) {
    m_show_tags = show_tags;
}

/**
 * @brief AnalysisSlider::set_show_on_slider
 * @param show
 */
void AnalysisSlider::set_show_on_slider(bool show) {
    show_on_slider = show;
}

/**
 * @brief AnalysisSlider::set_show_interval
 * @param show
 */
void AnalysisSlider::set_show_interval(bool show) {
    show_interval = show;
}

/**
 * @brief AnalysisSlider::clear_slider
 * Clear the rects vector.
 */
void AnalysisSlider::clear_slider() {
    qDebug() << "clear slider";
    rects.clear();
    frames.clear();
}

void AnalysisSlider::set_blocked(bool value) {
    m_blocked = value;
}

/**
 * @brief AnalysisSlider::is_blocked
 * @return
 */
bool AnalysisSlider::is_blocked() {
    return m_blocked;
}

/**
 * @brief AnalysisSlider::get_was_paused
 * @return
 */
bool AnalysisSlider::get_was_paused() {
    return m_was_paused;
}

/**
 * @brief AnalysisSlider::set_was_paused
 * @param value
 */
void AnalysisSlider::set_was_paused(bool value) {
    m_was_paused = value;
}
