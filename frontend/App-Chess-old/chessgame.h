#ifndef CHESSGAME_H
#define CHESSGAME_H

#include <QObject>
#include "chesssquare.h"
#include "chesspiece.h"

class ChessGame : public QObject
{
    Q_OBJECT

public:
    enum class Color {
        WHITE, BLACK
    };

public:
    explicit ChessGame(QObject *parent = nullptr);

    void startGame();

    void makeMove(const QString& fromSquare, const QString& toSquare);
    bool isCheckmate() const;

signals:
    void moveMade(const QString& fromSquare, const QString& toSquare);

private:
    Color currentTurn;

signals:
};

#endif // CHESSGAME_H
