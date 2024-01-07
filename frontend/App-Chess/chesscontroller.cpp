#include "chesscontroller.h"

ChessController::ChessController(ChessModel* model, ChessView* view, QObject *parent)
    : m_Model{model}, m_View{view}, QObject{parent}
{
    m_Model->InitializeChessboard();
    m_View->CreateChessboardGraphics(m_Model->GetChessboard());

    connect(m_Model, &ChessModel::ChessboardChanged, m_View, &ChessView::UpdateChessboard);
    connect(m_View, &ChessView::MoveMade, m_Model, &ChessModel::MakeMove);
}
