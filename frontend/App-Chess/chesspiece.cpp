#include "chesspiece.h"

ChessPiece::ChessPiece(const QString &imagePath, PieceType type, Color color, QWidget *parent)
    : QLabel(parent), m_PieceType(type), m_Color(color), m_Selected(false)
{
    setFixedSize(80,80);
    setContentsMargins(10,10,10,5);
    setPixmap(QPixmap(imagePath).scaled(size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    setScaledContents(true);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setAlignment(Qt::AlignCenter);
}

ChessPiece::PieceType ChessPiece::GetPieceType() const
{
    return m_PieceType;
}

ChessPiece::Color ChessPiece::GetColor() const
{
    return m_Color;
}

void ChessPiece::SetSelected(bool isSelected)
{
    m_Selected = isSelected;
}

bool ChessPiece::IsSelected() const
{
    return m_Selected;
}
