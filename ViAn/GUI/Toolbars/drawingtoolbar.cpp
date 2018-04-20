#include "drawingtoolbar.h"
#include "GUI/textdialog.h"

#include <QDebug>

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
    no_tool_act = new QAction(QIcon("../ViAn/Icons/cursor.png"), tr("No tool"), this);
    analysis_tool_act = new QAction(QIcon("../ViAn/Icons/analysis.png"), tr("Analysis tool"), this);
    zoom_in_tool_act = new QAction(QIcon("../ViAn/Icons/zoom_in.png"), tr("Zoom in tool"), this);
    zoom_out_tool_act = new QAction(QIcon("../ViAn/Icons/zoom_out.png"), tr("Zoom out tool"), this);
    zoom_tool_act = new QAction(QIcon("../ViAn/Icons/hand.png"), tr("Panning tool"), this);

    color_label = new QLabel();
    pixmap = new QPixmap(20, 20);
    pixmap->fill(color);
    color_label->setPixmap(*pixmap);
    color_tool_act = new QAction(QIcon("../ViAn/Icons/color.png"), tr("Color picker"), this);
    pen_tool_act = new QAction(QIcon("../ViAn/Icons/pen.png"), tr("Pen tool"), this);
    arrow_tool_act = new QAction(QIcon("../ViAn/Icons/arrow.png"), tr("Arrow tool"), this);
    rectangle_tool_act = new QAction(QIcon("../ViAn/Icons/box.png"), tr("Rectangle tool"), this);
    circle_tool_act = new QAction(QIcon("../ViAn/Icons/circle.png"), tr("Circle tool"), this);
    line_tool_act = new QAction(QIcon("../ViAn/Icons/line.png"), tr("Line tool"), this);
    text_tool_act = new QAction(QIcon("../ViAn/Icons/text.png"), tr("Text tool"), this);
    edit_tool_act = new QAction(QIcon("../ViAn/Icons/edit.png"), tr("Edit tool"), this);
    select_tool_act = new QAction(QIcon("../ViAn/Icons/select.png"), tr("Select tool"), this);
    delete_tool_act = new QAction(QIcon("../ViAn/Icons/clear.png"), tr("Delete current drawing"), this);

    //delete_tool_act->setShortcut(QKeySequence::Delete);
    zoom_in_tool_act->setShortcut(QKeySequence(Qt::Key_Plus));
    zoom_out_tool_act->setShortcut(QKeySequence(Qt::Key_Minus));

    tools = new QActionGroup(this);
    //tools->addAction(no_tool_act);
    tools->addAction(zoom_tool_act);
    tools->addAction(analysis_tool_act);
    tools->addAction(pen_tool_act);
    tools->addAction(arrow_tool_act);
    tools->addAction(rectangle_tool_act);
    tools->addAction(circle_tool_act);
    tools->addAction(line_tool_act);
    tools->addAction(edit_tool_act);
    //tools->addAction(select_tool_act);
    for (QAction* act: tools->actions()) {
        act->setCheckable(true);
    }
    tools->setExclusive(true);

    connect(color_tool_act, &QAction::triggered, this, &DrawingToolbar::color_tool_clicked);

    connect(no_tool_act, &QAction::triggered, this, &DrawingToolbar::no_tool_act_clicked);
    connect(analysis_tool_act, &QAction::triggered, this, &DrawingToolbar::analysis_tool_act_clicked);
    connect(zoom_in_tool_act, &QAction::triggered, this, &DrawingToolbar::zoom_in_tool_act_clicked);
    connect(zoom_out_tool_act, &QAction::triggered, this, &DrawingToolbar::zoom_out_tool_act_clicked);
    connect(zoom_tool_act, &QAction::triggered, this, &DrawingToolbar::zoom_tool_act_clicked);

    connect(pen_tool_act, &QAction::triggered, this, &DrawingToolbar::pen_tool_clicked);
    connect(arrow_tool_act, &QAction::triggered, this, &DrawingToolbar::arrow_tool_clicked);
    connect(rectangle_tool_act, &QAction::triggered, this, &DrawingToolbar::rectangle_tool_clicked);
    connect(circle_tool_act, &QAction::triggered, this, &DrawingToolbar::circle_tool_clicked);
    connect(line_tool_act, &QAction::triggered, this, &DrawingToolbar::line_tool_clicked);
    connect(text_tool_act, &QAction::triggered, this, &DrawingToolbar::text_tool_clicked);
    connect(edit_tool_act, &QAction::triggered, this, &DrawingToolbar::edit_tool_clicked);
    connect(select_tool_act, &QAction::triggered, this, &DrawingToolbar::select_tool_clicked);
}

/**
 * @brief DrawingToolbar::create_buttons
 * Adds all actions to the toolbar
 */
void DrawingToolbar::create_buttons() {
    addWidget(color_label);
    addAction(color_tool_act);
    addAction(zoom_in_tool_act);
    addAction(zoom_out_tool_act);
    addActions(tools->actions());
    addAction(text_tool_act);
    addAction(delete_tool_act);
}

/**
 * @brief DrawingToolbar::color_tool_clicked
 * Opens a color dialog when the color tool is clicked
 */
void DrawingToolbar::color_tool_clicked() {
    emit set_status_bar("Choose a color");
    color = QColorDialog::getColor();
    if (color.isValid()) {
        emit set_status_bar("Color " + color.name() + " chosen");
        emit set_color(color);
        pixmap->fill(color);
        color_label->setPixmap(*pixmap);
    }
}

/**
 * @brief On click events
 * for each tool:
 * set_status_bar("Some tool")
 * set_overlay(TOOL_TYPE)
 */
void DrawingToolbar::no_tool_act_clicked() {
    emit set_status_bar("No tool");
    emit set_overlay_tool(NONE);
}

void DrawingToolbar::analysis_tool_act_clicked() {
    emit set_status_bar("Analysis tool");
    emit set_overlay_tool(ANALYSIS_BOX);
}

void DrawingToolbar::zoom_in_tool_act_clicked() {
    emit set_status_bar("Zoom in");
    emit step_zoom(1.1);
}

void DrawingToolbar::zoom_out_tool_act_clicked() {
    emit set_status_bar("Zoom out");
    emit step_zoom(1/1.1);
}

void DrawingToolbar::zoom_tool_act_clicked() {
    emit set_status_bar("Panning tool");
    emit set_overlay_tool(ZOOM);
}

void DrawingToolbar::pen_tool_clicked() {
    emit set_status_bar("Pen tool");
    emit set_overlay_tool(PEN);
}

void DrawingToolbar::arrow_tool_clicked() {
    emit set_status_bar("Arrow tool");
    emit set_overlay_tool(ARROW);
}

void DrawingToolbar::rectangle_tool_clicked() {
    emit set_status_bar("Rectangle tool");
    emit set_overlay_tool(RECTANGLE);
}

void DrawingToolbar::circle_tool_clicked() {
    emit set_status_bar("Circle tool");
    emit set_overlay_tool(CIRCLE);
}

void DrawingToolbar::line_tool_clicked() {
    emit set_status_bar("Line tool");
    emit set_overlay_tool(LINE);
}

void DrawingToolbar::text_tool_clicked() {
    emit set_status_bar("Text tool");
    TextDialog* text_dialog = new TextDialog;
    text_dialog->deleteLater();
    connect(text_dialog, &TextDialog::text, this, &DrawingToolbar::send_text);
    text_dialog->exec();
    edit_tool_act->trigger();
}

void DrawingToolbar::edit_tool_clicked() {
    emit set_status_bar("Hand tool");
    emit set_overlay_tool(EDIT);
}

void DrawingToolbar::select_tool_clicked() {
    emit set_status_bar("Select tool");
    emit set_overlay_tool(SELECT);
}
