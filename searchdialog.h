#ifndef SEARCHDIALOG_H
#define SEARCHDIALOG_H

#include <QDialog>
#include <QPlainTextEdit>
#include "mdichild.h"

namespace Ui {
class SearchDialog;
}

class SearchDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SearchDialog(QWidget *parent = nullptr, MdiChild *textEdit = nullptr);
    ~SearchDialog();

private slots:
    void on_btFindNext_clicked();//查找下一个

    void on_btCancel_clicked();//取消

private:
    Ui::SearchDialog *ui;
    MdiChild *pTextEdit;
    int s;
    int b;
};

#endif // SEARCHDIALOG_H
