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
    ChessSquare() = default;

    ChessSquare(bool dark, const SquarePosition &position, QWidget *parent = nullptr);

    void SetPiece(ChessPiece *piece);
    ChessPiece *GetPiece();

    void RemoveChessPiece();

    void SetHighlightBackgroundColor();
    void SetNormalBackgroundColor();

    void SetCheckBackgroundColor();

    void SetValidBackgroundColor();

signals:
    void Clicked(const SquarePosition &position);

protected:
    void mousePressEvent(QMouseEvent *event) override;

public:
    SquarePosition Position;
    SquareStatus Status;
    bool Blocked;

private:
    ChessPiece *m_Piece;
    size_t m_Size;
    QColor m_NormalColor;
    QColor m_HighlightedColor;
};

#endif // CHESSSQUARE_H
