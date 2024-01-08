#include "chessmodel.h"
#include <QDebug>

ChessModel::ChessModel(QObject *parent)
    : QObject{parent}, m_CurrentTurn{Color::White}
{
}

void ChessModel::InitializeChessboard()
{
    bool dark = false;
    m_Chessboard.reserve(8);
    for (char row = '1'; row <= '8'; ++row)
    {
        dark = !dark;
        QVector<ChessSquare *> rowVector;
        rowVector.reserve(8);
        for (char col = 'A'; col <= 'H'; ++col)
        {
            ChessSquare::Position position{row - '1', col - 'A'};
            auto square = new ChessSquare(dark, position);
            dark = !dark;

            if (row == '1' || row == '2' || row == '7' || row == '8')
            {
                ChessPiece::Color color = (row == '1' || row == '2') ? ChessPiece::Color::White : ChessPiece::Color::Black;
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

                auto piece = new ChessPiece(pieceType, color, square);
                square->SetChessPiece(piece);
            }
            rowVector.push_back(square);
        }
        m_Chessboard.push_back(rowVector);
    }
}

QVector<QVector<ChessSquare *>> ChessModel::GetChessboard()
{
    return m_Chessboard;
}

void ChessModel::ClearActiveAndValidMoveStatuses()
{
    for (auto &row : m_Chessboard)
    {
        for (auto &square : row)
        {
            if (square->GetStatus() == ChessSquare::Status::Active || square->GetStatus() == ChessSquare::Status::ValidCapture || square->GetStatus() == ChessSquare::Status::ValidMove)
            {
                square->SetStatus(ChessSquare::Status::Normal);
            }
        }
    }
}

void ChessModel::ClearPreviousMoveStatuses()
{
    for (auto &row : m_Chessboard)
    {
        for (auto &square : row)
        {
            if (square->GetStatus() == ChessSquare::Status::PreviousMove)
            {
                square->SetStatus(ChessSquare::Status::Normal);
            }
        }
    }
}

void ChessModel::UpdateModelOnSquareClick(const ChessSquare::Position &position)
{
    ChessSquare *foundSquare = GetSquareByPosition(position);

    if (!foundSquare)
    {
        return;
    }

    bool isGoingToMakeMove = false;

    if (m_ActiveSquare && (foundSquare->GetStatus() == ChessSquare::Status::ValidMove || foundSquare->GetStatus() == ChessSquare::Status::ValidCapture))
    {
        isGoingToMakeMove = true;
    }

    ClearActiveAndValidMoveStatuses();

    m_Chessboard[3][3]->SetStatus(ChessSquare::Status::ValidCapture);

    if (isGoingToMakeMove)
    {
        MakeMove(foundSquare);
    }
    m_ActiveSquare = nullptr;

    if (foundSquare->GetChessPiece())
    {
        ChessPiece *piece = foundSquare->GetChessPiece();
        if (piece->GetColor() == m_CurrentTurn)
        {
            foundSquare->SetStatus(ChessSquare::Status::Active);
            m_ActiveSquare = foundSquare;

            switch (piece->GetPieceType())
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

void ChessModel::SetRookValidMoves() {}
void ChessModel::SetKnightValidMoves() {}
void ChessModel::SetBishopValidMoves() {}
void ChessModel::SetQueenValidMoves() {}
void ChessModel::SetKingValidMoves() {}
void ChessModel::SetPawnValidMoves()
{
    ChessPiece *piece = m_ActiveSquare->GetChessPiece();
    const int X = m_ActiveSquare->GetPosition().x;
    const int Y = m_ActiveSquare->GetPosition().y;

    int i = 1;

    if (m_CurrentTurn == Color::Black)
    {
        i = -1;
    }

    for (int x = X + i; i * x <= i * (X + i * (piece->IsMoved() ? 1 : 2)); x += i)
    {
        ChessSquare* square = GetSquareByPosition({x,Y});
        if (CheckIfFreeSquare(square))
        {
            m_Chessboard[x][Y]->SetStatus(ChessSquare::Status::ValidMove);
        }

    }
}

bool ChessModel::CheckIfEnemy(ChessSquare* square) {
    if (!square)
        return false;
    return square->GetChessPiece()->GetColor() != m_CurrentTurn;
}

bool ChessModel::CheckIfFreeSquare(ChessSquare* square)
{
    if (!square)
        return false;
    return square->GetChessPiece() == nullptr;
}

ChessSquare *ChessModel::GetSquareByPosition(const ChessSquare::Position &position)
{
    if (position.x >= 0 && position.x <= 7 && position.y >= 0 && position.y <= 7)
        return m_Chessboard[position.x][position.y];

    return nullptr;
}

ChessModel::~ChessModel()
{
    for (auto &row : m_Chessboard)
    {
        for (auto *square : row)
        {
            if (square)
            {
                if (square->GetChessPiece())
                {
                    delete square->GetChessPiece();
                    square->SetChessPiece(nullptr);
                }

                delete square;
            }
        }
    }
}

void ChessModel::MakeMove(ChessSquare *toSquare)
{
    if (m_ActiveSquare)
    {
        m_CurrentTurn = m_CurrentTurn == Color::White ? Color::Black : Color::White;
        toSquare->SetChessPiece(m_ActiveSquare->GetChessPiece());
        m_ActiveSquare->GetChessPiece()->SetMoved();
        ClearPreviousMoveStatuses();
        m_ActiveSquare->SetStatus(ChessSquare::Status::PreviousMove);
        toSquare->SetStatus(ChessSquare::Status::PreviousMove);
    }
}
