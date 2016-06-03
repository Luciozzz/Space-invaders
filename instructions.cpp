/** @file instructions.cpp
 * @brief Contains implementation of Instructions class. This class contains all labels and animated parts of the instructions screen.
 */

#include "instructions.h"
#include "ui_instructions.h"
#include <QPainter>

/** Constructor for Instructions class. Loads all images and initializes timers and variables necessary to animate images of player and enemies
 * @param parent is the parent widget
 */
Instructions::Instructions(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Instructions)
{
    ui->setupUi(this);

    // loads all instruction text
    instructions1 = QPixmap(":/image/IMAGES/instructions1.png");
    instructions2 = QPixmap(":/image/IMAGES/instructions2.png");
    instructions3 = QPixmap(":/image/IMAGES/instructions3.png");
    instructions3_5 = QPixmap(":/image/IMAGES/instructions3.5.png");
    instructions4 = QPixmap(":/image/IMAGES/instructions4.png");

    // loads images
    player = QPixmap(":/image/IMAGES/spaceship.png");
    enemy = QPixmap(":/image/IMAGES/invader.png");
    player_bullet = QPixmap(":/image/IMAGES/player_bullet.png");
    enemy_bullet = QPixmap(":/image/IMAGES/enemy_bullet.png");
    boss = QPixmap(":/image/IMAGES/boss.png");

    // sets up positions of images of player and enemies
    moving_right = true;
    boss_moving_right = true;
    player_position = std::make_pair(560, 0);
    starting_player_position = player_position;
    player_bullet_position = std::make_pair(568, 125);
    enemy_bullet_position = std::make_pair(566,220);
    boss_position = std::make_pair(500, 325);
    starting_boss_position = boss_position;

    // sets up timer for animating images
    move_timer = new QTimer(this);
    move_timer->setInterval(15);
    move_timer->start();

    QObject::connect(move_timer, SIGNAL(timeout()), this, SLOT(move_player()));
    QObject::connect(move_timer, SIGNAL(timeout()), this, SLOT(move_bullet()));
}


/** Destructor for Instructions class
 */
Instructions::~Instructions()
{
    delete ui;
}


/** This function is responsible for displaying all labels and images on the screen.
 */
void Instructions::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.drawPixmap(15,15,instructions1);
    p.drawPixmap(176,126,instructions2);
    p.drawPixmap(56,220,instructions3);
    p.drawPixmap(99,245,instructions3_5);
    p.drawPixmap(15,350,instructions4);
    p.drawPixmap(player_position.first, player_position.second, 50, 50, player);
    p.drawPixmap(player_bullet_position.first, player_bullet_position.second, 11, 15, player_bullet);
    p.drawPixmap(548, 130, 50, 50, player);
    p.drawPixmap(enemy_bullet_position.first, enemy_bullet_position.second, 11, 15, enemy_bullet);
    p.drawPixmap(540, 200, 65, 40, enemy);
    p.drawPixmap(boss_position.first, boss_position.second, 125, 66, boss);
}


/** Moves player and boss image from left to right
 */
void Instructions::move_player() {
    // if player is moving right and has not reached the far right of the screen, move it right
    if (player_position.first < starting_player_position.first+50 && moving_right) {
        player_position.first += 2;
    }

    // if player is at far right, then switch directions
    else if (player_position.first >= starting_player_position.first+50 && moving_right) {
        moving_right = false;
    }

    // if player is moving left is has not reached the far left of the screen, move it left
    else if (player_position.first > starting_player_position.first-50 && !moving_right) {
        player_position.first -= 2;
    }

    // if player is at far left, then switch directions
    else if (player_position.first <= starting_player_position.first-50 && !moving_right) {
        moving_right = true;
    }

    // if boss is moving right and has not reached the far right of the screen, move it right
    if (boss_position.first < starting_boss_position.first+30 && boss_moving_right) {
        boss_position.first += 1;
    }

    // if boss is at far right, then switch directions
    else if (boss_position.first >= starting_boss_position.first+30 && boss_moving_right) {
        boss_moving_right = false;
    }

    // if boss is moving left is has not reached the far left of the screen, move it left
    else if (boss_position.first > starting_boss_position.first-25 && !boss_moving_right) {
        boss_position.first -= 1;
    }

    // if boss is at far left, then switch directions
    else if (boss_position.first <= starting_boss_position.first-25 && !boss_moving_right) {
        boss_moving_right = true;
    }

    QCoreApplication::processEvents();
    repaint();
}


/** Moves player and enemy bullet animations
 */
void Instructions::move_bullet() {

    // moves player bullet up
    if (player_bullet_position.second > 50) {
        player_bullet_position.second -= 3;
    }

    // moves player bullet back
    else if (player_bullet_position.second <= 50) {
        player_bullet_position.second = 125;
    }

    // moves enemy bullet up
    if (enemy_bullet_position.second < 300) {
        enemy_bullet_position.second += 3;
    }

    // moves enemy bullet back
    else if (enemy_bullet_position.second >= 300) {
        enemy_bullet_position.second = 220;
    }
}

