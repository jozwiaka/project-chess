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
                    pieceType = ChessPiece::King;
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

void ChessModel::ClearPreviousMoveStatusesAndEnPassants()
{
    for (auto &row : m_Chessboard)
    {
        for (auto &square : row)
        {
            if (square->GetStatus() == ChessSquare::Status::PreviousMove)
            {
                square->SetStatus(ChessSquare::Status::Normal);
            }
            ChessPiece *piece = square->GetChessPiece();
            if (piece)
                piece->SetEnPassant(false);
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

    // m_Chessboard[3][3]->SetStatus(ChessSquare::Status::ValidCapture);

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

void ChessModel::SetRookValidMoves()
{
    ChessPiece *piece = m_ActiveSquare->GetChessPiece();
    const int X = m_ActiveSquare->GetPosition().x;
    const int Y = m_ActiveSquare->GetPosition().y;

    for (int x = X + 1; x <= 7; ++x)
    {
        ChessSquare *square = GetSquareByPosition({x, Y});
        if (CheckIfFreeSquare(square))
            square->SetStatus(ChessSquare::Status::ValidMove);
        else if (CheckIfEnemy(square))
        {
            square->SetStatus(ChessSquare::Status::ValidCapture);
            break;
        }
        else if (CheckIfAlly(square))
        {
            break;
        }
    }

    for (int x = X - 1; x >= 0; --x)
    {
        ChessSquare *square = GetSquareByPosition({x, Y});
        if (CheckIfFreeSquare(square))
            square->SetStatus(ChessSquare::Status::ValidMove);
        else if (CheckIfEnemy(square))
        {
            square->SetStatus(ChessSquare::Status::ValidCapture);
            break;
        }
        else if (CheckIfAlly(square))
        {
            break;
        }
    }

    for (int y = Y + 1; y <= 7; ++y)
    {
        ChessSquare *square = GetSquareByPosition({X, y});
        if (CheckIfFreeSquare(square))
            square->SetStatus(ChessSquare::Status::ValidMove);
        else if (CheckIfEnemy(square))
        {
            square->SetStatus(ChessSquare::Status::ValidCapture);
            break;
        }
        else if (CheckIfAlly(square))
        {
            break;
        }
    }

    for (int y = Y - 1; y >= 0; --y)
    {
        ChessSquare *square = GetSquareByPosition({X, y});
        if (CheckIfFreeSquare(square))
            square->SetStatus(ChessSquare::Status::ValidMove);
        else if (CheckIfEnemy(square))
        {
            square->SetStatus(ChessSquare::Status::ValidCapture);
            break;
        }
        else if (CheckIfAlly(square))
        {
            break;
        }
    }
}

void ChessModel::SetKnightValidMoves()
{
    ChessPiece *piece = m_ActiveSquare->GetChessPiece();
    const int X = m_ActiveSquare->GetPosition().x;
    const int Y = m_ActiveSquare->GetPosition().y;

    for (int x = X - 2; x <= X + 2; x += 4)
    {
        for (int y = Y - 1; y <= Y + 1; y += 2)
        {
            ChessSquare *square = GetSquareByPosition({x, y});
            if (CheckIfFreeSquare(square))
                square->SetStatus(ChessSquare::Status::ValidMove);
            else if (CheckIfEnemy(square))
                square->SetStatus(ChessSquare::Status::ValidCapture);
        }
    }

    for (int y = Y - 2; y <= Y + 2; y += 4)
    {
        for (int x = X - 1; x <= X + 1; x += 2)
        {
            ChessSquare *square = GetSquareByPosition({x, y});
            if (CheckIfFreeSquare(square))
                square->SetStatus(ChessSquare::Status::ValidMove);
            else if (CheckIfEnemy(square))
                square->SetStatus(ChessSquare::Status::ValidCapture);
        }
    }
}

void ChessModel::SetBishopValidMoves()
{
    ChessPiece *piece = m_ActiveSquare->GetChessPiece();
    const int X = m_ActiveSquare->GetPosition().x;
    const int Y = m_ActiveSquare->GetPosition().y;

    for (int x = X + 1; x <= 7; ++x)
    {
        ChessSquare *square = GetSquareByPosition({x, Y + x - X});
        if (CheckIfFreeSquare(square))
            square->SetStatus(ChessSquare::Status::ValidMove);
        else if (CheckIfEnemy(square))
        {
            square->SetStatus(ChessSquare::Status::ValidCapture);
            break;
        }
        else if (CheckIfAlly(square))
        {
            break;
        }
    }

    for (int x = X - 1; x >= 0; --x)
    {
        ChessSquare *square = GetSquareByPosition({x, Y + x - X});
        if (CheckIfFreeSquare(square))
            square->SetStatus(ChessSquare::Status::ValidMove);
        else if (CheckIfEnemy(square))
        {
            square->SetStatus(ChessSquare::Status::ValidCapture);
            break;
        }
        else if (CheckIfAlly(square))
        {
            break;
        }
    }

    for (int x = X + 1; x <= 7; ++x)
    {
        ChessSquare *square = GetSquareByPosition({x, Y - (x - X)});
        if (CheckIfFreeSquare(square))
            square->SetStatus(ChessSquare::Status::ValidMove);
        else if (CheckIfEnemy(square))
        {
            square->SetStatus(ChessSquare::Status::ValidCapture);
            break;
        }
        else if (CheckIfAlly(square))
        {
            break;
        }
    }

    for (int x = X - 1; x >= 0; --x)
    {
        ChessSquare *square = GetSquareByPosition({x, Y - (x - X)});
        if (CheckIfFreeSquare(square))
            square->SetStatus(ChessSquare::Status::ValidMove);
        else if (CheckIfEnemy(square))
        {
            square->SetStatus(ChessSquare::Status::ValidCapture);
            break;
        }
        else if (CheckIfAlly(square))
        {
            break;
        }
    }
}

void ChessModel::SetQueenValidMoves()
{
    ChessPiece *piece = m_ActiveSquare->GetChessPiece();
    const int X = m_ActiveSquare->GetPosition().x;
    const int Y = m_ActiveSquare->GetPosition().y;

    for (int x = X + 1; x <= 7; ++x)
    {
        ChessSquare *square = GetSquareByPosition({x, Y});
        if (CheckIfFreeSquare(square))
            square->SetStatus(ChessSquare::Status::ValidMove);
        else if (CheckIfEnemy(square))
        {
            square->SetStatus(ChessSquare::Status::ValidCapture);
            break;
        }
        else if (CheckIfAlly(square))
        {
            break;
        }
    }

    for (int x = X - 1; x >= 0; --x)
    {
        ChessSquare *square = GetSquareByPosition({x, Y});
        if (CheckIfFreeSquare(square))
            square->SetStatus(ChessSquare::Status::ValidMove);
        else if (CheckIfEnemy(square))
        {
            square->SetStatus(ChessSquare::Status::ValidCapture);
            break;
        }
        else if (CheckIfAlly(square))
        {
            break;
        }
    }

    for (int y = Y + 1; y <= 7; ++y)
    {
        ChessSquare *square = GetSquareByPosition({X, y});
        if (CheckIfFreeSquare(square))
            square->SetStatus(ChessSquare::Status::ValidMove);
        else if (CheckIfEnemy(square))
        {
            square->SetStatus(ChessSquare::Status::ValidCapture);
            break;
        }
        else if (CheckIfAlly(square))
        {
            break;
        }
    }

    for (int y = Y - 1; y >= 0; --y)
    {
        ChessSquare *square = GetSquareByPosition({X, y});
        if (CheckIfFreeSquare(square))
            square->SetStatus(ChessSquare::Status::ValidMove);
        else if (CheckIfEnemy(square))
        {
            square->SetStatus(ChessSquare::Status::ValidCapture);
            break;
        }
        else if (CheckIfAlly(square))
        {
            break;
        }
    }

    for (int x = X + 1; x <= 7; ++x)
    {
        ChessSquare *square = GetSquareByPosition({x, Y + x - X});
        if (CheckIfFreeSquare(square))
            square->SetStatus(ChessSquare::Status::ValidMove);
        else if (CheckIfEnemy(square))
        {
            square->SetStatus(ChessSquare::Status::ValidCapture);
            break;
        }
        else if (CheckIfAlly(square))
        {
            break;
        }
    }

    for (int x = X - 1; x >= 0; --x)
    {
        ChessSquare *square = GetSquareByPosition({x, Y + x - X});
        if (CheckIfFreeSquare(square))
            square->SetStatus(ChessSquare::Status::ValidMove);
        else if (CheckIfEnemy(square))
        {
            square->SetStatus(ChessSquare::Status::ValidCapture);
            break;
        }
        else if (CheckIfAlly(square))
        {
            break;
        }
    }

    for (int x = X + 1; x <= 7; ++x)
    {
        ChessSquare *square = GetSquareByPosition({x, Y - (x - X)});
        if (CheckIfFreeSquare(square))
            square->SetStatus(ChessSquare::Status::ValidMove);
        else if (CheckIfEnemy(square))
        {
            square->SetStatus(ChessSquare::Status::ValidCapture);
            break;
        }
        else if (CheckIfAlly(square))
        {
            break;
        }
    }

    for (int x = X - 1; x >= 0; --x)
    {
        ChessSquare *square = GetSquareByPosition({x, Y - (x - X)});
        if (CheckIfFreeSquare(square))
            square->SetStatus(ChessSquare::Status::ValidMove);
        else if (CheckIfEnemy(square))
        {
            square->SetStatus(ChessSquare::Status::ValidCapture);
            break;
        }
        else if (CheckIfAlly(square))
        {
            break;
        }
    }
}
void ChessModel::SetKingValidMoves()
{

    ChessPiece *piece = m_ActiveSquare->GetChessPiece();
    const int X = m_ActiveSquare->GetPosition().x;
    const int Y = m_ActiveSquare->GetPosition().y;

    for (int x = X - 1; x <= X+1; ++x)
    {
        for (int y = Y - 1; y <= Y+1; ++y)
        {
            ChessSquare *square = GetSquareByPosition({x, y});
            if (CheckIfFreeSquare(square))
                square->SetStatus(ChessSquare::Status::ValidMove);
            else if (CheckIfEnemy(square))
            {
                square->SetStatus(ChessSquare::Status::ValidCapture);
            }
        }
    }
}

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
        ChessSquare *square = GetSquareByPosition({x, Y});
        if (CheckIfFreeSquare(square))
        {
            square->SetStatus(ChessSquare::Status::ValidMove);
        }
        else
        {
            break;
        }
    }

    for (int y = Y - 1; y <= Y + 1; y += 2)
    {
        ChessSquare *square = GetSquareByPosition({X + i, y});
        if (CheckIfEnemy(square))
        {
            square->SetStatus(ChessSquare::Status::ValidCapture);
        }
    }

    for (int y = Y - 1; y <= Y + 1; y += 2)
    {
        ChessSquare *square = GetSquareByPosition({X, y});
        if (CheckIfEnemy(square))
        {
            if (square->GetChessPiece()->IsEnPassant())
                m_Chessboard[X + i][y]->SetStatus(ChessSquare::Status::ValidCapture);
        }
    }
}

bool ChessModel::CheckIfEnemy(ChessSquare *square)
{
    if (!square)
        return false;
    ChessPiece *piece = square->GetChessPiece();
    if (!piece)
        return false;
    return piece->GetColor() != m_CurrentTurn;
}

bool ChessModel::CheckIfAlly(ChessSquare *square)
{
    if (!square)
        return false;
    ChessPiece *piece = square->GetChessPiece();
    if (!piece)
        return false;
    return piece->GetColor() == m_CurrentTurn;
}

bool ChessModel::CheckIfFreeSquare(ChessSquare *square)
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
    if (m_ActiveSquare == toSquare)
    {
        return;
    }
    if (m_ActiveSquare)
    {
        ChessPiece *pieceToMove = m_ActiveSquare->GetChessPiece();

        int yDiff = toSquare->GetPosition().y - m_ActiveSquare->GetPosition().y;

        if (
            yDiff != 0 &&
            pieceToMove->GetPieceType() == ChessPiece::PieceType::Pawn)
        {
            ChessSquare::Position position{m_ActiveSquare->GetPosition().x, m_ActiveSquare->GetPosition().y + yDiff};
            ChessSquare *square = GetSquareByPosition(position);
            ChessPiece *piece = square->GetChessPiece();
            if (piece)
            {
                if (piece->IsEnPassant())
                {
                    delete square->GetChessPiece();
                    square->SetChessPiece(nullptr);
                }
            }
        }

        if (toSquare->GetChessPiece())
            delete toSquare->GetChessPiece();

        toSquare->SetChessPiece(pieceToMove);
        m_ActiveSquare->SetChessPiece(nullptr);

        pieceToMove->SetMoved();

        ClearPreviousMoveStatusesAndEnPassants();

        if (
            qAbs(m_ActiveSquare->GetPosition().x - toSquare->GetPosition().x) == 2 &&
            pieceToMove->GetPieceType() == ChessPiece::PieceType::Pawn)
        {
            pieceToMove->SetEnPassant(true);
        }

        m_ActiveSquare->SetStatus(ChessSquare::Status::PreviousMove);
        toSquare->SetStatus(ChessSquare::Status::PreviousMove);

        m_CurrentTurn = m_CurrentTurn == Color::White ? Color::Black : Color::White;
    }
}
