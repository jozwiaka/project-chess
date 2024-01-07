#include "chessmodel.h"
#include <memory>

ChessModel::ChessModel(QObject *parent)
    : QObject{parent}
{}

void ChessModel::InitializeChessboard(){
    bool dark = false;
    for (char row = '1'; row <= '8'; ++row)
    {
        dark = !dark;
        QVector<std::shared_ptr<ChessSquare>> rowVector;
        for (char col = 'A'; col <= 'H'; ++col)
        {
            ChessSquare::Position position{col, row};
            std::shared_ptr<ChessSquare> square = std::make_shared<ChessSquare>(dark, position);
            dark = !dark;

            if(row=='1'||row=='2'||row=='7'||row=='8'){
                ChessPiece::PieceType pieceType;
                ChessPiece::Color color;
                switch (row)
                {
                case '1':
                case '2':
                    color = ChessPiece::Color::White;
                    break;
                case '7':
                case '8':
                    color = ChessPiece::Color::Black;
                    break;
                }

                switch (row)
                {
                case '1':
                case '8':
                    switch (col)
                    {
                    case 'A':
                    case 'H':
                        pieceType = ChessPiece::Rook;
                        break;
                    case 'B':
                    case 'G':
                        pieceType = ChessPiece::Knight;
                        break;
                    case 'C':
                    case 'F':
                        pieceType = ChessPiece::Bishop;
                        break;
                    case 'D':
                        pieceType = ChessPiece::Queen;
                        break;
                    case 'E':
                        pieceType = ChessPiece::King;
                        break;
                    }
                    break;
                case '2':
                case '7':
                    pieceType = ChessPiece::Pawn;
                    break;
                }

                auto piece = std::make_unique<ChessPiece>(pieceType, color, square.get());
                square->SetChessPiece(std::move(piece));
            }
            rowVector.push_back(square);
        }
        m_Chessboard.push_back(rowVector);
    }
}

QVector<QVector<std::shared_ptr<ChessSquare>>> ChessModel::GetChessboard() {
    return m_Chessboard;
}

ChessModel::~ChessModel(){
}
