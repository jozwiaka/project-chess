#ifndef CHESSPIECE_H
#define CHESSPIECE_H

#include <QObject>
#include <QLabel>
#include <QPainter>

class ChessPiece : public QLabel
{
    Q_OBJECT

public:
    enum PieceType {
        Pawn,
        Rook,
        Knight,
        Bishop,
        Queen,
        King
    };

    enum Color {
        White,
        Black
    };



public:
    ChessPiece() = default;

    ChessPiece(PieceType type, Color color, QWidget *parent = nullptr);

    PieceType GetPieceType() const;
    Color GetColor() const;
    void SetSelected(bool selected);
    bool IsSelected() const;

private:
    PieceType m_PieceType;
    Color m_Color;
    bool m_Selected;

signals:
};

#endif // CHESSGAME_H
