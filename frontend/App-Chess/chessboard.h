#ifndef CHESSBOARD_H
#define CHESSBOARD_H
#include <QObject>
#include "chesssquare.h"
#include "chesspiece.h"

class Chessboard : public QObject
{
    Q_OBJECT

public:
    static Chessboard &GetInstance();
    ~Chessboard();

    Chessboard(const Chessboard &other) = delete;
    Chessboard &operator=(const Chessboard &other) = delete;

    QVector<ChessSquare *> operator[](int index);
    const QVector<ChessSquare *> operator[](int index) const;
    ChessSquare *GetSquareByPosition(const ChessSquare::SquarePosition &position);
    QVector<QVector<ChessSquare *>>::Iterator begin();
    QVector<QVector<ChessSquare *>>::Iterator end();

    QString Str();

signals:

private:
    QVector<QVector<ChessSquare *>> m_Data;

private:
    explicit Chessboard(QObject *parent = nullptr);
    void InitializeChessboard();
};
#endif // CHESSBOARD_H
