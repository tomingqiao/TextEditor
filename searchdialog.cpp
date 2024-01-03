#include "searchdialog.h"
#include "ui_searchdialog.h"
#include <QMessageBox>
#include "mdichild.h"

SearchDialog::SearchDialog(QWidget *parent, MdiChild *textEdit) :
    QDialog(parent),
    ui(new Ui::SearchDialog)
{
    ui->setupUi(this);
    pTextEdit = textEdit;

    ui->rbDown->setChecked(true);
    s = 0;
    b = 1;
    QTextCursor c = pTextEdit->textCursor();
    c.setPosition(0);
    pTextEdit->setTextCursor(c);
}

SearchDialog::~SearchDialog()
{
    delete ui;
}

void SearchDialog::on_btFindNext_clicked()
{
    QString target = ui->searchText->text();
    QString text = pTextEdit->toPlainText();
    QTextCursor c = pTextEdit->textCursor();
    int index = -1;
    if (text == "") {
        return;
    }

    if (ui->rbDown->isChecked()) {

        if (s == 1) {
            index = text.indexOf(target, c.position(),
                                 ui->cbCaseSensitive->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive);
            if (index >= 0) {
                c.setPosition(index);
                c.setPosition(index + target.length(), QTextCursor::KeepAnchor);
                pTextEdit->setTextCursor(c);
            }
            s--;
            b ++;
        }
        index = text.indexOf(target, c.position(),
                             ui->cbCaseSensitive->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive);
        if (index >= 0) {
            c.setPosition(index);
            c.setPosition(index + target.length(), QTextCursor::KeepAnchor);
            pTextEdit->setTextCursor(c);
        }
    } else if (ui->rbUp->isChecked()) {

        if (b == 1) {
            index = text.lastIndexOf(target, c.position() - text.length() - 1,
                                     ui->cbCaseSensitive->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive);
            if (index >= 0) {
                c.setPosition(index + target.length());
                c.setPosition(index, QTextCursor::KeepAnchor);
                pTextEdit->setTextCursor(c);
            }
            b--;
            s++;
        }
        index = text.lastIndexOf(target, c.position() - text.length() - 1,
                                 ui->cbCaseSensitive->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive);
        if (index >= 0) {
            c.setPosition(index + target.length());
            c.setPosition(index, QTextCursor::KeepAnchor);
            pTextEdit->setTextCursor(c);
        }
    }
    if (index == -1) {
        QMessageBox msg(this);
        msg.setWindowTitle("问题");
        msg.setText(QString("找不到 ") + target);
        msg.setStandardButtons(QMessageBox::Ok);
        msg.exec();
    }
}


void SearchDialog::on_btCancel_clicked()
{
    accept();
}

