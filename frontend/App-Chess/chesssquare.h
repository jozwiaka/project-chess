#ifndef CHESSSQUARE_H
#define CHESSSQUARE_H

#include <QLabel>
#include "chesspiece.h"
#include <QColor>

class ChessSquare : public QLabel
{
    Q_OBJECT

public:
    struct Position
    {
        int x, y;

        bool operator==(const Position &other)
        {
            return this->x == other.x && this->y == other.y;
        }
    };

    enum class Status
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

    ChessSquare(bool dark, const Position &position, QWidget *parent = nullptr);

    void SetChessPiece(ChessPiece *piece);
    ChessPiece *GetChessPiece();

    void RemoveChessPiece();

    void SetHighlightBackgroundColor();
    void SetNormalBackgroundColor();

    void SetCheckBackgroundColor();

    void SetValidBackgroundColor();

    void SetStatus(Status status);
    Status GetStatus() const;

    bool IsBlockedForKing();
    void SetBlockedForKing(bool blockedForKing);

    Position GetPosition() const;
signals:
    void Clicked(const Position &position);

protected:
    void mousePressEvent(QMouseEvent *event) override;

private:
    size_t m_Size;
    Position m_Position;
    Status m_Status;
    QColor m_NormalColor;
    QColor m_HighlightedColor;
    ChessPiece *m_ChessPiece;
    bool m_BlockedForKing;
};

#endif // CHESSSQUARE_H
