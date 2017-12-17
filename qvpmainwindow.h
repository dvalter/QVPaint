#ifndef QVPMAINWINDOW_H
#define QVPMAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
//#include <boost/bimap.hpp>
#include "qvpdocument.h"
#include "config.h"

class QVPMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QVPMainWindow(QWidget *parent = 0);
    ~QVPMainWindow();


public slots:
    void triggered(bool);
    void toggled(bool);
    void hovered();
    void changed();
    void coordUpdated(QPoint coord);
    void updateMode(QVP::editorMode);

//signals:
//    void updateCoord(QPoint coord);

private:
    QScrollArea* m_scrollArea;
    QToolBar* m_leftToolBar;
    //QLabel* m_imgLbl;
    QVPDocument* m_mainDocument;
    QLabel* m_coordXlbl;
    QLabel* m_coordYlbl;
    QLabel* m_toolLbl;
    QLabel* m_messageLbl;
    QMap<QVP::editorMode, QString> m_toolsMap;

    void initToolbar(QToolBar*, QList<QVP::editorMode>, Qt::ToolBarArea);
    void initToolsMap();
};


#endif // QVPMAINWINDOW_H
