#ifndef DIALOG4_H
#define DIALOG4_H

#include <QDialog>
#include <class_rege.h>
namespace Ui {
class Dialog4;
}

class Dialog4 : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog4(QWidget *parent = 0);
    ~Dialog4();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::Dialog4 *ui;
    class_rege *class_re;
};

#endif // DIALOG4_H
