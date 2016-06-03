/** @file mainwindow.h
 * @brief Contains declarations for variables, slots, and constructors for the MainWindow class.
 *
 * Declares all the functionality for displaying and navigating between menu screens.
 */


#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "gameboard.h"
#include "instructions.h"


/** @namespace Ui
 *  @brief GUI namespace
 */
namespace Ui {
class MainWindow;
}



/** @class MainWindow
 * @brief Creates menu screens and implements the functionality for navigating between them
 *
 * This class creates all menu screens and implements the functionality for navigating between them.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void select_level();
    void select_instructions();
    void return_to_menu();
    void menu_screen();
    void easy_game_begin();
    void medium_game_begin();
    void hard_game_begin();
    void impossible_game_begin();
    void game_over_screen();
    void win_screen();


private:
    Ui::MainWindow *ui;
    Gameboard* board;
    Instructions* instructions;
    int difficulty;

    QPixmap welcome_message;
    QPixmap select_difficulty;
    QPixmap win;
    QPixmap game_over_message;


};

#endif // MAINWINDOW_H
