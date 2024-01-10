#include "chessmodel.h"
#include <QDebug>
#include <QRandomGenerator>

ChessModel::ChessModel(const Chessboard::ChessboardType &board, QObject *parent)
    : Chessboard{board},
      QObject{parent},
      m_CurrentTurn{PlayerColor::White},
      m_ActiveSquare(nullptr), m_Check(false),
      m_CheckMate(false),
      ComputerTurn(new bool(true /*QRandomGenerator::global()->bounded(0, 2)*/)),
      PromotionProcedure(new bool(false))
{
    // MoveCNNModel(); //TODO
}

ChessModel::~ChessModel()
{
    for (auto &row : Chessboard)
    {
        for (auto *square : row)
        {
            if (square)
            {
                square->RemoveChessPiece();
                delete square;
            }
        }
    }
    delete ComputerTurn;
    delete PromotionProcedure;
}

void ChessModel::PromotePawnToTheType(ChessSquare *square, const ChessPiece::PieceType &pieceType)
{
    if (square)
    {
        ChessPiece *piece = square->GetPiece();
        if (piece)
        {
            if (piece->Type == ChessPiece::PieceType::Pawn && (square->Position.x == 0 || square->Position.x == 7))
            {
                auto promotedPiece = new ChessPiece(pieceType, piece->Color, square);
                square->RemoveChessPiece();
                square->SetPiece(promotedPiece);
            }
        }
    }
}

void ChessModel::ClearTemporaryStatuses()
{
    for (auto &row : Chessboard)
    {
        for (auto &square : row)
        {
            if (square->StatusTemporary == ChessSquare::SquareStatusTemporary::Active || square->StatusTemporary == ChessSquare::SquareStatusTemporary::ValidCapture || square->StatusTemporary == ChessSquare::SquareStatusTemporary::ValidMove)
            {
                square->StatusTemporary = ChessSquare::SquareStatusTemporary::Normal;
            }
        }
    }
}

void ChessModel::ClearAfterPreviousMove()
{
    for (auto &row : Chessboard)
    {
        for (auto &square : row)
        {
            if (square->Status == ChessSquare::SquareStatus::PreviousMove ||
                square->Status == ChessSquare::SquareStatus::Check)
            {
                square->Status = ChessSquare::SquareStatus::Normal;
            }
            ChessPiece *piece = square->GetPiece();
            if (piece)
                piece->EnPassant = false;
            square->Blocked = false;
        }
    }
    m_Check = false;
    m_CheckMate = false;
}

void ChessModel::UpdateModelOnSquareClick(const ChessSquare::SquarePosition &position)
{
    ChessSquare *foundSquare = GetSquareByPosition(position);

    if (!foundSquare)
    {
        return;
    }

    bool isGoingToMakeMove = false;

    if (m_ActiveSquare && (foundSquare->StatusTemporary == ChessSquare::SquareStatusTemporary::ValidMove || foundSquare->StatusTemporary == ChessSquare::SquareStatusTemporary::ValidCapture))
    {
        isGoingToMakeMove = true;
    }

    ClearTemporaryStatuses();

    if (isGoingToMakeMove)
    {
        MakeMove(foundSquare);
    }
    m_ActiveSquare = nullptr;

    if (foundSquare->GetPiece())
    {
        ChessPiece *piece = foundSquare->GetPiece();
        if (piece->Color == m_CurrentTurn)
        {
            foundSquare->StatusTemporary = ChessSquare::SquareStatusTemporary::Active;
            m_ActiveSquare = foundSquare;

            switch (piece->Type)
            {
            case ChessPiece::PieceType::Rook:
                SetRookValidMoves(foundSquare);
                break;
            case ChessPiece::PieceType::Knight:
                SetKnightValidMoves(foundSquare);
                break;
            case ChessPiece::PieceType::Bishop:
                SetBishopValidMoves(foundSquare);
                break;
            case ChessPiece::PieceType::Queen:
                SetQueenValidMoves(foundSquare);
                break;
            case ChessPiece::PieceType::King:
                SetKingValidMoves(foundSquare);
                break;
            case ChessPiece::PieceType::Pawn:
                SetPawnValidMoves(foundSquare);
                break;
            }
        }
    }

    emit UpdateGraphics();
}

void ChessModel::SetRookValidMoves(ChessSquare *square, bool blockSquaresInstead)
{
    ChessPiece *piece = square->GetPiece();
    const int X = square->Position.x;
    const int Y = square->Position.y;

    for (int x = X + 1; x <= 7; ++x)
    {
        if (CheckAndSet({x, Y}, blockSquaresInstead))
            break;
    }

    for (int x = X - 1; x >= 0; --x)
    {
        if (CheckAndSet({x, Y}, blockSquaresInstead))
            break;
    }

    for (int y = Y + 1; y <= 7; ++y)
    {
        if (CheckAndSet({X, y}, blockSquaresInstead))
            break;
    }

    for (int y = Y - 1; y >= 0; --y)
    {
        if (CheckAndSet({X, y}, blockSquaresInstead))
            break;
    }
}

void ChessModel::SetKnightValidMoves(ChessSquare *square, bool blockSquaresInstead)
{
    ChessPiece *piece = square->GetPiece();
    const int X = square->Position.x;
    const int Y = square->Position.y;

    for (int x = X - 2; x <= X + 2; x += 4)
    {
        for (int y = Y - 1; y <= Y + 1; y += 2)
        {
            if (CheckAndSet({x, y}, blockSquaresInstead))
                break;
        }
    }

    for (int y = Y - 2; y <= Y + 2; y += 4)
    {
        for (int x = X - 1; x <= X + 1; x += 2)
        {
            if (CheckAndSet({x, y}, blockSquaresInstead))
                break;
        }
    }
}

void ChessModel::SetBishopValidMoves(ChessSquare *square, bool blockSquaresInstead)
{
    ChessPiece *piece = square->GetPiece();
    const int X = square->Position.x;
    const int Y = square->Position.y;

    for (int x = X + 1; x <= 7; ++x)
    {
        if (CheckAndSet({x, Y + x - X}, blockSquaresInstead))
            break;
    }

    for (int x = X - 1; x >= 0; --x)
    {
        if (CheckAndSet({x, Y + x - X}, blockSquaresInstead))
            break;
    }

    for (int x = X + 1; x <= 7; ++x)
    {
        if (CheckAndSet({x, Y - (x - X)}, blockSquaresInstead))
            break;
    }

    for (int x = X - 1; x >= 0; --x)
    {
        if (CheckAndSet({x, Y - (x - X)}, blockSquaresInstead))
            break;
    }
}

void ChessModel::SetQueenValidMoves(ChessSquare *square, bool blockSquaresInstead)
{
    ChessPiece *piece = square->GetPiece();
    const int X = square->Position.x;
    const int Y = square->Position.y;

    for (int x = X + 1; x <= 7; ++x)
    {
        if (CheckAndSet({x, Y}, blockSquaresInstead))
            break;
    }

    for (int x = X - 1; x >= 0; --x)
    {
        if (CheckAndSet({x, Y}, blockSquaresInstead))
            break;
    }

    for (int y = Y + 1; y <= 7; ++y)
    {
        if (CheckAndSet({X, y}, blockSquaresInstead))
            break;
    }

    for (int y = Y - 1; y >= 0; --y)
    {
        if (CheckAndSet({X, y}, blockSquaresInstead))
            break;
    }

    for (int x = X + 1; x <= 7; ++x)
    {
        if (CheckAndSet({x, Y + x - X}, blockSquaresInstead))
            break;
    }

    for (int x = X - 1; x >= 0; --x)
    {
        if (CheckAndSet({x, Y + x - X}, blockSquaresInstead))
            break;
    }

    for (int x = X + 1; x <= 7; ++x)
    {
        if (CheckAndSet({x, Y - (x - X)}, blockSquaresInstead))
            break;
    }

    for (int x = X - 1; x >= 0; --x)
    {
        if (CheckAndSet({x, Y - (x - X)}, blockSquaresInstead))
            break;
    }
}

void ChessModel::SetPawnValidMoves(ChessSquare *square, bool blockSquaresInstead)
{
    ChessPiece *piece = square->GetPiece();
    const int X = square->Position.x;
    const int Y = square->Position.y;

    int i = 1;

    if (m_CurrentTurn == PlayerColor::Black)
    {
        i = -1;
    }

    for (int x = X + i; i * x <= i * (X + i * (piece->Moved ? 1 : 2)); x += i)
    {
        ChessSquare *square = GetSquareByPosition({x, Y});
        if (CheckIfFreeSquare(square))
        {
            if (!blockSquaresInstead)
            {
                square->StatusTemporary = ChessSquare::SquareStatusTemporary::ValidMove;
            }
        }
        else
        {
            break;
        }
    }

    for (int y = Y - 1; y <= Y + 1; y += 2)
    {
        ChessSquare *square = GetSquareByPosition({X + i, y});
        if (CheckIfFreeSquare(square) || CheckIfAlly(square))
        {
            if (blockSquaresInstead)
            {
                square->Blocked = true;
            }
        }
        else if (CheckIfEnemy(square))
        {
            if (blockSquaresInstead)
            {
                if (square->GetPiece()->Type == ChessPiece::PieceType::King)
                {
                    m_Check = true;
                    square->Status = ChessSquare::SquareStatus::Check;
                }
            }
            else
            {
                square->StatusTemporary = ChessSquare::SquareStatusTemporary::ValidCapture;
            }
        }
    }

    for (int y = Y - 1; y <= Y + 1; y += 2)
    {
        ChessSquare *square = GetSquareByPosition({X, y});
        if (CheckIfEnemy(square))
        {
            if (square->GetPiece()->EnPassant)
                Chessboard[X + i][y]->StatusTemporary = ChessSquare::SquareStatusTemporary::ValidCapture;
        }
    }
}

void ChessModel::SetKingValidMoves(ChessSquare *square, bool blockSquaresInstead)
{
    ChessPiece *piece = square->GetPiece();
    const int X = square->Position.x;
    const int Y = square->Position.y;

    for (int x = X - 1; x <= X + 1; ++x)
    {
        for (int y = Y - 1; y <= Y + 1; ++y)
        {
            ChessSquare *square = GetSquareByPosition({x, y});
            if (square)
            {
                if (!square->Blocked)
                {
                    if (CheckIfFreeSquare(square))
                    {
                        if (blockSquaresInstead)
                        {
                            square->Blocked = true;
                        }
                        else
                        {
                            square->StatusTemporary = ChessSquare::SquareStatusTemporary::ValidMove;
                        }
                    }
                    else if (CheckIfEnemy(square))
                    {
                        if (!blockSquaresInstead)
                        {
                            square->StatusTemporary = ChessSquare::SquareStatusTemporary::ValidCapture;
                        }
                    }
                }
            }
        }
    }

    ChessPiece *rook1 = Chessboard[square->Position.x][0]->GetPiece();
    if (rook1)
    {
        if (
            !piece->Moved &&
            !rook1->Moved &&
            CheckIfFreeSquare(GetSquareByPosition({square->Position.x, 1})) &&
            CheckIfFreeSquare(GetSquareByPosition({square->Position.x, 2})) &&
            CheckIfFreeSquare(GetSquareByPosition({square->Position.x, 3})) &&
            !GetSquareByPosition({square->Position.x, 1})->Blocked &&
            !GetSquareByPosition({square->Position.x, 2})->Blocked &&
            !GetSquareByPosition({square->Position.x, 3})->Blocked)
        {
            if (!blockSquaresInstead)
            {
                Chessboard[square->Position.x][2]->StatusTemporary = ChessSquare::SquareStatusTemporary::ValidMove;
            }
        }
    }

    ChessPiece *rook2 = Chessboard[square->Position.x][7]->GetPiece();
    if (rook2)
    {
        if (
            !piece->Moved &&
            !rook2->Moved &&
            CheckIfFreeSquare(GetSquareByPosition({square->Position.x, 6})) &&
            CheckIfFreeSquare(GetSquareByPosition({square->Position.x, 5})) &&
            !GetSquareByPosition({square->Position.x, 6})->Blocked &&
            !GetSquareByPosition({square->Position.x, 5})->Blocked)
        {
            if (!blockSquaresInstead)
            {
                Chessboard[square->Position.x][6]->StatusTemporary = ChessSquare::SquareStatusTemporary::ValidMove;
            }
        }
    }
}

bool ChessModel::CheckIfEnemy(ChessSquare *square)
{
    if (!square)
        return false;
    ChessPiece *piece = square->GetPiece();
    if (!piece)
        return false;
    return piece->Color != m_CurrentTurn;
}

bool ChessModel::CheckIfAlly(ChessSquare *square)
{
    if (!square)
        return false;
    ChessPiece *piece = square->GetPiece();
    if (!piece)
        return false;
    return piece->Color == m_CurrentTurn;
}

bool ChessModel::CheckIfFreeSquare(ChessSquare *square)
{
    if (!square)
        return false;
    return square->GetPiece() == nullptr;
}

ChessSquare *ChessModel::GetSquareByPosition(const ChessSquare::SquarePosition &position)
{
    if (position.x >= 0 && position.x <= 7 && position.y >= 0 && position.y <= 7)
        return Chessboard[position.x][position.y];

    return nullptr;
}

void ChessModel::MakeMove(ChessSquare *toSquare)
{
    if (m_ActiveSquare == toSquare)
    {
        return;
    }
    if (m_ActiveSquare)
    {
        ChessPiece *pieceToMove = m_ActiveSquare->GetPiece();
        int xDiff = toSquare->Position.x - m_ActiveSquare->Position.x;
        int yDiff = toSquare->Position.y - m_ActiveSquare->Position.y;

        // Castling
        if (
            pieceToMove->Type == ChessPiece::PieceType::King &&
            qAbs(yDiff) == 2)
        {
            int yRock = yDiff > 0 ? 7 : 0;
            int yRockNew = yDiff > 0 ? 5 : 3;
            ChessSquare *square = GetSquareByPosition({m_ActiveSquare->Position.x, yRock});
            GetSquareByPosition({m_ActiveSquare->Position.x, yRockNew})->SetPiece(square->GetPiece());
            square->SetPiece(nullptr);
        }

        // En passant
        if (
            pieceToMove->Type == ChessPiece::PieceType::Pawn &&
            yDiff != 0)
        {
            ChessSquare::SquarePosition position{m_ActiveSquare->Position.x, m_ActiveSquare->Position.y + yDiff};
            ChessSquare *square = GetSquareByPosition(position);
            ChessPiece *piece = square->GetPiece();
            if (piece)
            {
                if (piece->EnPassant)
                {
                    square->RemoveChessPiece();
                }
            }
        }

        toSquare->RemoveChessPiece();
        toSquare->SetPiece(pieceToMove);
        m_ActiveSquare->SetPiece(nullptr);

        pieceToMove->Moved = true;

        PromotePawnToTheType(toSquare, ChessPiece::PieceType::Queen);

        ClearAfterPreviousMove();

        if (
            qAbs(xDiff) == 2 &&
            pieceToMove->Type == ChessPiece::PieceType::Pawn)
        {
            pieceToMove->EnPassant = true;
        }

        m_ActiveSquare->Status = ChessSquare::SquareStatus::PreviousMove;
        toSquare->Status = ChessSquare::SquareStatus::PreviousMove;

        CheckValidKingMovesAndCheck();

        // ValidMovesUnderCheck();

        m_CurrentTurn = m_CurrentTurn == PlayerColor::White ? PlayerColor::Black : PlayerColor::White;

        *ComputerTurn = !*ComputerTurn;

        // MoveCNNModel(); //TODO
    }
}

void ChessModel::CheckValidKingMovesAndCheck()
{
    for (auto &row : Chessboard)
    {
        for (auto *square : row)
        {
            ChessPiece *piece = square->GetPiece();
            if (piece)
            {
                if (piece->Color == m_CurrentTurn)
                {
                    switch (piece->Type)
                    {
                    case ChessPiece::PieceType::Rook:
                        SetRookValidMoves(square, true);
                        break;
                    case ChessPiece::PieceType::Knight:
                        SetKnightValidMoves(square, true);
                        break;
                    case ChessPiece::PieceType::Bishop:
                        SetBishopValidMoves(square, true);
                        break;
                    case ChessPiece::PieceType::Queen:
                        SetQueenValidMoves(square, true);
                        break;
                    case ChessPiece::PieceType::King:
                        SetKingValidMoves(square, true);
                        break;
                    case ChessPiece::PieceType::Pawn:
                        SetPawnValidMoves(square, true);
                        break;
                    }
                }
            }
        }
    }
}

bool ChessModel::CheckAndSet(const ChessSquare::SquarePosition &position, bool blockSquaresInstead)
{
    ChessSquare *square = GetSquareByPosition(position);
    if (CheckIfFreeSquare(square))
    {
        if (blockSquaresInstead)
        {
            square->Blocked = true;
        }
        else
        {
            square->StatusTemporary = ChessSquare::SquareStatusTemporary::ValidMove;
        }
    }
    else if (CheckIfEnemy(square))
    {
        if (blockSquaresInstead)
        {
            if (square->GetPiece()->Type == ChessPiece::PieceType::King)
            {
                m_Check = true;
                square->Status = ChessSquare::SquareStatus::Check;
            }
        }
        else
        {
            square->StatusTemporary = ChessSquare::SquareStatusTemporary::ValidCapture;
        }
        return true;
    }
    else if (CheckIfAlly(square))
    {
        if (blockSquaresInstead)
        {
            square->Blocked = true;
        }
        return true;
    }

    return false;
}

// bool ChessModel::ValidMovesUnderCheck()
// {
//     for (auto &row : Chessboard)
//     {
//         for (auto *square : row)
//         {
//             ChessPiece *piece = square->GetPiece();
//             if (piece)
//             {
//                 if (piece->Color == m_CurrentTurn)
//                 {
//                     switch (piece->Type)
//                     {
//                     case ChessPiece::PieceType::Rook:
//                         SetRookValidMoves(square, true);
//                         break;
//                     case ChessPiece::PieceType::Knight:
//                         SetKnightValidMoves(square, true);
//                         break;
//                     case ChessPiece::PieceType::Bishop:
//                         SetBishopValidMoves(square, true);
//                         break;
//                     case ChessPiece::PieceType::Queen:
//                         SetQueenValidMoves(square, true);
//                         break;
//                     case ChessPiece::PieceType::King:
//                         SetKingValidMoves(square, true);
//                         break;
//                     case ChessPiece::PieceType::Pawn:
//                         SetPawnValidMoves(square, true);
//                         break;
//                     }
//                 }
//             }
//         }
//     }
// }

void ChessModel::MoveCNNModel()
{
    // static QVector<ChessSquare::SquarePosition> moves{{4,7},{6,7},{4,6},{6,6}};
    static QVector<ChessSquare::SquarePosition> moves{{3, 2}, {1, 2}, {3, 1}, {1, 1}};

    if (*ComputerTurn)
    {
        UpdateModelOnSquareClick(moves.back());
        moves.pop_back();
        UpdateModelOnSquareClick(moves.back());
        moves.pop_back();
    }
}
