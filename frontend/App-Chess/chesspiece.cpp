#include "chesspiece.h"

ChessPiece::ChessPiece(PieceType type, Color color, QWidget *parent)
    : QLabel(parent), m_PieceType(type), m_Color(color), m_Selected(false), m_Moved(false), m_EnPassant(false)
{
    QString pieceImagePath;
    switch (m_PieceType)
    {
    case ChessPiece::PieceType::Rook:
        pieceImagePath = (m_Color == Color::White) ? "../../resources/w_rook_1x_ns.png" : "../../resources/b_rook_1x_ns.png";
        break;
    case ChessPiece::PieceType::Knight:
        pieceImagePath = (m_Color == Color::White) ? "../../resources/w_knight_1x_ns.png" : "../../resources/b_knight_1x_ns.png";
        break;
    case ChessPiece::PieceType::Bishop:
        pieceImagePath = (m_Color == Color::White) ? "../../resources/w_bishop_1x_ns.png" : "../../resources/b_bishop_1x_ns.png";
        break;
    case ChessPiece::PieceType::Queen:
        pieceImagePath = (m_Color == Color::White) ? "../../resources/w_queen_1x_ns.png" : "../../resources/b_queen_1x_ns.png";
        break;
    case ChessPiece::PieceType::King:
        pieceImagePath = (m_Color == Color::White) ? "../../resources/w_king_1x_ns.png" : "../../resources/b_king_1x_ns.png";
        break;
    case ChessPiece::PieceType::Pawn:
        pieceImagePath = (m_Color == Color::White) ? "../../resources/w_pawn_1x_ns.png" : "../../resources/b_pawn_1x_ns.png";
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

bool ChessPiece::IsEnPassant()
{
    return m_EnPassant;
}

void ChessPiece::SetEnPassant(bool enPassant)
{
    m_EnPassant = enPassant;
}
