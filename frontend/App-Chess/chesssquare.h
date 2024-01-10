#ifndef CHESSSQUARE_H
#define CHESSSQUARE_H

#include <QLabel>
#include "chesspiece.h"
#include <QColor>

class ChessSquare : public QLabel
{
    Q_OBJECT

public:
    struct SquarePosition
    {
        int x, y;

        bool operator==(const SquarePosition &other)
        {
            return this->x == other.x && this->y == other.y;
        }
    };

public:
    enum class SquareStatus
    {
        Normal,
        Active,
        PreviousMove,
        ValidMove,
        ValidCapture,
        Check,
    };

public:
    SquarePosition Position;
    SquareStatus Status;
    bool Blocked;

public:
    ChessSquare() = default;

    ChessSquare(bool dark, const SquarePosition &position, QWidget *parent = nullptr);

    void SetPiece(ChessPiece *piece);
    ChessPiece *GetPiece();

    void RemoveChessPiece();

    void SetHighlightBackgroundColor();
    void SetNormalBackgroundColor();

    void SetCheckBackgroundColor();

    void SetValidBackgroundColor();

protected:
    void mousePressEvent(QMouseEvent *event) override;

private:
    ChessPiece *m_Piece;
    size_t m_Size;
    QColor m_NormalColor;
    QColor m_HighlightedColor;

signals:
    void Clicked(const SquarePosition &position);
};

#endif // CHESSSQUARE_H
