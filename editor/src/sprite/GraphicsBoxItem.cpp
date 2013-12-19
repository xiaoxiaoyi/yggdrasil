#include "sprite/GraphicsBoxItem.hpp"
#include <iostream>

namespace yggeditor{

GraphicsBoxItem::GraphicsBoxItem(const int x, const int y, const int w, const int h)
    : QGraphicsRectItem( x, y, w, h, nullptr ), m_MouseInitialPos(0.0f, 0.0f), m_Rotation(0.0f) {
    setFlag(QGraphicsItem::GraphicsItemFlag::ItemIsMovable);
    setFlag(QGraphicsItem::GraphicsItemFlag::ItemIsSelectable);
    setBrush(QBrush(QColor(128, 128, 128, 64)));
}

/*
 *
 ***** Qt Event *****
 *
 */

void GraphicsBoxItem::mousePressEvent(QGraphicsSceneMouseEvent * event){
    m_MouseInitialPos = mapToScene(event->pos());
    QGraphicsItem::mousePressEvent(event);
}

void GraphicsBoxItem::mouseMoveEvent(QGraphicsSceneMouseEvent * event){
    if( event->buttons() & Qt::LeftButton )
        QGraphicsItem::mouseMoveEvent(event);

    else if( event->buttons() & Qt::RightButton ){
        QPointF currentPos = mapToScene( event->pos() );

        m_Rotation += (currentPos.x() + currentPos.y()) - (m_MouseInitialPos.x() + m_MouseInitialPos.y());

        QTransform xform;
        xform.translate( rect().width()/2.0f, rect().height()/2.0f);
        xform.rotate(rotation() + m_Rotation);
        xform.translate( -rect().width()/2.0f, -rect().height()/2.0f);
        setTransform(xform);

        m_MouseInitialPos = currentPos;
    }
}

}
