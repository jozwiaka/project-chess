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

public:
    ChessPiece(const QString &imagePath, PieceType type, QWidget *parent = nullptr);

    PieceType getPieceType() const;
    bool isWhite() const;
    void setSelected(bool selected);
    bool isSelected() const;

private:
    PieceType pieceType;
    bool selected;

signals:
};

#endif // CHESSGAME_H
