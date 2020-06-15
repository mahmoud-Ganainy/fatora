#ifndef MYSQL_SET_H
#define MYSQL_SET_H

#include <QDialog>

namespace Ui {
class mysql_set;
}

class mysql_set : public QDialog
{
    Q_OBJECT

public:
    explicit mysql_set(QWidget *parent = 0);
    ~mysql_set();

private:
    Ui::mysql_set *ui;
};

#endif // MYSQL_SET_H
