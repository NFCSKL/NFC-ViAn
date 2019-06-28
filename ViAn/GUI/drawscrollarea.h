#ifndef DRAWSCROLLAREA_H
#define DRAWSCROLLAREA_H

#include <QScrollArea>

class DrawScrollArea : public QScrollArea
{
    Q_OBJECT
public:
    explicit DrawScrollArea(QWidget *parent = nullptr);

signals:
    void new_size(QSize size);

public slots:

protected:
    void resizeEvent(QResizeEvent* event);
};

#endif // DRAWSCROLLAREA_H
