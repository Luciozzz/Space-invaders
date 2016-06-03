/** @file mainwindow.cpp
 * @brief Contains implementation of the Mainwindow class. This class displays and organizes menu screens.
 *
 * This class is responsible for displaying and organizing menu screens and the game screen.
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "gameboard.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>


/** Constructor for MainWindow
 * @param parent is the parent of the window
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setFixedSize(700,500);

    // loads title images
    welcome_message = QPixmap(":/image/IMAGES/welcome.png");
    select_difficulty = QPixmap(":/image/IMAGES/select_difficulty.png");
    win = QPixmap(":/image/IMAGES/congratulations.png");
    game_over_message = QPixmap(":/image/IMAGES/game_over.png");

    // displays main menu
    this->menu_screen();

}


/** Destructor for MainWindow class
 */
MainWindow::~MainWindow()
{
    delete ui;
}


/** Displays the main menu screen
 */
void MainWindow::menu_screen() {
    QWidget* central = new QWidget;

    // sets title of menu screen
    QLabel* welcome = new QLabel;
    welcome->setPixmap(welcome_message);
    welcome->setAlignment(Qt::AlignCenter);

    // creates "start game" button that goes to level select screen when pressed
    QPushButton* start_game_button = new QPushButton;
    start_game_button->setText("Start Game");
    QObject::connect(start_game_button,SIGNAL(clicked(bool)),this,SLOT(select_level()));

    // creates "instructions" button that goes to instructions screen when pressed
    QPushButton* instructions_button = new QPushButton;
    instructions_button->setText("Instructions");
    QObject::connect(instructions_button,SIGNAL(clicked(bool)),this,SLOT(select_instructions()));

    // creates "quit" button that quits application when pressed
    QPushButton* quit_button = new QPushButton;
    quit_button->setText("Quit");
    QObject::connect(quit_button,SIGNAL(clicked(bool)),this,SLOT(close()));

    // adds buttons to layout
    QVBoxLayout* buttons = new QVBoxLayout;
    buttons->addWidget(welcome);
    buttons->addWidget(start_game_button);
    buttons->addWidget(instructions_button);
    buttons->addWidget(quit_button);

    // adds layout to central widget
    central->setLayout(buttons);

    // sets central widget
    this->setCentralWidget(central);
}


/** Displays the level select screen
 */
void MainWindow::select_level() {

    // clears current screen
    QWidget* wid = this->centralWidget();
    wid->setParent(nullptr);

    QWidget* central = new QWidget;

    // sets title of screen
    QLabel* difficulty = new QLabel();
    difficulty->setPixmap(select_difficulty);
    difficulty->setAlignment(Qt::AlignCenter);

    // creates "easy" button that starts easy game when pressed
    QPushButton* easy_button = new QPushButton;
    easy_button->setText("Easy");
    QObject::connect(easy_button,SIGNAL(clicked(bool)),this,SLOT(easy_game_begin()));

    // creates "medium" button that starts medium game when pressed
    QPushButton* medium_button = new QPushButton;
    medium_button->setText("Medium");
    QObject::connect(medium_button,SIGNAL(clicked(bool)),this,SLOT(medium_game_begin()));

    // creates "hard" button that starts hard game when pressed
    QPushButton* hard_button = new QPushButton;
    hard_button->setText("Hard");
    QObject::connect(hard_button,SIGNAL(clicked(bool)),this,SLOT(hard_game_begin()));

    // creates "impossible" button that starts impossible game when pressed
    QPushButton* impossible_button = new QPushButton;
    impossible_button->setText("Impossible");
    QObject::connect(impossible_button,SIGNAL(clicked(bool)),this,SLOT(impossible_game_begin()));

    // creates "back" button that goes back to menu screen when pressed
    QPushButton* go_back = new QPushButton;
    go_back->setText("Back");
    QObject::connect(go_back,SIGNAL(clicked(bool)),this,SLOT(return_to_menu()));

    // add buttons to layout
    QVBoxLayout* buttons = new QVBoxLayout;
    buttons->addWidget(difficulty);
    buttons->addWidget(easy_button);
    buttons->addWidget(medium_button);
    buttons->addWidget(hard_button);
    buttons->addWidget(impossible_button);
    buttons->addWidget(go_back);

    // add layout to central widget
    central->setLayout(buttons);

    // set central widget
    this->setCentralWidget(central);

}

/** Displays the instructions screen
 */
void MainWindow::select_instructions() {

    // clears current screen
    QWidget* wid = this->centralWidget();
    wid->setParent(nullptr);

    QWidget* central = new QWidget;

    // creates instructions
    instructions = new Instructions(this);

    // creates "back" button that returns to menu screen when pressed
    QPushButton* go_back = new QPushButton;
    go_back->setText("Back");
    QObject::connect(go_back,SIGNAL(clicked(bool)),this,SLOT(return_to_menu()));

    // add instructions and button to layout
    QVBoxLayout* instructions_layout = new QVBoxLayout;
    instructions_layout->addWidget(instructions);
    instructions_layout->addWidget(go_back);

    // add layout to central widget
    central->setLayout(instructions_layout);

    // set central widget
    this->setCentralWidget(central);
}


/** Returns to menu screen
 */
void MainWindow::return_to_menu() {

    // clears current screen
    QWidget* wid = this->centralWidget();
    wid->setParent(nullptr);

    // displays menu screen
    this->menu_screen();
}


/** Displays win screen
 */
void MainWindow::win_screen() {

    // clears current screen
    QWidget* wid = this->centralWidget();
    wid->setParent(nullptr);

    QWidget* central = new QWidget;

    // create win message
    QLabel* win_message = new QLabel;
    win_message->setPixmap(win);
    win_message->setAlignment(Qt::AlignCenter);

    // create "play again" button that returns to level select screen when pressed
    QPushButton* play_again_button = new QPushButton;
    play_again_button->setText("Play again?");
    QObject::connect(play_again_button,SIGNAL(clicked(bool)),this,SLOT(select_level()));

    // create "return to menu" button that returns to menu screen when pressed
    QPushButton* return_to_menu_button = new QPushButton;
    return_to_menu_button->setText("Return to menu");
    QObject::connect(return_to_menu_button,SIGNAL(clicked(bool)),this,SLOT(return_to_menu()));

    // add buttons to layout
    QVBoxLayout* win_layout = new QVBoxLayout;
    win_layout->addWidget(win_message);
    win_layout->addWidget(play_again_button);
    win_layout->addWidget(return_to_menu_button);

    // add layout to central widget
    central->setLayout(win_layout);

    // set central widget
    this->setCentralWidget(central);
}


/** Displays game over screen
 */
void MainWindow::game_over_screen() {

    // clears current screen
    QWidget* wid = this->centralWidget();
    wid->setParent(nullptr);

    QWidget* central = new QWidget;

    // creates game over message
    QLabel* game_over = new QLabel;
    game_over->setPixmap(game_over_message);
    game_over->setAlignment(Qt::AlignCenter);

    // creates "retry" button that restarts game depending on which difficulty the player was initially on
    QPushButton* retry_button = new QPushButton;
    retry_button->setText("Retry");

    if (difficulty == 1) {
        QObject::connect(retry_button,SIGNAL(clicked(bool)),this,SLOT(easy_game_begin()));
    }

    if (difficulty == 2) {
        QObject::connect(retry_button,SIGNAL(clicked(bool)),this,SLOT(medium_game_begin()));
    }

    if (difficulty == 3) {
        QObject::connect(retry_button,SIGNAL(clicked(bool)),this,SLOT(hard_game_begin()));
    }

    if (difficulty == 4) {
        QObject::connect(retry_button,SIGNAL(clicked(bool)),this,SLOT(impossible_game_begin()));
    }

    // creates "return to menu" button that returns to menu screen when pressed
    QPushButton* return_to_menu_button = new QPushButton;
    return_to_menu_button->setText("Return to menu");
    QObject::connect(return_to_menu_button,SIGNAL(clicked(bool)),this,SLOT(return_to_menu()));

    // adds buttons to layout
    QVBoxLayout* game_over_layout = new QVBoxLayout;
    game_over_layout->addWidget(game_over);
    game_over_layout->addWidget(retry_button);
    game_over_layout->addWidget(return_to_menu_button);

    // adds layout to central widget
    central->setLayout(game_over_layout);

    // sets central widget
    this->setCentralWidget(central);

}


/** Starts an easy game
 */
void MainWindow::easy_game_begin() {

    // clears current screen
    QWidget* wid = this->centralWidget();
    wid->setParent(nullptr);

    // creates gameboard with corresponding "easy" settings. In the order they are initialized, they are: enemy speed, enemy fire rate, boss speed, boss fire rate, and boss health.
    board = new Gameboard(this,600,600,30,1000,20);
    this->setCentralWidget(board);
    difficulty = 1;
}


/** Starts a medium game
 */
void MainWindow::medium_game_begin() {

    // clears current screen
    QWidget* wid = this->centralWidget();
    wid->setParent(nullptr);

    // creates gameboard with corresponding "medium" settings. In the order they are initialized, they are: enemy speed, enemy fire rate, boss speed, boss fire rate, and boss health.
    board = new Gameboard(this,550,400,30,750,25);
    this->setCentralWidget(board);
    difficulty = 2;
}


/** Starts a hard game
 */
void MainWindow::hard_game_begin() {

    // clears current screen
    QWidget* wid = this->centralWidget();
    wid->setParent(nullptr);

    // creates gameboard with corresponding "hard" settings. In the order they are initialized, they are: enemy speed, enemy fire rate, boss speed, boss fire rate, and boss health.
    board = new Gameboard(this,500,200,20,400,30);
    this->setCentralWidget(board);
    difficulty = 3;
}


/** Starts an impossible game
 */
void MainWindow::impossible_game_begin() {

    // clears current screen
    QWidget* wid = this->centralWidget();
    wid->setParent(nullptr);

    // creates gameboard with corresponding "impossible" settings. In the order they are initialized, they are: enemy speed, enemy fire rate, boss speed, boss fire rate, and boss health.
    board = new Gameboard(this,450,100,10,300,40);
    this->setCentralWidget(board);
    difficulty = 4;
}

