/** @file gameboard.cpp
 * @brief Contains implementation of Gameboard class. This class contains everything needed to play the "game" part of the app.
 */

#include "gameboard.h"
#include "ui_gameboard.h"
#include "mainwindow.h"
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTimer>
#include <QPainter>
#include <vector>
#include <QKeyEvent>
#include <QShowEvent>
#include <random>
#include <chrono>

// create random number generator to determine enemy firing
unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
std::default_random_engine generator (seed);


/** Contructor for the main gameboard. Loads images and initializes all timers and variables necessary to make the game work.
 * @param parent is the parent of the gameboard
 * @param new_enemy_speed is the speed of enemy movement
 * @param new_enemy_fire_rate is the rate that the enemies fire bullets
 * @param new_boss_speed is the speed of boss movement
 * @param new_boss_fire_rate is the rate that the boss fires bullets
 * @param new_boss_health is the amount of hits requires to defeat the boss
 */
Gameboard::Gameboard(QWidget *parent, int new_enemy_speed, int new_enemy_fire_rate, int new_boss_speed, int new_boss_fire_rate, int new_boss_health) :
    QWidget(parent),
    ui(new Ui::Gameboard)
{
    ui->setupUi(this);

    // set enemy speed and fire rate
    enemy_speed = new_enemy_speed;
    enemy_fire_rate = new_enemy_fire_rate;

    // set boss speed and fire rate
    boss_speed = new_boss_speed;
    boss_fire_rate = new_boss_fire_rate;

    // initialize positions of enemies. Positions stored as std::pair<int,int> with x and y coordinates.
    for (int i = 30; i < 500; i += 50) {
        for (int j = 40; j < 150; j += 50) {
            enemy_positions.push_back(std::make_pair(i,j));
        }
    }

    // load all images
    invader = QPixmap(":/image/IMAGES/invader.png");
    spaceship = QPixmap(":/image/IMAGES/spaceship.png");
    player_bullet = QPixmap(":/image/IMAGES/player_bullet.png");
    enemy_bullet = QPixmap(":/image/IMAGES/enemy_bullet.png");
    boss_text = QPixmap(":/image/IMAGES/boss_message.png");
    win_text = QPixmap(":/image/IMAGES/win_message.png");
    boss = QPixmap(":/image/IMAGES/boss.png");
    lives_remaining_message = QPixmap(":/image/IMAGES/lives_remaining.png");
    boss_health_message = QPixmap(":/image/IMAGES/boss_health.png");
    enemy_bullet_left = QPixmap(":/image/IMAGES/enemy_bullet_left.png");
    enemy_bullet_right = QPixmap(":/image/IMAGES/enemy_bullet_right.png");
    QPixmap explosion1(":/image/IMAGES/explosion1.png");
    QPixmap explosion2(":/image/IMAGES/explosion2.png");
    QPixmap explosion3(":/image/IMAGES/explosion3.png");
    QPixmap explosion4(":/image/IMAGES/explosion4.png");
    QPixmap explosion5(":/image/IMAGES/explosion5.png");
    QPixmap explosion6(":/image/IMAGES/explosion6.png");
    QPixmap explosion7(":/image/IMAGES/explosion7.png");
    QPixmap explosion8(":/image/IMAGES/explosion8.png");
    QPixmap explosion9(":/image/IMAGES/explosion9.png");
    QPixmap explosion10(":/image/IMAGES/explosion10.png");
    QPixmap explosion11(":/image/IMAGES/explosion11.png");
    QPixmap explosion12(":/image/IMAGES/explosion12.png");
    QPixmap explosion13(":/image/IMAGES/explosion13.png");
    QPixmap explosion14(":/image/IMAGES/explosion14.png");

    // add each frame of explosion animation to vector
    explosions.push_back(explosion1);
    explosions.push_back(explosion2);
    explosions.push_back(explosion3);
    explosions.push_back(explosion4);
    explosions.push_back(explosion5);
    explosions.push_back(explosion6);
    explosions.push_back(explosion7);
    explosions.push_back(explosion8);
    explosions.push_back(explosion9);
    explosions.push_back(explosion10);
    explosions.push_back(explosion11);
    explosions.push_back(explosion12);
    explosions.push_back(explosion13);
    explosions.push_back(explosion14);

   // enemy_positions.push_back(std::make_pair(30,40));

    // set initial player position
    player_position = std::make_pair(350,410);

    // set initial boss position
    boss_position = std::make_pair(250,40);

    // set initial lives
    lives_count = 3;

    // enemies initially moving right
    moving_right = true;

    // player is initially alive
    alive = true;

    // boss initially moves right
    boss_moving_right = true;

    // boss battle has not occurred yet, so set variables related to boss to false
    boss_message = false;
    start_boss_battle = false;
    boss_alive = false;
    win_message = false;

    // set initial boss health
    boss_health = new_boss_health;
    total_boss_health = new_boss_health;

    // starts timer for smooth left/right movement of player
    startTimer(15);

    // timer only allows player to shoot once per 300 milliseconds
    shoot_timer = new QTimer(this);
    shoot_timer->setInterval(300);
    shoot_timer->setSingleShot(true);

    // timer creates delay between last death and game over screen
    game_over_timer = new QTimer(this);
    game_over_timer->setInterval(2000);
    game_over_timer->setSingleShot(true);
    QObject::connect(game_over_timer,SIGNAL(timeout()), this, SLOT(emit_game_over()));

    // timer determines how quickly enemies move
    enemy_timer = new QTimer(this);
    enemy_timer->setInterval(enemy_speed);
    enemy_timer->start();
    QObject::connect(enemy_timer,SIGNAL(timeout()), this, SLOT(move_enemies()));

    // timer determines speed of bullets and collision detection
    bullet_timer = new QTimer(this);
    bullet_timer->setInterval(10);
    bullet_timer->start();
    QObject::connect(bullet_timer,SIGNAL(timeout()), this, SLOT(move_bullets()));
    QObject::connect(bullet_timer,SIGNAL(timeout()), this, SLOT(remove_enemy()));
    QObject::connect(bullet_timer,SIGNAL(timeout()),this, SLOT(player_hit()));
    QObject::connect(bullet_timer,SIGNAL(timeout()),this,SLOT(move_boss_bullet()));
    QObject::connect(bullet_timer,SIGNAL(timeout()),this,SLOT(player_hit_boss()));
    QObject::connect(bullet_timer,SIGNAL(timeout()),this,SLOT(boss_hit()));

    // timer used to create explosion animation
    explosion_timer = new QTimer(this);
    explosion_timer->setInterval(50);
    explosion_timer->start();
    QObject::connect(explosion_timer,SIGNAL(timeout()),this,SLOT(draw_explosion()));

    // timer used to create boss explosion animation
    boss_explosion_timer = new QTimer(this);
    boss_explosion_timer->setInterval(100);
    QObject::connect(boss_explosion_timer, SIGNAL(timeout()),this,SLOT(draw_boss_explosion()));

    // timer determines speed of boss movement
    boss_move_timer = new QTimer (this);
    boss_move_timer->setInterval(boss_speed);
    QObject::connect(boss_move_timer,SIGNAL(timeout()),this,SLOT(move_boss()));

    // timer determines how long player takes to respawn after dying
    respawn_timer = new QTimer(this);
    respawn_timer->setInterval(2000);
    QObject::connect(respawn_timer,SIGNAL(timeout()), this, SLOT(respawn()));

    // timer determines how long "boss battle" message remains on screen. Also creates a delay between defeating last enemy and appearance of boss battle message.
    boss_battle_timer = new QTimer(this);
    boss_battle_timer->setInterval(2000);
    QObject::connect(boss_battle_timer,SIGNAL(timeout()),this, SLOT(boss_battle_message()));

    // timer determines how quickly enemies fire bullets
    enemy_fire_bullet_timer = new QTimer(this);
    enemy_fire_bullet_timer->setInterval(enemy_fire_rate);
    enemy_fire_bullet_timer->start();
    QObject::connect(enemy_fire_bullet_timer,SIGNAL(timeout()), this, SLOT(enemy_fire_bullet()));

    // timer determines how quickly boss fires bullets
    boss_fire_rate_timer = new QTimer(this);
    boss_fire_rate_timer->setInterval(boss_fire_rate);
    QObject::connect(boss_fire_rate_timer,SIGNAL(timeout()),this,SLOT(boss_fire_bullet()));

    // timer determines how long "you win" message remains on screen. Also creates delay between defeating boss and appearance of win message.
    win_message_timer = new QTimer(this);
    win_message_timer->setInterval(2000);
    QObject::connect(win_message_timer,SIGNAL(timeout()),this,SLOT(win_message_appear()));

    // display game over or win screen when the corresponding signal is emitted
    QObject::connect(this,SIGNAL(game_over()),parent,SLOT(game_over_screen()));
    QObject::connect(this,SIGNAL(win_game()),parent,SLOT(win_screen()));
}

/** Destructor for Gameboard class
 */
Gameboard::~Gameboard()
{
    delete ui;
}



/** This function is responsible for displaying everything on the game screen, including the player, enemies, and all text and messages.
 */
void Gameboard::paintEvent(QPaintEvent *) {
    QPainter p(this);
    p.setPen(Qt::black);
    p.setBrush(Qt::black);

    // If boss battle is taking place
    //
    //
    if (start_boss_battle) {

        // draw the "Lives Remaining" label at the top of the screen
        p.drawPixmap(0,10,182,26,lives_remaining_message);
        int lives_drawn = 0;

        // draw images of spaceships next to "Lives Remaining" to indicate how many lives are left
        while (lives_drawn < lives_count-1) {
            p.drawPixmap(175+30*lives_drawn,10,25,25,spaceship);
            ++lives_drawn;
        }

        // draw "Boss Health" message that will be displayed below the boss health bar
        p.drawPixmap(0,50,138,26,boss_health_message);

        // draw the player bullets
        for (auto& x : player_bullet_positions)
            p.drawPixmap(x.first, x.second, 11, 15, player_bullet);

        // draw the boss bullets. Because the boss fires three bullets at a time in different directions, three separate bullet images were created to match whichever direction the bullet is being fired. The bullets must be matched to their proper image.
        for (auto& x : boss_bullet_positions) {
            if (std::get<2>(x) == 1)
                p.drawPixmap(std::get<0>(x), std::get<1>(x), 20, 20, enemy_bullet_left);
            if (std::get<2>(x) == 2)
                p.drawPixmap(std::get<0>(x), std::get<1>(x), 11, 15, enemy_bullet);
            if (std::get<2>(x) == 3)
                p.drawPixmap(std::get<0>(x), std::get<1>(x), 20, 20, enemy_bullet_right);
        }

        // if player is alive, draw the player
        if (alive)
            p.drawPixmap(player_position.first-10, player_position.second, 30, 30, spaceship);

        // if explosions are taking place on screen, draw the explosions. The std::vector explosion_locations stores both the x and y coordinates of the explosions and an integer indicating which frame of the explosion is being displayed.
        if (explosion_locations.size() > 0) {
            for (const auto& x : explosion_locations)
                p.drawPixmap(x.first.first-10, x.first.second, 30, 30, explosions[x.second]);
        }

        // if the boss is alive, draw the boss and the health bar.
        if (boss_alive) {
            p.setBrush(Qt::red);
            p.drawRect(10,40,680-(680/total_boss_health)*(total_boss_health-boss_health),10);
            p.drawPixmap(boss_position.first-50,boss_position.second,100,53,boss);
        }

        // if boss is dead, start two timers that will display the win message and an explosion animation
        if (boss_health == 0) {
            if (!win_message_timer->isActive())
                win_message_timer->start();
            if (!boss_explosion_timer->isActive())
                boss_explosion_timer->start();

            boss_alive = false;

            // draw the explosion
            p.drawPixmap(boss_explosion_location.first.first-40, boss_explosion_location.first.second, 80, 80, explosions[boss_explosion_location.second]);

        }

        // display win message
        if (win_message)
            p.drawPixmap(170,100,385,54,win_text);

        // if player has 0 lives, then start a timer that will display the game over screen
        if (lives_count < 1) {
            if (!game_over_timer->isActive())
                game_over_timer->start();
        }
    }




    // When all enemies have been defeated but the boss has not appeared yet.
    //
    //
    else if (enemy_positions.empty()) {

        // draw the "Lives Remaining" label at the top of the screen
        p.drawPixmap(0,10,182,26,lives_remaining_message);
        int lives_drawn = 0;

        // draw images of spaceships next to "Lives Remaining" to indicate how many lives are left
        while (lives_drawn < lives_count-1) {
            p.drawPixmap(175+30*lives_drawn,10,25,25,spaceship);
            ++lives_drawn;
        }

        // draw player bullets
        for (auto& x : player_bullet_positions)
            p.drawPixmap(x.first, x.second, 11, 15, player_bullet);

        // draw enemy bullets
        for (auto& x : enemy_bullet_positions)
            p.drawPixmap(x.first, x.second, 11, 15, enemy_bullet);

        // if player is alive, draw player. It is possible for player to be hit by a bullet after all enemies have been defeated.
        if (alive)
            p.drawPixmap(player_position.first-10, player_position.second, 30, 30, spaceship);

        // start timer that will display the "boss battle" message and start the boss battle
        if (!boss_battle_timer->isActive())
            boss_battle_timer->start();

        // if explosions are occuring, draw explosions
        if (explosion_locations.size() > 0) {
            for (const auto& x : explosion_locations)
                p.drawPixmap(x.first.first-10, x.first.second, 30, 30, explosions[x.second]);
        }

        // display win message
        if (boss_message)
            p.drawPixmap(90,100,534,54,boss_text);

        // if player has 0 lives, then start timer that will display game over screen. Again, it is still possible for a player to be hit by a bullet after all enemies have been defeated.
        if (lives_count < 1) {
            if (!game_over_timer->isActive())
                game_over_timer->start();
        }
    }




    // Before the boss battle and before all enemies have been defeated. In other words, the first main level.
    //
    //
    else {

        // draw the "Lives Remaining" label at the top of the screen
        p.drawPixmap(0,10,182,26,lives_remaining_message);
        int lives_drawn = 0;

        // draw images of spaceships next to "Lives Remaining" to indicate how many lives are left
        while (lives_drawn < lives_count-1) {
            p.drawPixmap(175+30*lives_drawn,10,25,25,spaceship);
            ++lives_drawn;
        }

        // draw enemies
        for (const auto& x : enemy_positions)
            p.drawPixmap(x.first-12, x.second, 35, 23, invader);

        // if explosions are occurring, draw explosions
        if (explosion_locations.size() > 0) {
            for (const auto& x : explosion_locations)
                p.drawPixmap(x.first.first-10, x.first.second, 30, 30, explosions[x.second]);
        }

        // player is alive, draw player
        if (alive)
            p.drawPixmap(player_position.first-10, player_position.second, 30, 30, spaceship);

        // draw player bullets
        for (auto& x : player_bullet_positions)
            p.drawPixmap(x.first, x.second, 11, 15, player_bullet);

        // draw enemy bullets
        for (auto& x : enemy_bullet_positions)
            p.drawPixmap(x.first, x.second, 11, 15, enemy_bullet);

        // if player has 0 lives or enemies reach the botton of the screen, start a timer that will display game over screen
        if (lives_count < 1 || enemy_positions[enemy_positions.size()-1].second > 400) {
            if (!game_over_timer->isActive())
                game_over_timer->start();
        }
    }
}

/** Records the state of key presses in the QMap keys. Normally, when a key is held down there is a delay before the key is repeated. Storing the state of the key presses and rapidly checking the states will bypass this. This function also fires a bullet from the player's position when space is pressed.
 * @param e is the key press event
 */
void Gameboard::keyPressEvent(QKeyEvent *e) {

    keys[e->key()] = true;
    if (keys[Qt::Key_Space]) {

        // if shoot timer is active, then player won't be able to fire. This sets the fastest fire rate of the player.
        if (!shoot_timer->isActive()) {
            player_bullet_positions.push_back(player_position);
            shoot_timer->start();
        }
    }

    QWidget::keyPressEvent(e);

}


/** Records when a key is released.
 * @param e is the key release event
 */
void Gameboard::keyReleaseEvent(QKeyEvent *e) {
    keys[e->key()] = false;
    QWidget::keyReleaseEvent(e);
}


/** Enables smooth player movement. Every time the timer times out, the state of the keys (whether they are pressed or not) is checked. If the left or right keys are pressed down, then the player will be moved accordingly. This will bypass the slight delay that normally occurs when a key is held down.
 */
void Gameboard::timerEvent(QTimerEvent *) {
    if (keys[Qt::Key_Left]) {
        if (player_position.first > 10)
            player_position.first -= 5;
    }

    if (keys[Qt::Key_Right]) {
        if (player_position.first < 680)
            player_position.first += 5;
    }

    QCoreApplication::processEvents();
    repaint();

}


/** Sets the focus of the gameboard when it first appears
 * @param e is the show event
 */
void Gameboard::showEvent(QShowEvent *e) {
    this->activateWindow();
    this->setFocus();
    QWidget::showEvent(e);
}


/** Move the boss from side to side.
 */
void Gameboard::move_boss() {

    // if boss is moving right and has not reached the far right of the screen, move it right
    if (boss_position.first + 20 < 700 && boss_moving_right) {
        boss_position.first += 3;
    }

    // if boss is at far right, then switch directions
    else if (boss_position.first + 20 >= 700 && boss_moving_right) {
        boss_moving_right = false;
    }

    // if boss is moving left is has not reached the far left of the screen, move it left
    else if (boss_position.first > 0 && !boss_moving_right) {
        boss_position.first -= 3;
    }

    // if boss is at far left, then switch directions
    else if (boss_position.first <= 0 && !boss_moving_right) {
        boss_moving_right = true;
    }
}



/** Move the enemies side to side.
 */
void Gameboard::move_enemies() {

    // if enemies are present
    if (enemy_positions.size() > 0) {

        // if enemies are moving right and rightmost enemy has not reached the far right of the screen, move enemies right
        if (enemy_positions[enemy_positions.size()-1].first + 20 < 700 && moving_right) {
            for (auto& x : enemy_positions)
                x.first += 20;
        }

        // if rightmost enemy is at far right of screen, then move enemies down and change directions
        else if (enemy_positions[enemy_positions.size()-1].first + 20 >= 700 && moving_right) {
            for (auto& x : enemy_positions)
                x.second += 20;
            moving_right = false;
        }

        // if enemies are moving left and leftmost enemy has not reached the far left of the screen, move enemies left
        else if (enemy_positions[0].first - 20 > 0 && !moving_right) {
            for (auto& x : enemy_positions)
                x.first -= 20;
        }

        // if leftmost enemy is at far left of screen, then move enemies down and change directions
        else if (enemy_positions[0].first - 20 <= 0 && !moving_right) {
            for (auto& x : enemy_positions)
                x.second += 20;
            moving_right = true;
        }
    }
}


/** Move player's and enemies' bullets. Also remove bullets they go offscreen.
 */
void Gameboard::move_bullets() {
    bool player_removed = false;
    bool enemy_removed = false;
    int player_to_be_removed;
    int enemy_to_be_removed;

    // move each of the player's bullets up
    for (size_t i = 0, n = player_bullet_positions.size(); i < n; ++i) {
        player_bullet_positions[i].second -= 3;

        // if player's bullets reach the top of the screen, then remove them
        if (player_bullet_positions[i].second < 0) {
            player_removed = true;
            player_to_be_removed = i;
        }
    }

    // move each of the enemies' bullets down
    for (size_t i = 0, n = enemy_bullet_positions.size(); i < n; ++i) {
        enemy_bullet_positions[i].second += 3;

        // if enemies' bullets reach bottom of the screen, then remove them
        if (enemy_bullet_positions[i].second > 550) {
            enemy_removed = true;
            enemy_to_be_removed = i;
        }

    }

    // if player's bullets to be removed, then erase them from the vector containing all the player's bullet positions
    if (player_removed) {
       player_bullet_positions.erase(player_bullet_positions.begin() + player_to_be_removed);
    }

    // if enemies' bullets to be removed, then erase them from the vector containing all the enemies' bullet positions
    if (enemy_removed) {
       enemy_bullet_positions.erase(enemy_bullet_positions.begin() + enemy_to_be_removed);
    }
}

/** Randomly fires bullets from enemies. Enemy bullets are stored in a vector of std::pairs that contain the x and y coordinates of the bullets.
 */
void Gameboard::enemy_fire_bullet() {

    // if enemies present, then select an enemy at random and store its location in the vector containing the enemies' bullet positions
    if (enemy_positions.size() > 0) {
        std::uniform_int_distribution<int> distribution(0,enemy_positions.size()-1);
        enemy_bullet_positions.push_back(enemy_positions[distribution(generator)]);
    }
}


/** Fires bullets from boss. Boss bullets are stored in a vector of std::tuples containing three elements. The first two elements in the tuple are the x and y coordinates of the bullet. The third element is an integer than indicates which direction the boss's bullets should go. 1 corresponds to moving at a 45 degree angle to the left, 2 corresponds to moving straight down, and 3 corrresponds to moving at a 45 degree angle to the right.
 */
void Gameboard::boss_fire_bullet() {

    // boss fires one of each type of bullet at a time
    if (boss_alive) {
        boss_bullet_positions.push_back(std::make_tuple(boss_position.first, boss_position.second, 1));
        boss_bullet_positions.push_back(std::make_tuple(boss_position.first, boss_position.second, 2));
        boss_bullet_positions.push_back(std::make_tuple(boss_position.first, boss_position.second, 3));
    }
}


/** Move boss bullets down or diagonally depending on the type of bullet. Also removes bullets if they go offscreen.
 */
void Gameboard::move_boss_bullet() {
    bool boss_removed = false;
    int boss_to_be_removed;

    for (size_t i = 0, n = boss_bullet_positions.size(); i < n; ++i) {

        // 1 corresponds to a bullet that is moving 45 degrees to the left
        if (std::get<2>(boss_bullet_positions[i]) == 1) {
            std::get<1>(boss_bullet_positions[i]) += 2;
            std::get<0>(boss_bullet_positions[i]) -= 2;
        }

        // 2 corresponds to a bullet that is moving straight down
        if (std::get<2>(boss_bullet_positions[i]) == 2) {
            std::get<1>(boss_bullet_positions[i]) += 3;
        }

        // 3 corresponds to a bullet that is moving 45 degrees to the right
        if (std::get<2>(boss_bullet_positions[i]) == 3) {
            std::get<1>(boss_bullet_positions[i]) += 2;
            std::get<0>(boss_bullet_positions[i]) += 2;
        }

        // if bullets reach the bottom or sides of the screen, then remove them
        if (std::get<1>(boss_bullet_positions[i]) > 550 || std::get<0>(boss_bullet_positions[i]) < 10 || std::get<0>(boss_bullet_positions[i]) > 700) {
            boss_removed = true;
            boss_to_be_removed = i;
        }
    }

    // if bullets to be removed, then erase them from the vector of boss bullet locations
    if (boss_removed) {
        boss_bullet_positions.erase(boss_bullet_positions.begin() + boss_to_be_removed);
    }
}


/** This function detects collisions between player bullets and enemies. Removes enemies and bullets if player bullets hit them.
 */
void Gameboard::remove_enemy() {
    bool removed = false;
    int to_be_removed_enemy;
    int to_be_removed_bullet;
    std::pair<int,int> position;

    // checks each bullets with each enemy to determine if their positions overlap. If they do, then stores their positions in their respective vectors for removal.
    if (enemy_positions.size() > 0) {
        for (size_t i = 0, n = player_bullet_positions.size(); i < n; ++i) {
            for (size_t j = 0, m = enemy_positions.size(); j < m; ++j) {
                if ((player_bullet_positions[i].first > enemy_positions[j].first-20 && player_bullet_positions[i].first < enemy_positions[j].first+20) && (player_bullet_positions[i].second > enemy_positions[j].second-11 && player_bullet_positions[i].second < enemy_positions[j].second+11)) {
                    removed = true;
                    position = enemy_positions[j];
                    to_be_removed_enemy = j;
                    to_be_removed_bullet = i;
                }
            }
        }
    }

    // removes enemy and bullet from their corresponding position vectors
    if (removed) {
        player_bullet_positions.erase(player_bullet_positions.begin() + to_be_removed_bullet);
        enemy_positions.erase(enemy_positions.begin() + to_be_removed_enemy);

        // adds position of enemy death to explosion location vector
        explosion_locations.push_back(std::make_pair(position, 0));
    }
}


/** Checks for collisions between enemy bullets and player. If collisions occur, then remove bullets and decrement lives count.
 */
void Gameboard::player_hit() {
    bool removed = false;
    int to_be_removed_bullet;
    std::pair<int,int> position;

    // compares each enemy bullet with player to see if their positions overlap. If they do, then stores the positions of the player and the bullet for removal
    for (size_t i = 0, n = enemy_bullet_positions.size(); i < n; ++i) {
        if ((enemy_bullet_positions[i].first > player_position.first-15 && enemy_bullet_positions[i].first < player_position.first+15) && (enemy_bullet_positions[i].second > player_position.second-10 && enemy_bullet_positions[i].second < player_position.second+10)) {
            removed = true;
            to_be_removed_bullet = i;
            position = player_position;
        }
    }

    // if collision occurs
    if (removed) {

        // remove enemy bullet from vector of enemy bullet locations
        enemy_bullet_positions.erase(enemy_bullet_positions.begin() + to_be_removed_bullet);

        // decrement lives count
        --lives_count;
        alive = false;

        // temporarily move player off screen while player respawns
        player_position = std::make_pair(-50,-50);

        // add explosion to explosion locations vector
        explosion_locations.push_back(std::make_pair(position, 0));

        // start respawn timer
        respawn_timer->start();
    }
}


/** Checks for collisions between boss bullets and player. If collisions occur, then remove boss bullets and decrement lives count.
 */
void Gameboard::player_hit_boss() {
    bool removed = false;
    int to_be_removed_bullet;
    std::pair<int,int> position;

    // compares each boss bullet with player to see if their positions overlap. If they do, then stores the positions of the player and the bullet for removal
    for (size_t i = 0, n = boss_bullet_positions.size(); i < n; ++i) {
        if ((std::get<0>(boss_bullet_positions[i]) > player_position.first-15 && std::get<0>(boss_bullet_positions[i]) < player_position.first+15) && (std::get<1>(boss_bullet_positions[i]) > player_position.second-10 && std::get<1>(boss_bullet_positions[i]) < player_position.second+10)) {
            removed = true;
            to_be_removed_bullet = i;
            position = player_position;
        }
    }

    // if collision occurs
    if (removed) {

        // remove boss bullet from vector of boss bullet locations
        boss_bullet_positions.erase(boss_bullet_positions.begin() + to_be_removed_bullet);

        // decrement lives count
        --lives_count;
        alive = false;

        // temporarily move player off screen while player respawns
        player_position = std::make_pair(-50,-50);

        // add explosion to explosion locations vector
        explosion_locations.push_back(std::make_pair(position,0));

        // start respawn timer
        respawn_timer->start();
    }
}


/** Checks for collisions between player bullets and boss. If collisions occur, then remove player bullets and decrement boss health.
 */
void Gameboard::boss_hit() {
    bool removed = false;
    int to_be_removed_bullet;

    // compares each player bullet with boss to see if their positions overlap. If they do, then stores the positions of the bullet for removal
    if (boss_alive) {
        for (size_t i = 0, n = player_bullet_positions.size(); i < n; ++i) {
            if ((player_bullet_positions[i].first > boss_position.first-50 && player_bullet_positions[i].first < boss_position.first+50) && (player_bullet_positions[i].second > boss_position.second-30 && player_bullet_positions[i].second < boss_position.second+30)) {
                removed = true;
                to_be_removed_bullet = i;
            }
        }

        // if collision occurs, then remove bullet from vector of player bullet locations and decrement boss health
        if (removed) {
            player_bullet_positions.erase(player_bullet_positions.begin() + to_be_removed_bullet);
            --boss_health;
        }

        // if boss has no health left, then set boss explosion location to boss's current location
        if (boss_health == 0)
            boss_explosion_location = std::make_pair(boss_position,0);
    }
}

/** Draws explosions for players and enemies. Explosion locations are stored in a vector of std::pairs, where the first element is itself an std::pair that stores the x and y coordinates of the explosion, and the second element is an integer between 0 and 13 that indicates which frame of the explosion is being displayed. This function increments the second element for each object, which advances the explosion animation to the next frame. It also removes the explosion once all 14 frames have been displayed.
 */
void Gameboard::draw_explosion() {
    bool removed = false;
    int removed_pos;
    for (int i = 0, n = explosion_locations.size(); i < n; ++i) {

        // increment integer representing explosion frame
        if (explosion_locations[i].second < 13)
            ++explosion_locations[i].second;
        else {
            removed = true;
            removed_pos = i;
        }
    }

    // remove explosion once animation is finished
    if (removed) {
        explosion_locations.erase(explosion_locations.begin() + removed_pos);
    }

    QCoreApplication::processEvents();
    repaint();
}


/** Draws explosion for boss.
 */
void Gameboard::draw_boss_explosion() {
    if (boss_explosion_location.second < 13)

        // increments explosion frame
        ++boss_explosion_location.second;
    else

        // moves animation offscreen once it is finished
        boss_explosion_location.first = std::make_pair(-50,-50);
}


/** Emits a game_over signal. Connected to the game over timer.
 */
void Gameboard::emit_game_over()
{
    emit game_over();
}


/** This function is called after the player has been hit. It causes the player to reappear on screen and stops the respawn timer.
 */
void Gameboard::respawn() {
    player_position = std::make_pair(350,410);
    alive = true;
    respawn_timer->stop();
}

/** After the final enemy is defeated, a timer connected to this function will start. This function will be called 2 times total. Before the first time the function is called, no message will be displayed but the player and bullets will still be displayed. This creates a delay before the appearance of the message. Once the function is called for the first time, it will display the "boss battle" message. The message will remain on screen for the duration of the timer interval until the function is called for the second time, which removes the message and starts the boss battle.
 */
void Gameboard::boss_battle_message() {

    // if boss message is not displayed, then it will be displayed
    if (boss_message == false)
        boss_message = true;

    // if boss message is already displayed, then it will be removed and the boss battle will start
    else if (boss_message == true) {
        boss_message = false;
        start_boss_battle = true;
        boss_alive = true;
        boss_battle_timer->stop(); // stops the boss battle timer so function is only called twice
        enemy_fire_bullet_timer->stop();
        enemy_timer->stop();
        boss_move_timer->start();
        boss_fire_rate_timer->start();
    }

}

/** This function operates similarly to Gameboard::boss_battle_message(). It will be called 2 times total and displays the win message after a delay. After the win message is displayed, the win signal is emitted.
 */
void Gameboard::win_message_appear() {

    // if win message is not displayed, then it will be displayed
    if (win_message == false)
        win_message = true;

    // if win message is already displayed, then the win signal is emitted
    else if (win_message == true) {
        win_message_timer->stop();
        emit win_game();
    }
}




