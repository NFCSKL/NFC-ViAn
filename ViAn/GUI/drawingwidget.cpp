#include <QHeaderView>
#include "drawingwidget.h"
#include <QMenu>
#include <QDebug>
#include <QDropEvent>
#include <QShortcut>

DrawingWidget::DrawingWidget(QWidget *parent) : QTreeWidget(parent) {
    header()->close();
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(tree_item_clicked(QTreeWidgetItem*,int)));
    connect(this, &DrawingWidget::customContextMenuRequested, this, &DrawingWidget::context_menu);

    // Widget only shortcut for creating a new folder
    QShortcut* delete_sc = new QShortcut(this);
    //delete_sc->setContext(Qt::WidgetWithChildrenShortcut);
    delete_sc->setKey(QKeySequence(QKeySequence::Delete));
    connect(delete_sc, &QShortcut::activated, this, &DrawingWidget::remove_item);

    connect(this, SIGNAL(itemChanged(QTreeWidgetItem*,int)), this, SLOT(item_changed(QTreeWidgetItem*)));
}

void DrawingWidget::set_overlay(Overlay* overlay) {
    clear_overlay();

    m_overlay = overlay;
    update_from_overlay();
    connect(m_overlay, SIGNAL(new_drawing(Shapes*, int)), this, SLOT(add_drawing(Shapes*, int)));
}

void DrawingWidget::clear_overlay() {
    if (m_overlay != nullptr) {
        save_item_data();
        disconnect(m_overlay, SIGNAL(new_drawing(Shapes*, int)), this, SLOT(add_drawing(Shapes*, int)));
        m_overlay = nullptr;
    }
    clear();
}

void DrawingWidget::set_video_project(VideoProject *vid_proj) {
    if (m_vid_proj != vid_proj) {
        m_vid_proj = vid_proj;
        set_overlay(vid_proj->get_overlay());
    }
}

/**
 * @brief DrawingWidget::update_from_overlay
 * Creates tree from drawings in the overlay
 */
void DrawingWidget::update_from_overlay() {
    if (m_overlay == nullptr) return;
    for (auto& overlay : m_overlay->get_overlays()) {
        if (overlay.second.size() != 0) {
            // Check if the frame nr already exist in widget tree
            if (findItems(QString::number(overlay.first), Qt::MatchFixedString).empty()) {
                FrameItem* frame_item = new FrameItem(overlay.first);
                insertTopLevelItem(topLevelItemCount(), frame_item);
                add_drawings_to_frame(frame_item);
                dynamic_cast<QTreeWidgetItem*>(frame_item)->setExpanded(true);
            }
        }
    }
}

/**
 * @brief DrawingWidget::add_drawings_to_frame
 * Adds all drawings to the drawing widget tree
 * @param f_item
 */
void DrawingWidget::add_drawings_to_frame(FrameItem* f_item) {
    std::vector<Shapes*> list;
    try {
        list = m_overlay->get_overlays().at(f_item->get_frame());
    } catch (const std::out_of_range& e) {
        qWarning() << "Can't load drawings, frame out of range";
        return;
    }

    for (auto shape : list) {
        ShapeItem* s_item;
        switch (shape->get_shape()) {
        case RECTANGLE: {
            s_item = new RectItem(dynamic_cast<Rectangle*>(shape));
            f_item->addChild(s_item);
            break;
        }
        case CIRCLE: {
            s_item = new CircleItem(dynamic_cast<Circle*>(shape));
            f_item->addChild(s_item);
            break;
        }
        case LINE: {
            s_item = new LineItem(dynamic_cast<Line*>(shape));
            f_item->addChild(s_item);
            break;
        }
        case ARROW: {
            s_item = new ArrowItem(dynamic_cast<Arrow*>(shape));
            f_item->addChild(s_item);
            break;
        }
        case TEXT: {
            s_item = new TextItem(dynamic_cast<Text*>(shape));
            f_item->addChild(s_item);
            break;
        }
        case PEN: {
            s_item = new PenItem(dynamic_cast<Pen*>(shape));
            f_item->addChild(s_item);
            break;
        }
        default:
            break;
        }
        if (shape == m_overlay->get_current_drawing()) {
            setCurrentItem(s_item);
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
        frame_item->setExpanded(true);
        setCurrentItem(rect_item);
        break;
    }
    case CIRCLE: {
        CircleItem* circle_item = new CircleItem(dynamic_cast<Circle*>(shape));
        frame_item->addChild(circle_item);
        frame_item->setExpanded(true);
        setCurrentItem(circle_item);
        break;
    }
    case LINE: {
        LineItem* line_item = new LineItem(dynamic_cast<Line*>(shape));
        frame_item->addChild(line_item);
        frame_item->setExpanded(true);
        setCurrentItem(line_item);
        break;
    }
    case ARROW: {
        ArrowItem* arrow_item = new ArrowItem(dynamic_cast<Arrow*>(shape));
        frame_item->addChild(arrow_item);
        frame_item->setExpanded(true);
        setCurrentItem(arrow_item);
        break;
    }
    case TEXT: {
        TextItem* text_item = new TextItem(dynamic_cast<Text*>(shape));
        frame_item->addChild(text_item);
        frame_item->setExpanded(true);
        setCurrentItem(text_item);
        break;
    }
    case PEN: {
        PenItem* pen_item = new PenItem(dynamic_cast<Pen*>(shape));
        frame_item->addChild(pen_item);
        frame_item->setExpanded(true);
        setCurrentItem(pen_item);
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

/**
 * @brief DrawingWidget::save_item_data
 * Stores GUI information of each tree item into its data member
 * @param item
 */
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
        case ARROW_ITEM:
        case PEN:
        case TEXT_ITEM: {
            auto a_item = dynamic_cast<ShapeItem*>(child);
            a_item->update_shape_name();
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
        FrameItem* frame_item = dynamic_cast<FrameItem*>(item);
        emit jump_to_frame(m_vid_proj, frame_item->get_frame());
        emit set_current_drawing(nullptr);
        break;
    }
    case RECT_ITEM:
    case CIRCLE_ITEM:
    case LINE_ITEM:
    case ARROW_ITEM:
    case TEXT_ITEM:
    case PEN_ITEM: {
        ShapeItem* shape_item = dynamic_cast<ShapeItem*>(item);
        Shapes* shape = shape_item->get_shape();
        emit jump_to_frame(m_vid_proj, shape->get_frame());
        emit set_current_drawing(shape);
        emit set_tool_hand();
        break;
    }
    default:
        break;
    }
}

/**
 * @brief DrawingWidget::context_menu
 * @param point
 * Slot function triggered by customContextMenuRequested
 * Creates a context menu
 */
void DrawingWidget::context_menu(const QPoint &point) {
    if (m_vid_proj == nullptr) return;
    QMenu menu(this);
    QTreeWidgetItem* item = itemAt(point);
    if (item == nullptr) return;
    switch (item->type()) {
    case FRAME_ITEM:
        menu.addAction("Delete", this, SLOT(remove_item()));
        break;
    case RECT_ITEM:
    case CIRCLE_ITEM:
    case LINE_ITEM:
    case ARROW_ITEM:
    case PEN_ITEM:
    case TEXT_ITEM:
        menu.addAction("Rename", this, SLOT(rename_item()));
        menu.addAction("Delete", this, SLOT(remove_item()));
        break;
    default:
        break;
    }
    menu.exec(mapToGlobal(point));
}

/**
 * @brief DrawingWidget::rename_item
 * Slot function to start edit the current item
 */
void DrawingWidget::rename_item() {
    editItem(currentItem());
}

/**
 * @brief DrawingWidget::item_changed
 * @param item
 * Slot function called on the signal itemChanged.
 * Updates the item with the newly entered name
 */
void DrawingWidget::item_changed(QTreeWidgetItem* item) {
    auto a_item = dynamic_cast<ShapeItem*>(item);
    a_item->update_shape_name();
    m_overlay->set_overlay_changed();
    if (item->type() == TEXT_ITEM) {
        Text* text = dynamic_cast<TextItem*>(item)->get_shape();
        emit update_text(text->get_name(), text);
    }
}

/**
 * @brief DrawingWidget::remove_item
 * Slot function for removing the current item from the tree
 */
void DrawingWidget::remove_item() {
    if (!currentItem()) return;
    remove_from_tree(currentItem());
}

/**
 * @brief DrawingWidget::remove_from_tree
 * @param item
 * Deletes item from the drawing tree.
 * Calls itself recusively in case of a frame_item with multiple drawings.
 * Also deletes the frame_item in case the item of the last drawing of that frame
 */
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
    case CIRCLE_ITEM:
    case LINE_ITEM:
    case ARROW_ITEM:
    case TEXT_ITEM:
    case PEN_ITEM:
        shape = dynamic_cast<ShapeItem*>(item)->get_shape();
        emit delete_drawing(shape);
        parent = item->parent();
        delete item;
        if (parent->childCount() == 0) delete parent;
        break;
    default:
        break;
    }
}
