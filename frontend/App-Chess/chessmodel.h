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

private:
    struct FENData
    {
        QString PiecePlacementData = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";
        QString ActiveColor = "w";
        QString CastlingAvailability = "KQkq";
        QString EnPassantTargetSquare = "-"; // e6
        int HalfmoveClock = 0;
        int FullmoveNumber = 1;

        QString Str() const
        {
            return PiecePlacementData + " " +
                   ActiveColor + " " +
                   CastlingAvailability + " " +
                   EnPassantTargetSquare + " " +
                   QString::number(HalfmoveClock) + " " +
                   QString::number(FullmoveNumber);
        }
    };

public:
    using PlayerColor = ChessPiece::PieceColor;

public:
    bool *ComputerTurn;

public:
    explicit ChessModel(QObject *parent = nullptr);

    void UpdateModelOnSquareClick(const ChessSquare::SquarePosition &position);

    ~ChessModel();

    void OnPromotionPieceSelected(const ChessPiece::PieceType &type);

    void MoveCNNModel();

signals:
    void UpdateChessboardGraphics();
    void ShowPromotionDialog(const ChessPiece::PieceColor &color);
    void ShowEndGameDialog(const QString &message);

private:
    PlayerColor m_CurrentTurn;
    ChessSquare *m_FromSquare;
    ChessSquare *m_SquareUnderPromotion;
    Chessboard &m_Board;
    bool m_Check;
    FENData *m_FENData;
    int m_HalfMoveClock;
    bool m_Terminated;

private:
    void ClearTemporaryStatuses();
    void ClearAfterPreviousMove();

    void SetRookValidMoves(ChessSquare *source, Mode mode, bool &outCheckDetected);
    void SetKnightValidMoves(ChessSquare *source, Mode mode, bool &outCheckDetected);
    void SetBishopValidMoves(ChessSquare *source, Mode mode, bool &outCheckDetected);
    void SetQueenValidMoves(ChessSquare *source, Mode mode, bool &outCheckDetected);
    void SetKingValidMoves(ChessSquare *source, Mode mode, bool &outCheckDetected);
    void SetPawnValidMoves(ChessSquare *source, Mode mode, bool &outCheckDetected);

    void MakeMove(ChessSquare *toSquare);
    void PerformCastling(ChessSquare *toSquare);
    void PerformEnPassant(ChessSquare *toSquare);
    void SetAsEnPassant(ChessSquare *toSquare);
    void PerformPromotion(ChessSquare *toSquare);
    void MovePiece(ChessSquare *toSquare);
    void SetPreviousMove(ChessSquare *toSquare);
    void EndOfTurn();

    bool SetValidMove(ChessSquare *source, ChessSquare *target, Mode mode, bool &outCheckDetected); // fix
    bool CheckIfFreeSquare(ChessSquare *target);                                                    // fix
    bool CheckIfEnemy(ChessSquare *source, ChessSquare *target);                                    // fix
    bool CheckIfAlly(ChessSquare *source, ChessSquare *target);                                     // fix

    void ValidateKingMovesAndCheck();
    void ValidateMovesUnderCheck();

    void PromotePawnToTheType(const ChessPiece::PieceType &type);

    void UpdateFENData();
};

#endif // CHESSMODEL_H
