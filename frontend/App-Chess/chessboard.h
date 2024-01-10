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
    ChessboardType Board;

public:
    static Chessboard &GetInstance();
    ~Chessboard();
    Chessboard(const Chessboard &other) = delete;
    Chessboard &operator=(const Chessboard &other) = delete;

private:
    explicit Chessboard(QObject *parent = nullptr);
    void InitializeChessboard();

signals:
};
#endif // CHESSBOARD_H
