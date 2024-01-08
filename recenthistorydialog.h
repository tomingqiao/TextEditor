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
    void updateList();
    void setMainRecentFileList(QStringList *mrfl);
    void doOn_buttonConfirm_clicked();

private slots:
    void on_buttonConfirm_clicked();

    void on_buttonClose_clicked();

    void on_buttonDeleteAll_clicked();

    void on_buttonDelete_clicked();

private:
    Ui::RecentHistoryDialog *ui;
};

#endif // RECENTHISTORYDIALOG_H
