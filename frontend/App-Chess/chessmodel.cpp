#include "chessmodel.h"
#include <QDebug>
#include <QRandomGenerator>
#include "cnnmodel.h"
// #include <fstream>

ChessModel::ChessModel(QObject *parent)
    : m_Board{Chessboard::GetInstance()},
      QObject{parent},
      m_CurrentTurn{PlayerColor::White},
      m_FromSquare(nullptr),
      m_SquareUnderPromotion(nullptr),
      m_Check(false),
      m_HalfMoveClock(0),
      m_FENData(new FENData),
      ComputerTurn(new bool(true /*QRandomGenerator::global()->bounded(0, 2)*/)) // TODO
{
}

ChessModel::~ChessModel()
{
    delete ComputerTurn;
    delete m_FENData;
}

void ChessModel::ClearTemporaryStatuses()
{
    for (auto &x : m_Board)
    {
        for (auto &square : x)
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
    for (auto &x : m_Board)
    {
        for (auto &square : x)
        {
            if (square->Status == ChessSquare::SquareStatus::PreviousMove ||
                square->Status == ChessSquare::SquareStatus::Check)
            {
                square->Status = ChessSquare::SquareStatus::Normal;
            }
            square->BlockedForKing = false;
            square->BlockedPieces.clear();
            ChessPiece *piece = square->GetPiece();
            if (piece)
                piece->EnPassant = false;
        }
    }
    m_Check = false;
}

void ChessModel::UpdateModelOnSquareClick(const ChessSquare::SquarePosition &position)
{
    ChessSquare *clickedSquare = m_Board.GetSquareByPosition(position);

    if (!clickedSquare)
        return;

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
            bool _;
            switch (piece->Type)
            {
            case ChessPiece::PieceType::Rook:
                SetRookValidMoves(clickedSquare, Mode::Validate, _);
                break;
            case ChessPiece::PieceType::Knight:
                SetKnightValidMoves(clickedSquare, Mode::Validate, _);
                break;
            case ChessPiece::PieceType::Bishop:
                SetBishopValidMoves(clickedSquare, Mode::Validate, _);
                break;
            case ChessPiece::PieceType::Queen:
                SetQueenValidMoves(clickedSquare, Mode::Validate, _);
                break;
            case ChessPiece::PieceType::King:
                SetKingValidMoves(clickedSquare, Mode::Validate, _);
                break;
            case ChessPiece::PieceType::Pawn:
                SetPawnValidMoves(clickedSquare, Mode::Validate, _);
                break;
            }
        }
    }

    emit UpdateChessboardGraphics();
}

void ChessModel::SetRookValidMoves(ChessSquare *source, Mode mode, bool &outCheckDetected)
{
    const int X = source->Position.x;
    const int Y = source->Position.y;

    for (int x = X + 1; x < 8; ++x)
    {
        if (SetValidMove(source, m_Board.GetSquareByPosition({x, Y}), mode, outCheckDetected))
            break;
    }

    for (int x = X - 1; x >= 0; --x)
    {
        if (SetValidMove(source, m_Board.GetSquareByPosition({x, Y}), mode, outCheckDetected))
            break;
    }

    for (int y = Y + 1; y < 8; ++y)
    {
        if (SetValidMove(source, m_Board.GetSquareByPosition({X, y}), mode, outCheckDetected))
            break;
    }

    for (int y = Y - 1; y >= 0; --y)
    {
        if (SetValidMove(source, m_Board.GetSquareByPosition({X, y}), mode, outCheckDetected))
            break;
    }
}

void ChessModel::SetKnightValidMoves(ChessSquare *source, Mode mode, bool &outCheckDetected)
{
    const int X = source->Position.x;
    const int Y = source->Position.y;

    for (int x = X - 2; x <= X + 2; x += 4)
    {
        for (int y = Y - 1; y <= Y + 1; y += 2)
        {
            SetValidMove(source, m_Board.GetSquareByPosition({x, y}), mode, outCheckDetected);
        }
    }

    for (int y = Y - 2; y <= Y + 2; y += 4)
    {
        for (int x = X - 1; x <= X + 1; x += 2)
        {
            SetValidMove(source, m_Board.GetSquareByPosition({x, y}), mode, outCheckDetected);
        }
    }
}

void ChessModel::SetBishopValidMoves(ChessSquare *source, Mode mode, bool &outCheckDetected)
{
    const int X = source->Position.x;
    const int Y = source->Position.y;

    for (int x = X + 1; x < 8; ++x)
    {
        if (SetValidMove(source, m_Board.GetSquareByPosition({x, Y + x - X}), mode, outCheckDetected))
            break;
    }

    for (int x = X - 1; x >= 0; --x)
    {
        if (SetValidMove(source, m_Board.GetSquareByPosition({x, Y + x - X}), mode, outCheckDetected))
            break;
    }

    for (int x = X + 1; x < 8; ++x)
    {
        if (SetValidMove(source, m_Board.GetSquareByPosition({x, Y - (x - X)}), mode, outCheckDetected))
            break;
    }

    for (int x = X - 1; x >= 0; --x)
    {
        if (SetValidMove(source, m_Board.GetSquareByPosition({x, Y - (x - X)}), mode, outCheckDetected))
            break;
    }
}

void ChessModel::SetQueenValidMoves(ChessSquare *source, Mode mode, bool &outCheckDetected)
{
    const int X = source->Position.x;
    const int Y = source->Position.y;

    for (int x = X + 1; x < 8; ++x)
    {
        if (SetValidMove(source, m_Board.GetSquareByPosition({x, Y}), mode, outCheckDetected))
            break;
    }

    for (int x = X - 1; x >= 0; --x)
    {
        if (SetValidMove(source, m_Board.GetSquareByPosition({x, Y}), mode, outCheckDetected))
            break;
    }

    for (int y = Y + 1; y < 8; ++y)
    {
        if (SetValidMove(source, m_Board.GetSquareByPosition({X, y}), mode, outCheckDetected))
            break;
    }

    for (int y = Y - 1; y >= 0; --y)
    {
        if (SetValidMove(source, m_Board.GetSquareByPosition({X, y}), mode, outCheckDetected))
            break;
    }

    for (int x = X + 1; x < 8; ++x)
    {
        if (SetValidMove(source, m_Board.GetSquareByPosition({x, Y + x - X}), mode, outCheckDetected))
            break;
    }

    for (int x = X - 1; x >= 0; --x)
    {
        if (SetValidMove(source, m_Board.GetSquareByPosition({x, Y + x - X}), mode, outCheckDetected))
            break;
    }

    for (int x = X + 1; x < 8; ++x)
    {
        if (SetValidMove(source, m_Board.GetSquareByPosition({x, Y - (x - X)}), mode, outCheckDetected))
            break;
    }

    for (int x = X - 1; x >= 0; --x)
    {
        if (SetValidMove(source, m_Board.GetSquareByPosition({x, Y - (x - X)}), mode, outCheckDetected))
            break;
    }
}

void ChessModel::SetPawnValidMoves(ChessSquare *source, Mode mode, bool &outCheckDetected)
{
    ChessPiece *piece = source->GetPiece();
    const int X = source->Position.x;
    const int Y = source->Position.y;

    int i = 1;

    if (piece->Color == ChessPiece::PieceColor::Black)
    {
        i = -1;
    }

    for (int x = X + i; i * x <= i * (X + i * (piece->Moved ? 1 : 2)); x += i)
    {
        ChessSquare *target = m_Board.GetSquareByPosition({x, Y});
        if (target)
        {
            if (!target->IsPieceBlocked(piece))
            {
                if (mode == Mode::Validate)
                {
                    if (CheckIfFreeSquare(target))
                    {
                        target->StatusTemporary = ChessSquare::SquareStatusTemporary::ValidMove;
                    }
                    else
                    {
                        break;
                    }
                }
                else if (mode == Mode::CheckKingAndBlockSquares)
                {
                    if (!CheckIfFreeSquare(target))
                    {
                        break;
                    }
                }
            }
        }
    }
    for (int y = Y - 1; y <= Y + 1; y += 2)
    {
        ChessSquare *target = m_Board.GetSquareByPosition({X + i, y});
        if (target)
        {
            if (!target->IsPieceBlocked(piece))
            {
                if (mode == Mode::Validate)
                {
                    if (CheckIfEnemy(source, target))
                    {
                        target->StatusTemporary = ChessSquare::SquareStatusTemporary::ValidCapture;
                    }
                }
                else if (mode == Mode::CheckKingAndBlockSquares)
                {
                    if (CheckIfFreeSquare(target) || CheckIfAlly(source, target))
                    {
                        target->BlockedForKing = true;
                    }
                    else if (CheckIfEnemy(source, target))
                    {
                        if (target->GetPiece()->Type == ChessPiece::PieceType::King)
                        {
                            target->Status = ChessSquare::SquareStatus::Check;
                            m_Check = true;
                        }
                    }
                }
                else if (mode == Mode::DetectCheck)
                {
                    if (CheckIfEnemy(source, target))
                    {
                        if (target->GetPiece()->Type == ChessPiece::PieceType::King)
                        {
                            outCheckDetected = true;
                            return;
                        }
                    }
                }
            }
        }
    }

    for (int y = Y - 1; y <= Y + 1; y += 2)
    {
        if (mode == Mode::Validate)
        {
            ChessSquare *target = m_Board.GetSquareByPosition({X, y});
            if (target)
            {
                if (!target->IsPieceBlocked(piece))
                {
                    if (CheckIfEnemy(source, target))
                    {
                        if (target->GetPiece()->EnPassant)
                            m_Board[X + i][y]->StatusTemporary = ChessSquare::SquareStatusTemporary::ValidCapture;
                    }
                }
            }
        }
    }
}

void ChessModel::SetKingValidMoves(ChessSquare *source, Mode mode, bool &outCheckDetected)
{
    ChessPiece *piece = source->GetPiece();
    const int X = source->Position.x;
    const int Y = source->Position.y;

    for (int x = X - 1; x <= X + 1; ++x)
    {
        for (int y = Y - 1; y <= Y + 1; ++y)
        {
            ChessSquare *target = m_Board.GetSquareByPosition({x, y});
            if (target)
            {
                if (!target->IsPieceBlocked(piece) && !target->BlockedForKing)
                {

                    if (mode == Mode::Validate)
                    {
                        if (CheckIfFreeSquare(target))
                        {
                            target->StatusTemporary = ChessSquare::SquareStatusTemporary::ValidMove;
                        }
                        else if (CheckIfEnemy(source, target))
                        {
                            target->StatusTemporary = ChessSquare::SquareStatusTemporary::ValidCapture;
                        }
                    }
                    else if (mode == Mode::CheckKingAndBlockSquares)
                    {
                        if (CheckIfFreeSquare(target))
                        {
                            target->BlockedForKing = true;
                        }
                        else if (CheckIfAlly(source, target))
                        {
                            target->BlockedForKing = true;
                        }
                    }
                }
            }
        }
    }

    if (mode == Mode::Validate)
    {
        ChessPiece *rookAtKingSide = m_Board[source->Position.x][7]->GetPiece();
        if (rookAtKingSide)
        {
            if (
                !piece->Moved &&
                !rookAtKingSide->Moved &&
                CheckIfFreeSquare(m_Board[source->Position.x][6]) &&
                CheckIfFreeSquare(m_Board[source->Position.x][5]) &&
                !m_Board[source->Position.x][6]->BlockedForKing &&
                !m_Board[source->Position.x][5]->BlockedForKing)
            {
                m_Board[source->Position.x][6]->StatusTemporary = ChessSquare::SquareStatusTemporary::ValidMove;
            }
        }

        ChessPiece *rookAtQueenSide = m_Board[source->Position.x][0]->GetPiece();
        if (rookAtQueenSide)
        {
            if (
                !piece->Moved &&
                !rookAtQueenSide->Moved &&
                CheckIfFreeSquare(m_Board[source->Position.x][1]) &&
                CheckIfFreeSquare(m_Board[source->Position.x][2]) &&
                CheckIfFreeSquare(m_Board[source->Position.x][3]) &&
                !m_Board[source->Position.x][1]->BlockedForKing &&
                !m_Board[source->Position.x][2]->BlockedForKing &&
                !m_Board[source->Position.x][3]->BlockedForKing)
            {
                m_Board[source->Position.x][2]->StatusTemporary = ChessSquare::SquareStatusTemporary::ValidMove;
            }
        }
    }
}

bool ChessModel::SetValidMove(ChessSquare *source, ChessSquare *target, Mode mode, bool &outCheckDetected)
{
    if (target)
    {
        ChessPiece *piece = source->GetPiece();
        if (!target->IsPieceBlocked(piece))
        {
            if (mode == Mode::Validate)
            {
                if (CheckIfFreeSquare(target))
                {
                    target->StatusTemporary = ChessSquare::SquareStatusTemporary::ValidMove;
                }
                else if (CheckIfEnemy(source, target))
                {
                    target->StatusTemporary = ChessSquare::SquareStatusTemporary::ValidCapture;
                    return true;
                }
                else if (CheckIfAlly(source, target))
                {
                    return true;
                }
            }
            else if (mode == Mode::CheckKingAndBlockSquares)
            {
                if (CheckIfFreeSquare(target))
                {
                    target->BlockedForKing = true;
                }
                else if (CheckIfEnemy(source, target))
                {
                    if (target->GetPiece()->Type == ChessPiece::PieceType::King)
                    {
                        target->Status = ChessSquare::SquareStatus::Check;
                        m_Check = true;
                    }
                    return true;
                }
                else if (CheckIfAlly(source, target))
                {
                    target->BlockedForKing = true;
                    return true;
                }
            }
            else if (mode == Mode::DetectCheck)
            {
                if (CheckIfEnemy(source, target))
                {
                    if (target->GetPiece()->Type == ChessPiece::PieceType::King)
                    {
                        outCheckDetected = true;
                    }
                    return true;
                }
                else if (CheckIfAlly(source, target))
                {
                    return true;
                }
            }
        }
        else
        {
            return true;
        }
    }
    return false;
}

bool ChessModel::CheckIfFreeSquare(ChessSquare *target)
{
    if (!target)
        return false;
    return target->GetPiece() == nullptr;
}

bool ChessModel::CheckIfEnemy(ChessSquare *source, ChessSquare *target)
{
    if (!source || !target)
        return false;
    ChessPiece *sourcePiece = source->GetPiece();
    ChessPiece *targetPiece = target->GetPiece();
    if (!sourcePiece || !targetPiece)
        return false;
    return sourcePiece->Color != targetPiece->Color;
}

bool ChessModel::CheckIfAlly(ChessSquare *source, ChessSquare *target)
{
    if (!source || !target)
        return false;
    ChessPiece *sourcePiece = source->GetPiece();
    ChessPiece *targetPiece = target->GetPiece();
    if (!sourcePiece || !targetPiece)
        return false;
    return sourcePiece->Color == targetPiece->Color;
}

void ChessModel::PerformCastling(ChessSquare *toSquare)
{
    ChessPiece *pieceToMove = m_FromSquare->GetPiece();
    int yDiff = toSquare->Position.y - m_FromSquare->Position.y;

    if (
        pieceToMove->Type == ChessPiece::PieceType::King &&
        qAbs(yDiff) == 2)
    {
        int yRook = yDiff > 0 ? 7 : 0;
        int yRookNew = yDiff > 0 ? 5 : 3;
        ChessSquare *square = m_Board[m_FromSquare->Position.x][yRook];
        m_Board[m_FromSquare->Position.x][yRookNew]->SetPiece(square->GetPiece());
        square->SetPiece(nullptr);
    }
}

void ChessModel::PerformEnPassant(ChessSquare *toSquare)
{
    ChessPiece *pieceToMove = m_FromSquare->GetPiece();
    int yDiff = toSquare->Position.y - m_FromSquare->Position.y;
    if (
        pieceToMove->Type == ChessPiece::PieceType::Pawn &&
        yDiff != 0)
    {
        ChessSquare::SquarePosition position{m_FromSquare->Position.x, m_FromSquare->Position.y + yDiff};
        ChessSquare *square = m_Board.GetSquareByPosition(position);
        ChessPiece *piece = square->GetPiece();
        if (piece)
        {
            if (piece->EnPassant)
            {
                square->RemoveChessPiece();
            }
        }
    }
}
void ChessModel::SetAsEnPassant(ChessSquare *toSquare)
{
    ChessPiece *pieceToMove = toSquare->GetPiece();
    int xDiff = toSquare->Position.x - m_FromSquare->Position.x;
    if (
        qAbs(xDiff) == 2 &&
        pieceToMove->Type == ChessPiece::PieceType::Pawn)
    {
        pieceToMove->EnPassant = true;
    }
}

void ChessModel::PerformPromotion(ChessSquare *toSquare)
{
    ChessPiece *pieceToMove = toSquare->GetPiece();
    if (pieceToMove->Type == ChessPiece::PieceType::Pawn && (toSquare->Position.x == 0 || toSquare->Position.x == 7))
    {
        m_SquareUnderPromotion = toSquare;
        if (!*ComputerTurn)
        {
            emit ShowPromotionDialog(pieceToMove->Color);
        }
    }
}

void ChessModel::EndOfTurn()
{
    m_FromSquare = nullptr;
    m_CurrentTurn = m_CurrentTurn == PlayerColor::White ? PlayerColor::Black : PlayerColor::White;
    *ComputerTurn = !*ComputerTurn;
}

void ChessModel::MovePiece(ChessSquare *toSquare)
{
    ChessPiece *pieceToMove = m_FromSquare->GetPiece();
    if (toSquare->GetPiece() || pieceToMove->Type == ChessPiece::PieceType::Pawn)
    {
        m_HalfMoveClock = 0;
    }
    else
    {
        ++m_HalfMoveClock;
    }
    pieceToMove->Moved = true;
    toSquare->RemoveChessPiece();
    toSquare->SetPiece(pieceToMove);
    m_FromSquare->SetPiece(nullptr);
}

void ChessModel::SetPreviousMove(ChessSquare *toSquare)
{
    m_FromSquare->Status = ChessSquare::SquareStatus::PreviousMove;
    toSquare->Status = ChessSquare::SquareStatus::PreviousMove;
}

void ChessModel::MakeMove(ChessSquare *toSquare)
{
    if (!m_FromSquare || !toSquare || m_FromSquare == toSquare)
        return;

    // Pre-clear phase
    PerformCastling(toSquare);
    PerformEnPassant(toSquare);
    MovePiece(toSquare);
    PerformPromotion(toSquare);

    // Clear
    ClearAfterPreviousMove();

    // Post-clear phase
    SetAsEnPassant(toSquare);
    SetPreviousMove(toSquare);
    ValidateKingMovesAndCheck();

    // End of turn
    EndOfTurn();

    // Beginning of the next turn
    ValidateMovesUnderCheck();

    UpdateFENData();
    QString data = m_FENData->Str();
    qDebug() << data;
    MoveCNNModel(); // TODO
}

void ChessModel::ValidateMovesUnderCheck()
{
    bool noValidSquares = true;
    for (auto &allyRow : m_Board)
    {
        for (auto *allySquare : allyRow)
        {
            ChessPiece *allyPiece = allySquare->GetPiece();
            if (allyPiece)
            {
                if (allyPiece->Color == m_CurrentTurn)
                {
                    bool _;
                    switch (allyPiece->Type)
                    {
                    case ChessPiece::PieceType::Rook:
                        SetRookValidMoves(allySquare, Mode::Validate, _);
                        break;
                    case ChessPiece::PieceType::Knight:
                        SetKnightValidMoves(allySquare, Mode::Validate, _);
                        break;
                    case ChessPiece::PieceType::Bishop:
                        SetBishopValidMoves(allySquare, Mode::Validate, _);
                        break;
                    case ChessPiece::PieceType::Queen:
                        SetQueenValidMoves(allySquare, Mode::Validate, _);
                        break;
                    case ChessPiece::PieceType::King:
                        SetKingValidMoves(allySquare, Mode::Validate, _);
                        break;
                    case ChessPiece::PieceType::Pawn:
                        SetPawnValidMoves(allySquare, Mode::Validate, _);
                        break;
                    }

                    for (int x = 0; x < 8; ++x)
                    {
                        for (int y = 0; y < 8; ++y)
                        {
                            if (m_Board[x][y]->StatusTemporary == ChessSquare::SquareStatusTemporary::ValidMove ||
                                m_Board[x][y]->StatusTemporary == ChessSquare::SquareStatusTemporary::ValidCapture)
                            {
                                ChessPiece *piece1 = allySquare->GetPiece();
                                ChessPiece *piece2 = m_Board[x][y]->GetPiece();

                                allySquare->SetPiece(nullptr);
                                m_Board[x][y]->SetPiece(piece1);

                                for (auto &enemyRow : m_Board)
                                {
                                    for (auto *enemySquare : enemyRow)
                                    {
                                        ChessPiece *enemyPiece = enemySquare->GetPiece();
                                        if (enemyPiece)
                                        {
                                            if (enemyPiece->Color != m_CurrentTurn)
                                            {
                                                bool outCheckDetected = false;
                                                switch (enemyPiece->Type)
                                                {
                                                case ChessPiece::PieceType::Rook:
                                                    SetRookValidMoves(enemySquare, Mode::DetectCheck, outCheckDetected);
                                                    break;
                                                case ChessPiece::PieceType::Knight:
                                                    SetKnightValidMoves(enemySquare, Mode::DetectCheck, outCheckDetected);
                                                    break;
                                                case ChessPiece::PieceType::Bishop:
                                                    SetBishopValidMoves(enemySquare, Mode::DetectCheck, outCheckDetected);
                                                    break;
                                                case ChessPiece::PieceType::Queen:
                                                    SetQueenValidMoves(enemySquare, Mode::DetectCheck, outCheckDetected);
                                                    break;
                                                case ChessPiece::PieceType::King:
                                                    SetKingValidMoves(enemySquare, Mode::DetectCheck, outCheckDetected);
                                                    break;
                                                case ChessPiece::PieceType::Pawn:
                                                    SetPawnValidMoves(enemySquare, Mode::DetectCheck, outCheckDetected);
                                                    break;
                                                }
                                                if (outCheckDetected)
                                                {
                                                    m_Board[x][y]->BlockedPieces.push_back(piece1);
                                                    break;
                                                }
                                            }
                                        }
                                    }
                                }
                                allySquare->SetPiece(piece1);
                                m_Board[x][y]->SetPiece(piece2);
                                if (!m_Board[x][y]->IsPieceBlocked(piece1))
                                {
                                    noValidSquares = false;
                                }
                            }
                        }
                    }
                    ClearTemporaryStatuses();
                }
            }
        }
    }

    QString message;
    if (noValidSquares)
    {
        if (m_Check)
        {
            PlayerColor winner = m_CurrentTurn == PlayerColor::White ? PlayerColor::Black : PlayerColor::White;
            if (winner == ChessPiece::PieceColor::White)
            {
                message = "White win!";
            }
            else
            {
                message = "Black win!";
            }
        }
        else
        {
            message = "Stale mate.";
        }
        emit ShowEndGameDialog(message);
    }
    else if (m_HalfMoveClock == 50)
    {
        message = "Draw - 50 rule";
        emit ShowEndGameDialog(message);
    }
}

void ChessModel::ValidateKingMovesAndCheck()
{
    for (auto &x : m_Board)
    {
        for (auto *square : x)
        {
            ChessPiece *piece = square->GetPiece();
            if (piece)
            {
                if (piece->Color == m_CurrentTurn)
                {
                    bool _;
                    switch (piece->Type)
                    {
                    case ChessPiece::PieceType::Rook:
                        SetRookValidMoves(square, Mode::CheckKingAndBlockSquares, _);
                        break;
                    case ChessPiece::PieceType::Knight:
                        SetKnightValidMoves(square, Mode::CheckKingAndBlockSquares, _);
                        break;
                    case ChessPiece::PieceType::Bishop:
                        SetBishopValidMoves(square, Mode::CheckKingAndBlockSquares, _);
                        break;
                    case ChessPiece::PieceType::Queen:
                        SetQueenValidMoves(square, Mode::CheckKingAndBlockSquares, _);
                        break;
                    case ChessPiece::PieceType::King:
                        SetKingValidMoves(square, Mode::CheckKingAndBlockSquares, _);
                        break;
                    case ChessPiece::PieceType::Pawn:
                        SetPawnValidMoves(square, Mode::CheckKingAndBlockSquares, _);
                        break;
                    }
                }
            }
        }
    }
}

void ChessModel::MoveCNNModel()
{
    CNNModel model;
    auto position = model.Run(m_FENData->Str());

    qDebug()<<position.x;
    qDebug()<<position.y;
    // static QVector<ChessSquare::SquarePosition> moves{
    //     {7, 5}, {6, 6}, {6, 6}, {5, 7}, {5, 7}, {4, 7}, {4, 7}, {3, 7}, {3, 7}, {1, 7}};

    if (*ComputerTurn)
    {
        // UpdateModelOnSquareClick(moves.back());
        // moves.pop_back();
        // UpdateModelOnSquareClick(moves.back());
        // moves.pop_back();
        // PromotePawnToTheType(ChessPiece::PieceType::Queen);
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
                ValidateKingMovesAndCheck();
            }
        }
    }
}

void ChessModel::UpdateFENData()
{
    m_FENData->EnPassantTargetSquare = "-";
    if (m_CurrentTurn == PlayerColor::White)
    {
        m_FENData->ActiveColor = "w";
        ++m_FENData->FullmoveNumber;
    }
    else
    {
        m_FENData->ActiveColor = "b";
    }

    QString CastlingAvailabilityWhite = "";
    QString CastlingAvailabilityBlack = "";

    for (auto &row : m_Board)
    {
        for (auto *source : row)
        {
            ChessPiece *piece = source->GetPiece();
            if (piece)
            {
                if (piece->EnPassant)
                {
                    m_FENData->EnPassantTargetSquare = source->Position.Str();
                }

                if (piece->Type == ChessPiece::PieceType::King)
                {
                    ChessPiece *rookAtKingSide = m_Board[source->Position.x][7]->GetPiece();
                    if (rookAtKingSide)
                    {
                        if (
                            !piece->Moved &&
                            !rookAtKingSide->Moved)
                        {
                            if (piece->Color == ChessPiece::PieceColor::White)
                            {
                                CastlingAvailabilityWhite += "K";
                            }
                            else
                            {
                                CastlingAvailabilityBlack += "k";
                            }
                        }
                    }

                    ChessPiece *rookAtQueenSide = m_Board[source->Position.x][0]->GetPiece();
                    if (rookAtQueenSide)
                    {
                        if (
                            !piece->Moved &&
                            !rookAtQueenSide->Moved)
                        {
                            if (piece->Color == ChessPiece::PieceColor::White)
                            {
                                CastlingAvailabilityWhite += "Q";
                            }
                            else
                            {
                                CastlingAvailabilityBlack += "q";
                            }
                        }
                    }
                }
            }
        }
    }

    m_FENData->CastlingAvailability = CastlingAvailabilityWhite + CastlingAvailabilityBlack;
    if (m_FENData->CastlingAvailability == "")
    {
        m_FENData->CastlingAvailability = "-";
    }
    m_FENData->PiecePlacementData = m_Board.Str();
    m_FENData->HalfmoveClock = m_HalfMoveClock;
}
