#ifndef CHESSMODEL_H
#define CHESSMODEL_H

#include <QObject>
#include "chesspiece.h"
#include "cnnmodel.h"
#include "chesssquare.h"
#include <array>
#include <QVector>
#include <QThread>

class ChessModel : public QObject
{
    Q_OBJECT
public:
    using Color = ChessPiece::Color;

public:
    explicit ChessModel(QObject *parent = nullptr);

    void InitializeChessboard();

    void UpdateModelOnSquareClick(const ChessSquare::SquarePosition &position);

    ~ChessModel();

    void MakeMove(ChessSquare *toSquare);

    void MoveCNNModel();

signals:
    void UpdateGraphics();

private:
    void ClearActiveAndValidMoveStatuses();
    void ClearPreviousMoveStatusesAndEnPassants();

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

    bool ValidMovesUnderCheck();
    QVector<QVector<ChessSquare *>> DeepCopyChessboard();

    void PromotePawnToTheType(ChessSquare *square, const ChessPiece::PieceType &pieceType);

public:
    bool *ComputerTurn;
    bool *PromotionProcedure;
    QVector<QVector<ChessSquare *>> Chessboard;

private:
    Color m_CurrentTurn;
    ChessSquare *m_ActiveSquare;
    bool m_Check;
    bool m_CheckMate;
};

#endif // CHESSMODEL_H
