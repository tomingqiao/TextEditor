#ifndef REPLACEDIALOG_H
#define REPLACEDIALOG_H

#include <QDialog>
#include "mdichild.h"

namespace Ui {
class ReplaceDialog;
}

class ReplaceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ReplaceDialog(QWidget *parent = nullptr, MdiChild *textEdit = nullptr);
    ~ReplaceDialog();

private slots:
    void on_btFindNext_clicked();

    void on_btReplace_clicked();

    void on_btReplaceAll_clicked();

    void on_btCancel_clicked();

private:
    Ui::ReplaceDialog *ui;
    MdiChild *pTextEdit;
    int s;
    int b;
};

#endif // REPLACEDIALOG_H
