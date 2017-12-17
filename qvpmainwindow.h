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

    void initToolbar(QToolBar*, QStringList, Qt::ToolBarArea);
};


#endif // QVPMAINWINDOW_H
