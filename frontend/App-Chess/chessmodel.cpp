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

void ChessModel::UpdateModelOnSquareClick(const ChessSquare::Position& position) {
    for(auto& row : m_Chessboard)
    {
        for(auto& square : row)
        {
            if(square->GetPosition()==position)
            {
                qDebug()<<position.x<<position.y<<"\n";
                break;
            }
        }
    }

    for(int r=0;r<8;++r)
    {
        for(int c=0;c<8;++c)
        {
            m_Chessboard[r][c]->SetStatus(ChessSquare::Status::Active);
        }
    }

    emit UpdateGraphics();
}

ChessModel::~ChessModel(){
}
