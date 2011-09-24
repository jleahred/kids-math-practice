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
    ui->stackedWidget->setCurrentIndex(0);
    init_options_and_answer();
    for(int i=1; i<=9; ++i)
    {
        connect(get_option_widget(i), SIGNAL(signal_clicked(qoption_button*)),
                    this, SLOT(slot_option_selected(qoption_button*)));
    }
}


void math_practice::start_game(void)
{
    config.active_options = ui->config_options->value();
    config.repetitions = ui->config_repetitions->value();

    ui->stackedWidget->setCurrentIndex(1);
    status.pending_repetitions = config.repetitions;
    status.started = QDateTime::currentDateTime();
    status.fails = 0;
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
        get_option_widget(i)->setStyleSheet("");
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

void math_practice::game_ended(void)
{
    ui->stackedWidget->setCurrentIndex(2);
    init_options_and_answer();
    generate_report();
}

void math_practice::generate_report(void)
{
    ui->report->clear();
    ui->report->setText(ui->report->text().append("started: " + status.started.toString()));
    ui->report->setText(ui->report->text().append("\n"));
    ui->report->setText(ui->report->text().append("ended: " + QDateTime::currentDateTime().toString()));
    ui->report->setText(ui->report->text().append("\n"));
    ui->report->setText(ui->report->text().append("time: " + QString::number(status.started.msecsTo(QDateTime::currentDateTime())/1000./60.))+ " min");
    ui->report->setText(ui->report->text().append("\n"));
    ui->report->setText(ui->report->text().append("repetitions: " + QString::number(config.repetitions)));
    ui->report->setText(ui->report->text().append("\n"));
    ui->report->setText(ui->report->text().append("options: " + QString::number(config.active_options)));
    ui->report->setText(ui->report->text().append("\n"));
    ui->report->setText(ui->report->text().append("failed: " + QString::number(status.fails)));
    ui->report->setText(ui->report->text().append("\n"));
    ui->report->setText(ui->report->text().append("penalization: " + QString::number(config.wrong_option_penalization)));
    ui->report->setText(ui->report->text().append("\n"));
}


void  math_practice::slot_option_selected(qoption_button*  sender)
{
    if(sender->is_correct())
    {
        status.pending_repetitions -= 1;
        if(status.pending_repetitions == 0)
        {
            game_ended();
        }
        else
        {
            fill_game();
        }
    }
    else
    {
        sender->setStyleSheet("color: red");
        status.pending_repetitions += config.wrong_option_penalization;
        ++status.fails;
        ui->pending->setText(QString::number(status.pending_repetitions));
    }
}




void math_practice::on_start_clicked()
{
    start_game();
}

void math_practice::on_reset_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}



void math_practice::fill_game(void)
{
    fill_game_basic_adds();
}


void math_practice::fill_game_basic_adds(void)
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

        int wrong_value = sum1 + sum2;
        while(wrong_value == sum1 + sum2)
        {
            wrong_value = rand()%20 +1;
        }
        get_option_widget(option)->setText(QString::number(wrong_value));
        get_option_widget(option)->set_correct(false);
        get_option_widget(option)->setVisible(true);
        --wrong_options;
    }
}

