/** @mainpage Space Invaders
 * @author Douglas Yao
 * @date June 3, 2016
 *
 * This app creates a game influenced by the Space Invaders arcade game. The game will have a menu screen,
 * an instruction screen, and a level select screen where the player can select one of four difficulties.
 * The game consists of two levels. The first level is similar to classic Space Invaders where the player
 * controls a spaceship and fires bullets at rows of enemies that slowly advance toward the player.
 * After finishing the first level, the player advances to a boss stage where they have to defeat a single boss.
 *
 */

/** @file main.cpp
 * @brief standard Qt main
 */

#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("Space Invaders");
    w.show();

    return a.exec();
}
