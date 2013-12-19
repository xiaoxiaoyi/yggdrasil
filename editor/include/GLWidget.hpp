#ifndef __GLWIDGET_HPP__
#define __GLWIDGET_HPP__

#include <QGLWidget>
#include <QOpenGLFunctions>

#include "game/Map.hpp"

#if 0

class MainWindow;

struct GLWidgetObject{
    GLuint vao;
    GLuint vbo;
    GLenum mode;
    size_t size;
};

class GLWidget : public QGLWidget {
    Q_OBJECT

public:
    GLWidget(const QSurfaceFormat& format, MainWindow * mainwindow, QWidget * parent = nullptr);
    void addStaticChain(const std::vector<glm::vec2> & sc, const bool loop);
    void clearObjects(void);

protected:
    virtual void initializeGL();
    virtual void paintGL();
    virtual void resizeGL(int width, int height);

private:
    void mousePressEvent(QMouseEvent *event);

    //QGLShaderProgram m_shader;
    //QGLBuffer m_vertexBuffer;
    std::vector<GLWidgetObject> m_GObjects;
    QOpenGLContext* contextGL;
    const QSurfaceFormat& glFormat;
    QOpenGLFunctions* m_GLFunctions;

    MainWindow * m_Mainwindow;
};

#endif

#endif // __GLWIDGET_HPP__
