#ifndef CHESSSQUARE_H
#define CHESSSQUARE_H

#include <QLabel>
#include <QColor>
#include "chesspiece.h"

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
        PreviousMove,
        Check,
    };

    enum class SquareStatusTemporary
    {
        Normal,
        Active,
        ValidMove,
        ValidCapture,

    };

public:
    SquarePosition Position;
    SquareStatus Status;
    SquareStatusTemporary StatusTemporary;
    bool BlockedForKing;
    QVector<ChessPiece *> BlockedPieces;

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

    bool IsPieceBlocked(ChessPiece *piece);

signals:
    void Clicked(const SquarePosition &position);

protected:
    void mousePressEvent(QMouseEvent *event) override;

private:
    ChessPiece *m_Piece;
    const size_t m_Size;
    const QColor m_NormalColor;
    const QColor m_HighlightedColor;
};

#endif // CHESSSQUARE_H
