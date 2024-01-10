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

    PieceType GetPieceType() const;
    PieceColor GetColor() const;

    void SetSelected(bool selected);
    bool IsSelected() const;

    bool IsMoved();
    void SetMoved();

    bool IsEnPassant();
    void SetEnPassant(bool enPassant);

private:
    PieceType m_PieceType;
    PieceColor m_Color;
    bool m_Selected;
    bool m_Moved;
    bool m_EnPassant;

signals:
};

#endif // CHESSGAME_H
