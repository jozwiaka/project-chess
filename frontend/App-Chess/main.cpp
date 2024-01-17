#include "config.h"
#include "chessgame.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ChessGame game;
    game.Start();
    return a.exec();
}
