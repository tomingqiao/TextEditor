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
    void on_btFindNext_clicked();//查找下一个

    void on_btReplace_clicked();//替换

    void on_btReplaceAll_clicked();//替换所有

    void on_btCancel_clicked();//取消

private:
    Ui::ReplaceDialog *ui;
    MdiChild *pTextEdit;
    int s;
    int b;
};

#endif // REPLACEDIALOG_H
