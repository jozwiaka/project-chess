#ifndef CHESSBOARD_H
#define CHESSBOARD_H
#include <QObject>

class Chessboard : public QObject
{
    Q_OBJECT
public:
    explicit Chessboard(QObject *parent = nullptr);

signals:
};

#endif // CHESSBOARD_H
