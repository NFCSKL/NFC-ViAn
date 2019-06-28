#include "drawingwidget.h"

#include "DrawingItems/arrowitem.h"
#include "DrawingItems/circleitem.h"
#include "DrawingItems/frameitem.h"
#include "DrawingItems/lineitem.h"
#include "DrawingItems/penitem.h"
#include "DrawingItems/rectitem.h"
#include "DrawingItems/textitem.h"
#include "Project/videoproject.h"
#include "Video/overlay.h"

#include <QColorDialog>
#include <QDebug>
#include <QHeaderView>
#include <QMenu>
#include <QMessageBox>
#include <QShortcut>


DrawingWidget::DrawingWidget(QWidget *parent) : QTreeWidget(parent) {
    setContextMenuPolicy(Qt::CustomContextMenu);

    setColumnCount(3);
    header()->resizeSection(0, 170);
    header()->resizeSection(1, 40);
    header()->resizeSection(2, 35);

    headerItem()->setText(0, "Frame - Drawings");
    headerItem()->setText(1, "Color");
    headerItem()->setText(2, "Hide");

    // Shortcut for deleting item
    QShortcut* delete_sc = new QShortcut(QKeySequence::Delete, this);
    delete_sc->setContext(Qt::WidgetWithChildrenShortcut);
    connect(delete_sc, &QShortcut::activated, this, &DrawingWidget::delete_item);

    // Connects for items clicked or changed in tree
    connect(this, &DrawingWidget::itemClicked, this, &DrawingWidget::tree_item_clicked);
    connect(this, &DrawingWidget::customContextMenuRequested, this, &DrawingWidget::context_menu);
    connect(this, &DrawingWidget::currentItemChanged, this, [this]{ tree_item_clicked(currentItem());});
    connect(this, &DrawingWidget::itemChanged, this, &DrawingWidget::item_changed);
}

void DrawingWidget::set_overlay(Overlay* overlay) {
    clear_overlay();

    m_overlay = overlay;
    update_from_overlay();
    connect(m_overlay, &Overlay::new_drawing, this, &DrawingWidget::add_drawing);
    connect(m_overlay, &Overlay::clean_overlay, this, &DrawingWidget::clear_overlay);
    connect(m_overlay, &Overlay::select_current, this, &DrawingWidget::set_current_selected);
    connect(m_overlay, &Overlay::set_tool_zoom, this, &DrawingWidget::set_tool_zoom);
    connect(m_overlay, &Overlay::set_tool_edit, this, &DrawingWidget::set_tool_edit);
}

void DrawingWidget::clear_overlay() {
    if (m_overlay != nullptr) {
        m_overlay->set_current_drawing(nullptr);
        save_item_data();
        disconnect(m_overlay, &Overlay::new_drawing, this, &DrawingWidget::add_drawing);
        disconnect(m_overlay, &Overlay::clean_overlay, this, &DrawingWidget::clear_overlay);
        disconnect(m_overlay, &Overlay::select_current, this, &DrawingWidget::set_current_selected);
        disconnect(m_overlay, &Overlay::set_tool_zoom, this, &DrawingWidget::set_tool_zoom);
        disconnect(m_overlay, &Overlay::set_tool_edit, this, &DrawingWidget::set_tool_edit);
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
    blockSignals(true);
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
    blockSignals(false);
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
        case PEN_ITEM:
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

void DrawingWidget::set_current_selected(Shapes* shape, int frame_nr) {
    QList<QTreeWidgetItem*> list = findItems(QString::number(frame_nr), Qt::MatchFixedString);
    if (list.empty()) return;
    FrameItem* frame_item = dynamic_cast<FrameItem*>(list.at(0));
    for (int i = 0; i != frame_item->childCount(); ++i) {
        ShapeItem* shape_item = dynamic_cast<ShapeItem*>(frame_item->child(i));
        if (shape_item->get_shape() == shape) {
            blockSignals(true);
            setCurrentItem(shape_item);
            blockSignals(false);
            return;
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
    if (!item) return;
    switch (item->type()) {
    case FRAME_ITEM: {
        emit set_current_drawing(nullptr);
        FrameItem* frame_item = dynamic_cast<FrameItem*>(item);
        VideoState state;
        state = m_vid_proj->get_video()->state;
        state.frame = frame_item->get_frame();
        // Reset these to default values so the zoomer will fit to viewport
        state.center = QPoint(-1, -1);
        state.scale_factor = -1;
        emit jump_to_frame(m_vid_proj, state);
        break;
    }
    case RECT_ITEM:
    case CIRCLE_ITEM:
    case ARROW_ITEM:
    case LINE_ITEM:
    case PEN_ITEM:
    case TEXT_ITEM: {
        ShapeItem* shape_item = dynamic_cast<ShapeItem*>(item);
        Shapes* shape = shape_item->get_shape();
        if (col == 1) {
            QColor color = QColorDialog::getColor();
            if (color.isValid()) {
                shape->set_color(color);
                shape_item->update_shape_color();
            }
        } else if (col == 2) {
            shape_item->update_show_icon(shape->toggle_show());
        }
        emit set_tool_edit();
        emit set_current_drawing(shape);
        VideoState state;
        state = m_vid_proj->get_video()->state;
        state.frame = shape->get_frame();
        // Reset these to default values so the zoomer will fit to viewport
        state.center = QPoint(-1, -1);
        state.scale_factor = -1;
        emit jump_to_frame(m_vid_proj, state);
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
        menu.addAction("Delete", this, &DrawingWidget::remove_item);
        break;
    case RECT_ITEM:
    case CIRCLE_ITEM:
    case LINE_ITEM:
    case ARROW_ITEM:
    case PEN_ITEM:
    case TEXT_ITEM:
        menu.addAction("Rename", this, &DrawingWidget::rename_item);
        menu.addAction("Delete", this, &DrawingWidget::remove_item);
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
    remove_from_tree(currentItem());
}

/**
 * @brief DrawingWidget::remove_item_frame
 * Called when a frame is removed from a sequence.
 * Updates the drawings and widget with the new frame numbers
 * @param frame
 */
void DrawingWidget::remove_item_frame(int frame) {
    blockSignals(true);
    auto root = invisibleRootItem();
    bool found = false;
    QTreeWidgetItem* item = nullptr;
    for (int i = 0; i < root->childCount(); i++) {
        QTreeWidgetItem* t_item = root->child(i);
        if (found) {
            // Lowering frame by one
            FrameItem* f_item = dynamic_cast<FrameItem*>(t_item);
            f_item->set_frame(f_item->get_frame()-1);
            // Update all shapes with the new frame
            for (int j = 0; j < t_item->childCount(); j++) {
                ShapeItem* s_item = dynamic_cast<ShapeItem*>(t_item->child(j));
                Shapes* shape = s_item->get_shape();
                shape->set_frame(shape->get_frame()-1);
            }
        }
        if (t_item->text(0) == QString::number(frame) && !found) {
            item = t_item;
            found = true;
        }
    }
    if (found) {
        remove_from_tree(item);
    }
    m_overlay->remove_frame(frame);
    blockSignals(false);
}

void DrawingWidget::delete_item() {
    if (!currentItem() || m_overlay->get_tool() != EDIT) return;
    ShapeItem* item = dynamic_cast<ShapeItem*>(currentItem());

    if (item->type() == FRAME_ITEM) {
        FrameItem* f_item = dynamic_cast<FrameItem*>(item);
        if (f_item->get_frame() == m_overlay->get_current_frame()) {
            QMessageBox msg_box;
            msg_box.setIcon(QMessageBox::Warning);
            msg_box.setMinimumSize(300,130);
            msg_box.setText("Deleting drawings on frame "+QString::number(f_item->get_frame())+"\n"
                            "This will delete all drawings on this frame");
            msg_box.setInformativeText("Do you wish to continue?");
            msg_box.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            msg_box.setDefaultButton(QMessageBox::No);
            int reply = msg_box.exec();
            if (reply == QMessageBox::No) return;

            remove_from_tree(f_item);
            clearSelection();
        }
    } else if (item->get_shape() != m_overlay->get_current_drawing()) {
        return;
    } else {
        remove_from_tree(item);
        clearSelection();
        emit set_current_drawing(nullptr);
    }
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
        blockSignals(true);
        delete item;
        blockSignals(false);
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
        blockSignals(true);
        if (parent->childCount() == 0) delete parent;
        blockSignals(false);
        break;
    default:
        break;
    }
}
