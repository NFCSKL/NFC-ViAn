#include "drawingtoolbar.h"

#include <QIcon>
#include <QColorDialog>

/**
 * @brief DrawingToolbar::DrawingToolbar
 */
DrawingToolbar::DrawingToolbar() {
    create_actions();
    create_buttons();
}

/**
 * @brief DrawingToolbar::create_actions
 * Creates all toolbar actions
 */
void DrawingToolbar::create_actions() {
    color_tool_act = new QAction(QIcon("../ViAn/Icons/color.png"), tr("Color picker"), this);
    pen_tool_act = new QAction(QIcon("../ViAn/Icons/pen.png"), tr("Pen tool"), this);
    arrow_tool_act = new QAction(QIcon("../ViAn/Icons/arrow.png"), tr("Arrow tool"), this);
    box_tool_act = new QAction(QIcon("../ViAn/Icons/box.png"), tr("Rectangle tool"), this);
    circle_tool_act = new QAction(QIcon("../ViAn/Icons/circle.png"), tr("Circle tool"), this);
    line_tool_act = new QAction(QIcon("../ViAn/Icons/line.png"), tr("Line tool"), this);
    text_tool_act = new QAction(QIcon("../ViAn/Icons/text.png"), tr("Text tool"), this);
    undo_tool_act = new QAction(QIcon("../ViAn/Icons/undo.png"), tr("Undo last drawing"), this);
    clear_tool_act = new QAction(QIcon("../ViAn/Icons/clear.png"), tr("Clear all drawings"), this);

    tools = new QActionGroup(this);
    tools->addAction(pen_tool_act);
    tools->addAction(arrow_tool_act);
    tools->addAction(box_tool_act);
    tools->addAction(circle_tool_act);
    tools->addAction(text_tool_act);
    for (QAction* act: tools->actions()) {
        act->setCheckable(true);
    }
    tools->setExclusive(true);

    connect(color_tool_act, &QAction::triggered, this, &DrawingToolbar::color_tool_clicked);
}

/**
 * @brief DrawingToolbar::create_buttons
 * Adds all actions to the toolbar
 */
void DrawingToolbar::create_buttons() {
    addAction(color_tool_act);
    addActions(tools->actions());
    addAction(undo_tool_act);
    addAction(clear_tool_act);
}

/**
 * @brief DrawingToolbar::color_tool_clicked
 * Opens a color dialog when the color tool is clicked
 */
void DrawingToolbar::color_tool_clicked() {
    emit set_status_bar("Choose a color");
    QColor color = QColorDialog::getColor();
    emit set_status_bar("Color " + color.name() + " chosen");
    if (color.isValid()) {
        // TODO set overlay color
    }
}




