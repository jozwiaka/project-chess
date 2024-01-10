#include "chessgame.h"

ChessGame::ChessGame() : m_Board{Chessboard()},
                         m_Model{m_Board.Board},
                         m_View{m_Board.Board, m_Model.ComputerTurn, m_Model.PromotionProcedure},
                         m_Controller(&m_Model, &m_View) {}

void ChessGame::Start()
{
    m_View.show();
    m_Model.MoveCNNModel();
}
