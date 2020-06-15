#ifndef DIALOG3_H
#define DIALOG3_H

#include <QDialog>
#include <sea_rege.h>
namespace Ui {
class Dialog3;
}

class Dialog3 : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog3(QWidget *parent = 0);
    ~Dialog3();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::Dialog3 *ui;
    sea_rege *sea;
};

#endif // DIALOG3_H
