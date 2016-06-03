/** @file gameboard.h
 * @brief Contains declarations for variables, slots, and constructors for the Gameboard class.
 *
 * Declares all the functionality for the "game" part of the app to run.
 */

#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <QWidget>
#include <vector>
#include <utility>
#include <QMap>
#include <QLabel>
#include <QTimer>
#include <tuple>
#include <QPixmap>


/** @namespace Ui
 *  @brief GUI namespace
 */
namespace Ui {
class Gameboard;
}



/** @class Gameboard
 * @brief The actual "game" part of the app
 *
 * This class contains everything needed to play Space Invaders.
 */
class Gameboard : public QWidget
{
    Q_OBJECT

public:
    explicit Gameboard(QWidget *parent, int new_enemy_speed, int new_enemy_fire_rate, int new_boss_speed, int new_boss_fire_rate, int new_boss_health);
    ~Gameboard();
    void paintEvent(QPaintEvent*);
    void keyPressEvent(QKeyEvent *e);
    void keyReleaseEvent(QKeyEvent *e);
    void timerEvent(QTimerEvent* );
    void showEvent(QShowEvent *e);

signals:
    void game_over();
    void win_game();

public slots:
    void move_enemies();
    void move_bullets();
    void remove_enemy();
    void enemy_fire_bullet();
    void player_hit();
    void respawn();
    void boss_battle_message();
    void move_boss();
    void move_boss_bullet();
    void boss_fire_bullet();
    void player_hit_boss();
    void boss_hit();
    void win_message_appear();
    void draw_explosion();
    void draw_boss_explosion();
    void emit_game_over();


private:
    Ui::Gameboard *ui;

    // stores the state of key presses for smooth movement
    QMap<int,bool> keys;

    // all the images in the game
    QPixmap invader;
    QPixmap spaceship;
    QPixmap enemy_bullet;
    QPixmap enemy_bullet_left;
    QPixmap enemy_bullet_right;
    QPixmap player_bullet;
    QPixmap boss_text;
    QPixmap win_text;
    QPixmap boss;
    QPixmap lives_remaining_message;
    QPixmap boss_health_message;
    std::vector<QPixmap> explosions;

    // vectors that store explosion locations
    std::vector<std::pair<std::pair<int,int>, int>> explosion_locations;
    std::pair<std::pair<int,int>, int> boss_explosion_location;

    // timers related to explosions
    QTimer* explosion_timer;
    QTimer* boss_explosion_timer;

    // timers related to displaying messages
    QTimer* boss_battle_timer;
    QTimer* game_over_timer;
    QTimer* win_message_timer;


    // ************** PLAYER VARIABLES ****************//

    // timers related to player respawn/fire-rate
    QTimer* shoot_timer;
    QTimer* respawn_timer;

    // vectors that store player location and bullet locations
    std::vector<std::pair<int,int>> player_bullet_positions;
    std::pair<int,int> player_position;

    // other variables related to player
    bool alive;
    bool moving_right;
    int lives_count;


    // ************** ENEMY VARIABLES *****************//

    // timers related to enemy movement/fire-rate
    QTimer* enemy_timer;
    QTimer* bullet_timer;
    QTimer* enemy_fire_bullet_timer;

    // vectors that store enemy locations and bullet locations
    std::vector<std::pair<int,int>> enemy_bullet_positions;
    std::vector<std::pair<int,int>> enemy_positions;

    // other variables related to enemies
    int enemy_speed;
    int enemy_fire_rate;


    // ************** BOSS VARIABLES ****************//

    // timers related to boss movement/fire-rate
    QTimer* boss_move_timer;
    QTimer* boss_bullet_timer;
    QTimer* boss_fire_rate_timer;

    // vectors that store boss position and bullet positions
    std::pair<int,int> boss_position;
    std::vector<std::tuple<int,int,int>> boss_bullet_positions;

    // other variables related to boss
    bool boss_message;
    bool start_boss_battle;
    bool boss_moving_right;
    int boss_health;
    int total_boss_health;
    int boss_speed;
    int boss_fire_rate;
    bool win_message;
    bool boss_alive;

};



#endif // GAMEBOARD_H
