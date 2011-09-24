#ifndef MATH_PRACTICE_H
#define MATH_PRACTICE_H

#include <QMainWindow>



class qoption_button;


struct  s_config
{
    int  repetitions;
    int  active_options;
    int  wrong_option_penalization;

    s_config() :    repetitions(10),
                    active_options (3),
                    wrong_option_penalization(1)
    {};
};

struct  s_status
{
    int  pending_repetitions;

    s_status() : pending_repetitions(0) {};
};



namespace Ui {
    class math_practice;
}

class math_practice : public QMainWindow
{
    Q_OBJECT

public:
    explicit math_practice(QWidget *parent = 0);
    ~math_practice();

private:
    Ui::math_practice *ui;

    qoption_button*  get_option_widget(int option);
    void init(void);
    void start_game(void);
    void init_options_and_answer(void);
    void fill_game(void);

    s_config   config;
    s_status   status;

private slots:
    void on_actionStart_game_triggered();
    void  slot_option_selected(bool  is_correct);
};

#endif // MATH_PRACTICE_H
