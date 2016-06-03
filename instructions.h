/** @file instructions.h
 * @brief Contains declarations for variables, slots, and constructors for the Instructions class.
 *
 * Declares all the functionality for the instructions screen.
 */

#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include <QWidget>
#include <QTimer>
#include <QPixmap>


/** @namespace Ui
 *  @brief GUI namespace
 */
namespace Ui {
class Instructions;
}



/** @class Instructions
 * @brief Creates the instructions screen
 *
 * This class contains all animations and text on the instructions screen.
 */
class Instructions : public QWidget
{
    Q_OBJECT

public:
    explicit Instructions(QWidget *parent = 0);
    ~Instructions();
    void paintEvent (QPaintEvent *);

public slots:
    void move_player();
    void move_bullet();

private:
    Ui::Instructions *ui;

    // all variables used to animate images of player and enemies
    std::pair<int,int> player_position;
    std::pair<int,int> starting_player_position;
    std::pair<int,int> player_bullet_position;
    std::pair<int,int> enemy_bullet_position;
    std::pair<int,int> boss_position;
    std::pair<int,int> starting_boss_position;

    QTimer* move_timer;
    QTimer* bullet_timer;

    bool moving_right;
    bool boss_moving_right;

    QPixmap instructions1;
    QPixmap instructions2;
    QPixmap instructions3;
    QPixmap instructions3_5;
    QPixmap instructions4;
    QPixmap player;
    QPixmap player_bullet;
    QPixmap enemy;
    QPixmap enemy_bullet;
    QPixmap boss;
};


#endif // INSTRUCTIONS_H
