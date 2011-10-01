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

    int exi = rand() % config.v_ex_types.size();
    switch(config.v_ex_types[exi])
    {
        case s_config::et_sum1:     fill_game_basic_adds();  break;
        case s_config::et_mult1:    fill_game_basic_products();  break;
        case s_config::et_subs1:    fill_game_basic_subs();  break;
        case s_config::et_div1:     fill_game_basic_divs();  break;
        case s_config::et_revsum:   fill_game_basic_reverse_adds();  break;
        case s_config::et_revprod:  fill_game_basic_reverse_products();  break;
        case s_config::et_previus:  fill_game_previus();  break;
        case s_config::et_next:     fill_game_next();  break;
    }
}


void math_practice::fill_game_basic_adds(void)
{
    int correct_option = rand() % 9 + 1;
    int sum1 = rand() % 11;
    int sum2 = rand() % 11;

    ui->question->setText(QString::number(sum1) + "  +  " + QString::number(sum2) + "  =");

    get_option_widget(correct_option)->setText(QString::number(sum1+sum2));
    get_option_widget(correct_option)->setVisible(true);
    get_option_widget(correct_option)->set_correct(true);

    int wrong_options = config.active_options-1;
    while(wrong_options != 0)
    {
        int option = rand() % 9 + 1;
        if (get_option_widget(option)->isVisible())   continue;

        int wrong_value = sum1 + sum2;
        while(wrong_value == sum1 + sum2)
        {
            wrong_value = rand()%21;
        }
        get_option_widget(option)->setText(QString::number(wrong_value));
        get_option_widget(option)->set_correct(false);
        get_option_widget(option)->setVisible(true);
        --wrong_options;
    }
}

void math_practice::fill_game_basic_products(void)
{
    int correct_option = rand() % 9 + 1;
    int pr1 = rand() % 12;
    int pr2 = rand() % 12;

    ui->question->setText(QString::number(pr1) + "  x  " + QString::number(pr2) + "  =");

    get_option_widget(correct_option)->setText(QString::number(pr1 * pr2));
    get_option_widget(correct_option)->setVisible(true);
    get_option_widget(correct_option)->set_correct(true);

    int wrong_options = config.active_options-1;
    while(wrong_options != 0)
    {
        int option = rand() % 9 + 1;
        if (get_option_widget(option)->isVisible())   continue;

        int wrong_value = pr1 * pr2;
        while(wrong_value == pr1 * pr2)
        {
            wrong_value = rand()%121;
        }
        get_option_widget(option)->setText(QString::number(wrong_value));
        get_option_widget(option)->set_correct(false);
        get_option_widget(option)->setVisible(true);
        --wrong_options;
    }
}

void math_practice::fill_game_basic_subs(void)
{
    int correct_option = rand() % 9 + 1;
    int min = rand() % 11;
    int sus = rand() % min;

    ui->question->setText(QString::number(min) + "  -  " + QString::number(sus) + "  =");

    get_option_widget(correct_option)->setText(QString::number(min - sus));
    get_option_widget(correct_option)->setVisible(true);
    get_option_widget(correct_option)->set_correct(true);

    int wrong_options = config.active_options-1;
    while(wrong_options != 0)
    {
        int option = rand() % 9 + 1;
        if (get_option_widget(option)->isVisible())   continue;

        int wrong_value = min - sus;
        while(wrong_value == min - sus)
        {
            wrong_value = rand()%10;
        }
        get_option_widget(option)->setText(QString::number(wrong_value));
        get_option_widget(option)->set_correct(false);
        get_option_widget(option)->setVisible(true);
        --wrong_options;
    }
}

void math_practice::fill_game_basic_divs(void)
{
    int correct_option = rand() % 9 + 1;
    int div = rand() % 10 + 1;
    int coc = rand() % 11;

    ui->question->setText(QString::number(div * coc) + "  /  " + QString::number(div) + "  =");

    get_option_widget(correct_option)->setText(QString::number(coc));
    get_option_widget(correct_option)->setVisible(true);
    get_option_widget(correct_option)->set_correct(true);

    int wrong_options = config.active_options-1;
    while(wrong_options != 0)
    {
        int option = rand() % 9 + 1;
        if (get_option_widget(option)->isVisible())   continue;

        int wrong_value = coc;
        while(wrong_value == coc)
        {
            wrong_value = rand()%11;
        }
        get_option_widget(option)->setText(QString::number(wrong_value));
        get_option_widget(option)->set_correct(false);
        get_option_widget(option)->setVisible(true);
        --wrong_options;
    }
}


void math_practice::fill_game_basic_reverse_adds(void)
{
    int correct_option = rand() % 9 + 1;
    int min = rand() % 11;
    int sus = rand() % min;

    if(rand()%2 == 0)
        ui->question->setText(QString::number(min - sus) + "  +   _   =  " + QString::number(min));
    else
        ui->question->setText(QString("  _   +   ")  + QString::number(min - sus) + "  =  " + QString::number(min));
    get_option_widget(correct_option)->setText(QString::number(sus));
    get_option_widget(correct_option)->setVisible(true);
    get_option_widget(correct_option)->set_correct(true);

    int wrong_options = config.active_options-1;
    while(wrong_options != 0)
    {
        int option = rand() % 9 + 1;
        if (get_option_widget(option)->isVisible())   continue;

        int wrong_value = sus;
        while(wrong_value == sus)
        {
            wrong_value = rand()%10;
        }
        get_option_widget(option)->setText(QString::number(wrong_value));
        get_option_widget(option)->set_correct(false);
        get_option_widget(option)->setVisible(true);
        --wrong_options;
    }
}

void math_practice::fill_game_basic_reverse_products(void)
{
    int correct_option = rand() % 9 + 1;
    int div = rand() % 10 + 1;
    int coc = rand() % 10 + 1;

    int good = 0;
    if(rand()%2 == 0)
        good = div;
    else
        good = coc;
    if(rand()%2 == 0)
        ui->question->setText(QString::number(div*coc/good) + "  x   _   =  " + QString::number(div * coc));
    else
        ui->question->setText(QString(  "  _  x  ") + QString::number(div*coc/good) + "  =  " + QString::number(div * coc));

    get_option_widget(correct_option)->setText(QString::number(good));
    get_option_widget(correct_option)->setVisible(true);
    get_option_widget(correct_option)->set_correct(true);

    int wrong_options = config.active_options-1;
    while(wrong_options != 0)
    {
        int option = rand() % 9 + 1;
        if (get_option_widget(option)->isVisible())   continue;

        int wrong_value = good;
        while(wrong_value == good)
        {
            wrong_value = rand()%11;
        }
        get_option_widget(option)->setText(QString::number(wrong_value));
        get_option_widget(option)->set_correct(false);
        get_option_widget(option)->setVisible(true);
        --wrong_options;
    }
}

void math_practice::fill_game_previus(void)
{
    int correct_option = rand() % 9 + 1;
    int value = (rand()%1000+1)*1000;
    value +=  (rand()%3)-1;

    ui->question->setText(QString::number(value) + "  -1   =  ");

    get_option_widget(correct_option)->setText(QString::number(value-1));
    get_option_widget(correct_option)->setVisible(true);
    get_option_widget(correct_option)->set_correct(true);

    int wrong_options = config.active_options-1;
    while(wrong_options != 0)
    {
        int option = rand() % 9 + 1;
        if (get_option_widget(option)->isVisible())   continue;

        int wrong_value = value-1;
        while(wrong_value == value-1)
        {
            int dif;
            int tens;
            if(rand()%2)  tens =100;  else  tens=1000;
            dif = rand()%5 * tens;
            if(rand()%2==0)
                wrong_value = value-1 - dif;
            else
                wrong_value = value-1 + dif;
        }
        get_option_widget(option)->setText(QString::number(wrong_value));
        get_option_widget(option)->set_correct(false);
        get_option_widget(option)->setVisible(true);
        --wrong_options;
    }
}

void math_practice::fill_game_next(void)
{
    int correct_option = rand() % 9 + 1;
    int value = (rand()%1000+1)*1000;
    value +=  (rand()%3)-1;

    ui->question->setText(QString::number(value) + "  +1   =  ");

    get_option_widget(correct_option)->setText(QString::number(value+1));
    get_option_widget(correct_option)->setVisible(true);
    get_option_widget(correct_option)->set_correct(true);

    int wrong_options = config.active_options-1;
    while(wrong_options != 0)
    {
        int option = rand() % 9 + 1;
        if (get_option_widget(option)->isVisible())   continue;

        int wrong_value = value+1;
        while(wrong_value == value+1)
        {
            int dif;
            int tens;
            if(rand()%2)  tens =100;  else  tens=1000;
            dif = rand()%5 * tens;
            if(rand()%2==0)
                wrong_value = value+1 - dif;
            else
                wrong_value = value+1 + dif;
        }
        get_option_widget(option)->setText(QString::number(wrong_value));
        get_option_widget(option)->set_correct(false);
        get_option_widget(option)->setVisible(true);
        --wrong_options;
    }
}

