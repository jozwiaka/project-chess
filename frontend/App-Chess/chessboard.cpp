#include "chessboard.h"

Chessboard &Chessboard::GetInstance()
{
    static Chessboard instance;
    return instance;
}

Chessboard::Chessboard(QObject *parent)
    : QObject{parent}
{
    InitializeChessboard();
}

void Chessboard::InitializeChessboard()
{
    bool dark = false;
    Board.reserve(8);
    for (char row = '1'; row <= '8'; ++row)
    {
        dark = !dark;
        QVector<ChessSquare *> rowVector;
        rowVector.reserve(8);
        for (char col = 'A'; col <= 'H'; ++col)
        {
            ChessSquare::SquarePosition position{row - '1', col - 'A'};
            auto square = new ChessSquare(dark, position);
            dark = !dark;

            if (row == '1' || row == '2' || row == '7' || row == '8')
            {
                ChessPiece::PieceColor color = (row == '1' || row == '2') ? ChessPiece::PieceColor::White : ChessPiece::PieceColor::Black;
                ChessPiece::PieceType pieceType;

                switch (row)
                {
                case '1':
                case '8':
                    switch (col)
                    {
                    case 'A':
                    case 'H':
                        pieceType = ChessPiece::PieceType::Rook;
                        break;
                    case 'B':
                    case 'G':
                        pieceType = ChessPiece::PieceType::Knight;
                        break;
                    case 'C':
                    case 'F':
                        pieceType = ChessPiece::PieceType::Bishop;
                        break;
                    case 'D':
                        pieceType = ChessPiece::PieceType::Queen;
                        break;
                    case 'E':
                        pieceType = ChessPiece::PieceType::King;
                        break;
                    }
                    break;
                case '2':
                case '7':
                    pieceType = ChessPiece::PieceType::Pawn;
                    break;
                default:
                    continue;
                }

                auto piece = new ChessPiece(pieceType, color, square);
                square->SetPiece(piece);
            }
            rowVector.push_back(square);
        }
        Board.push_back(rowVector);
    }
}
