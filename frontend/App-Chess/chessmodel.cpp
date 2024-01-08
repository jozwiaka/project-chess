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
            ChessSquare::Position position{row, col};
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

ChessSquare* ChessModel::FindSquare(const ChessSquare::Position& position) {
    return m_Chessboard[position.x-'1'][position.y-'A'].get();
}

void ChessModel::ClearStatuses() {
    for(auto& row : m_Chessboard)
    {
        for(auto& square : row)
        {
            if(square->GetStatus()==ChessSquare::Status::Active
                || square->GetStatus()==ChessSquare::Status::ValidCapture
                || square->GetStatus()==ChessSquare::Status::ValidMove
                || square->GetStatus()==ChessSquare::Status::LastMove)
            {
                square->SetStatus(ChessSquare::Status::Normal);
            }
        }
    }
    m_ActiveSquare = nullptr;
}

void ChessModel::UpdateModelOnSquareClick(const ChessSquare::Position& position)
{
    ChessSquare* foundSquare = FindSquare(position);

    if(m_ActiveSquare && (foundSquare->GetStatus() == ChessSquare::Status::ValidMove || foundSquare->GetStatus() == ChessSquare::Status::ValidCapture))
    {
        MakeMove(foundSquare);
    }

    ClearStatuses();

    if(foundSquare->IsChessPiece())
    {
        auto& piece = foundSquare->GetChessPieceRef();
        if(piece.GetColor()==m_CurrentTurn)
        {
            foundSquare->SetStatus(ChessSquare::Status::Active);
            m_ActiveSquare = foundSquare;

            switch (piece.GetPieceType())
            {
            case ChessPiece::Rook:
                SetRookValidMoves();
                break;
            case ChessPiece::Knight:
                SetKnightValidMoves();
                break;
            case ChessPiece::Bishop:
                SetBishopValidMoves();
                break;
            case ChessPiece::Queen:
                SetQueenValidMoves();
                break;
            case ChessPiece::King:
                SetKingValidMoves();
                break;
            case ChessPiece::Pawn:
                SetPawnValidMoves();
                break;
            }
        }
    }

    emit UpdateGraphics();
}

void ChessModel::SetRookValidMoves(){}
void ChessModel::SetKnightValidMoves(){}
void ChessModel::SetBishopValidMoves(){}
void ChessModel::SetQueenValidMoves(){}
void ChessModel::SetKingValidMoves(){}
void ChessModel::SetPawnValidMoves(){}


ChessModel::~ChessModel(){
}

void ChessModel::MakeMove(ChessSquare* toSquare) {
    if(m_ActiveSquare) {
        m_CurrentTurn = m_CurrentTurn == Color::White ? Color::Black : Color::White;
        toSquare->SetChessPiece(std::move(m_ActiveSquare->GetChessPiece()));
    }
}
