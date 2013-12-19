#include "MapEditorWindow.hpp"
#include "sprite/SpriteEditorWindow.hpp"
#include "tinyxml2.h"

#include "ui_MapEditorWindow.h"

#define QT_NO_OPENGL
#include <QtWidgets>

#include <iostream>

#if 0
MainWindow::MainWindow(QWidget * parent) : QMainWindow(parent), ui(new Ui::MainWindow), m_BuildingChain(false) {
    ui->setupUi(this);

    glFormat.setVersion( 3, 3 );
    glFormat.setProfile( QGLFormat::CoreProfile );
    //glFormat.setSampleBuffers( true );

    glWidget = new GLWidget( glFormat, this, this );
    this->setCentralWidget(glWidget);

    // Connect signal and slot
    //Menu
    connect(ui->action_Switch_to_Sprite_Editor, SIGNAL(triggered()), this, SLOT(switchToSpriteEditor()));
    connect(ui->action_Open_Scene, SIGNAL(triggered()), this, SLOT(openScene()));
    connect(ui->action_Save_Scene, SIGNAL(triggered()), this, SLOT(saveScene()));
    connect(ui->action_New_Scene, SIGNAL(triggered()), this, SLOT(newScene()));

    //Dock
    connect(ui->action_Insert_Platform, SIGNAL(triggered()), this, SLOT(switchDock()));
    this->addDockWidget(Qt::DockWidgetArea::LeftDockWidgetArea, &m_PlatformDockWidget);
    m_PlatformDockWidget.setVisible(false);
}

MainWindow::~MainWindow() {
    delete glWidget;
    delete ui;
}

void MainWindow::detectedClickOnGLWidget(QMouseEvent * event){
    if( ui->action_Insert_Platform->isChecked()){
        glWidget->clearObjects();

        if( event->button() == Qt::LeftButton ){
            QPoint point = event->pos();
            std::cout << "Add position (" << float(point.x())/glWidget->width()*2.0 - 1.0 << ", " << (1.0 - float(point.y())/glWidget->height())*2.0 - 1.0 << ")" << std::endl;

            if( !m_BuildingChain ){
                ygg::StaticChain sc;
                sc.m_Vertices.push_back(glm::vec2(float(point.x())/glWidget->width()*2.0 - 1.0, (1.0 - float(point.y())/glWidget->height())*2.0 - 1.0));
                sc.m_bLoop = false;
                m_Map.m_StaticChains.push_back(sc);
                m_BuildingChain = true;
            }

            else{
                m_Map.m_StaticChains.back().m_Vertices.push_back( glm::vec2(
                                float(point.x())/glWidget->width() * 2.0 - 1.0,
                                (1.0 - float(point.y())/glWidget->height()) * 2.0 - 1.0));
            }


        }

        else if( event->button() == Qt::RightButton && m_BuildingChain){
            m_Map.m_StaticChains.back().m_bLoop = m_PlatformDockWidget.widget()->findChild<QRadioButton*>("loopRadioButton")->isChecked();
            m_BuildingChain = false;
        }

        for(auto o : m_Map.m_StaticChains)
            glWidget->addStaticChain(o.m_Vertices, o.m_bLoop);
    }
}

void MainWindow::saveAsXML(const std::string fileName) const{
    tinyxml2::XMLDocument doc;
    doc.Parse("<?xml version=\"1.0\" encoding=\"UTF-8\"?>");

    tinyxml2::XMLElement * mapNode = doc.NewElement("map");
    mapNode->SetAttribute("player", "");


    //tinyxml2::XMLText * text = doc.NewText("1.0.0");

    for(auto sc : m_Map.m_StaticChains){
        tinyxml2::XMLElement * scNode = doc.NewElement("chain");
        scNode->SetAttribute("loop", sc.m_bLoop);

        for(auto vx : sc.m_Vertices){
            tinyxml2::XMLElement * vxNode = doc.NewElement("vertex");

            vxNode->SetAttribute("x", vx.x);
            vxNode->SetAttribute("y", vx.y);

            scNode->InsertEndChild(vxNode);
        }
        mapNode->InsertEndChild(scNode);
    }

    doc.InsertEndChild(mapNode);

    /*tinyxml2::XMLPrinter printer;
    doc.Print();*/
    doc.SaveFile(fileName.c_str());
}

// Public Slot

void MainWindow::openScene(void){
    QString fileName = QFileDialog::getOpenFileName(this, "Open File", QString(), "XML (*.xml)");

    if(fileName.size() == 0)
        return;

    //Reset Map & Widget
    glWidget->clearObjects();
    m_Map.m_StaticChains.clear();

    m_Map.loadFromXML(fileName.toStdString().c_str());

    for(auto o : m_Map.m_StaticChains)
        glWidget->addStaticChain(o.m_Vertices, o.m_bLoop);
}

void MainWindow::saveScene(void){
    QString fileName = QFileDialog::getSaveFileName(this, "Save File", QString(), "XML (*.xml)");

    if(fileName.size() == 0)
        return;

    saveAsXML(fileName.toStdString());
}

void MainWindow::newScene(void){
    glWidget->clearObjects();
    m_Map.m_StaticChains.clear();
    glWidget->update();
    m_BuildingChain = false;
}


void MainWindow::switchToSpriteEditor(void){
    qApp->exit(ygg::SPRITE_EDITOR_ID);
}

void MainWindow::switchDock(void){
    if( ui->action_Insert_Platform->isChecked())
        m_PlatformDockWidget.setVisible(true);

    else
        m_PlatformDockWidget.setVisible(false);
}

#endif
