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
    using ChessboardType = std::vector<std::vector<std::shared_ptr<ChessSquare>>>;

public:
    struct Position {
        char x, y;
    };

public:
    ChessSquare() = default;

    ChessSquare(bool dark, const Position& position, QWidget *parent = nullptr);

    void SetChessPiece(std::unique_ptr<ChessPiece> piece);
    ChessPiece *GetChessPiece() const;

    void HighlightSquare();
    void ResetSquareColor();

signals:
    void Clicked(const Position& position);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    Position m_Position;
    QColor m_NormalColor;
    QColor m_HighlightedColor;
    bool m_IsHighlighted;
    std::unique_ptr<ChessPiece> m_ChessPiece;
};

#endif // CHESSSQUARE_H
