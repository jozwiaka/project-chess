#include "chesscontroller.h"
#include "chessboard.h"

ChessController::ChessController(ChessModel *model, ChessView *view, QObject *parent)
    : m_Model{model}, m_View{view}, QObject{parent}
{
    connect(m_View, &ChessView::SquareClicked, m_Model, &ChessModel::UpdateModelOnSquareClick);
    connect(m_Model, &ChessModel::UpdateChessboardGraphics, m_View, &ChessView::UpdateChessboardGraphics);

    connect(m_View, &ChessView::PromotedPieceSelected, m_Model, &ChessModel::OnPromotionPieceSelected);
    connect(m_Model, &ChessModel::ShowPromotionDialog, m_View, &ChessView::CreatePromotionDialog);

    connect(m_Model, &ChessModel::ShowEndGameDialog, m_View, &ChessView::CreateEndGameDialog);


    for (char row = 0; row < 8; ++row)
    {
        for (char col = 0; col < 8; ++col)
        {
            connect(Chessboard::GetInstance()[row][col], &ChessSquare::Clicked, m_View, &ChessView::OnSquareClicked);
        }
    }
}
