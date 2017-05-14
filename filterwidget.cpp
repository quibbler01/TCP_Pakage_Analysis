#include "filterwidget.h"
#include "ui_filterwidget.h"

filterWidget::filterWidget(QWidget *parent) :
    QWidget(parent),
    fi(new Ui::filterWidget)
{
    fi->setupUi(this);
}

filterWidget::~filterWidget()
{
    delete fi;
}
