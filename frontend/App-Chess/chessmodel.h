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

    ~ChessModel();


signals:

private:
    QVector<QVector<std::shared_ptr<ChessSquare>>> m_Chessboard;
};

#endif // CHESSMODEL_H
