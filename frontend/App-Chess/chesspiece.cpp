#include "chesspiece.h"

ChessPiece::ChessPiece(PieceType type, Color color, QWidget *parent)
    : QLabel(parent), m_PieceType(type), m_Color(color), m_Selected(false), m_Moved(false)
{
    QString pieceImagePath;
    switch (m_PieceType)
    {
    case ChessPiece::Rook:
        pieceImagePath = (m_Color == Color::White) ? "../App-Chess/resources/w_rook_1x_ns.png" : "../App-Chess/resources/b_rook_1x_ns.png";
        break;
    case ChessPiece::Knight:
        pieceImagePath = (m_Color == Color::White) ? "../App-Chess/resources/w_knight_1x_ns.png" : "../App-Chess/resources/b_knight_1x_ns.png";
        break;
    case ChessPiece::Bishop:
        pieceImagePath = (m_Color == Color::White) ? "../App-Chess/resources/w_bishop_1x_ns.png" : "../App-Chess/resources/b_bishop_1x_ns.png";
        break;
    case ChessPiece::Queen:
        pieceImagePath = (m_Color == Color::White) ? "../App-Chess/resources/w_queen_1x_ns.png" : "../App-Chess/resources/b_queen_1x_ns.png";
        break;
    case ChessPiece::King:
        pieceImagePath = (m_Color == Color::White) ? "../App-Chess/resources/w_king_1x_ns.png" : "../App-Chess/resources/b_king_1x_ns.png";
        break;
    case ChessPiece::Pawn:
        pieceImagePath = (m_Color == Color::White) ? "../App-Chess/resources/w_pawn_1x_ns.png" : "../App-Chess/resources/b_pawn_1x_ns.png";
        break;
    }
    setFixedSize(80,80);
    setContentsMargins(10,10,10,5);
    setPixmap(QPixmap(pieceImagePath).scaled(size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
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

void ChessPiece::SetMoved() {
    m_Moved = true;
}

bool ChessPiece::IsMoved() {
    return m_Moved;
}
