#ifndef CHESSGAME_H
#define CHESSGAME_H

#include <QObject>

class ChessGame : public QObject
{
    Q_OBJECT
public:
    explicit ChessGame(QObject *parent = nullptr);

    void start()
    {

    }

signals:
};

#endif // CHESSGAME_H
