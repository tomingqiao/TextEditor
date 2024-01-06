#include "recenthistorydialog.h"
#include "ui_recenthistorydialog.h"
#include <QFile>
#include <QtDebug>
#include <QListWidgetItem>

RecentHistoryDialog::RecentHistoryDialog(QWidget *parent, QStringList *mrfl) :
    QDialog(parent),
    ui(new Ui::RecentHistoryDialog)
{
    ui->setupUi(this);
    setMainRecentFileList(mrfl);
    QFile file("E:\\qt\\project\\mynotepad\\recentFileList.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "打开失败！";
    }

    if (!file.atEnd()) {
        QByteArray line = file.readLine();
        QString str(line);
        str.remove("\n");
        if (str == "1") {
            ui->checkBoxSaveFileList->setChecked(true);
        } else if (str == "0")
            ui->checkBoxSaveFileList->setChecked(false);
    }
    if (!file.atEnd()) {
        QByteArray line = file.readLine();
        QString str(line);
        str.remove("\n");
        if (str == "1") {
            ui->checkBoxLoadFinalFile->setChecked(true);
            loadFinalFile = 1;
        } else if (str == "0") {
            ui->checkBoxLoadFinalFile->setChecked(false);
            loadFinalFile = 0;
        }

    }
    if (!file.atEnd()) {
        QByteArray line = file.readLine();
        QString str(line);
        str.remove("\n");
        firstFileName = str;

    }
    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        QString str(line);
        str.remove("\n");
        if (!str.isEmpty()) {
            recentFileList.append(str);
        }

    }
    if (!mainRecentFileList->isEmpty()) {
        for (int i = mainRecentFileList->length() - 1; i >= 0 ; i-- ) {
            if (recentFileList.contains(mainRecentFileList->at(i))) {
                recentFileList.removeOne(mainRecentFileList->at(i));
            }
            recentFileList.prepend(mainRecentFileList->at(i));
        }
    }
    *mainRecentFileList = recentFileList;
    if (!recentFileList.isEmpty()) {
        firstFileName = recentFileList.at(0);
    }

    updateList();
}

RecentHistoryDialog::~RecentHistoryDialog()
{
    delete ui;
}

void RecentHistoryDialog::updateList()
{
    ui->listWidgetRecentFile->clear();
    for (int i = 0; i < recentFileList.size(); ++i) {
        QString str = recentFileList.at(i);
        QListWidgetItem *item = new QListWidgetItem;
        item->setText(str);
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        ui->listWidgetRecentFile->addItem(item);
    }
}

void RecentHistoryDialog::setMainRecentFileList(QStringList *mrfl)
{
    mainRecentFileList = mrfl;
}

void RecentHistoryDialog::doOn_buttonConfirm_clicked()
{
    on_buttonConfirm_clicked();
}

void RecentHistoryDialog::on_buttonConfirm_clicked()
{
    *mainRecentFileList = recentFileList;
    QStringList lines;
    QString line;
    int row = 0;
    QString sfl;
    QString lff;
    if (ui->checkBoxSaveFileList->isChecked()) {
        sfl = "1";
    } else {
        sfl = "0";
    }
    if (ui->checkBoxLoadFinalFile->isChecked()) {
        lff = "1";
        if (!recentFileList.isEmpty()) {
            firstFileName = recentFileList.at(0);
        }

    } else {
        lff = "0";
        firstFileName = "0";
    }
    QFile file("E:\\qt\\project\\mynotepad\\recentFileList.txt");//打开该文件进入编辑模式
    if (file.open(QIODevice::WriteOnly)) { //如果被打开
        file.resize("tate.txt", 0); //清空内容

        QTextStream stream( &file );//开始写入文本
        stream << sfl << "\r\n" << lff << "\r\n" << firstFileName << "\r\n";
        if (ui->checkBoxSaveFileList->isChecked()) {
            while (row <
                    (ui->listWidgetRecentFile->count())) { //从wistlidget里一行一行的读取文本并存入lines里边
                line = ui->listWidgetRecentFile->item(row)->text();
                row++;
                lines << line;
            }
            for ( QStringList::Iterator it = lines.begin(); it != lines.end(); ++it )
                stream << *it <<
                       "\r\n"; //注意是"\r\n"！！！！！！如果是单个"\n"  保存文本的时候它不保留换行符！！！！
        }

        file.close();
    }
    close();
}


void RecentHistoryDialog::on_buttonClose_clicked()
{
    close();
}


void RecentHistoryDialog::on_buttonDeleteAll_clicked()
{
    recentFileList.clear();
    updateList();
}


void RecentHistoryDialog::on_buttonDelete_clicked()
{

    QListWidgetItem *selectedItem = ui->listWidgetRecentFile->currentItem();
    if (selectedItem) {
        QString text = selectedItem->text();
        recentFileList.removeOne(text);
    } else {

    }
    updateList();
}

