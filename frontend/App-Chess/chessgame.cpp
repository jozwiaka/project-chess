#include "chessgame.h"

ChessGame::ChessGame() : m_Model{Chessboard::GetInstance()},
                         m_View{Chessboard::GetInstance(), m_Model.ComputerTurn},
                         m_Controller(&m_Model, &m_View) {}

void ChessGame::Start()
{
    m_View.show();
    m_Model.MoveCNNModel(); // TODO
}
