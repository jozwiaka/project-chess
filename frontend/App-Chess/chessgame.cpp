#include "chessgame.h"

ChessGame::ChessGame(QObject *parent)
    : QObject{parent}, whiteTurn(true)
{}

void ChessGame::startGame()
{
    whiteTurn = true;
}

void ChessGame::makeMove(const QString& fromSquare, const QString& toSquare)
{

    whiteTurn = !whiteTurn;
}

bool ChessGame::isCheckmate() const {
    return false;
}
