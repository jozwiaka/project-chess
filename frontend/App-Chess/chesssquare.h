#ifndef CHESSSQUARE_H
#define CHESSSQUARE_H

#include <QLabel>
#include "chesspiece.h"
#include <QColor>

class ChessSquare : public QLabel
{
    Q_OBJECT

public:
    struct Position {
        int x, y;

        bool operator==(const Position& other)
        {
            return this->x==other.x&&this->y==other.y;
        }
    };

    enum class Status {
        Normal,
        Active,
        PreviousMove,
        ValidMove,
        ValidCapture,
    };

public:
    ChessSquare() = default;

    ChessSquare(bool dark, const Position& position, QWidget *parent = nullptr);

    void SetChessPiece(ChessPiece* piece);
    ChessPiece* GetChessPiece();

    void SetHighlightBckgroundColor();
    void SetNormalBackgroundColor();

    void SetBorder();
    void UnsetBorder();

    void SetStatus(Status status);
    Status GetStatus() const;

    Position GetPosition() const;
signals:
    void Clicked(const Position& position);

protected:
    void mousePressEvent(QMouseEvent *event) override;

private:
    size_t m_Size;
    Position m_Position;
    Status m_Status;
    QColor m_NormalColor;
    QColor m_HighlightedColor;
    ChessPiece* m_ChessPiece;
    bool m_GloballyValid;
};

#endif // CHESSSQUARE_H
