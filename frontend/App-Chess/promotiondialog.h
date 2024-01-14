#ifndef PROMOTIONDIALOG_H
#define PROMOTIONDIALOG_H

#include <QDialog>
#include <QPushButton>
#include "chesspiece.h"
#include "chesssquare.h"
#include <QMap>

namespace Ui
{
    class PromotionDialog;
}

class PromotionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PromotionDialog(const ChessPiece::PieceColor &color, QWidget *parent = nullptr);

signals:
    void PieceSelected(const ChessPiece::PieceType &selectedPiece);

private slots:
    void OnPromotionButtonClicked();

private:
    QMap<QPushButton*, ChessPiece::PieceType> m_ButtonPieceTypeMap;
    QList<QPushButton *> m_PromotionButtons;
};

#endif // PROMOTIONDIALOG_H
