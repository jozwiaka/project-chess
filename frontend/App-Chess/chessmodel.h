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
    enum class Mode
    {
        Validate,
        CheckKingAndBlockSquares,
        DetectCheck,
    };

public:
    using PlayerColor = ChessPiece::PieceColor;

public:
    bool *ComputerTurn;

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

    void SetRookValidMoves(ChessSquare *source, Mode mode, bool &outCheckDetected);
    void SetKnightValidMoves(ChessSquare *source, Mode mode, bool &outCheckDetected);
    void SetBishopValidMoves(ChessSquare *source, Mode mode, bool &outCheckDetected);
    void SetQueenValidMoves(ChessSquare *source, Mode mode, bool &outCheckDetected);
    void SetKingValidMoves(ChessSquare *source, Mode mode, bool &outCheckDetected);
    void SetPawnValidMoves(ChessSquare *source, Mode mode, bool &outCheckDetected);

    bool SetValidMove(ChessSquare *source, ChessSquare *target, Mode mode, bool &outCheckDetected); // fix
    bool CheckIfFreeSquare(ChessSquare *target);                                                    // fix
    bool CheckIfEnemy(ChessSquare *source, ChessSquare *target);                                    // fix
    bool CheckIfAlly(ChessSquare *source, ChessSquare *target);                                     // fix

    void ValidateKingMovesAndCheck();
    void ValidateMovesUnderCheck();

    void PromotePawnToTheType(const ChessPiece::PieceType &type);

signals:
    void UpdateChessboardGraphics();
    void ShowPromotionDialog(const ChessPiece::PieceColor &color);
    void ShowEndGameDialog(const PlayerColor &color);
};

#endif // CHESSMODEL_H
