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
    const PieceType Type;
    const PieceColor Color;
    bool Moved;
    bool EnPassant;

public:
    ChessPiece(const PieceType &type, const PieceColor &color, QWidget *parent = nullptr);
    static QString GetPieceImageByColorAndType(const PieceType &type, const PieceColor &color);

signals:
};
#endif // CHESSGAME_H
