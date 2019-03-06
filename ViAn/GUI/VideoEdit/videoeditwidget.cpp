#include "videoeditwidget.h"

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

    connect(generate_video_btn, &QPushButton::clicked, videoedit_list, &VideoEditList::generate_video);
}
