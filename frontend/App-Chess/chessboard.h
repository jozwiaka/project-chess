#ifndef CHESSBOARD_H
#define CHESSBOARD_H
#include <QObject>
#include "chesssquare.h"
#include "chesspiece.h"

class Chessboard : public QObject
{
    Q_OBJECT

private:
    using ChessboardType = QVector<QVector<ChessSquare *>>;

public:
    static Chessboard &GetInstance();
    ~Chessboard();

    Chessboard(const Chessboard &other) = delete;
    Chessboard &operator=(const Chessboard &other) = delete;

    QVector<ChessSquare *> operator[](int index);
    const QVector<ChessSquare *> operator[](int index) const;
    ChessSquare *GetSquareByPosition(const ChessSquare::SquarePosition &position);
    ChessboardType::Iterator begin();
    ChessboardType::Iterator end();

    QString GetChessboardAsString();

private:
    ChessboardType m_Board;

private:
    explicit Chessboard(QObject *parent = nullptr);
    void InitializeChessboard();

signals:
};
#endif // CHESSBOARD_H
