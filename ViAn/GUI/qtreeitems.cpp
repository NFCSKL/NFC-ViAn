#include "qtreeitems.h"
/**
 * @brief MyQTreeWidgetItem::MyQTreeWidgetItem
 * @param type of the widget
 * @param string, the name of the widget
 * @param id
 */
MyQTreeWidgetItem::MyQTreeWidgetItem(TYPE type, QString string, int id) {
    QTreeWidgetItem();
    this->id = id;
    this->type = type;
    this->name = string;
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
    if (qText.length() >= 30) {
        qText.chop(qText.length()-30);
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

