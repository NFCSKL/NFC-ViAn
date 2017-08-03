#ifndef DRAWINGTOOLBAR_H
#define DRAWINGTOOLBAR_H

#include <QToolBar>
#include <QAction>
#include <QActionGroup>
#include "Video/shapes/shape.h"

class DrawingToolbar : public QToolBar
{
    Q_OBJECT
    void create_actions();
    void create_buttons();
    void set_exclusive_checkability();
public:
    DrawingToolbar();

    QAction* color_tool_act;
    QAction* pen_tool_act;
    QAction* arrow_tool_act;
    QAction* rectangle_tool_act;
    QAction* circle_tool_act;
    QAction* line_tool_act;
    QAction* text_tool_act;
    QAction* undo_tool_act;
    QAction* redo_tool_act;
    QAction* clear_tool_act;

    QActionGroup* tools;

public slots:
    void color_tool_clicked();

private slots:
    void pen_tool_clicked();
    void arrow_tool_clicked();
    void rectangle_tool_clicked();
    void circle_tool_clicked();
    void line_tool_clicked();
    void text_tool_clicked();
signals:
    void set_status_bar(QString);
    void set_color(QColor);
    void set_overlay_tool(SHAPES);
    void undo(void);
    void clear(void);
};

#endif // DRAWINGTOOLBAR_H
