#ifndef MATH_PRACTICE_H
#define MATH_PRACTICE_H

#include <QMainWindow>
#include <QDateTime>
#include <QListWidgetItem>


#include <vector>


class qoption_button;



struct  s_config
{
    enum  en_extype {       et_sum1,
                            et_mult1,
                            et_subs1,
                            et_div1,
                            et_revsum,
                            et_revprod,
                            et_previus,
                            et_next
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

struct  s_status
{
    int  pending_repetitions;
    QDateTime   started;
    int  fails;

    s_status() : pending_repetitions(0),
                 started(QDateTime::currentDateTime()),
                 fails(0)
    {};
};


struct s_game
{
    QString  styles;
    QString  question;
    QString  result;
    QStringList  wrong_options;
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
    void game_ended(void);
    void generate_report(void);



    void fill_game(void);

    s_game  get_game_basic_adds(void);
    s_game  get_game_basic_products(void);
    s_game  get_game_basic_subs(void);
    s_game  get_game_basic_divs(void);
    s_game  get_game_basic_reverse_adds(void);
    s_game  get_game_basic_reverse_products(void);
    s_game  get_game_previus(void);
    s_game  get_game_next(void);



    s_config   config;
    s_status   status;

private slots:
    void on_exercices_list_itemChanged(QListWidgetItem* item);
    void on_reset_clicked();
    void on_start_clicked();
    void slot_option_selected(qoption_button*  sender);
};

#endif // MATH_PRACTICE_H
