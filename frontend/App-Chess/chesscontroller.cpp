#include "chesscontroller.h"
#include <QCoreApplication>

ChessController::ChessController(ChessModel* model, ChessView* view, QObject *parent)
    : m_Model{model}, m_View{view}, QObject{parent}
{
    m_Model->InitializeChessboard();
    m_View->CreateChessboardGraphics(m_Model->GetChessboard());

    connect(m_View, &ChessView::SquareClicked, m_Model, &ChessModel::UpdateModelOnSquareClick);
    connect(m_Model, &ChessModel::UpdateGraphics, m_View, &ChessView::UpdateChessboardGraphics);

    m_View->show();

}
