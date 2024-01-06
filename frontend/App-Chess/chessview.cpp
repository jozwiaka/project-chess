#include "chessview.h"
#include "./ui_chessview.h"

ChessView::ChessView(QWidget *parent)
    : QMainWindow(parent)
    , ui(std::make_unique<Ui::ChessView>())
{
    ui->setupUi(this);
}

ChessView::~ChessView()
{
}
