#ifndef QVPMAINWINDOW_H
#define QVPMAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
//#include <boost/bimap.hpp>
#include "qvpdocument.h"
#include "config.h"

typedef QPair<QVP::editorMode, QString> QVPToolPair;

class QVPMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QVPMainWindow(QWidget *parent = 0);
    ~QVPMainWindow();


public slots:

    void coordUpdated(QPoint coord);
    void updateMode(QVP::editorMode);
    void open();
    void save();



private:
    QScrollArea* m_scrollArea;
    QToolBar* m_leftToolBar;
    //QLabel* m_imgLbl;
    QVPDocument* m_mainDocument;
    QLabel* m_coordXlbl;
    QLabel* m_coordYlbl;
    QLabel* m_toolLbl;
    QLabel* m_messageLbl;
    QList<QVPToolPair> m_toolsList;
    QActionGroup* m_toolActionGroup;


    void initToolbar(QToolBar*, QList<QVPToolPair>, Qt::ToolBarArea);
    void initToolsList();
};


#endif // QVPMAINWINDOW_H
