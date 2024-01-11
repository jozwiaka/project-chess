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

    void SetRookValidMoves(ChessSquare *square, bool blockSquares = false);
    void SetKnightValidMoves(ChessSquare *square, bool blockSquares = false);
    void SetBishopValidMoves(ChessSquare *square, bool blockSquares = false);
    void SetQueenValidMoves(ChessSquare *square, bool blockSquares = false);
    void SetKingValidMoves(ChessSquare *square, bool blockSquares = false);
    void SetPawnValidMoves(ChessSquare *square, bool blockSquares = false);

    bool CheckIfFreeSquare(ChessSquare *square); // fix
    bool CheckIfEnemy(ChessSquare *square);      // fix
    bool CheckIfAlly(ChessSquare *square);       // fix
    void ValidOpponentKingMovesAndCheck();

    bool SetValidMove(ChessSquare *square, bool blockSquare); // fix

    void PromotePawnToTheType(const ChessPiece::PieceType &type);

    void ValidMovesUnderCheck();

signals:
    void UpdateChessboardGraphics();
    void ShowPromotionDialog(const ChessPiece::PieceColor &color);
};

#endif // CHESSMODEL_H
