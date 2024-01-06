#ifndef CHESSSQUARE_H
#define CHESSSQUARE_H

#include <QLabel>
#include "chesspiece.h"
#include <QColor>
#include <memory>

class ChessSquare : public QLabel
{
    Q_OBJECT

public:
    struct Position {
        char x, y;
    };

public:
    ChessSquare(bool dark, const Position& position, QWidget *parent = nullptr);

    void setChessPiece(std::unique_ptr<ChessPiece> piece);
    ChessPiece *getChessPiece() const;

    void highlightSquare();
    void resetSquareColor();

signals:
    void clicked(const Position& position);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    Position position;
    QColor normalColor;
    QColor highlightedColor;
    bool isHighlighted;
    std::unique_ptr<ChessPiece> chessPiece;
};

#endif // CHESSSQUARE_H
