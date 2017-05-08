#include "qtreeitems.h"
/**
 * @brief MyQTreeWidgetItem::MyQTreeWidgetItem
 * @param type of the widget
 * @param name of the widget
 * @param id
 */
MyQTreeWidgetItem::MyQTreeWidgetItem(TYPE type, QString name, ID id) {
    QTreeWidgetItem();
    this->id = id;
    this->type = type;
    this->name = name;
}

/**
 * @brief QTreeVideoItem::QTreeVideoItem
 * @param type of the widget
 * @param name of the widget
 * @param id
 */
QTreeVideoItem::QTreeVideoItem(TYPE type, QString name, ID id) : MyQTreeWidgetItem(type,name,id){
    this->bookmarks.clear();
}

/**
 * @brief MyQTreeWidgetItem::~MyQTreeWidgetItem
 */
MyQTreeWidgetItem::~MyQTreeWidgetItem() {
}

/**
 * @brief MyQTreeWidgetItem::set_text
 * @param text shown in the tree
 */
void MyQTreeWidgetItem::set_text(std::string text) {
    QString qText = QString::fromStdString(text);
    if (qText.length() >= TEXT_LENGTH) {
        qText.chop(qText.length()-TEXT_LENGTH-2);
        qText.append("...");
    }
    setText(0, qText);
}

/**
 * @brief MyQTreeWidgetItem::set_text_from_filepath
 * @param filepath of the file,
 *  the text will be the name of the file
 */
void MyQTreeWidgetItem::set_text_from_filepath(std::string filepath) {
    std::string reverse (filepath.rbegin(), filepath.rend());
    std::string txet = reverse.substr(0, reverse.find("/")); // txet = text in reverse
    std::string text (txet.rbegin(), txet.rend());
    set_text(text);
}

/**
 * @brief MyQTreeWidgetItem::get_name
 * @return returns the name of the object without the rest of the filepath
 */
std::string MyQTreeWidgetItem::get_name() {
    std::string stdname = name.toStdString();
    std::string reverse (stdname.rbegin(), stdname.rend());
    std::string txet = reverse.substr(0, reverse.find("/")); // txet = text in reverse
    std::string text (txet.rbegin(), txet.rend());
    return text;
}

/**
 * @brief operator ==
 * @param item1
 * @param item2
 * @return if the same true
 */
bool MyQTreeWidgetItem::operator==(MyQTreeWidgetItem* item){

    bool name = this->name == item->name;
    bool id = this->id == item->id;
    bool type = this->type == item->type;
    bool parent = this->parent() == item->parent();
    return name && id && type && parent && ((QTreeWidgetItem*)this == (QTreeWidgetItem*)item);
}
