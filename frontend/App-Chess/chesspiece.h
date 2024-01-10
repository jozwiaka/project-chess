#ifndef CHESSPIECE_H
#define CHESSPIECE_H

#include <QObject>
#include <QLabel>
#include <QPainter>

class ChessPiece : public QLabel
{
    Q_OBJECT

public:
    enum class PieceType
    {
        Pawn,
        Rook,
        Knight,
        Bishop,
        Queen,
        King
    };

    enum class PieceColor
    {
        White = 0,
        Black = 1
    };

public:
    ChessPiece() = default;

    ChessPiece(PieceType type, PieceColor color, QWidget *parent = nullptr);

signals:

public:
    PieceType Type;
    PieceColor Color;
    bool Moved;
    bool EnPassant;
};

#endif // CHESSGAME_H
