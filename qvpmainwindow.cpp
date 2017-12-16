#include "qvpmainwindow.h"
#include <stdlib.h>

QVPMainWindow::QVPMainWindow(QWidget *parent)
    : QMainWindow(parent),
      m_scrollArea(new QScrollArea),
      m_leftToolBar(new QToolBar("Tools")),
      //m_imgLbl(new QLabel)
      m_mainDocument(new QVPDocument)
{
    QStringList toolbarElementsList({"selection", "dot", "line", "ellipse", "elliptic-curve", "cross"});
    initToolbar(m_leftToolBar, toolbarElementsList, (Qt::LeftToolBarArea));
//    QImage img(":/sample.png");
//    m_imgLbl->setBackgroundRole(QPalette::Base);
//    m_imgLbl->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
//    m_imgLbl->setScaledContents(true);
//    m_imgLbl->setPixmap(QPixmap::fromImage(img));

    m_scrollArea->setBackgroundRole(QPalette::Dark);
    m_scrollArea->setWidget(m_mainDocument);
//    m_scrollArea->setVisible(false);



    m_scrollArea->setVisible(true);
    setCentralWidget(m_scrollArea);
//    setCentralWidget(m_mainDocument);
}

QVPMainWindow::~QVPMainWindow()
{

}



void QVPMainWindow::initToolbar(QToolBar * toolBar, QStringList elements, Qt::ToolBarArea area)
{
    QActionGroup* acts = new QActionGroup(this);
    for (QString name : elements){
        QAction* act = new QAction(QPixmap(":/" + name + ".svg"),
                                  name);
        act->setCheckable(true);
        acts->addAction(act);
        QObject::connect(act, &QAction::hovered, this, &QVPMainWindow::hovered);
        QObject::connect(act, &QAction::changed, this, &QVPMainWindow::changed);
        QObject::connect(act, &QAction::triggered, this, &QVPMainWindow::triggered);
        QObject::connect(act, &QAction::toggled, this, &QVPMainWindow::toggled);
    }
    toolBar->addActions(acts->actions());
    toolBar->setFloatable(false);
    toolBar->setMovable(false);
    addToolBar(area, m_leftToolBar);
}

void QVPMainWindow::toggled(bool b){
    qDebug() << __FUNCTION__ << " " << (unsigned long long)QObject::sender() << " value:" << b;
}

void QVPMainWindow::triggered(bool b){
    static QSet<QObject*> set;
    QObject* sender = QObject::sender();
    if (!set.contains(sender)){
        set.insert(sender);
    } else {
        QAction* btn = qobject_cast<QAction*>(sender);

//        if (btn->isChecked()){
//            btn->setChecked(false);
//        }
    }
    qDebug() << __FUNCTION__ << " " << (unsigned long long)sender << " value:" << b;
}

void QVPMainWindow::hovered(){
    qDebug() << __FUNCTION__ << " " << (unsigned long long)QObject::sender();
}

void QVPMainWindow::changed(){
    qDebug() << __FUNCTION__ << " " << (unsigned long long)QObject::sender();
}
