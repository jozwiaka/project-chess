#include "promotiondialog.h"

PromotionDialog::PromotionDialog(const ChessPiece::PieceColor &color, QWidget *parent)
    : QDialog(parent)
{
    QList<ChessPiece::PieceType> promotionChoices = {ChessPiece::PieceType::Queen, ChessPiece::PieceType::Rook, ChessPiece::PieceType::Bishop, ChessPiece::PieceType::Knight};

    for (const auto &type : promotionChoices)
    {
        auto button = new QPushButton(this);
        QString pieceImagePath = ChessPiece::GetPieceImageByColorAndType(type, color); // Use a method to get image path
        button->setIcon(QIcon(pieceImagePath));
        button->setIconSize(QSize(80, 80));
        connect(button, &QPushButton::clicked, this, &PromotionDialog::OnPromotionButtonClicked);
        m_PromotionButtons.append(button);
    }
}

PromotionDialog::~PromotionDialog()
{
}

void PromotionDialog::OnPromotionButtonClicked()
{
    QPushButton *clickedButton = qobject_cast<QPushButton *>(sender());
    if (clickedButton)
    {
        int index = m_PromotionButtons.indexOf(clickedButton);
        if (index != -1)
        {
            emit PieceSelected(static_cast<ChessPiece::PieceType>(index));
            accept(); // Close the dialog
        }
    }
}
