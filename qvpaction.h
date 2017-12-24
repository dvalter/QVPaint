#ifndef QVPACTION_H
#define QVPACTION_H

#include <QAction>
#include "config.h"

class QVPAction : public QAction
{
    Q_OBJECT
public:
    QVPAction(const QIcon &icon, const QString &label, const QVP::editorMode mode, QObject *parent=nullptr);

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
