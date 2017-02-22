/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QScrollBar>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QScrollBar *verticalScrollBar;
    QTreeWidget *treeWidget;
    QLabel *label;
    QScrollArea *scrollArea_2;
    QWidget *scrollAreaWidgetContents_2;
    QScrollBar *verticalScrollBar_2;
    QListWidget *dokument;
    QWidget *widget;
    QVBoxLayout *verticalLayout;
    QSlider *horizontalSlider;
    QProgressBar *progressBar;
    QWidget *widget1;
    QHBoxLayout *horizontalLayout;
    QPushButton *play;
    QPushButton *pause;
    QPushButton *stop;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(518, 379);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        scrollArea = new QScrollArea(centralWidget);
        scrollArea->setObjectName(QStringLiteral("scrollArea"));
        scrollArea->setGeometry(QRect(0, 0, 120, 311));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 118, 309));
        verticalScrollBar = new QScrollBar(scrollAreaWidgetContents);
        verticalScrollBar->setObjectName(QStringLiteral("verticalScrollBar"));
        verticalScrollBar->setGeometry(QRect(100, 0, 16, 311));
        verticalScrollBar->setOrientation(Qt::Vertical);
        treeWidget = new QTreeWidget(scrollAreaWidgetContents);
        treeWidget->headerItem()->setText(0, QString());
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem(treeWidget);
        QTreeWidgetItem *__qtreewidgetitem1 = new QTreeWidgetItem(__qtreewidgetitem);
        __qtreewidgetitem1->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEditable|Qt::ItemIsDragEnabled|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
        new QTreeWidgetItem(treeWidget);
        new QTreeWidgetItem(treeWidget);
        new QTreeWidgetItem(treeWidget);
        treeWidget->setObjectName(QStringLiteral("treeWidget"));
        treeWidget->setGeometry(QRect(0, -20, 101, 331));
        scrollArea->setWidget(scrollAreaWidgetContents);
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(120, 10, 261, 151));
        label->setCursor(QCursor(Qt::WaitCursor));
        label->setPixmap(QPixmap(QString::fromUtf8("../harry_styles.jpeg")));
        label->setScaledContents(true);
        scrollArea_2 = new QScrollArea(centralWidget);
        scrollArea_2->setObjectName(QStringLiteral("scrollArea_2"));
        scrollArea_2->setGeometry(QRect(400, 0, 120, 311));
        scrollArea_2->setWidgetResizable(true);
        scrollAreaWidgetContents_2 = new QWidget();
        scrollAreaWidgetContents_2->setObjectName(QStringLiteral("scrollAreaWidgetContents_2"));
        scrollAreaWidgetContents_2->setGeometry(QRect(0, 0, 118, 309));
        verticalScrollBar_2 = new QScrollBar(scrollAreaWidgetContents_2);
        verticalScrollBar_2->setObjectName(QStringLiteral("verticalScrollBar_2"));
        verticalScrollBar_2->setGeometry(QRect(0, 0, 16, 311));
        verticalScrollBar_2->setOrientation(Qt::Vertical);
        dokument = new QListWidget(scrollAreaWidgetContents_2);
        dokument->setObjectName(QStringLiteral("dokument"));
        dokument->setGeometry(QRect(20, 0, 101, 311));
        scrollArea_2->setWidget(scrollAreaWidgetContents_2);
        widget = new QWidget(centralWidget);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(120, 170, 271, 51));
        verticalLayout = new QVBoxLayout(widget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalSlider = new QSlider(widget);
        horizontalSlider->setObjectName(QStringLiteral("horizontalSlider"));
        horizontalSlider->setCursor(QCursor(Qt::PointingHandCursor));
        horizontalSlider->setMouseTracking(false);
        horizontalSlider->setOrientation(Qt::Horizontal);

        verticalLayout->addWidget(horizontalSlider);

        progressBar = new QProgressBar(widget);
        progressBar->setObjectName(QStringLiteral("progressBar"));
        progressBar->setCursor(QCursor(Qt::WhatsThisCursor));
        progressBar->setValue(24);

        verticalLayout->addWidget(progressBar);

        widget1 = new QWidget(centralWidget);
        widget1->setObjectName(QStringLiteral("widget1"));
        widget1->setGeometry(QRect(180, 230, 155, 29));
        horizontalLayout = new QHBoxLayout(widget1);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        play = new QPushButton(widget1);
        play->setObjectName(QStringLiteral("play"));
        play->setEnabled(true);
        play->setCursor(QCursor(Qt::PointingHandCursor));
        play->setStyleSheet(QStringLiteral("background-color: qradialgradient(spread:repeat, cx:0.5, cy:0.5, radius:0.077, fx:0.5, fy:0.5, stop:0 rgba(0, 169, 255, 147), stop:0.497326 rgba(0, 0, 0, 147), stop:1 rgba(0, 169, 255, 147));"));
        play->setAutoDefault(false);
        play->setDefault(false);
        play->setFlat(false);

        horizontalLayout->addWidget(play);

        pause = new QPushButton(widget1);
        pause->setObjectName(QStringLiteral("pause"));
        pause->setCursor(QCursor(Qt::PointingHandCursor));
        pause->setStyleSheet(QStringLiteral("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(255, 0, 0, 255), stop:0.166 rgba(255, 255, 0, 255), stop:0.333 rgba(0, 255, 0, 255), stop:0.5 rgba(0, 255, 255, 255), stop:0.666 rgba(0, 0, 255, 255), stop:0.833 rgba(255, 0, 255, 255), stop:1 rgba(255, 0, 0, 255));"));

        horizontalLayout->addWidget(pause);

        stop = new QPushButton(widget1);
        stop->setObjectName(QStringLiteral("stop"));
        stop->setCursor(QCursor(Qt::PointingHandCursor));
        stop->setStyleSheet(QStringLiteral("background-color: qradialgradient(spread:pad, cx:0.5, cy:0.5, radius:0.5, fx:0.5, fy:0.5, stop:0 rgba(0, 0, 0, 255), stop:0.19397 rgba(0, 0, 0, 255), stop:0.202312 rgba(122, 97, 0, 255), stop:0.495514 rgba(76, 58, 0, 255), stop:0.504819 rgba(255, 255, 255, 255), stop:0.79 rgba(255, 255, 255, 255), stop:1 rgba(255, 158, 158, 255));"));

        horizontalLayout->addWidget(stop);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 518, 27));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);
        QWidget::setTabOrder(horizontalSlider, play);
        QWidget::setTabOrder(play, pause);
        QWidget::setTabOrder(pause, stop);
        QWidget::setTabOrder(stop, scrollArea);
        QWidget::setTabOrder(scrollArea, scrollArea_2);

        retranslateUi(MainWindow);
        QObject::connect(horizontalSlider, SIGNAL(valueChanged(int)), progressBar, SLOT(setValue(int)));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));

        const bool __sortingEnabled = treeWidget->isSortingEnabled();
        treeWidget->setSortingEnabled(false);
        QTreeWidgetItem *___qtreewidgetitem = treeWidget->topLevelItem(0);
        ___qtreewidgetitem->setText(0, QApplication::translate("MainWindow", "Harry", 0));
        QTreeWidgetItem *___qtreewidgetitem1 = ___qtreewidgetitem->child(0);
        ___qtreewidgetitem1->setText(0, QApplication::translate("MainWindow", "Bild", 0));
        QTreeWidgetItem *___qtreewidgetitem2 = treeWidget->topLevelItem(1);
        ___qtreewidgetitem2->setText(0, QApplication::translate("MainWindow", "Liam", 0));
        QTreeWidgetItem *___qtreewidgetitem3 = treeWidget->topLevelItem(2);
        ___qtreewidgetitem3->setText(0, QApplication::translate("MainWindow", "Niall", 0));
        QTreeWidgetItem *___qtreewidgetitem4 = treeWidget->topLevelItem(3);
        ___qtreewidgetitem4->setText(0, QApplication::translate("MainWindow", "Louis", 0));
        treeWidget->setSortingEnabled(__sortingEnabled);

        label->setText(QString());
#ifndef QT_NO_TOOLTIP
        horizontalSlider->setToolTip(QApplication::translate("MainWindow", "<html><head/><body><p align=\"center\">Slider</p></body></html>", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        progressBar->setToolTip(QApplication::translate("MainWindow", "<html><head/><body><p>Progress</p></body></html>", 0));
#endif // QT_NO_TOOLTIP
        play->setText(QString());
        pause->setText(QString());
#ifndef QT_NO_WHATSTHIS
        stop->setWhatsThis(QApplication::translate("MainWindow", "<html><head/><body><p><br/></p></body></html>", 0));
#endif // QT_NO_WHATSTHIS
        stop->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
