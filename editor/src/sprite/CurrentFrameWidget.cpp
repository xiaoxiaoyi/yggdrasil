#include "sprite/CurrentFrameWidget.hpp"

#include <iostream>

namespace yggeditor {

CurrentFrameWidget::CurrentFrameWidget() : QWidget() {
    m_CurrentFrameView = new yggeditor::GraphicsCurrentFrameView();
}

GraphicsCurrentFrameView * CurrentFrameWidget::getView( void ){
    return m_CurrentFrameView;
}

}
