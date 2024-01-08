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

        bool operator==(const Position& other)
        {
            return this->x==other.x&&this->y==other.y;
        }
    };

    enum class Status {
        Normal,
        Active,
        LastMove,
        ValidMove,
        ValidCapture,
    };

public:
    ChessSquare() = default;

    ChessSquare(bool dark, const Position& position, QWidget *parent = nullptr);

    void SetChessPiece(std::unique_ptr<ChessPiece> piece);
    std::unique_ptr<ChessPiece> GetChessPiece();
    const ChessPiece& GetChessPieceRef() const;
    bool IsChessPiece() const;

    void HighlightSquare();
    void ResetSquareColor();

    void SetStatus(Status status);
    Status GetStatus() const;

    Position GetPosition() const;
signals:
    void Clicked(const Position& position);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    size_t m_Size;
    Position m_Position;
    Status m_Status;
    QColor m_NormalColor;
    QColor m_HighlightedColor;
    bool m_IsHighlighted;
    std::unique_ptr<ChessPiece> m_ChessPiece;
};

#endif // CHESSSQUARE_H
