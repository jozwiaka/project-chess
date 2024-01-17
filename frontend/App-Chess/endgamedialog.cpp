#include "config.h"
#include "EndGameDialog.h"
#include <QVBoxLayout>
#include <QMap>
#include <QLabel>
#include <QString>

EndGameDialog::EndGameDialog(const QString &message, QWidget *parent)
    : QDialog(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    QLabel *label = new QLabel(message, this);
    layout->addWidget(label);
    setLayout(layout);
}
