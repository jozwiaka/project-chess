#include "chessgame.h"
#include "chesssquare.h"
#include <memory>

ChessGame::ChessGame(QObject *parent)
    : QObject{parent}, currentTurn(Color::WHITE)
{}

void ChessGame::startGame()
{
    currentTurn = Color::WHITE;
}

void ChessGame::makeMove(const QString& fromSquare, const QString& toSquare)
{

    currentTurn = Color::WHITE;
}

bool ChessGame::isCheckmate() const {
    return false;
}
