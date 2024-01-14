#ifndef ENDGAMEDIALOG_H
#define ENDGAMEDIALOG_H

#include <QObject>

#include <QDialog>
#include <QPushButton>
#include "chesspiece.h"
#include "chesssquare.h"
#include <QMap>

namespace Ui
{
    class EndGameDialog;
}

class EndGameDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EndGameDialog(const QString &message, QWidget *parent = nullptr);

signals:

private:
};

#endif // ENDGAMEDIALOG_H
