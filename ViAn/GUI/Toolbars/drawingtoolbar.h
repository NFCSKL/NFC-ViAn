#ifndef DRAWINGTOOLBAR_H
#define DRAWINGTOOLBAR_H

#include <QToolBar>
#include <QAction>
#include <QActionGroup>

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
    QAction* box_tool_act;
    QAction* circle_tool_act;
    QAction* line_tool_act;
    QAction* text_tool_act;
    QAction* undo_tool_act;
    QAction* clear_tool_act;

    QActionGroup* tools;

private slots:
    void color_tool_clicked();
signals:
    void set_status_bar(QString);
};

#endif // DRAWINGTOOLBAR_H
