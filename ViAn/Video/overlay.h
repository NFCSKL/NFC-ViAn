#ifndef OVERLAY_H
#define OVERLAY_H

#include <QImage>


class overlay {

public:
    overlay();

    bool is_showing_overlay();
    void set_showing_overlay(bool value);
    void toggle_overlay();
    void draw_overlay(QImage &img);

private:
    bool show_overlay = false;
};

#endif // OVERLAY_H
