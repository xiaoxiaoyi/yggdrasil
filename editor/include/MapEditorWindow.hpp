#ifndef __MAINWINDOW_H__
#define __MAINWINDOW_H__

#include <QMainWindow>
#include <GLWidget.hpp>

#include "game/Map.hpp"
#include "common_editor.hpp"
#include "PlatformDockWidget.hpp"

#if 0
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget * parent = nullptr);
    ~MainWindow();
    void detectedClickOnGLWidget(QMouseEvent * event);
    void saveAsXML(const std::string fileName) const;

public slots:
    void openScene(void);
    void saveScene(void);
    void newScene(void);
    void switchToSpriteEditor(void);
    void switchDock(void);

private:

    // Ui
    Ui::MainWindow * ui;

    // Local Data
    QGLFormat glFormat;
    //GLWidget * glWidget;

    //Dock
    PlatformDockWidget m_PlatformDockWidget;

    ygg::Map m_Map;

    //State variable
    bool m_BuildingChain;
};
#endif

#endif // __MAINWINDOW_H__
