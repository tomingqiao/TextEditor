#ifndef RECENTHISTORYDIALOG_H
#define RECENTHISTORYDIALOG_H

#include <QDialog>

namespace Ui {
class RecentHistoryDialog;
}

class RecentHistoryDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RecentHistoryDialog(QWidget *parent = nullptr, QStringList *mrfl = nullptr);
    ~RecentHistoryDialog();

    QStringList recentFileList;
    QStringList *mainRecentFileList;
    int loadFinalFile;
    QString firstFileName;
    void updateList();//更新listWidget
    void setMainRecentFileList(QStringList *mrfl);//获得主窗口历史列表
    void doOn_buttonConfirm_clicked();//执行确定按钮

private slots:
    void on_buttonConfirm_clicked();

    void on_buttonClose_clicked();

    void on_buttonDeleteAll_clicked();

    void on_buttonDelete_clicked();

private:
    Ui::RecentHistoryDialog *ui;
};

#endif // RECENTHISTORYDIALOG_H
