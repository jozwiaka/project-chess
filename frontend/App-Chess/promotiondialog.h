#ifndef PROMOTIONDIALOG_H
#define PROMOTIONDIALOG_H

#include <QDialog>
#include <QButtonGroup>
#include <QRadioButton>
#include "chesspiece.h" // Assuming ChessPiece::PieceType is used for the promotion choices

class PromotionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PromotionDialog(QWidget *parent = nullptr);
    ~PromotionDialog();

signals:
    void PieceSelected(const ChessPiece::PieceType &selectedPiece);

private slots:
    void OnPromotionButtonClicked();

private:
    QButtonGroup *m_PromotionButtonGroup;

    ChessPiece::PieceType GetSelectedPiece() const;
};

#endif // PROMOTIONDIALOG_H
