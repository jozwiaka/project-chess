#include "chessmodel.h"
#include <QDebug>
#include <QRandomGenerator>

ChessModel::ChessModel(const Chessboard::ChessboardType &board, QObject *parent)
    : m_Board{board},
      QObject{parent},
      m_CurrentTurn{PlayerColor::White},
      m_FromSquare(nullptr),
      m_SquareUnderPromotion(nullptr),
      m_Check(false),
      m_CheckMate(false),
      ComputerTurn(new bool(true /*QRandomGenerator::global()->bounded(0, 2)*/)),
      PromotionProcedure(new bool(false))
{
}

ChessModel::~ChessModel()
{
    delete ComputerTurn;
    delete PromotionProcedure;
}

void ChessModel::ClearTemporaryStatuses()
{
    for (auto &row : m_Board)
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
    ClearTemporaryStatuses();
    for (auto &row : m_Board)
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
    ChessSquare *clickedSquare = Chessboard::GetSquareByPosition(position);

    if (!clickedSquare)
    {
        return;
    }

    if (m_FromSquare && (clickedSquare->StatusTemporary == ChessSquare::SquareStatusTemporary::ValidMove || clickedSquare->StatusTemporary == ChessSquare::SquareStatusTemporary::ValidCapture))
    {
        MakeMove(clickedSquare);
    }
    else if (clickedSquare->GetPiece())
    {
        ClearTemporaryStatuses();
        ChessPiece *piece = clickedSquare->GetPiece();
        if (piece->Color == m_CurrentTurn)
        {
            clickedSquare->StatusTemporary = ChessSquare::SquareStatusTemporary::Active;
            m_FromSquare = clickedSquare;

            switch (piece->Type)
            {
            case ChessPiece::PieceType::Rook:
                SetRookValidMoves(clickedSquare);
                break;
            case ChessPiece::PieceType::Knight:
                SetKnightValidMoves(clickedSquare);
                break;
            case ChessPiece::PieceType::Bishop:
                SetBishopValidMoves(clickedSquare);
                break;
            case ChessPiece::PieceType::Queen:
                SetQueenValidMoves(clickedSquare);
                break;
            case ChessPiece::PieceType::King:
                SetKingValidMoves(clickedSquare);
                break;
            case ChessPiece::PieceType::Pawn:
                SetPawnValidMoves(clickedSquare);
                break;
            }
        }
    }

    emit UpdateChessboardGraphics();
}

void ChessModel::SetRookValidMoves(ChessSquare *square, bool blockSquares)
{
    ChessPiece *piece = square->GetPiece();
    const int X = square->Position.x;
    const int Y = square->Position.y;

    for (int x = X + 1; x <= 7; ++x)
    {
        if (CheckAndSet({x, Y}, blockSquares))
            break;
    }

    for (int x = X - 1; x >= 0; --x)
    {
        if (CheckAndSet({x, Y}, blockSquares))
            break;
    }

    for (int y = Y + 1; y <= 7; ++y)
    {
        if (CheckAndSet({X, y}, blockSquares))
            break;
    }

    for (int y = Y - 1; y >= 0; --y)
    {
        if (CheckAndSet({X, y}, blockSquares))
            break;
    }
}

void ChessModel::SetKnightValidMoves(ChessSquare *square, bool blockSquares)
{
    ChessPiece *piece = square->GetPiece();
    const int X = square->Position.x;
    const int Y = square->Position.y;

    for (int x = X - 2; x <= X + 2; x += 4)
    {
        for (int y = Y - 1; y <= Y + 1; y += 2)
        {
            if (CheckAndSet({x, y}, blockSquares))
                break;
        }
    }

    for (int y = Y - 2; y <= Y + 2; y += 4)
    {
        for (int x = X - 1; x <= X + 1; x += 2)
        {
            if (CheckAndSet({x, y}, blockSquares))
                break;
        }
    }
}

void ChessModel::SetBishopValidMoves(ChessSquare *square, bool blockSquares)
{
    ChessPiece *piece = square->GetPiece();
    const int X = square->Position.x;
    const int Y = square->Position.y;

    for (int x = X + 1; x <= 7; ++x)
    {
        if (CheckAndSet({x, Y + x - X}, blockSquares))
            break;
    }

    for (int x = X - 1; x >= 0; --x)
    {
        if (CheckAndSet({x, Y + x - X}, blockSquares))
            break;
    }

    for (int x = X + 1; x <= 7; ++x)
    {
        if (CheckAndSet({x, Y - (x - X)}, blockSquares))
            break;
    }

    for (int x = X - 1; x >= 0; --x)
    {
        if (CheckAndSet({x, Y - (x - X)}, blockSquares))
            break;
    }
}

void ChessModel::SetQueenValidMoves(ChessSquare *square, bool blockSquares)
{
    ChessPiece *piece = square->GetPiece();
    const int X = square->Position.x;
    const int Y = square->Position.y;

    for (int x = X + 1; x <= 7; ++x)
    {
        if (CheckAndSet({x, Y}, blockSquares))
            break;
    }

    for (int x = X - 1; x >= 0; --x)
    {
        if (CheckAndSet({x, Y}, blockSquares))
            break;
    }

    for (int y = Y + 1; y <= 7; ++y)
    {
        if (CheckAndSet({X, y}, blockSquares))
            break;
    }

    for (int y = Y - 1; y >= 0; --y)
    {
        if (CheckAndSet({X, y}, blockSquares))
            break;
    }

    for (int x = X + 1; x <= 7; ++x)
    {
        if (CheckAndSet({x, Y + x - X}, blockSquares))
            break;
    }

    for (int x = X - 1; x >= 0; --x)
    {
        if (CheckAndSet({x, Y + x - X}, blockSquares))
            break;
    }

    for (int x = X + 1; x <= 7; ++x)
    {
        if (CheckAndSet({x, Y - (x - X)}, blockSquares))
            break;
    }

    for (int x = X - 1; x >= 0; --x)
    {
        if (CheckAndSet({x, Y - (x - X)}, blockSquares))
            break;
    }
}

void ChessModel::SetPawnValidMoves(ChessSquare *square, bool blockSquares)
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
        ChessSquare *square = Chessboard::GetSquareByPosition({x, Y});
        if (CheckIfFreeSquare(square))
        {
            if (!blockSquares)
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
        ChessSquare *square = Chessboard::GetSquareByPosition({X + i, y});
        if (CheckIfFreeSquare(square) || CheckIfAlly(square))
        {
            if (blockSquares)
            {
                square->Blocked = true;
            }
        }
        else if (CheckIfEnemy(square))
        {
            if (blockSquares)
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
        ChessSquare *square = Chessboard::GetSquareByPosition({X, y});
        if (CheckIfEnemy(square))
        {
            if (square->GetPiece()->EnPassant)
                m_Board[X + i][y]->StatusTemporary = ChessSquare::SquareStatusTemporary::ValidCapture;
        }
    }
}

void ChessModel::SetKingValidMoves(ChessSquare *square, bool blockSquares)
{
    ChessPiece *piece = square->GetPiece();
    const int X = square->Position.x;
    const int Y = square->Position.y;

    for (int x = X - 1; x <= X + 1; ++x)
    {
        for (int y = Y - 1; y <= Y + 1; ++y)
        {
            ChessSquare *square = Chessboard::GetSquareByPosition({x, y});
            if (square)
            {
                if (!square->Blocked)
                {
                    if (CheckIfFreeSquare(square))
                    {
                        if (blockSquares)
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
                        if (!blockSquares)
                        {
                            square->StatusTemporary = ChessSquare::SquareStatusTemporary::ValidCapture;
                        }
                    }
                }
            }
        }
    }

    ChessPiece *rook1 = m_Board[square->Position.x][0]->GetPiece();
    if (rook1)
    {
        if (
            !piece->Moved &&
            !rook1->Moved &&
            CheckIfFreeSquare(Chessboard::GetSquareByPosition({square->Position.x, 1})) &&
            CheckIfFreeSquare(Chessboard::GetSquareByPosition({square->Position.x, 2})) &&
            CheckIfFreeSquare(Chessboard::GetSquareByPosition({square->Position.x, 3})) &&
            !Chessboard::GetSquareByPosition({square->Position.x, 1})->Blocked &&
            !Chessboard::GetSquareByPosition({square->Position.x, 2})->Blocked &&
            !Chessboard::GetSquareByPosition({square->Position.x, 3})->Blocked)
        {
            if (!blockSquares)
            {
                m_Board[square->Position.x][2]->StatusTemporary = ChessSquare::SquareStatusTemporary::ValidMove;
            }
        }
    }

    ChessPiece *rook2 = m_Board[square->Position.x][7]->GetPiece();
    if (rook2)
    {
        if (
            !piece->Moved &&
            !rook2->Moved &&
            CheckIfFreeSquare(Chessboard::GetSquareByPosition({square->Position.x, 6})) &&
            CheckIfFreeSquare(Chessboard::GetSquareByPosition({square->Position.x, 5})) &&
            !Chessboard::GetSquareByPosition({square->Position.x, 6})->Blocked &&
            !Chessboard::GetSquareByPosition({square->Position.x, 5})->Blocked)
        {
            if (!blockSquares)
            {
                m_Board[square->Position.x][6]->StatusTemporary = ChessSquare::SquareStatusTemporary::ValidMove;
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

void ChessModel::MakeMove(ChessSquare *toSquare)
{
    if (!m_FromSquare || !toSquare || m_FromSquare == toSquare)
    {
        return;
    }

    ChessPiece *pieceToMove = m_FromSquare->GetPiece();
    int xDiff = toSquare->Position.x - m_FromSquare->Position.x;
    int yDiff = toSquare->Position.y - m_FromSquare->Position.y;

    // Castling
    if (
        pieceToMove->Type == ChessPiece::PieceType::King &&
        qAbs(yDiff) == 2)
    {
        int yRock = yDiff > 0 ? 7 : 0;
        int yRockNew = yDiff > 0 ? 5 : 3;
        ChessSquare *square = Chessboard::GetSquareByPosition({m_FromSquare->Position.x, yRock});
        Chessboard::GetSquareByPosition({m_FromSquare->Position.x, yRockNew})->SetPiece(square->GetPiece());
        square->SetPiece(nullptr);
    }

    // En passant
    if (
        pieceToMove->Type == ChessPiece::PieceType::Pawn &&
        yDiff != 0)
    {
        ChessSquare::SquarePosition position{m_FromSquare->Position.x, m_FromSquare->Position.y + yDiff};
        ChessSquare *square = Chessboard::GetSquareByPosition(position);
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
    m_FromSquare->SetPiece(nullptr);

    pieceToMove->Moved = true;

    ClearAfterPreviousMove();

    if (
        qAbs(xDiff) == 2 &&
        pieceToMove->Type == ChessPiece::PieceType::Pawn)
    {
        pieceToMove->EnPassant = true;
    }

    m_FromSquare->Status = ChessSquare::SquareStatus::PreviousMove;
    toSquare->Status = ChessSquare::SquareStatus::PreviousMove;

    if (pieceToMove->Type == ChessPiece::PieceType::Pawn && (toSquare->Position.x == 0 || toSquare->Position.x == 7))
    {
        m_SquareUnderPromotion = toSquare;
        if (!*ComputerTurn)
        {
            emit ShowPromotionDialog(pieceToMove->Color);
        }
    }

    ValidOpponentKingMovesAndCheck();

    m_CurrentTurn = m_CurrentTurn == PlayerColor::White ? PlayerColor::Black : PlayerColor::White;
    *ComputerTurn = !*ComputerTurn;

    m_FromSquare = nullptr;

    ValidMovesUnderCheck();

    // MoveCNNModel(); // TODO
}

void ChessModel::ValidMovesUnderCheck()
{
}

void ChessModel::ValidOpponentKingMovesAndCheck()
{
    for (auto &row : m_Board)
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

bool ChessModel::CheckAndSet(const ChessSquare::SquarePosition &position, bool blockSquares)
{
    ChessSquare *square = Chessboard::GetSquareByPosition(position);
    if (CheckIfFreeSquare(square))
    {
        if (blockSquares)
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
        if (blockSquares)
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
        if (blockSquares)
        {
            square->Blocked = true;
        }
        return true;
    }

    return false;
}

void ChessModel::MoveCNNModel()
{
    static QVector<ChessSquare::SquarePosition> moves{
        {7, 5}, {6, 6}, {6, 6}, {5, 7}, {5, 7}, {4, 7}, {4, 7}, {3, 7}, {3, 7}, {1, 7}};

    if (*ComputerTurn)
    {
        UpdateModelOnSquareClick(moves.back());
        moves.pop_back();
        UpdateModelOnSquareClick(moves.back());
        moves.pop_back();
        PromotePawnToTheType(ChessPiece::PieceType::Queen);
    }
}

void ChessModel::OnPromotionPieceSelected(const ChessPiece::PieceType &type)
{
    PromotePawnToTheType(type);
}

void ChessModel::PromotePawnToTheType(const ChessPiece::PieceType &type)
{
    if (m_SquareUnderPromotion)
    {
        ChessPiece *piece = m_SquareUnderPromotion->GetPiece();
        if (piece)
        {
            if (piece->Type == ChessPiece::PieceType::Pawn && (m_SquareUnderPromotion->Position.x == 0 || m_SquareUnderPromotion->Position.x == 7))
            {
                auto promotedPiece = new ChessPiece(type, piece->Color, m_SquareUnderPromotion);
                m_SquareUnderPromotion->RemoveChessPiece();
                m_SquareUnderPromotion->SetPiece(promotedPiece);
                m_SquareUnderPromotion = nullptr;
                ValidOpponentKingMovesAndCheck();
            }
        }
    }
}
