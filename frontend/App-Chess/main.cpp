#include "chessview.h"
#include "chesscontroller.h"
#include "chessmodel.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ChessModel model;
    ChessView view{model.ComputerTurn, model.PromotionProcedure};
    ChessController controller(&model, &view);

    return a.exec();
}
