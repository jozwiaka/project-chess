#ifndef CHESSPIECE_H
#define CHESSPIECE_H

#include <QObject>
#include <QLabel>
#include <QPainter>
#include "chessgame.h"

class ChessPiece : public QLabel
{
    Q_OBJECT

public:
    enum PieceType {
        Empty,
        Pawn,
        Rook,
        Knight,
        Bishop,
        Queen,
        King
    };



public:
    ChessPiece(const QString &imagePath, PieceType type, ChessGame::Color color, QWidget *parent = nullptr);

    PieceType getPieceType() const;
    ChessPiece getColor() const;
    void setSelected(bool selected);
    bool isSelected() const;

private:
    PieceType pieceType;
    bool selected;

signals:
};

#endif // CHESSGAME_H
