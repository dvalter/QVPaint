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
