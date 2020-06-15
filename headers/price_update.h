#ifndef PRICE_UPDATE_H
#define PRICE_UPDATE_H

#include <QDialog>

namespace Ui {
class price_update;
}

class price_update : public QDialog
{
    Q_OBJECT

public:
    explicit price_update(QWidget *parent = 0);
    ~price_update();
    int update_state;
    double profit;
    double profit_perc;
    double buy;
    double sell;

private slots:
    void on_checkBox_stateChanged(int arg1);

    void on_buy_price_editingFinished();

    void on_profit_editingFinished();

    void on_profit_perc_editingFinished();

    void on_sell_price_editingFinished();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();


    void on_buy_price_returnPressed();

    void on_profit_returnPressed();

    void on_profit_perc_returnPressed();

    void on_sell_price_returnPressed();

private:
    Ui::price_update *ui;
};

#endif // PRICE_UPDATE_H
