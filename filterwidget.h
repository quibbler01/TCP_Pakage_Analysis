#ifndef FILTERWIDGET_H
#define FILTERWIDGET_H

#include <QWidget>

namespace Ui {
class filterWidget;
}

class filterWidget : public QWidget
{
    Q_OBJECT

public:
    explicit filterWidget(QWidget *parent = 0);
    ~filterWidget();

public:
    Ui::filterWidget *fi;
};

#endif // FILTERWIDGET_H
