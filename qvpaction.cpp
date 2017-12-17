#include "qvpaction.h"


QVPAction::QVPAction(const QIcon& icon, const QString& label, const QVP::editorMode mode, QObject* parent):
    QAction(icon, label, parent),
    m_mode(mode)
{

}
