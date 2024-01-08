#include "chessview.h"
#include "./ui_chessview.h"

ChessView::ChessView(QWidget *parent)
    : QMainWindow(parent), m_Ui(new Ui::ChessView())
{
    m_Ui->setupUi(this);
}

ChessView::~ChessView()
{
    delete m_Ui;
    delete m_ChessboardLayout;
}

void ChessView::CreateChessboardGraphics(QVector<QVector<ChessSquare *>> chessboard)
{
    m_ChessboardLayout = new QGridLayout(m_Ui->centralwidget);
    m_ChessboardLayout->setSpacing(0);
    m_ChessboardLayout->setAlignment(Qt::AlignCenter);
    m_ChessboardLayout->setContentsMargins(0, 0, 0, 0);

    for (char row = 0; row < 8; ++row)
    {
        for (char col = 0; col < 8; ++col)
        {
            m_ChessboardLayout->addWidget(chessboard[row][col], 8 - 1 - row, col);
            connect(chessboard[row][col], &ChessSquare::Clicked, this, &ChessView::OnSquareClicked);
        }
    }
}

void ChessView::OnSquareClicked(const ChessSquare::Position &position)
{
    // ChessSquare *clickedSquare = qobject_cast<ChessSquare *>(sender());
    emit SquareClicked(position);
}

void ChessView::UpdateChessboardGraphics()
{
    for (int row = 0; row < 8; ++row)
    {
        for (int col = 0; col < 8; ++col)
        {
            ChessSquare *square = qobject_cast<ChessSquare *>(m_ChessboardLayout->itemAtPosition(row, col)->widget());

            switch (square->GetStatus())
            {
            case ChessSquare::Status::Normal:
                square->SetNormalBackgroundColor();
                break;
            case ChessSquare::Status::Active:
            case ChessSquare::Status::PreviousMove:
                square->SetHighlightBackgroundColor();
                break;
            case ChessSquare::Status::ValidMove:
            case ChessSquare::Status::ValidCapture:
                square->SetValidBackgroundColor();
                break;
            default:
                break;
            }
        }
    }
}
