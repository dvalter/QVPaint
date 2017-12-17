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
    initToolsMap();
    initToolbar(m_leftToolBar, m_toolsMap.keys(), (Qt::LeftToolBarArea));

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

void QVPMainWindow::initToolsMap()
{
    m_toolsMap.insert(QVP::selectShape, "Selection");
    m_toolsMap.insert(QVP::drawDot, "Dot");
    m_toolsMap.insert(QVP::drawLine, "Line");
    m_toolsMap.insert(QVP::drawEllipse, "Ellipse");
    m_toolsMap.insert(QVP::drawEllipticCurve, "Elliptic Curve");
}

void QVPMainWindow::initToolbar(QToolBar * toolBar, QList<QVP::editorMode> elements, Qt::ToolBarArea area)
{
    QActionGroup* acts = new QActionGroup(this);
    for (QVP::editorMode mode : elements){
        QString name = m_toolsMap[mode];
        QVPAction* act = new QVPAction(QPixmap(":/" + name + ".svg"), name, mode);
        act->setCheckable(true);
        acts->addAction(act);

        QObject::connect(act, &QAction::hovered, this, &QVPMainWindow::hovered);
        QObject::connect(act, &QAction::changed, this, &QVPMainWindow::changed);
        QObject::connect(act, &QAction::triggered, this, &QVPMainWindow::triggered);
        QObject::connect(act, &QAction::toggled, this, &QVPMainWindow::toggled);

        QObject::connect(act, &QVPAction::toggled, this, &QVPMainWindow::updateMode);
        QObject::connect(act, &QVPAction::toggled, m_mainDocument, &QVPDocument::setEditorMode);

        if(mode == QVP::selectShape){
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

    QString name = m_toolsMap[mode];
    m_toolLbl->setText(name.rightJustified(20 - name.length(), ' '));

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
