#include "chesssquare.h"
#include <QMouseEvent>
#include <QColor>

ChessSquare::ChessSquare(bool dark, const Position &position, QWidget *parent)
    : QLabel(parent), m_Position(position), m_Status(Status::Normal), m_Size(80), m_ChessPiece(nullptr), m_BlockedForKing(false)
{
    setFixedSize(m_Size, m_Size);
    setMargin(0);
    setScaledContents(true); // Ensure the square image fits the label size
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setAlignment(Qt::AlignCenter);
    m_NormalColor = dark ? QColor(101, 67, 33) : QColor(193, 154, 107);
    m_HighlightedColor = dark ? QColor(255, 220, 185) : QColor(255, 236, 210);
    SetNormalBackgroundColor();
}

void ChessSquare::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    emit Clicked(m_Position);
}

void ChessSquare::SetHighlightBackgroundColor()
{
    setStyleSheet(QString("background-color: %1;border: none;").arg(m_HighlightedColor.name()));
}

void ChessSquare::SetNormalBackgroundColor()
{
    setStyleSheet(QString("background-color: %1;border: none;").arg(m_NormalColor.name()));
}

void ChessSquare::SetValidBackgroundColor()
{
    setStyleSheet(QString("background-color: lightgreen; border: 1px solid black;"));
}

void ChessSquare::SetCheckBackgroundColor()
{
    setStyleSheet(QString("background-color: red; border: 1px solid black;"));
}

void ChessSquare::SetChessPiece(ChessPiece *piece)
{
    m_ChessPiece = piece;
    if (m_ChessPiece)
    {
        m_ChessPiece->setParent(this);
        m_ChessPiece->show();
    }
}

ChessPiece *ChessSquare::GetChessPiece()
{
    return m_ChessPiece;
}

void ChessSquare::RemoveChessPiece()
{
    if(m_ChessPiece)
    {
        delete m_ChessPiece;
        m_ChessPiece = nullptr;
    }
}

void ChessSquare::SetStatus(Status status)
{
    m_Status = status;
}

ChessSquare::Status ChessSquare::GetStatus() const
{
    return m_Status;
}

ChessSquare::Position ChessSquare::GetPosition() const
{
    return m_Position;
}

bool ChessSquare::IsBlockedForKing()
{
    return m_BlockedForKing;
}

void ChessSquare::SetBlockedForKing(bool blockedForKing)
{
    m_BlockedForKing = blockedForKing;
}
