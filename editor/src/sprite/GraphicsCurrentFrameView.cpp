#include "sprite/GraphicsCurrentFrameView.hpp"
#include "sprite/GraphicsBoxItem.hpp"

#include <QPixmap>
#include <QGraphicsItem>
#include <iostream>

namespace yggeditor{

GraphicsCurrentFrameView::GraphicsCurrentFrameView() : m_State(CurrentFrameViewState::NONE){
    m_CurrentFrameScene = new QGraphicsScene;
    setScene(m_CurrentFrameScene);

    QPixmap pmImg;
    pmImg.load("data/images/sprites/Arthur.png");
    m_CurrentFrameScene->addPixmap(pmImg);

    GraphicsBoxItem * item = new GraphicsBoxItem();
    m_CurrentFrameScene->addItem(item);

    // Set Policy
    setDragMode(QGraphicsView::ScrollHandDrag);
    setHorizontalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
    setVerticalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
}

void GraphicsCurrentFrameView::createBox(const int x, const int y, const int w, const int h){
    GraphicsBoxItem * item = new GraphicsBoxItem(x, y, w ,h);
    m_CurrentFrameScene->addItem(item);
}


/*
 *
 ***** Qt Event *****
 *
 */

void GraphicsCurrentFrameView::mousePressEvent(QMouseEvent *event){
    QGraphicsView::mousePressEvent(event);
    QPointF p = mapToScene(event->x(), event->y());

    if( m_State == CREATE_BOX )
        createBox(int(p.x()), int(p.y()), 100, 100);
}

void GraphicsCurrentFrameView::wheelEvent ( QWheelEvent * event ){
    if (event->modifiers() & Qt::ControlModifier) {
        qreal factor = pow((double)2, event->delta() / 240.0);
        QGraphicsView::scale(factor, factor);
    }
    else
        QGraphicsView::wheelEvent(event);
}

void GraphicsCurrentFrameView::keyPressEvent(QKeyEvent * event){
    if(event->key() == Qt::Key_C)
        m_State = CREATE_BOX;
    else
        m_State = NONE;

}

}
