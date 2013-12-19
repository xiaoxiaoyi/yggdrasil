#include "ui_SpriteEditorWindow.h"
#include "sprite/SpriteEditorWindow.hpp"

#include <stdexcept>
#include <iostream>

#include <QGLWidget>
#include <QMouseEvent>
#include <QFileDialog>

SpriteEditorWindow::SpriteEditorWindow(QWidget * parent) :
    QMainWindow(parent), ui(new Ui::SpriteEditorWindow) {
    ui->setupUi(this);

    // Set Subpart
    m_CurrentFrameWidget = new yggeditor::CurrentFrameWidget;
    m_Animation = new QTextEdit;
    m_TimeLine = new QWidget;

    //Split Window
    m_CurrentFrameWindow = m_CentralArea.addSubWindow(m_CurrentFrameWidget, Qt::FramelessWindowHint);
    m_AnimationWindow = m_CentralArea.addSubWindow(m_Animation, Qt::FramelessWindowHint);
    m_TimeLineWindows = m_CentralArea.addSubWindow(m_TimeLine, Qt::FramelessWindowHint);

    //Create Widgets

    ///Current Frame
    m_CurrentFrameLayout = new QVBoxLayout();
    m_CurrentFrameLayout->addWidget(m_CurrentFrameWidget->getView());
    m_CurrentFrameWidget->setLayout(m_CurrentFrameLayout);

    ///TimeLine
    m_TimeLineQmlView = new QDeclarativeView;
    m_TimeLineQmlView->setSource(QUrl::fromLocalFile("ui/TimeLine.qml"));

    m_TimeLineLayout = new QVBoxLayout(m_TimeLine);
    m_TimeLineLayout->addWidget(m_TimeLineQmlView);
    m_TimeLineQmlView->setResizeMode(QDeclarativeView::SizeRootObjectToView);

    this->setCentralWidget(&m_CentralArea);

    // Connection
    connect(ui->actionOpenImage, SIGNAL(triggered()), this, SLOT(openImage()));
    connect(ui->actionSwitch_to_Map_Editor, SIGNAL(triggered()), this, SLOT(switchToMapEditor()));
}

SpriteEditorWindow::~SpriteEditorWindow() {
    delete ui;
}

void SpriteEditorWindow::resizeEvent(QResizeEvent * e) {
    int currentFrameWindowWidthTmp = e->size().width()/2;
    int currentFrameWindowHeightTmp = 3*(e->size().height() - menuBar()->height() - statusBar()->height())/4;
    int currentFrameWindowSize = (currentFrameWindowWidthTmp < currentFrameWindowHeightTmp)
            ? currentFrameWindowWidthTmp : currentFrameWindowHeightTmp;

    static bool firstInit(true);

    if(!firstInit)
        m_CurrentFrameWidget->getView()->scale(float(currentFrameWindowSize) / m_CurrentFrameWindow->width(), float(currentFrameWindowSize )/ m_CurrentFrameWindow->height());
    firstInit = false;

    m_CurrentFrameWindow->resize( currentFrameWindowSize,  currentFrameWindowSize);
    m_AnimationWindow->resize( e->size().width()/2, 3*(e->size().height() - menuBar()->height() - statusBar()->height())/4 );
    m_TimeLineWindows->resize( e->size().width(), (e->size().height() - menuBar()->height() - statusBar()->height())/4 );

    m_AnimationWindow->move(e->size().width()/2, 0);
    m_TimeLineWindows->move(0, 3*(e->size().height() - menuBar()->height() - statusBar()->height())/4 );
}

void SpriteEditorWindow::openImage(void) {
    QString file = QFileDialog::getOpenFileName(this, "Open File", QString(), "Images (*.png *.gif *.jpg *.jpeg)");

    //label.show();
    //m_pGLWidget->m_Pixmap.load(file);
    //m_pGLWidget->m_Pixmap = m_pGLWidget->m_Pixmap.transformed(scale);
}

void SpriteEditorWindow::switchToMapEditor(void){
    qApp->exit(ygg::MAP_EDITOR_ID);
}
