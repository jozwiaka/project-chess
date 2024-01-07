#include "chessmodel.h"
#include <memory>
#include <QDebug>

ChessModel::ChessModel(QObject *parent)
    : QObject{parent}, m_CurrentTurn{Color::White}
{}

void ChessModel::InitializeChessboard(){
    bool dark = false;
    m_Chessboard.reserve(8);
    for (char row = '1'; row <= '8'; ++row)
    {
        dark = !dark;
        QVector<std::shared_ptr<ChessSquare>> rowVector;
        rowVector.reserve(8);
        for (char col = 'A'; col <= 'H'; ++col)
        {
            ChessSquare::Position position{col, row};
            auto square =  std::make_shared<ChessSquare>(dark, position);
            dark = !dark;

            if(row=='1'||row=='2'||row=='7'||row=='8'){
                ChessPiece::Color color = (row=='1'||row=='2') ? ChessPiece::Color::White  : ChessPiece::Color::Black;
                ChessPiece::PieceType pieceType;

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

void ChessModel::UpdateModelOnSquareClick(const ChessSquare::Position& position)
{
    for(auto& row : m_Chessboard)
    {
        for(auto& square : row)
        {
            if(square->GetStatus()==ChessSquare::Status::Active
                || square->GetStatus()==ChessSquare::Status::AvailableCapture
                || square->GetStatus()==ChessSquare::Status::AvailableMove
                || square->GetStatus()==ChessSquare::Status::LastMove)
            {
                square->SetStatus(ChessSquare::Status::Normal);
            }
        }
    }

    for(auto& row : m_Chessboard)
    {
        for(auto& square : row)
        {
            if(square->GetPosition() == position)
            {
                if(square->IsChessPiece())
                {
                    auto& piece = square->GetChessPieceRef();
                    if(piece.GetColor()==m_CurrentTurn)
                    {
                        square->SetStatus(ChessSquare::Status::Active);

                        switch (piece.GetPieceType())
                        {
                        case ChessPiece::Rook:
                            break;
                        case ChessPiece::Knight:
                            break;
                        case ChessPiece::Bishop:
                            break;
                        case ChessPiece::Queen:
                            break;
                        case ChessPiece::King:
                            break;
                        case ChessPiece::Pawn:
                            break;
                        }
                    }
                }
                break;
            }
        }
    }

    for (char row = 0; row < 8; ++row)
    {
        for (char col = 0; col < 8; ++col)
        {
        }
    }

    emit UpdateGraphics();
}

ChessModel::~ChessModel(){
}

void ChessModel::MakeMove(std::shared_ptr<ChessSquare> fromSquare, std::shared_ptr<ChessSquare> toSquare) {
    m_CurrentTurn = Color::White ? Color::Black : Color::White;
    toSquare->SetChessPiece(std::move(fromSquare->GetChessPiece()));
}
