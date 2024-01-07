#include "chessview.h"
#include "./ui_chessview.h"

ChessView::ChessView(QWidget *parent)
    : QMainWindow(parent)
    , m_Ui(std::make_unique<Ui::ChessView>())
{
    m_Ui->setupUi(this);
}

ChessView::~ChessView()
{
}

void ChessView::CreateChessboardGraphics(QVector<QVector<std::shared_ptr<ChessSquare>>> chessboard){
    m_ChessboardLayout = std::make_unique<QGridLayout>(m_Ui->centralwidget);
    m_ChessboardLayout->setSpacing(0);
    m_ChessboardLayout->setAlignment(Qt::AlignCenter);

    for (char row = 0; row < 8; ++row)
    {
        for (char col = 0; col < 8; ++col)
        {
            m_ChessboardLayout->addWidget(chessboard[row][col].get(), 8-row, col);
            connect(chessboard[row][col].get(), &ChessSquare::Clicked, this, &ChessView::OnSquareClicked);
        }
    }
}

void ChessView::OnSquareClicked(const ChessSquare::Position& position)
{
    ChessSquare *clickedSquare = qobject_cast<ChessSquare *>(sender());

    if (clickedSquare)
    {
        ChessPiece *clickedPiece = clickedSquare->GetChessPiece();
        QTextStream(stdout) << "Position: " << position.x << position.y << "\n";
        //UpdateChessboard();

    }
}

void ChessView::MoveMade() {

}

