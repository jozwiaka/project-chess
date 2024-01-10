#include "chesssquare.h"
#include <QMouseEvent>
#include <QColor>

ChessSquare::ChessSquare(bool dark, const SquarePosition &position, QWidget *parent)
    : QLabel(parent), Position(position), Status(SquareStatus::Normal), m_Size(80), m_Piece(nullptr), Blocked(false)
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
    emit Clicked(Position);
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

void ChessSquare::SetPiece(ChessPiece *piece)
{
    m_Piece = piece;
    if (m_Piece)
    {
        m_Piece->setParent(this);
    }
}

ChessPiece *ChessSquare::GetPiece()
{
    return m_Piece;
}

void ChessSquare::RemoveChessPiece()
{
    if (m_Piece)
    {
        delete m_Piece;
        m_Piece = nullptr;
    }
}