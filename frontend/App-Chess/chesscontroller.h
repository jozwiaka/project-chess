#ifndef CHESSCONTROLLER_H
#define CHESSCONTROLLER_H

#include <QObject>
#include "chessmodel.h"
#include "chessview.h"
#include "cnnmodel.h"
#include "chessboard.h"

class ChessController : public QObject
{
    Q_OBJECT
public:
    explicit ChessController(ChessModel *model, ChessView *view, QObject *parent = nullptr);

signals:

private:
    ChessModel *m_Model;
    ChessView *m_View;
};

#endif // CHESSCONTROLLER_H
