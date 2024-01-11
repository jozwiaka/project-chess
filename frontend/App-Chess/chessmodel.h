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

public:
    explicit ChessModel(const Chessboard::ChessboardType &board, QObject *parent = nullptr);

    void UpdateModelOnSquareClick(const ChessSquare::SquarePosition &position);

    ~ChessModel();

    void OnPromotionPieceSelected(const ChessPiece::PieceType &type);

    void MoveCNNModel();

private:
    PlayerColor m_CurrentTurn;
    ChessSquare *m_FromSquare;
    ChessSquare *m_SquareUnderPromotion;
    bool m_Check;
    bool m_CheckMate;
    Chessboard::ChessboardType m_Board;

private:
    void MakeMove(ChessSquare *toSquare);
    void ClearTemporaryStatuses();
    void ClearAfterPreviousMove();

    void SetRookValidMoves(ChessSquare *source, bool blockSquares = false);
    void SetKnightValidMoves(ChessSquare *source, bool blockSquares = false);
    void SetBishopValidMoves(ChessSquare *source, bool blockSquares = false);
    void SetQueenValidMoves(ChessSquare *source, bool blockSquares = false);
    void SetKingValidMoves(ChessSquare *source, bool blockSquares = false);
    void SetPawnValidMoves(ChessSquare *source, bool blockSquares = false);

    bool SetValidMove(ChessSquare *source, ChessSquare *target, bool blockSquare); // fix
    bool CheckIfFreeSquare(ChessSquare *target);                                   // fix
    bool CheckIfEnemy(ChessSquare *source, ChessSquare *target);                   // fix
    bool CheckIfAlly(ChessSquare *source, ChessSquare *target);                    // fix

    void ValidAllyKingMovesAndCheck();

    void PromotePawnToTheType(const ChessPiece::PieceType &type);

    void ValidMovesUnderCheck();

signals:
    void UpdateChessboardGraphics();
    void ShowPromotionDialog(const ChessPiece::PieceColor &color);
};

#endif // CHESSMODEL_H
