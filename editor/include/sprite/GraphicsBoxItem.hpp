/**
 * \file GraphicsBoxItem.hpp
 * \author Hekiat
 * \date 23 july 2013
 */

#ifndef __GRAPHICSBOXITEM_HPP__
#define __GRAPHICSBOXITEM_HPP__

#include <QtGui>
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>

namespace yggeditor{

/**
 * \class GraphicsBoxItem GraphicsBoxItem.hpp
 * \brief Inherit from QGraphicsRectItem.
 *        It's a basic movable, rotable, scalable rectangle for a QGraphicsScene
 */
class GraphicsBoxItem : public QGraphicsRectItem {
public:
    GraphicsBoxItem(const int x = 0, const int y = 0, const int w = 100, const int h = 100);

    void mousePressEvent(QGraphicsSceneMouseEvent * event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent * event);

private:
    QPointF m_MouseInitialPos;
    qreal m_Rotation;
};

}
#endif // __GRAPHICSBOXITEM_HPP__
