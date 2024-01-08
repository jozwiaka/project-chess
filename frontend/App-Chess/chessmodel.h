#ifndef CHESSMODEL_H
#define CHESSMODEL_H

#include <QObject>
#include "chesspiece.h"
#include "cnnmodel.h"
#include "chesssquare.h"
#include <array>
#include <QVector>

class ChessModel : public QObject
{
    Q_OBJECT
public:
    using Color = ChessPiece::Color;

public:
    explicit ChessModel(QObject *parent = nullptr);

    void InitializeChessboard();

    QVector<QVector<ChessSquare*>> GetChessboard();

    void UpdateModelOnSquareClick(const ChessSquare::Position& position);

    ~ChessModel();

    void MakeMove(ChessSquare* toSquare);

signals:
    void UpdateGraphics();

private:
    void ClearStatuses();

    void SetRookValidMoves();
    void SetKnightValidMoves();
    void SetBishopValidMoves();
    void SetQueenValidMoves();
    void SetKingValidMoves();
    void SetPawnValidMoves();

    bool CheckIfPositionIsValid(const ChessSquare::Position& position);

private:
    QVector<QVector<ChessSquare*>> m_Chessboard;
    Color m_CurrentTurn;
    ChessSquare* m_ActiveSquare = nullptr;
};

#endif // CHESSMODEL_H
