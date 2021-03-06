#include "videoeditwidget.h"

#include "Project/project.h"
#include "Project/videoproject.h"

#include <QBoxLayout>
#include <QPushButton>
#include <QScrollArea>

VideoEditWidget::VideoEditWidget(QWidget *parent) : QWidget(parent)
{

    QPushButton* generate_video_btn = new QPushButton(tr("Generate video"));

    videoedit_list = new VideoEditList();
    scroll_area = new QScrollArea();

    scroll_area->setWidget(videoedit_list);
    scroll_area->setFrameStyle(0);
    scroll_area->setWidgetResizable(true);

    layout = new QVBoxLayout();
    layout->setAlignment(Qt::AlignBottom);
    layout->addWidget(scroll_area);
    layout->addWidget(generate_video_btn);
    setLayout(layout);

    connect(generate_video_btn, &QPushButton::clicked, this, &VideoEditWidget::generate_video);
    connect(this, &VideoEditWidget::interval_to_edit, videoedit_list, &VideoEditList::add_interval);
    connect(this, &VideoEditWidget::set_project, videoedit_list, &VideoEditList::set_project);
    connect(this, &VideoEditWidget::save_item_data, videoedit_list, &VideoEditList::save_item_data);
    connect(videoedit_list, &VideoEditList::set_video, this, &VideoEditWidget::set_video);
    connect(videoedit_list, &VideoEditList::add_video, this, &VideoEditWidget::add_video);
}

void VideoEditWidget::clear_intervals() {
    videoedit_list->clear();
}

void VideoEditWidget::generate_video() {
    videoedit_list->generate_video();
}
