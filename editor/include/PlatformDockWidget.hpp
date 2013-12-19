#ifndef __PLATFORMDOCKWIDGET_HPP__
#define __PLATFORMDOCKWIDGET_HPP__

#include <QDockWidget>

namespace Ui {
class PlatformDockWidget;
}

class PlatformDockWidget : public QDockWidget
{
    Q_OBJECT
    
public:
    explicit PlatformDockWidget(QWidget *parent = 0);
    ~PlatformDockWidget();
    
private:
    Ui::PlatformDockWidget *ui;
};

#endif // PLATFORMDOCKWIDGET_HPP
