#ifndef QVPACTION_H
#define QVPACTION_H

#include <QAction>
#include "config.h"

class QVPAction : public QAction
{
public:
    QVPAction(const QIcon &icon, const QString &label, const QVP::editorMode mode, QObject *parent=nullptr);
private:
    QVP::editorMode m_mode;
};

#endif // QVPACTION_H
