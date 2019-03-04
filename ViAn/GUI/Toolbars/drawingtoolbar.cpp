#include "drawingtoolbar.h"

#include "GUI/textdialog.h"

#include <QColorDialog>
#include <QLabel>

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
    zoom_in_tool_act = new QAction(QIcon(":/Icons/zoom_in.png"), tr("Zoom in"), this);
    zoom_out_tool_act = new QAction(QIcon(":/Icons/zoom_out.png"), tr("Zoom out"), this);
    analysis_tool_act = new QAction(QIcon(":/Icons/analys.png"), tr("ROI analysis"), this);
    zoom_tool_act = new QAction(QIcon(":/Icons/cursor.png"), tr("Pan/zoom tool"), this);
    edit_tool_act = new QAction(QIcon(":/Icons/cross.png"), tr("Edit tool"), this);
    rectangle_tool_act = new QAction(QIcon(":/Icons/box.png"), tr("Rectangle"), this);
    circle_tool_act = new QAction(QIcon(":/Icons/circle.png"), tr("Circle"), this);
    arrow_tool_act = new QAction(QIcon(":/Icons/arrow.png"), tr("Arrow"), this);
    line_tool_act = new QAction(QIcon(":/Icons/line.png"), tr("Line"), this);
    pen_tool_act = new QAction(QIcon(":/Icons/pen.png"), tr("Pen"), this);
    text_tool_act = new QAction(QIcon(":/Icons/text.png"), tr("Text"), this);
    color_label = new QLabel();
    pixmap = new QPixmap(COLOR_WIDTH, COLOR_HEIGHT);
    pixmap->fill(color);
    color_label->setPixmap(*pixmap);
    color_tool_act = new QAction(QIcon(":/Icons/color.png"), tr("Color picker"), this);
    delete_tool_act = new QAction(QIcon(":/Icons/clear.png"), tr("Delete current drawing"), this);

    zoom_tool_act->setShortcut(QKeySequence(Qt::Key_A));
    edit_tool_act->setShortcut(QKeySequence(Qt::Key_E));

    tools = new QActionGroup(this);
    tools->addAction(analysis_tool_act);
    tools->addAction(zoom_tool_act);
    tools->addAction(edit_tool_act);
    tools->addAction(rectangle_tool_act);
    tools->addAction(circle_tool_act);
    tools->addAction(arrow_tool_act);
    tools->addAction(line_tool_act);
    tools->addAction(pen_tool_act);
    for (QAction* act: tools->actions()) {
        act->setCheckable(true);
    }
    tools->setExclusive(true);

    connect(zoom_in_tool_act, &QAction::triggered, this, &DrawingToolbar::zoom_in_tool_act_clicked);
    connect(zoom_out_tool_act, &QAction::triggered, this, &DrawingToolbar::zoom_out_tool_act_clicked);
    connect(analysis_tool_act, &QAction::triggered, this, &DrawingToolbar::analysis_tool_act_clicked);
    connect(zoom_tool_act, &QAction::triggered, this, &DrawingToolbar::zoom_tool_act_clicked);
    connect(edit_tool_act, &QAction::triggered, this, &DrawingToolbar::edit_tool_clicked);

    connect(rectangle_tool_act, &QAction::triggered, this, &DrawingToolbar::rectangle_tool_clicked);
    connect(circle_tool_act, &QAction::triggered, this, &DrawingToolbar::circle_tool_clicked);
    connect(arrow_tool_act, &QAction::triggered, this, &DrawingToolbar::arrow_tool_clicked);
    connect(line_tool_act, &QAction::triggered, this, &DrawingToolbar::line_tool_clicked);
    connect(pen_tool_act, &QAction::triggered, this, &DrawingToolbar::pen_tool_clicked);
    connect(text_tool_act, &QAction::triggered, this, &DrawingToolbar::text_tool_clicked);
    connect(color_tool_act, &QAction::triggered, this, &DrawingToolbar::color_tool_clicked);
}

/**
 * @brief DrawingToolbar::create_buttons
 * Adds all actions to the toolbar
 */
void DrawingToolbar::create_buttons() {
    addAction(zoom_in_tool_act);
    addAction(zoom_out_tool_act);
    addActions(tools->actions());
    addAction(text_tool_act);
    addWidget(color_label);
    addAction(color_tool_act);
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
void DrawingToolbar::zoom_in_tool_act_clicked() {
    emit set_status_bar("Zoom in");
    emit step_zoom(1.1);
}

void DrawingToolbar::zoom_out_tool_act_clicked() {
    emit set_status_bar("Zoom out");
    emit step_zoom(1/1.1);
}

void DrawingToolbar::analysis_tool_act_clicked() {
    emit set_status_bar("Analysis tool");
    emit set_overlay_tool(ANALYSIS_BOX);
}

void DrawingToolbar::zoom_tool_act_clicked() {
    emit set_status_bar("Default zoom tool");
    emit set_overlay_tool(ZOOM);
}

void DrawingToolbar::edit_tool_clicked() {
    emit set_status_bar("Edit tool");
    emit set_overlay_tool(EDIT);
}

void DrawingToolbar::rectangle_tool_clicked() {
    emit set_status_bar("Rectangle tool");
    emit set_overlay_tool(RECTANGLE);
}

void DrawingToolbar::circle_tool_clicked() {
    emit set_status_bar("Circle tool");
    emit set_overlay_tool(CIRCLE);
}

void DrawingToolbar::arrow_tool_clicked() {
    emit set_status_bar("Arrow tool");
    emit set_overlay_tool(ARROW);
}

void DrawingToolbar::line_tool_clicked() {
    emit set_status_bar("Line tool");
    emit set_overlay_tool(LINE);
}

void DrawingToolbar::pen_tool_clicked() {
    emit set_status_bar("Pen tool");
    emit set_overlay_tool(PEN);
}

void DrawingToolbar::text_tool_clicked() {
    emit set_status_bar("Text tool");
    TextDialog* text_dialog = new TextDialog;
    text_dialog->deleteLater();
    connect(text_dialog, &TextDialog::accepted, edit_tool_act, &QAction::trigger);
    connect(text_dialog, &TextDialog::text, this, &DrawingToolbar::send_text);
    text_dialog->exec();
}
