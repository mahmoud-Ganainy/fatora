#ifndef DIALOG2_H
#define DIALOG2_H

#include <QDialog>
#include <cat_rege.h>
namespace Ui {
class Dialog2;
}

class Dialog2 : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog2(QWidget *parent = 0);
    ~Dialog2();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::Dialog2 *ui;
    cat_rege *cat;
};

#endif // DIALOG2_H
