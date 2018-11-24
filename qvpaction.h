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

#ifndef QVPACTION_H
#define QVPACTION_H

#include <QAction>
#include "config.h"

class QVPAction : public QAction {
Q_OBJECT
public:
    QVPAction(const QIcon &icon, const QString &label, QVP::editorMode mode, QObject *parent = nullptr);

public slots:

    void enable(bool state);

signals:

    void toggled(QVP::editorMode mode);

private:
    QVP::editorMode m_mode;

private slots:

    void slotToggled(bool state);
};

#endif // QVPACTION_H
