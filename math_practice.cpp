#include "math_practice.h"
#include "ui_math_practice.h"

#include "qoption_button.h"


#include <stdio.h>
#include <stdlib.h>
#include <time.h>



math_practice::math_practice(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::math_practice)
{
    ui->setupUi(this);

    srand ( time(NULL) );
    init();
}

void math_practice::init(void)
{
    ui->pending->setText("");
    ui->congratulation->setVisible(false);
    init_options_and_answer();
    for(int i=1; i<=9; ++i)
    {
        connect(get_option_widget(i), SIGNAL(signal_clicked(bool)),
                    this, SLOT(slot_option_selected(bool)));
    }
}


void math_practice::start_game(void)
{
    status.pending_repetitions = config.repetitions;
    init_options_and_answer();
    fill_game();
}



math_practice::~math_practice()
{
    delete ui;
}

void math_practice::init_options_and_answer(void)
{
    ui->question->setText("");
    for(int i=1; i<=9; ++i)
    {
        get_option_widget(i)->setVisible(false);
        get_option_widget(i)->set_correct(false);
    }
}



qoption_button*  math_practice::get_option_widget(int option)
{
    switch(option)
    {
        case 1:     return  ui->opt1;
        case 2:     return  ui->opt2;
        case 3:     return  ui->opt3;
        case 4:     return  ui->opt4;
        case 5:     return  ui->opt5;
        case 6:     return  ui->opt6;
        case 7:     return  ui->opt7;
        case 8:     return  ui->opt8;
        case 9:     return  ui->opt9;
    }
    throw "no valid button code";
}


void  math_practice::slot_option_selected(bool  is_correct)
{
    if(is_correct)
    {
        status.pending_repetitions -= 1;
        if(status.pending_repetitions == 0)
        {
            ui->congratulation->setVisible(true);
            init_options_and_answer();
        }
        else
        {
            fill_game();
        }
    }
    else
    {
        status.pending_repetitions += config.wrong_option_penalization;
        ui->pending->setText(QString::number(status.pending_repetitions));
    }
}

void math_practice::on_actionStart_game_triggered()
{
    start_game();
}


void math_practice::fill_game(void)
{
    init_options_and_answer();
    ui->pending->setText(QString::number(status.pending_repetitions));

    int correct_option = rand() % 9 + 1;
    int sum1 = rand() % 10 + 1;
    int sum2 = rand() % 10 + 1;

    ui->question->setText(QString::number(sum1) + "  +  " + QString::number(sum2) + "  =");

    get_option_widget(correct_option)->setText(QString::number(sum1+sum2));
    get_option_widget(correct_option)->setVisible(true);
    get_option_widget(correct_option)->set_correct(true);

    int wrong_options = config.active_options-1;
    while(wrong_options != 0)
    {
        int option = rand() % 9+1;
        if (get_option_widget(option)->isVisible())   continue;

        int wrong_value = correct_option;
        while(wrong_value != correct_option)
        {
            wrong_value = rand()%20 +1;
        }
        get_option_widget(option)->setText(QString::number(wrong_value));
        get_option_widget(option)->set_correct(false);
        get_option_widget(option)->setVisible(true);
        --wrong_options;
    }
}
