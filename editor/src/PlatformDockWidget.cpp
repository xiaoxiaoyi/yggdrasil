#include "PlatformDockWidget.hpp"
#include "ui_PlatformDockWidget.h"

PlatformDockWidget::PlatformDockWidget(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::PlatformDockWidget)
{
    ui->setupUi(this);
}

PlatformDockWidget::~PlatformDockWidget()
{
    delete ui;
}
