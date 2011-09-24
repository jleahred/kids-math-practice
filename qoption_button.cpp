#include "qoption_button.h"

qoption_button::qoption_button(QWidget *parent) :
    QPushButton(parent),  _is_correct(false)
{
    connect(this, SIGNAL(clicked()), this, SLOT(slot_clicked()));
}


void  qoption_button::set_correct(bool is_correct)
{
    _is_correct = is_correct;
}


void  qoption_button::slot_clicked(void)
{
    signal_clicked(this);
}
