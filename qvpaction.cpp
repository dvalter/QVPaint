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

#include "qvpaction.h"


QVPAction::QVPAction(const QIcon &icon, const QString &label, const QVP::editorMode mode, QObject *parent) :
        QAction(icon, label, parent),
        m_mode(mode) {
    connect(this, &QAction::toggled, this, &QVPAction::slotToggled);
}

void QVPAction::slotToggled(bool state) {
    if (state) {
        emit toggled(m_mode);
    }
}

void QVPAction::enable(bool state) {
    QAction::setEnabled(state);
}
