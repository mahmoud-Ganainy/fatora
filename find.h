#ifndef FIND_H
#define FIND_H

#include <QDialog>

namespace Ui {
class find;
}

class find : public QDialog
{
    Q_OBJECT

public:
    explicit find(QWidget *parent = 0);
    ~find();

private:
    Ui::find *ui;
};

#endif // FIND_H
