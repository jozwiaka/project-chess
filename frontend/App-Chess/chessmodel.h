#ifndef CHESSMODEL_H
#define CHESSMODEL_H

#include <QObject>
#include "chesspiece.h"
#include "cnnmodel.h"
#include "chesssquare.h"
#include <array>
#include <QVector>
#include <QThread>
#include "chessboard.h"

class ChessModel : public QObject
{
    Q_OBJECT

public:
    using PlayerColor = ChessPiece::PieceColor;

public:
    bool *ComputerTurn;
    bool *PromotionProcedure;
    Chessboard::ChessboardType Chessboard;

public:
    explicit ChessModel(const Chessboard::ChessboardType &board, QObject *parent = nullptr);

    void UpdateModelOnSquareClick(const ChessSquare::SquarePosition &position);

    ~ChessModel();

    void MakeMove(ChessSquare *toSquare);

    void MoveCNNModel();

private:
    PlayerColor m_CurrentTurn;
    ChessSquare *m_ActiveSquare;
    bool m_Check;
    bool m_CheckMate;

private:
    void ClearTemporaryStatuses();
    void ClearAfterPreviousMove();

    void SetRookValidMoves(ChessSquare *square, bool blockSquaresInstead = false);
    void SetKnightValidMoves(ChessSquare *square, bool blockSquaresInstead = false);
    void SetBishopValidMoves(ChessSquare *square, bool blockSquaresInstead = false);
    void SetQueenValidMoves(ChessSquare *square, bool blockSquaresInstead = false);
    void SetKingValidMoves(ChessSquare *square, bool blockSquaresInstead = false);
    void SetPawnValidMoves(ChessSquare *square, bool blockSquaresInstead = false);

    bool CheckIfFreeSquare(ChessSquare *square);
    bool CheckIfEnemy(ChessSquare *square);
    bool CheckIfAlly(ChessSquare *square);
    void CheckValidKingMovesAndCheck();

    ChessSquare *GetSquareByPosition(const ChessSquare::SquarePosition &position);

    bool CheckAndSet(const ChessSquare::SquarePosition &position, bool blockSquaresInstead);

    // bool ValidMovesUnderCheck();

    void PromotePawnToTheType(ChessSquare *square, const ChessPiece::PieceType &pieceType);

signals:
    void UpdateGraphics();
};

#endif // CHESSMODEL_H
