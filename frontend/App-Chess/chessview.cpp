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
