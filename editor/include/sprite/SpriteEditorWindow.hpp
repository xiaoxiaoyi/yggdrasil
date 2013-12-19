#ifndef __SPRITEEDITORWINDOW_HPP__
#define __SPRITEEDITORWINDOW_HPP__

#include <QMainWindow>
#include <memory>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QTextEdit>

#include <QGraphicsObject>
#include <QGraphicsWidget>
#include <QGraphicsView>
#include <QVBoxLayout>
#include <QtDeclarative/QDeclarativeView>

#include "common_editor.hpp"
#include "GraphicsBoxItem.hpp"
#include "GraphicsCurrentFrameView.hpp"
#include "CurrentFrameWidget.hpp"

namespace Ui {
class SpriteEditorWindow;
}

class GLSpriteEditorWidget;

class SpriteEditorWindow : public QMainWindow {
    Q_OBJECT
    
public:
    explicit SpriteEditorWindow(QWidget *parent = 0);
    ~SpriteEditorWindow();
    void resizeEvent(QResizeEvent * e);

public slots:
    void openImage(void);
    void switchToMapEditor(void);

private:
    Ui::SpriteEditorWindow * ui;

    // Animation
    QTextEdit * m_Animation;

    // TimeLine
    QWidget * m_TimeLine;
    QDeclarativeView * m_TimeLineQmlView;
    QVBoxLayout * m_TimeLineLayout;

    // Current Frame
    yggeditor::CurrentFrameWidget * m_CurrentFrameWidget;
    QVBoxLayout * m_CurrentFrameLayout;

    // Window
    QMdiArea m_CentralArea; // Subwindow Container
    QMdiSubWindow * m_CurrentFrameWindow;
    QMdiSubWindow * m_AnimationWindow;
    QMdiSubWindow * m_TimeLineWindows;
};

#endif // __SPRITEEDITORWINDOW_HPP__
