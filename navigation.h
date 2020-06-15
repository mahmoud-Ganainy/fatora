#ifndef NAVIGATION_H
#define NAVIGATION_H

#include <QWidget>

namespace Ui {
class navigation;
}

class navigation : public QWidget
{
    Q_OBJECT

public:
    explicit navigation(QWidget *parent = nullptr);
    ~navigation();

private:
    Ui::navigation *ui;
};

#endif // NAVIGATION_H
