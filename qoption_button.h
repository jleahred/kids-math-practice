#ifndef QOPTION_BUTTOM_H
#define QOPTION_BUTTOM_H

#include <QPushButton>

class qoption_button : public QPushButton
{
    Q_OBJECT
public:
    explicit qoption_button(QWidget *parent = 0);

    void  set_correct(bool is_correct);

signals:
    void  signal_clicked(bool  correct);


public slots:
    void  slot_clicked(void);

private:
    bool  _is_correct;
};

#endif // QOPTION_BUTTOM_H
