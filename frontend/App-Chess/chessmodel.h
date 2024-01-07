#ifndef CHESSMODEL_H
#define CHESSMODEL_H

#include <QObject>
#include "chesspiece.h"
#include "cnnmodel.h"
#include "chesssquare.h"
#include <array>
#include <memory>
#include <QVector>

class ChessModel : public QObject
{
    Q_OBJECT
public:
    using Color = ChessPiece::Color;

public:
    explicit ChessModel(QObject *parent = nullptr);

    void InitializeChessboard();

    QVector<QVector<std::shared_ptr<ChessSquare>>> GetChessboard();

    void UpdateModelOnSquareClick(const ChessSquare::Position& position);

    ~ChessModel();

    void MakeMove(ChessSquare* toSquare);

signals:
    void UpdateGraphics();

private:
    ChessSquare* FindSquare(const ChessSquare::Position& position);
    void ClearStatuses();

private:
    QVector<QVector<std::shared_ptr<ChessSquare>>> m_Chessboard;
    Color m_CurrentTurn;
    ChessSquare* m_ActiveSquare = nullptr;
};

#endif // CHESSMODEL_H
