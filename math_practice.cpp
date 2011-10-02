#include "math_practice.h"
#include "ui_math_practice.h"

#include "qoption_button.h"

#include <iostream>


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
    config.wrong_option_penalization = ui->penalizations->value();
    config.v_ex_types.clear();
    for(int i=0; i<ui->exercices_list->count(); ++i)
    {
        if(ui->exercices_list->item(i)->checkState() == Qt::Checked)
            config.v_ex_types.push_back(s_config::en_extype(i));
    }

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


void math_practice::on_exercices_list_itemChanged(QListWidgetItem* /*item*/)
{
    for(int i=0; i<ui->exercices_list->count(); ++i)
    {
        if(ui->exercices_list->item(i)->checkState() == Qt::Checked)
        {
            ui->start->setEnabled(true);
            return;
        }
    }
    ui->start->setEnabled(false);
}


void math_practice::fill_game(void)
{
    init_options_and_answer();
    ui->pending->setText(QString::number(status.pending_repetitions));

    s_game  game;
    int exi = rand() % config.v_ex_types.size();
    switch(config.v_ex_types[exi])
    {
        case s_config::et_sum1:     game = get_game_basic_adds();  break;
        case s_config::et_mult1:    game = get_game_basic_products();  break;
        case s_config::et_subs1:    game = get_game_basic_subs();  break;
        case s_config::et_div1:     game = get_game_basic_divs();  break;
        case s_config::et_revsum:   game = get_game_basic_reverse_adds();  break;
        case s_config::et_revprod:  game = get_game_basic_reverse_products();  break;
        case s_config::et_previus:  game = get_game_previus();  break;
        case s_config::et_next:     game = get_game_next();  break;
    }


    ui->question->setText(game.question);

    int correct_option = rand() % 9 + 1;
    get_option_widget(correct_option)->setText(game.result);
    get_option_widget(correct_option)->setVisible(true);
    get_option_widget(correct_option)->set_correct(true);

    QString  wrong_option_value;
    foreach(wrong_option_value, game.wrong_options)
    {
        int wrong_option_pos;
        do
            wrong_option_pos = rand() % 9 + 1;
        while(get_option_widget(wrong_option_pos)->isVisible());

        get_option_widget(wrong_option_pos)->setText(wrong_option_value);
        get_option_widget(wrong_option_pos)->set_correct(false);
        get_option_widget(wrong_option_pos)->setVisible(true);
    }
}


s_game  math_practice::get_game_basic_adds(void)
{
    s_game result;

    int sum1 = rand() % 11;
    int sum2 = rand() % 11;

    result.question = QString::number(sum1) + "  +  " + QString::number(sum2) + "  =";
    result.result = QString::number(sum1 + sum2);

    int wrong_options = config.active_options-1;
    while(wrong_options != 0)
    {
        QString wrong_value = result.result;
        while(wrong_value == result.result)
        {
            wrong_value = QString::number(rand()%21);
        }
        result.wrong_options.push_back(wrong_value);
        --wrong_options;
    }
    return result;
}

s_game  math_practice::get_game_basic_products(void)
{
    s_game result;

    int pr1 = rand() % 12;
    int pr2 = rand() % 12;

    result.question = QString::number(pr1) + "  x  " + QString::number(pr2) + "  =";
    result.result = QString::number(pr1 * pr2);

    int wrong_options = config.active_options-1;
    while(wrong_options != 0)
    {
        QString wrong_value = result.result;
        while(wrong_value == result.result)
        {
            wrong_value = QString::number(rand()%121);
        }
        result.wrong_options.push_back(wrong_value);
        --wrong_options;
    }
    return result;
}

s_game  math_practice::get_game_basic_subs(void)
{
    s_game result;

    int min = rand() % 11;
    int sus = rand() % min;

    result.question = QString::number(min) + "  -  " + QString::number(sus) + "  =";
    result.result = QString::number(min - sus);

    int wrong_options = config.active_options-1;
    while(wrong_options != 0)
    {
        QString wrong_value = result.result;
        while(wrong_value == result.result)
        {
            wrong_value = QString::number(rand()%10);
        }
        result.wrong_options.push_back(wrong_value);
        --wrong_options;
    }
    return result;
}

s_game  math_practice::get_game_basic_divs(void)
{
    s_game result;

    int div = rand() % 10 + 1;
    int coc = rand() % 11;

    result.question = QString::number(div * coc) + "  /  " + QString::number(div) + "  =";
    result.result = QString::number(coc);

    int wrong_options = config.active_options-1;
    while(wrong_options != 0)
    {
        QString wrong_value = result.result;
        while(wrong_value == result.result)
        {
            wrong_value = QString::number(rand()%11);
        }
        result.wrong_options.push_back(wrong_value);
        --wrong_options;
    }
    return result;
}


s_game  math_practice::get_game_basic_reverse_adds(void)
{
    s_game result;

    int min = rand() % 10 + 1;
    int sus = rand() % min;

    if(rand()%2 == 0)
        result.question = QString::number(min - sus) + "  +   _   =  " + QString::number(min);
    else
        result.question = QString("  _   +   ")  + QString::number(min - sus) + "  =  " + QString::number(min);
    result.result = QString::number(sus);

    int wrong_options = config.active_options-1;
    while(wrong_options != 0)
    {
        QString wrong_value = result.result;
        while(wrong_value == result.result)
        {
            wrong_value = QString::number(rand()%10);
        }
        result.wrong_options.push_back(wrong_value);
        --wrong_options;
    }
    return result;
}

s_game  math_practice::get_game_basic_reverse_products(void)
{
    s_game result;

    int div = rand() % 10 + 1;
    int coc = rand() % 10 + 1;

    int good = 0;
    if(rand()%2 == 0)
        good = div;
    else
        good = coc;
    if(rand()%2 == 0)
        result.question = (QString::number(div*coc/good) + "  x   _   =  " + QString::number(div * coc));
    else
        result.question = (QString(  "  _  x  ") + QString::number(div*coc/good) + "  =  " + QString::number(div * coc));
    result.result = QString::number(good);

    int wrong_options = config.active_options-1;
    while(wrong_options != 0)
    {
        QString wrong_value = result.result;
        while(wrong_value == result.result)
        {
            wrong_value = QString::number(rand()%11);
        }
        result.wrong_options.push_back(wrong_value);
        --wrong_options;
    }
    return result;
}

s_game  math_practice::get_game_previus(void)
{
    s_game result;

    int value = (rand()%1000+1)*1000;
    value +=  (rand()%3)-1;

    result.result = QString::number(value-1);
    result.question = (QString::number(value) + "  -1   =  ");


    int wrong_options = config.active_options-1;
    while(wrong_options != 0)
    {
        QString wrong_value = result.result;
        while(wrong_value == result.result)
        {
            int dif;
            int tens;
            if(rand()%2)  tens =100;  else  tens=1000;
            dif = rand()%5 * tens;
            if(rand()%2==0)
                wrong_value = QString::number(value-1 - dif);
            else
                wrong_value = QString::number(value-1 + dif);
        }
        result.wrong_options.push_back(wrong_value);
        --wrong_options;
    }
    return result;
}

s_game  math_practice::get_game_next(void)
{
    s_game result;

    int value = (rand()%1000+1)*1000;
    value +=  (rand()%3)-1;

    result.result = QString::number(value+1);
    result.question = (QString::number(value) + "  +1   =  ");


    int wrong_options = config.active_options-1;
    while(wrong_options != 0)
    {
        QString wrong_value = result.result;
        while(wrong_value == result.result)
        {
            int dif;
            int tens;
            if(rand()%2)  tens =100;  else  tens=1000;
            dif = rand()%5 * tens;
            if(rand()%2==0)
                wrong_value = QString::number(value+1 - dif);
            else
                wrong_value = QString::number(value+1 + dif);
        }
        result.wrong_options.push_back(wrong_value);
        --wrong_options;
    }
    return result;
}

