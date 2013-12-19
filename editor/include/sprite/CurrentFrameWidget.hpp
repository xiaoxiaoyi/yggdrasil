/**
 * \file GraphicsBoxItem.hpp
 * \author Hekiat
 * \date 24 july 2013
 */

#ifndef __CURRENTFRAMEWIDGET_HPP__
#define __CURRENTFRAMEWIDGET_HPP__

#include <QWidget>
#include "GraphicsCurrentFrameView.hpp"

namespace yggeditor {

class CurrentFrameWidget : public QWidget {
public:
    CurrentFrameWidget();
    GraphicsCurrentFrameView * getView( void );

private:
    GraphicsCurrentFrameView * m_CurrentFrameView;
};

}

#endif // __CURRENTFRAMEWIDGET_H__
