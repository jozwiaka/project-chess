#include "chesspiece.h"

ChessPiece::ChessPiece(const PieceType &type, const PieceColor &color, QWidget *parent)
    : QLabel(parent), Type(type), Color(color), Moved(false), EnPassant(false)
{
    QString pieceImagePath = GetPieceImageByColorAndType(Type, Color);
    setFixedSize(80, 80);
    setContentsMargins(10, 10, 10, 5);
    setPixmap(QPixmap(pieceImagePath).scaled(size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    setScaledContents(true);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setAlignment(Qt::AlignCenter);
}

QString ChessPiece::GetPieceImageByColorAndType(const PieceType &type, const PieceColor &color) {
    QString pieceImagePath;
    switch (type)
    {
    case ChessPiece::PieceType::Rook:
        pieceImagePath = (color == PieceColor::White) ? "../../resources/w_rook_1x_ns.png" : "../../resources/b_rook_1x_ns.png";
        break;
    case ChessPiece::PieceType::Knight:
        pieceImagePath = (color == PieceColor::White) ? "../../resources/w_knight_1x_ns.png" : "../../resources/b_knight_1x_ns.png";
        break;
    case ChessPiece::PieceType::Bishop:
        pieceImagePath = (color == PieceColor::White) ? "../../resources/w_bishop_1x_ns.png" : "../../resources/b_bishop_1x_ns.png";
        break;
    case ChessPiece::PieceType::Queen:
        pieceImagePath = (color == PieceColor::White) ? "../../resources/w_queen_1x_ns.png" : "../../resources/b_queen_1x_ns.png";
        break;
    case ChessPiece::PieceType::King:
        pieceImagePath = (color == PieceColor::White) ? "../../resources/w_king_1x_ns.png" : "../../resources/b_king_1x_ns.png";
        break;
    case ChessPiece::PieceType::Pawn:
        pieceImagePath = (color == PieceColor::White) ? "../../resources/w_pawn_1x_ns.png" : "../../resources/b_pawn_1x_ns.png";
        break;
    }

    return pieceImagePath;
}
