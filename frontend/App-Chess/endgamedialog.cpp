#include "EndGameDialog.h"
#include <QVBoxLayout>
#include <QMap>
#include <QLabel>
#include <QString>

EndGameDialog::EndGameDialog(const ChessPiece::PieceColor &color, QWidget *parent)
    : QDialog(parent)
{

    QVBoxLayout *layout = new QVBoxLayout(this);

    QString str;
    if(color==ChessPiece::PieceColor::White)
    {
        str="White win!";
    }
    else{
        str="Black win!";
    }

    QLabel *label = new QLabel(str, this);

    layout->addWidget(label);

    setLayout(layout);
}
