#include "chesscontroller.h"

ChessController::ChessController(ChessModel *model, ChessView *view, QObject *parent)
    : m_Model{model}, m_View{view}, QObject{parent}
{
    connect(m_View, &ChessView::SquareClicked, m_Model, &ChessModel::UpdateModelOnSquareClick);
    connect(m_Model, &ChessModel::UpdateChessboardGraphics, m_View, &ChessView::UpdateChessboardGraphics);
    connect(m_View, &ChessView::PromotedPieceSelected, m_Model, &ChessModel::OnPromotionPieceSelected);
    connect(m_Model, &ChessModel::ShowPromotionDialog, m_View, &ChessView::CreatePromotionDialog);
}
