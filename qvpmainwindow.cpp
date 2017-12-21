#include "qvpmainwindow.h"
#include <stdlib.h>
#include "qvpaction.h"

QString msgLabelText(const QString& str)
{
    QString labelText = "<P><b><i><FONT COLOR='#ff0000' FONT SIZE = 4>";
    labelText .append(str);
    labelText .append("</i></b></P></br>");
    return labelText;
}

QVPMainWindow::QVPMainWindow(QWidget *parent)
    : QMainWindow(parent),
      m_scrollArea(new QScrollArea),
      m_leftToolBar(new QToolBar("Tools")),
      m_mainDocument(new QVPDocument),
      m_coordXlbl(new QLabel),
      m_coordYlbl(new QLabel),
      m_toolLbl(new QLabel),
      m_messageLbl(new QLabel)
{

    //QStringList toolbarElementsList({"selection", "dot", "line", "ellipse", "elliptic-curve", "cross"});
    initToolsList();
    initToolbar(m_leftToolBar, m_toolsList, (Qt::LeftToolBarArea));

    m_scrollArea->setBackgroundRole(QPalette::Dark);
    m_scrollArea->setWidget(m_mainDocument);

    m_scrollArea->setVisible(true);
    setCentralWidget(m_scrollArea);

    m_coordXlbl->setFont(QFont("Monospace"));
    m_coordYlbl->setFont(QFont("Monospace"));
    m_toolLbl->setFont(QFont("Monospace"));
    m_coordXlbl->setText(QString("X:").append(QString::number(0, 'd', 5)));
    m_coordYlbl->setText(QString("Y:").append(QString::number(0, 'd', 5)));

    m_messageLbl->setStyleSheet("font-weight: bold; color: red");
    m_messageLbl->setText("EARLY PRE-ALPHA!");
    statusBar()->addWidget(m_coordXlbl);
    statusBar()->addWidget(m_coordYlbl);
    statusBar()->addWidget(m_toolLbl);
    statusBar()->addWidget(m_messageLbl);

    connect(m_mainDocument, &QVPDocument::updateCoord, this, &QVPMainWindow::coordUpdated);

}

void QVPMainWindow::coordUpdated(QPoint coord)
{
    m_coordXlbl->setText(QString().sprintf("%s: %4d", "X", coord.x()));
    m_coordYlbl->setText(QString().sprintf("%s: %4d", "Y", coord.y()));
}

QVPMainWindow::~QVPMainWindow()
{

}

void QVPMainWindow::initToolsList()
{
    m_toolsList.append(QVPToolPair(QVP::selectShape, "Selection"));
    m_toolsList.append(QVPToolPair(QVP::drawDot, "Dot"));
    m_toolsList.append(QVPToolPair(QVP::drawLine, "Line"));
    m_toolsList.append(QVPToolPair(QVP::drawEllipse, "Ellipse"));
    m_toolsList.append(QVPToolPair(QVP::drawEllipticCurve, "Elliptic Curve"));
}

void QVPMainWindow::initToolbar(QToolBar * toolBar, QList<QVPToolPair > elements, Qt::ToolBarArea area)
{
    QActionGroup* acts = new QActionGroup(this);
    for (QVPToolPair mode : elements){
        QVPAction* act = new QVPAction(QPixmap(":/" + mode.second + ".svg"), mode.second, mode.first);
        act->setCheckable(true);
        acts->addAction(act);

//        QObject::connect(act, &QAction::hovered, this, &QVPMainWindow::hovered);
//        QObject::connect(act, &QAction::changed, this, &QVPMainWindow::changed);
//        QObject::connect(act, &QAction::triggered, this, &QVPMainWindow::triggered);
//        QObject::connect(act, &QAction::toggled, this, &QVPMainWindow::toggled);

        QObject::connect(act, &QVPAction::toggled, this, &QVPMainWindow::updateMode);
        QObject::connect(act, &QVPAction::toggled, m_mainDocument, &QVPDocument::setEditorMode);

        if(mode.first == QVP::selectShape){
            act->setChecked(true);
        }
    }
    toolBar->addActions(acts->actions());
    toolBar->setFloatable(false);
    toolBar->setMovable(false);
    addToolBar(area, m_leftToolBar);
}

void QVPMainWindow::updateMode(QVP::editorMode mode)
{

    QString name("None");
    for (QVPToolPair pair : m_toolsList){
        if(pair.first == mode){
            name = pair.second;
        }
    }
    m_toolLbl->setText(name.rightJustified(20, ' '));

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
