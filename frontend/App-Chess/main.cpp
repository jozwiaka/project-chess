#include "chessview.h"
#include "chesscontroller.h"
#include "chessmodel.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Chessboard board;
    ChessModel model{board.Board};
    ChessView view{board.Board, model.ComputerStarts, model.ComputerTurn, model.PromotionProcedure};
    ChessController controller(&model, &view);
    view.show();
    return a.exec();
}
