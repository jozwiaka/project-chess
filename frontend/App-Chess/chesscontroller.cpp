#include "chesscontroller.h"

ChessController::ChessController(ChessModel* model, ChessView* view, QObject *parent)
    : m_Model{model}, m_View{view}, QObject{parent}
{
    m_Model->InitializeChessboard();
    m_View->InitializeChessboard(m_Model->GetChessboard());
}
