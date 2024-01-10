#ifndef PROMOTIONDIALOG_H
#define PROMOTIONDIALOG_H

#include <QDialog>
#include <QPushButton>
#include "chesspiece.h"
#include "chesssquare.h"

namespace Ui
{
    class PromotionDialog;
}

class PromotionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PromotionDialog(const ChessPiece::PieceColor &color, QWidget *parent = nullptr);
    ~PromotionDialog();

signals:
    void PieceSelected(const ChessPiece::PieceType &selectedPiece);

private slots:
    void OnPromotionButtonClicked();

private:
    QList<QPushButton *> m_PromotionButtons;
};

#endif // PROMOTIONDIALOG_H
