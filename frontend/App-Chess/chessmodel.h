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
    Chessboard::ChessboardType m_Board;

private:
    void MakeMove(ChessSquare *toSquare);
    void ClearTemporaryStatuses();
    void ClearAfterPreviousMove();

    void SetRookValidMoves(ChessSquare *source, bool checkKingAndBlockSquares = false);
    void SetKnightValidMoves(ChessSquare *source, bool checkKingAndBlockSquares = false);
    void SetBishopValidMoves(ChessSquare *source, bool checkKingAndBlockSquares = false);
    void SetQueenValidMoves(ChessSquare *source, bool checkKingAndBlockSquares = false);
    void SetKingValidMoves(ChessSquare *source, bool checkKingAndBlockSquares = false);
    void SetPawnValidMoves(ChessSquare *source, bool checkKingAndBlockSquares = false);

    bool SetValidMove(ChessSquare *source, ChessSquare *target, bool checkKingAndBlockSquares); // fix
    bool CheckIfFreeSquare(ChessSquare *target);                                                // fix
    bool CheckIfEnemy(ChessSquare *source, ChessSquare *target);                                // fix
    bool CheckIfAlly(ChessSquare *source, ChessSquare *target);                                 // fix

    void ValidateAllyKingMovesAndCheck();
    void ValidateMovesUnderCheck();

    void PromotePawnToTheType(const ChessPiece::PieceType &type);

signals:
    void UpdateChessboardGraphics();
    void ShowPromotionDialog(const ChessPiece::PieceColor &color);
};

#endif // CHESSMODEL_H
