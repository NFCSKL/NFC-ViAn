#include <QHeaderView>
#include "drawingwidget.h"
#include <QMenu>

DrawingWidget::DrawingWidget(QWidget *parent) : QTreeWidget(parent) {
    header()->close();
    setContextMenuPolicy(Qt::CustomContextMenu);
    setDragDropMode(QAbstractItemView::InternalMove);
    connect(this, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(tree_item_clicked(QTreeWidgetItem*,int)));
    connect(this, &DrawingWidget::customContextMenuRequested, this, &DrawingWidget::context_menu);
}

void DrawingWidget::set_overlay(Overlay* overlay) {
    m_overlay = overlay;
    clear();
    update_from_overlay();
    // Wrong, gets a new connect everytime
    connect(m_overlay, SIGNAL(new_drawing(Shapes*, int)), this, SLOT(add_drawing(Shapes*, int)));
}

void DrawingWidget::set_video_project(VideoProject *vid_proj) {
    if (m_vid_proj != vid_proj) { //disconnect then reconnect and check if nullptr
        m_vid_proj = vid_proj;
        set_overlay(vid_proj->get_overlay());
    }
}

void DrawingWidget::update_from_overlay() {
    if (m_overlay == nullptr) return;
    for (auto& overlay : m_overlay->get_overlays()) {
        if (overlay.second.size() != 0) {
            // Check if the frame nr already exist in widget tree
            if (findItems(QString::number(overlay.first), Qt::MatchFixedString).empty()) {
                FrameItem* frame_item = new FrameItem(overlay.first);
                insertTopLevelItem(topLevelItemCount(), frame_item);
                add_drawings_to_frame(frame_item);
            }
        }
    }
}

void DrawingWidget::add_drawings_to_frame(FrameItem* f_item) {
    for (Shapes* shape : m_overlay->get_overlays()[f_item->get_frame()]) {
        switch (shape->get_shape()) {
        case RECTANGLE: {
            RectItem* rect_item = new RectItem(dynamic_cast<Rectangle*>(shape));
            f_item->addChild(rect_item);
            break;
        }
        case CIRCLE: {
            CircleItem* circle_item = new CircleItem(dynamic_cast<Circle*>(shape));
            f_item->addChild(circle_item);
            break;
        }
        case LINE: {
            LineItem* line_item = new LineItem(dynamic_cast<Line*>(shape));
            f_item->addChild(line_item);
            break;
        }
        case ARROW: {
            ArrowItem* arrow_item = new ArrowItem(dynamic_cast<Arrow*>(shape));
            f_item->addChild(arrow_item);
            break;
        }
        default:
            break;
        }
    }
}

/**
 * @brief DrawingWidget::add_drawing
 * Slot function for when a new drawing is created.
 * Adds the drawing to the treeitem with the correct frame number
 * @param shape
 * @param frame_nr
 */
void DrawingWidget::add_drawing(Shapes *shape, int frame_nr) {
    FrameItem* frame_item;
    QList<QTreeWidgetItem*> list = findItems(QString::number(frame_nr), Qt::MatchFixedString);

    if (list.empty()) {
        frame_item = new FrameItem(frame_nr);
        add_item_in_order(frame_item);
    } else {
        frame_item = dynamic_cast<FrameItem*>(list.at(0));
    }
    switch (shape->get_shape()) {
    case RECTANGLE: {
        RectItem* rect_item = new RectItem(dynamic_cast<Rectangle*>(shape));
        frame_item->addChild(rect_item);
        rect_item->setText(0, "Rectangle");
        frame_item->setExpanded(true);
        break;
    }
    case CIRCLE: {
        CircleItem* circle_item = new CircleItem(dynamic_cast<Circle*>(shape));
        frame_item->addChild(circle_item);
        circle_item->setText(0, "Circle");
        frame_item->setExpanded(true);
        break;
    }
    case LINE: {
        LineItem* line_item = new LineItem(dynamic_cast<Line*>(shape));
        frame_item->addChild(line_item);
        line_item->setText(0, "Line");
        frame_item->setExpanded(true);
        break;
    }
    case ARROW: {
        ArrowItem* arrow_item = new ArrowItem(dynamic_cast<Arrow*>(shape));
        frame_item->addChild(arrow_item);
        arrow_item->setText(0, "Arrow");
        frame_item->setExpanded(true);
        break;
    }
    default:
        break;
    }
}

/**
 * @brief DrawingWidget::add_item_in_order
 * Adds item to the tree widget is ascending order.
 * Expects the tree widget to be sorted ascendingly and that the items have numbers as names.
 * @param item
 */
void DrawingWidget::add_item_in_order(FrameItem *item) {
    for (int i = 0; i < topLevelItemCount(); ++i) {
        int number = topLevelItem(i)->text(0).toInt();
        if (item->get_frame() < number) {
            insertTopLevelItem(i, item);
            return;
        }
    }
    insertTopLevelItem(topLevelItemCount(), item);
}

void DrawingWidget::save_item_data(QTreeWidgetItem *item) {
    if (item == nullptr) item = invisibleRootItem();
    for (auto i = 0; i < item->childCount(); ++i){
        auto child = item->child(i);

        switch (child->type()) {
        case FRAME_ITEM:
            save_item_data(child);
            break;
        case RECT_ITEM:
        case CIRCLE_ITEM:
        case LINE_ITEM:
        case ARROW_ITEM: {
            auto a_item = dynamic_cast<ShapeItem*>(child);
            a_item->rename();
            break;
        }
        default:
            break;
        }
    }
}

/**
 * @brief DrawingWidget::tree_item_clicked
 * Slot function for when a tree item is clicked.
 * Performs different operations based of the drawing type
 * @param item
 * @param col
 */
void DrawingWidget::tree_item_clicked(QTreeWidgetItem *item, const int &col) {
    Q_UNUSED(col)
    switch (item->type()) {
    case FRAME_ITEM: {
        qDebug() << "frame_item";
        FrameItem* frame_item = dynamic_cast<FrameItem*>(item);
        emit jump_to_frame(m_vid_proj, frame_item->get_frame());
        break;
    }
    case RECT_ITEM: {
        FrameItem* frame_item = dynamic_cast<FrameItem*>(item->parent());
        RectItem* rect_item = dynamic_cast<RectItem*>(item);
        emit jump_to_frame(m_vid_proj, frame_item->get_frame());
        emit set_current_drawing(rect_item->get_shape());
        break;
    }
    case CIRCLE_ITEM: {
        FrameItem* frame_item = dynamic_cast<FrameItem*>(item->parent());
        CircleItem* circle_item = dynamic_cast<CircleItem*>(item);
        emit jump_to_frame(m_vid_proj, frame_item->get_frame());
        emit set_current_drawing(circle_item->get_shape());
        break;
    }
    case LINE_ITEM: {
        FrameItem* frame_item = dynamic_cast<FrameItem*>(item->parent());
        LineItem* line_item = dynamic_cast<LineItem*>(item);
        emit jump_to_frame(m_vid_proj, frame_item->get_frame());
        emit set_current_drawing(line_item->get_shape());
        break;
    }
    case ARROW_ITEM: {
        FrameItem* frame_item = dynamic_cast<FrameItem*>(item->parent());
        ArrowItem* arrow_item = dynamic_cast<ArrowItem*>(item);
        emit jump_to_frame(m_vid_proj, frame_item->get_frame());
        emit set_current_drawing(arrow_item->get_shape());
        break;
    }
    default:
        break;
    }
}

void DrawingWidget::context_menu(const QPoint &point) {
    if (m_vid_proj == nullptr) return;
    QMenu menu(this);
    QTreeWidgetItem* item = itemAt(point);
    if (item == nullptr) return;
    switch (item->type()) {
    case FRAME_ITEM:
        menu.addAction("Remove", this, SLOT(remove_item()));
        break;
    case RECT_ITEM:
    case CIRCLE_ITEM:
    case LINE_ITEM:
    case ARROW_ITEM:
        menu.addAction("Rename", this, SLOT(rename_item()));
        menu.addAction("Remove", this, SLOT(remove_item()));
        break;
    default:
        break;
    }
    menu.exec(mapToGlobal(point));
}

void DrawingWidget::rename_item() {
    editItem(currentItem());
}

void DrawingWidget::remove_item() {
    remove_from_tree(currentItem());
}

void DrawingWidget::remove_from_tree(QTreeWidgetItem *item) {
    Shapes* shape;
    QTreeWidgetItem* parent;
    switch (item->type()) {
    case FRAME_ITEM:
        while (item->childCount() != 0) {
            remove_from_tree(item->child(0));
        }
        delete item;
        break;
    case RECT_ITEM:
        shape = dynamic_cast<RectItem*>(item)->get_shape();
        emit delete_drawing(shape);
        parent = item->parent();
        delete item;
        if (parent->childCount() == 0) delete parent;
        break;
    case CIRCLE_ITEM:
        shape = dynamic_cast<CircleItem*>(item)->get_shape();
        emit delete_drawing(shape);
        parent = item->parent();
        delete item;
        if (parent->childCount() == 0) delete parent;
        break;
    case LINE_ITEM:
        shape = dynamic_cast<LineItem*>(item)->get_shape();
        emit delete_drawing(shape);
        parent = item->parent();
        delete item;
        if (parent->childCount() == 0) delete parent;
        break;
    case ARROW_ITEM:
        shape = dynamic_cast<ArrowItem*>(item)->get_shape();
        emit delete_drawing(shape);
        parent = item->parent();
        delete item;
        if (parent->childCount() == 0) delete parent;
        break;
    default:
        break;
    }
}
