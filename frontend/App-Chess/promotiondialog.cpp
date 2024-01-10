#include "promotiondialog.h"

PromotionDialog::PromotionDialog(QWidget *parent)
    : QDialog(parent)
{
    m_PromotionButtonGroup = new QButtonGroup(this);

    // QList<ChessPiece::PieceType> promotionChoices = {ChessPiece::Queen, ChessPiece::Rook, ChessPiece::Bishop, ChessPiece::Knight};

    // for (const auto &pieceType : promotionChoices)
    // {
    //     auto radioButton = new QRadioButton(this);
    //     radioButton->setText(ChessPiece::PieceTypeToString(pieceType)); // You need a method to convert PieceType to a string
    //     m_PromotionButtonGroup->addButton(radioButton, static_cast<int>(pieceType));
    //     ui->verticalLayout->addWidget(radioButton);
    // }

    // connect(m_PromotionButtonGroup, QOverload<QAbstractButton *>::of(&QButtonGroup::buttonClicked), this, &PromotionDialog::OnPromotionButtonClicked);
}

PromotionDialog::~PromotionDialog()
{
}

void PromotionDialog::OnPromotionButtonClicked()
{
    // emit PieceSelected(GetSelectedPiece());
    accept(); // Close the dialog
}

ChessPiece::PieceType PromotionDialog::GetSelectedPiece() const
{
    return static_cast<ChessPiece::PieceType>(m_PromotionButtonGroup->checkedId());
}
