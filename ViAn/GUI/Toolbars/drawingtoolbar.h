#ifndef DRAWINGTOOLBAR_H
#define DRAWINGTOOLBAR_H

#include <QToolBar>
#include <QAction>
#include <QActionGroup>
#include "Video/shapes/shapes.h"

/**
 * @brief The DrawingToolbar class
 * Toolbar for drawing tool
 */
class DrawingToolbar : public QToolBar
{
    Q_OBJECT
    void create_actions();
    void create_buttons();
public:
    DrawingToolbar();

    QAction* no_tool_act;
    QAction* analysis_tool_act;
    QAction* zoom_in_tool_act;
    QAction* zoom_out_tool_act;
    QAction* zoom_tool_act;
    QAction* color_tool_act;
    QAction* pen_tool_act;
    QAction* arrow_tool_act;
    QAction* rectangle_tool_act;
    QAction* circle_tool_act;
    QAction* line_tool_act;
    QAction* text_tool_act;
    QAction* hand_tool_act;
    QAction* delete_tool_act;

    QActionGroup* tools;

public slots:
    void color_tool_clicked();

private slots:
    void no_tool_act_clicked();
    void analysis_tool_act_clicked();
    void zoom_in_tool_act_clicked();
    void zoom_out_tool_act_clicked();
    void zoom_tool_act_clicked();

    void pen_tool_clicked();
    void arrow_tool_clicked();
    void rectangle_tool_clicked();
    void circle_tool_clicked();
    void line_tool_clicked();
    void text_tool_clicked();
    void hand_tool_clicked();
signals:
    void set_status_bar(QString);
    void set_color(QColor);
    void set_overlay_tool(SHAPES);
    void step_zoom(double);
    void clear(void);
};

#endif // DRAWINGTOOLBAR_H
