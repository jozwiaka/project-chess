#ifndef CHESSBOARD_H
#define CHESSBOARD_H
#include <QObject>
#include "chesssquare.h"
#include "chesspiece.h"

class Chessboard : public QObject
{
    Q_OBJECT

public:
    using ChessboardType = QVector<QVector<ChessSquare *>>;

public:
    explicit Chessboard(QObject *parent = nullptr);

private:
    void InitializeChessboard();

public:
    ChessboardType Board;

signals:
};

#endif // CHESSBOARD_H
