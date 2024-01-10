#ifndef CHESSGAME_H
#define CHESSGAME_H
#include "chessboard.h"
#include "chessmodel.h"
#include "chessview.h"
#include "chesscontroller.h"
class ChessGame
{
public:
    ChessGame();

    void Start();

private:
    Chessboard m_Board;
    ChessModel m_Model;
    ChessView m_View;
    ChessController m_Controller;
};

#endif // CHESSGAME_H
