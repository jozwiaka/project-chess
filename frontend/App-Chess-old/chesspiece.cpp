#include "chesspiece.h"

ChessPiece::ChessPiece(const QString &imagePath, PieceType type, QWidget *parent)
    : QLabel(parent), pieceType(type), selected(false)
{
    setFixedSize(80,80);
    setContentsMargins(10,10,10,5);
    setPixmap(QPixmap(imagePath).scaled(size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    setScaledContents(true);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setAlignment(Qt::AlignCenter);
}

ChessPiece::PieceType ChessPiece::getPieceType() const
{
    return pieceType;
}

bool ChessPiece::isWhite() const
{
    // Assuming pieces with odd values are white, and even values are black
    return pieceType % 2 != 0;
}

void ChessPiece::setSelected(bool isSelected)
{
    selected = isSelected;
    // You can modify the appearance of the piece based on whether it's selected or not
    // For example, change the border color or background
}

bool ChessPiece::isSelected() const
{
    return selected;
}
