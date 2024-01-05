#ifndef CHESSGAME_H
#define CHESSGAME_H

#include <QObject>
#include <array>
#include "chesssquare.h"
#include "chesspiece.h"

class ChessGame : public QObject
{
    Q_OBJECT
public:
    explicit ChessGame(QObject *parent = nullptr);

    void startGame();

    void makeMove(const QString& fromSquare, const QString& toSquare);
    bool isCheckmate() const;

private:
    bool whiteTurn;
    //std::array<ChessSquare, 64> squares;

signals:
};

#endif // CHESSGAME_H
