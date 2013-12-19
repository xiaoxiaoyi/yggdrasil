/**
 * \file GraphicsBoxItem.hpp
 * \author Hekiat
 * \date 23 july 2013
 */

#ifndef __GRAPHICSCURRENTFRAMEVIEW_HPP__
#define __GRAPHICSCURRENTFRAMEVIEW_HPP__

#include <QtGui>
#include <QGraphicsView>

namespace yggeditor{

enum CurrentFrameViewState{
    NONE,
    CREATE_BOX
};

/**
 * \class GraphicsCurrentFrameView GraphicsCurrentFrameView.hpp
 * \brief Inherit from QGraphicsView.
 *        It's a basic display area, contening graphic items.
 */
class GraphicsCurrentFrameView : public QGraphicsView {
    Q_OBJECT
public:
    GraphicsCurrentFrameView();
    void createBox(const int x, const int y, const int w, const int h);
protected:
    void wheelEvent ( QWheelEvent * event );
    void mousePressEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);

private:
    QGraphicsScene * m_CurrentFrameScene;
    CurrentFrameViewState m_State;
};

}


#endif // __GRAPHICSCURRENTFRAMEVIEW_HPP__
