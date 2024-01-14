#include "chessgame.h"

ChessGame::ChessGame() : m_Model{},
                         m_View{m_Model.ComputerTurn},
                         m_Controller(&m_Model, &m_View) {}

void ChessGame::Start()
{
    m_View.show();
    m_Model.MoveCNNModel(); // TODO
}
