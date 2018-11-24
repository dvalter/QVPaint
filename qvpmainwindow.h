/*
 * Copyright (C) 2017-2018 Dmitry Valter
 *
 * QVPaint is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License, version 3
 *  or the GNU Lesser General Public License, version 3 as published
 *  by the Free Software Foundation or either under the terms of the
 *  Do What the Fuck You Want to Public License, Version 2 with
 *  The No Warranty Exception 1.0.
 *
 *  QVPaint is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License,  for more details.
 *
 *  You should have received a copy of the GNU General Public License,
 *  GNU Lesser General Public License and Do What the Fuck You
 *  Want to Public License along with QVPaint. If not, see
 *  <https://www.gnu.org/licenses/> and <http://www.wtfpl.net>.
 */

#ifndef QVPMAINWINDOW_H
#define QVPMAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include "qvpdocument.h"
#include "config.h"

typedef QPair<QVP::editorMode, QString> QVPToolPair;

class QVPMainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit QVPMainWindow(QWidget *parent = nullptr);

    ~QVPMainWindow() override;


public slots:

    void coordUpdated(QPoint coord);

    void updateMode(QVP::editorMode);

    void open();

    void save();

    void resetToSelection();

    void putMessage(QString text, bool isError);

    void openNewWindow(QWidget *window);


private:
    QScrollArea *m_scrollArea;
    QToolBar *m_leftToolBar;
    QVPDocument *m_mainDocument;
    QLabel *m_coordXlbl;
    QLabel *m_coordYlbl;
    QLabel *m_toolLbl;
    QLabel *m_messageLbl;
    QList<QVPToolPair> m_toolsList;
    QActionGroup *m_toolActionGroup;
    QAction *m_selectAction = nullptr;
    QMenu *m_toolMenu = nullptr;

protected:
    void keyPressEvent(QKeyEvent *ke) override;

    void initToolbar(QToolBar *, QList<QVPToolPair>, Qt::ToolBarArea);

    void initToolsList();
};


#endif // QVPMAINWINDOW_H
