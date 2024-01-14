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

Chessboard::~Chessboard()
{
}

void Chessboard::InitializeChessboard()
{
    bool dark = false;
    m_Data.reserve(8);
    for (char x = '1'; x <= '8'; ++x)
    {
        dark = !dark;
        QVector<ChessSquare *> xVector;
        xVector.reserve(8);
        for (char y = 'A'; y <= 'H'; ++y)
        {
            ChessSquare::SquarePosition position{x - '1', y - 'A'};
            auto square = new ChessSquare(dark, position);
            dark = !dark;

            if (x == '1' || x == '2' || x == '7' || x == '8')
            {
                ChessPiece::PieceColor yor = (x == '1' || x == '2') ? ChessPiece::PieceColor::White : ChessPiece::PieceColor::Black;
                ChessPiece::PieceType type;

                switch (x)
                {
                case '1':
                case '8':
                    switch (y)
                    {
                    case 'A':
                    case 'H':
                        type = ChessPiece::PieceType::Rook;
                        break;
                    case 'B':
                    case 'G':
                        type = ChessPiece::PieceType::Knight;
                        break;
                    case 'C':
                    case 'F':
                        type = ChessPiece::PieceType::Bishop;
                        break;
                    case 'D':
                        type = ChessPiece::PieceType::Queen;
                        break;
                    case 'E':
                        type = ChessPiece::PieceType::King;
                        break;
                    }
                    break;
                case '2':
                case '7':
                    type = ChessPiece::PieceType::Pawn;
                    break;
                default:
                    continue;
                }

                auto piece = new ChessPiece(type, yor, square);
                square->SetPiece(piece);
            }
            xVector.push_back(square);
        }
        m_Data.push_back(xVector);
    }
}

ChessSquare *Chessboard::GetSquareByPosition(const ChessSquare::SquarePosition &position)
{
    if (position.x < 0 || position.x >= 8 || position.y < 0 || position.y >= 8)
    {
        return nullptr;
    }

    return GetInstance().m_Data[position.x][position.y];
}

QVector<ChessSquare *> Chessboard::operator[](int index)
{
    return m_Data[index];
}

const QVector<ChessSquare *> Chessboard::operator[](int index) const
{
    return m_Data[index];
}

QVector<QVector<ChessSquare *>>::Iterator Chessboard::begin()
{
    return m_Data.begin();
}

QVector<QVector<ChessSquare *>>::Iterator Chessboard::end()
{
    return m_Data.end();
}
