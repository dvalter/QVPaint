#include "qvpmainwindow.h"
#include "qvpaction.h"

namespace QVP {
    static const char *xdgIcons[] = {[selectShape] =       "edit-select",
            [drawDot] =           "edit-map",
            [drawLine] =          "draw-line",
            [drawEllipse] =       "draw-ellipse",
            [drawEllipticCurve] = "tool_curve",
            [move] =              "transform-move",
            [makeOrtho] =         "snap-orthogonal",
            [crossLine] =         "edit-cut",
            [clipRectangle] =     "select-rectangular",
            [setUp] =             "preferences-activities"};

    static const char *customIcons[] = {[selectShape] =       "cursor-arrow",
            [drawDot] =           "edit-map",
            [drawLine] =          "draw-line",
            [drawEllipse] =       "draw-ellipse",
            [drawEllipticCurve] = "draw-arc",
            [move] =              "move",
            [makeOrtho] =         "orthogonal",
            [crossLine] =         "cut-line",
            [clipRectangle] =     "cut-rectangular",
            [setUp] =             "shape-params"};
}

QVPMainWindow::QVPMainWindow(QWidget *parent)
        : QMainWindow(parent),
          m_scrollArea(new QScrollArea(this)),
          m_leftToolBar(new QToolBar("Tools")),
          m_mainDocument(new QVPDocument(this)),
          m_coordXlbl(new QLabel(this)),
          m_coordYlbl(new QLabel(this)),
          m_toolLbl(new QLabel(this)),
          m_messageLbl(new QLabel(this)),
          m_toolActionGroup(new QActionGroup(this)) {

    initToolsList();
    initToolbar(m_leftToolBar, m_toolsList, (Qt::LeftToolBarArea));

    m_scrollArea->setBackgroundRole(QPalette::Dark);
    m_scrollArea->setWidget(m_mainDocument);

    m_scrollArea->setVisible(true);
    setCentralWidget(m_scrollArea);

    m_coordXlbl->setFont(QFont("Monospace"));
    m_coordYlbl->setFont(QFont("Monospace"));
    m_toolLbl->setFont(QFont("Monospace"));
    m_coordXlbl->setText(QString("X:").append(QString::number(0, 'd', 5)));
    m_coordYlbl->setText(QString("Y:").append(QString::number(0, 'd', 5)));

    m_messageLbl->setStyleSheet("font-weight: bold; color: red");
    m_messageLbl->setText(""/*EARLY PRE-ALPHA!"*/);
    statusBar()->addWidget(m_coordXlbl);
    statusBar()->addWidget(m_coordYlbl);
    statusBar()->addWidget(m_toolLbl);
    statusBar()->addWidget(m_messageLbl);

    connect(m_mainDocument, &QVPDocument::updateCoord, this, &QVPMainWindow::coordUpdated);
    connect(m_mainDocument, &QVPDocument::switchToSelection, this, &QVPMainWindow::resetToSelection);
    connect(m_mainDocument, &QVPDocument::sendMsgToUI, this, &QVPMainWindow::putMessage);
    connect(m_mainDocument, &QVPDocument::showSetupWindow, this, &QVPMainWindow::openNewWindow);
    resetToSelection();
}


void QVPMainWindow::coordUpdated(QPoint coord) {
    m_coordXlbl->setText(QString().sprintf("%s: %4d", "X", coord.x()));
    m_coordYlbl->setText(QString().sprintf("%s: %4d", "Y", coord.y()));
}

QVPMainWindow::~QVPMainWindow() {
    for (auto &act : m_toolActionGroup->actions()) {
        m_toolActionGroup->removeAction(act);
        delete act;
    }

}

void QVPMainWindow::initToolsList() {
    m_toolsList.append(QVPToolPair(QVP::selectShape, "Selection"));
    m_toolsList.append(QVPToolPair(QVP::drawDot, "Dot"));
    m_toolsList.append(QVPToolPair(QVP::drawLine, "Line"));
    m_toolsList.append(QVPToolPair(QVP::drawEllipse, "Ellipse"));
    m_toolsList.append(QVPToolPair(QVP::drawEllipticCurve, "Elliptic curve"));
    m_toolsList.append(QVPToolPair(QVP::move, "Move"));
    m_toolsList.append(QVPToolPair(QVP::makeOrtho, "Orthogonal line"));
    m_toolsList.append(QVPToolPair(QVP::crossLine, "Cut with line"));
    m_toolsList.append(QVPToolPair(QVP::clipRectangle, "Clip with rectangle"));
    m_toolsList.append(QVPToolPair(QVP::setUp, "Set up shape"));
}

void QVPMainWindow::initToolbar(QToolBar *toolBar, QList<QVPToolPair> elements, Qt::ToolBarArea area) {
    QMenu *const fileMenu = menuBar()->addMenu("&File");
    fileMenu->addAction("&Open...", this, SLOT(open()), QKeySequence::Open);
    fileMenu->addAction("&Save As...", this, SLOT(save()), QKeySequence::SaveAs);
    fileMenu->addSeparator();
    fileMenu->addAction(tr("E&xit"), this, SLOT(close()), QKeySequence::Quit);

    m_toolMenu = menuBar()->addMenu("&Tools");

    int counter = 0;
    for (QVPToolPair mode : elements) {

        auto fallback = QPixmap(QCoreApplication::applicationDirPath() + "/imgs/" +
                                QVP::customIcons[mode.first] + ".svg");
        auto *act = new QVPAction(QIcon::fromTheme(QVP::xdgIcons[mode.first], fallback), mode.second, mode.first);

        act->setCheckable(true);
        m_toolActionGroup->addAction(act);

        m_toolMenu->addAction(act);


        QObject::connect(act, &QVPAction::toggled, this, &QVPMainWindow::updateMode);
        QObject::connect(act, &QVPAction::toggled, m_mainDocument, &QVPDocument::setEditorMode);

        if (mode.first == QVP::selectShape) {
            act->setChecked(true);
            m_selectAction = act;
        }

        if (counter > 4) {
            act->setEnabled(false);
            QObject::connect(m_mainDocument, &QVPDocument::shapeSelected, act, &QVPAction::enable);
        }
        counter++;
    }

    m_toolActionGroup->setExclusive(true);

    toolBar->addActions(m_toolActionGroup->actions());
    toolBar->setFloatable(false);
    toolBar->setMovable(false);
    addToolBar(area, m_leftToolBar);
}

void QVPMainWindow::updateMode(QVP::editorMode mode) {

    QString name("None");
    for (QVPToolPair pair : m_toolsList) {
        if (pair.first == mode) {
            name = pair.second;
        }
    }
    m_toolLbl->setText(name.rightJustified(20, ' '));

}

void QVPMainWindow::open() {
    const QString fileName =
            QFileDialog::getOpenFileName(
                    this, "Open File", QDir::currentPath());
    if (!fileName.isEmpty()) {
        m_mainDocument->loadFromFile(fileName);
    }
}

void QVPMainWindow::save() {
    const QString initialPath = QDir::currentPath() + "/untitled.qvpi";

    const QString fileName =
            QFileDialog::getSaveFileName(
                    this, "Save As", initialPath, "QVPI Files (*.qvpi);;All Files (*)");

    if (!(!fileName.isEmpty() && m_mainDocument->saveToFile(fileName)))
        qCritical() << "FAILED TO SAVE";
}

void QVPMainWindow::resetToSelection() {
    if (m_selectAction) {
        m_selectAction->setChecked(true);
    }
}

void QVPMainWindow::putMessage(QString text, bool isError) {
    if (isError) {
        m_messageLbl->setStyleSheet("font-weight: bold; color: red");
    } else {
        m_messageLbl->setStyleSheet("font-weight: regular; color: light gray");
    }
    m_messageLbl->setText(text);
    qDebug() << text << " " << isError;
}

void QVPMainWindow::openNewWindow(QWidget *wgt) {
    qDebug() << wgt;
    wgt->show();
}

void QVPMainWindow::keyPressEvent(QKeyEvent *ke) {
    qDebug() << ke;
    if (ke->key() == Qt::Key_Escape) {
        ke->accept();
        resetToSelection();
    } else {
        ke->ignore();
    }
}
