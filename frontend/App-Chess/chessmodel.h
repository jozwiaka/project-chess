#ifndef CHESSMODEL_H
#define CHESSMODEL_H

#include <QObject>
#include "chesspiece.h"
class ChessModel : public QObject
{
    Q_OBJECT
public:
    using Color = ChessPiece::Color;

public:
    explicit ChessModel(QObject *parent = nullptr);

signals:
};

#endif // CHESSMODEL_H
