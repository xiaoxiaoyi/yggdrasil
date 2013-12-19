#include "GLWidget.hpp"
#include <stdexcept>
#include <iostream>
#include <QMouseEvent>
#include <QOpenGLContext>

#include "MapEditorWindow.hpp"

#if 0

GLWidget::GLWidget(const QSurfaceFormat& format, MainWindow * mainwindow, QWidget * parent)
    : QGLWidget(/*format, */parent), glFormat(format), m_Mainwindow(mainwindow){
    contextGL = new QOpenGLContext;
    contextGL->setFormat( format );
    contextGL->create();
}

void GLWidget::initializeGL() {
    contextGL->makeCurrent(windowHandle());
    m_GLFunctions = contextGL->versionFunctions();
}

void GLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT);

    glPointSize(10);
    glLineWidth(2);

    for(auto o : m_GObjects){
        glBindVertexArray(o.vao);
        glDrawArrays(o.mode, 0, o.size);
        glBindVertexArray(0);
    }

    swapBuffers();
    std::cout << "Refresh" << std::endl;
}

void GLWidget::resizeGL(int width, int height) {
    glViewport(0, 0, (GLint)width, (GLint)height);
}

void GLWidget::mousePressEvent(QMouseEvent * event) {
    m_Mainwindow->detectedClickOnGLWidget(event);
}

void GLWidget::addStaticChain(const std::vector<glm::vec2> & sc, const bool loop){
    GLuint vbo;
    GLuint vao;

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * sc.size(), sc.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_TRUE, 0, nullptr);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    if(loop)
        m_GObjects.push_back(GLWidgetObject{vao, vbo, GL_LINE_LOOP, sc.size()});
    else
        m_GObjects.push_back(GLWidgetObject{vao, vbo, GL_LINE_STRIP, sc.size()});
    update();
}

void GLWidget::clearObjects(void){
    m_GObjects.clear();
}

#endif
