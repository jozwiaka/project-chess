#include "chesspiece.h"

ChessPiece::ChessPiece(PieceType type, PieceColor color, QWidget *parent)
    : QLabel(parent), Type(type), m_Color(color), m_Moved(false), m_EnPassant(false)
{
    QString pieceImagePath;
    switch (Type)
    {
    case ChessPiece::PieceType::Rook:
        pieceImagePath = (m_Color == PieceColor::White) ? "../../resources/w_rook_1x_ns.png" : "../../resources/b_rook_1x_ns.png";
        break;
    case ChessPiece::PieceType::Knight:
        pieceImagePath = (m_Color == PieceColor::White) ? "../../resources/w_knight_1x_ns.png" : "../../resources/b_knight_1x_ns.png";
        break;
    case ChessPiece::PieceType::Bishop:
        pieceImagePath = (m_Color == PieceColor::White) ? "../../resources/w_bishop_1x_ns.png" : "../../resources/b_bishop_1x_ns.png";
        break;
    case ChessPiece::PieceType::Queen:
        pieceImagePath = (m_Color == PieceColor::White) ? "../../resources/w_queen_1x_ns.png" : "../../resources/b_queen_1x_ns.png";
        break;
    case ChessPiece::PieceType::King:
        pieceImagePath = (m_Color == PieceColor::White) ? "../../resources/w_king_1x_ns.png" : "../../resources/b_king_1x_ns.png";
        break;
    case ChessPiece::PieceType::Pawn:
        pieceImagePath = (m_Color == PieceColor::White) ? "../../resources/w_pawn_1x_ns.png" : "../../resources/b_pawn_1x_ns.png";
        break;
    }
    setFixedSize(80, 80);
    setContentsMargins(10, 10, 10, 5);
    setPixmap(QPixmap(pieceImagePath).scaled(size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    setScaledContents(true);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setAlignment(Qt::AlignCenter);
}
