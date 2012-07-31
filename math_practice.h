#ifndef MATH_PRACTICE_H
#define MATH_PRACTICE_H

#include <QMainWindow>
#include <QDateTime>
#include <QListWidgetItem>


#include <vector>


class qoption_button;



struct  s_config
{
    enum  en_extype {       et_add1_1cipher,
                            et_sum1,
                            et_mult1,
                            et_mult_rev,
                            et_subs1,
                            et_div1,
                            et_revsum,
                            et_revprod,
                            et_previus,
                            et_next,
                            et_aprox2
                        };

    int  repetitions;
    int  active_options;
    int  wrong_option_penalization;
    std::vector<en_extype>   v_ex_types;

    s_config() :    repetitions(10),
                    active_options (3),
                    wrong_option_penalization(1),
                    v_ex_types()
    {};
};


struct s_game
{
    QString                 styles;
    QString                 question;
    QString                 result;
    std::vector<QString>    wrong_options;
    QString                 date_time;
};


struct  s_status
{
    int                     pending_repetitions;
    QDateTime               started;
    int                     fails;
    std::list<s_game>       games_failed;

    s_status() : pending_repetitions(0),
                 started(QDateTime::currentDateTime()),
                 fails(0)
    {};
};



namespace YAML  {  class Emitter; class Node;  }

YAML::Emitter& operator << (YAML::Emitter&      out,  const s_game& g);
void           operator >> (const YAML::Node&   node,       s_game& g);


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
    void game_ended(void);
    void generate_report(void);



    void fill_game(void);

    s_game  get_game_add1_1cipher(void);
    s_game  get_game_basic_adds(void);
    s_game  get_game_basic_products(void);
    s_game  get_game_basic_subs(void);
    s_game  get_game_basic_divs(void);
    s_game  get_game_basic_reverse_adds(void);
    s_game  get_game_basic_reverse_products(void);
    s_game  get_game_previus(void);
    s_game  get_game_next(void);
    s_game  get_game_mult_table_inverse(void);
    s_game  get_game_aprox_num_to(void);



    s_config   config;
    s_status   status;
    s_game     current_game;

private slots:
    void on_exercices_list_itemChanged(QListWidgetItem* item);
    void on_reset_clicked();
    void on_start_clicked();
    void slot_option_selected(qoption_button*  sender);
};

#endif // MATH_PRACTICE_H
