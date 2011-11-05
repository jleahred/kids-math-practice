#include "math_practice.h"
#include "ui_math_practice.h"

#include "qoption_button.h"

#include <iostream>


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include<iostream>
#include<fstream>


#include <yaml-cpp-0.2.7/include/yaml-cpp/emitter.h>
#include <yaml-cpp-0.2.7/include/yaml-cpp/stlemitter.h>
#include <yaml-cpp-0.2.7/include/yaml-cpp/node.h>
#include <yaml-cpp-0.2.7/include/yaml-cpp/parser.h>


YAML::Emitter& operator << (YAML::Emitter& out, const QStringList& l)
{
    QString  s;
    out << YAML::BeginSeq;

    foreach(s, l)
        out << s.toStdString();

    out << YAML::EndSeq;

    return out;
}


YAML::Emitter& operator << (YAML::Emitter& out, const QString& s)
{
    out << s.toStdString();
     return out;
}


YAML::Emitter& operator << (YAML::Emitter& out, const s_game& g) {
    out << YAML::BeginMap
        <<  YAML::Key << "question"      <<  YAML::Value << g.question
        <<  YAML::Key << "result"        <<  YAML::Value << g.result
        <<  YAML::Key << "styles"        <<  YAML::Value << g.styles
        <<  YAML::Key << "date_time"     <<  YAML::Value << g.date_time
        <<  YAML::Key << "wrong_options" <<  YAML::Value << g.wrong_options
     << YAML::EndMap;
     return out;
}

void           operator >> (const YAML::Node&   node, QString& s)
{
    std::string value;
    node >> value;
    s = value.c_str();
}

void           operator >> (const YAML::Node&   node, std::vector<QString>& sv)
{
    for(unsigned i = 0; i < node.size(); i++)
    {
        QString  s;
        node[i] >> s;

        sv.push_back(s);
    }
}

void           operator >> (const YAML::Node&   node, s_game& g)
{
    node["question"] >> g.question;
    node["result"] >> g.result;
    node["styles"] >> g.styles;
    node["date_time"] >> g.date_time;
    node["wrong_options"] >> g.wrong_options;
}





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
    status.games_failed.clear();
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


void write_errors(const s_status&  status)
{
    std::ifstream fin("./errors.yaml");
    YAML::Parser parser(fin);

    YAML::Node node_errors;
    parser.GetNextDocument(node_errors);

    std::list<s_game>  lgame_errors;
    for(YAML::Iterator it=node_errors.begin();it!=node_errors.end();++it)
    {
        s_game local_game;
        *it >> local_game;
        lgame_errors.push_back(local_game);
    }
    //node_errors >> lgame_errors;


    //  add current fails
    for(auto it=status.games_failed.begin(); it!=status.games_failed.end(); ++it)
        lgame_errors.push_back(*it);


    YAML::Emitter  em_errors;
    em_errors << lgame_errors;
    std::ofstream fout("./errors.yaml");
    fout << em_errors.c_str();
}


void math_practice::game_ended(void)
{
    ui->stackedWidget->setCurrentIndex(2);
    init_options_and_answer();
    generate_report();
    write_errors(status);
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
    ui->report->setText(ui->report->text().append("failed: " + QString::number(status.games_failed.size())));
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
        status.games_failed.push_back(current_game);
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
        case s_config::et_mult_rev: game=  get_game_mult_table_inverse();  break;
        case s_config::et_subs1:    game = get_game_basic_subs();  break;
        case s_config::et_div1:     game = get_game_basic_divs();  break;
        case s_config::et_revsum:   game = get_game_basic_reverse_adds();  break;
        case s_config::et_revprod:  game = get_game_basic_reverse_products();  break;
        case s_config::et_previus:  game = get_game_previus();  break;
        case s_config::et_next:     game = get_game_next();  break;
        case s_config::et_aprox2:   game = get_game_aprox_num_to();  break;
    }
    game.date_time  = QDateTime::currentDateTime().toString();

    current_game = game;

    ui->question->setText(game.question);

    int correct_option = rand() % 9 + 1;
    get_option_widget(correct_option)->setText(game.result);
    get_option_widget(correct_option)->setVisible(true);
    get_option_widget(correct_option)->set_correct(true);

    QString  wrong_option_value;
    int      wrong_options_counter=1;
    foreach(wrong_option_value, game.wrong_options)
    {
        int wrong_option_pos;
        do
            wrong_option_pos = rand() % 9 + 1;
        while(get_option_widget(wrong_option_pos)->isVisible());

        get_option_widget(wrong_option_pos)->setText(wrong_option_value);
        get_option_widget(wrong_option_pos)->set_correct(false);
        get_option_widget(wrong_option_pos)->setVisible(true);
        ++wrong_options_counter;
        if(wrong_options_counter > config.active_options-1)
            break;
    }
}


s_game  math_practice::get_game_basic_adds(void)
{
    s_game result;

    int sum1 = rand() % 11;
    int sum2 = rand() % 11;

    result.question = QString::number(sum1) + "  +  " + QString::number(sum2) + "  =";
    result.result = QString::number(sum1 + sum2);

    int wrong_options = 20;
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

    int wrong_options = 20;
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

    int min = rand() % 10 + 1;
    int sus = rand() % min;

    result.question = QString::number(min) + "  -  " + QString::number(sus) + "  =";
    result.result = QString::number(min - sus);

    int wrong_options = 20;
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

    int wrong_options = 20;
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

    int wrong_options = 20;
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

    int wrong_options = 20;
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


    int wrong_options = 20;
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


    int wrong_options = 20;
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

s_game  math_practice::get_game_mult_table_inverse(void)
{
    s_game result;

    int prod1 = rand()%11;
    int prod2 = rand()%11;

    result.question = QString::number(prod1 * prod2);
    result.result = (QString::number(prod1) + "  x  " + QString::number(prod2) + "  =");


    int wrong_options = 20;
    while(wrong_options != 0)
    {
        QString wrong_value_r = result.question;
        QString wrong_value;
        while(wrong_value_r == result.question)
        {
            int wprod1 = rand()%11;
            int wprod2 = rand()%11;
            wrong_value = (QString::number(wprod1) + "  x  " + QString::number(wprod2) + "  =");
            wrong_value_r = QString::number(wprod1 * wprod2);
        }
        result.wrong_options.push_back(wrong_value);
        --wrong_options;
    }
    return result;
}

s_game  math_practice::get_game_aprox_num_to(void)
{
    s_game result;

    int num = rand()%10000;
    int pow = 1;
    QString question;
    switch (rand()%4)
    {
        case  0:    pow = 1;
                    question = "U";
                    break;
        case  1:    pow = 10;
                    question = "D";
                    break;
        case  2:    pow = 100;
                    question = "C";
                    break;
        case  3:    pow = 1000;
                    question = "UM";
                    break;
    }

    result.question = "Aprox " + QString::number(num) + "  to  " + question;
    result.result = QString::number((num + 5*pow/10)/pow * pow);


    int wrong_options = 20;
    while(wrong_options != 0)
    {
        QString wrong_value = result.result;
        while(wrong_value == result.result)
        {
            int random = rand()%4;
            if(random == 0  &&  pow>10)
                wrong_value = QString::number((num + 5*pow/100)/(pow/10) * pow/10);
            else if(random==1)
                wrong_value = QString::number((num + 5*pow/1)/(pow*10) * pow*10);
            else
                wrong_value = QString::number((num + rand()%3*pow/1)/pow * pow);
        }
        result.wrong_options.push_back(wrong_value);
        --wrong_options;
    }
    return result;
}

void math_practice::on_select_exercices_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}
