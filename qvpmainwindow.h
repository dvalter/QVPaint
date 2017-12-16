#ifndef QVPMAINWINDOW_H
#define QVPMAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
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

private:
    QScrollArea* m_scrollArea;
    QToolBar* m_leftToolBar;
    //QLabel* m_imgLbl;
    QVPDocument* m_mainDocument;

    void initToolbar(QToolBar*, QStringList, Qt::ToolBarArea);
};


#endif // QVPMAINWINDOW_H
