#include "recenthistorydialog.h"
#include "ui_recenthistorydialog.h"
#include <QFile>
#include <QtDebug>
#include <QListWidgetItem>
#include "mdichild.h"
#include "mainwindow.h"

RecentHistoryDialog::RecentHistoryDialog(QWidget *parent, QStringList *mrfl) :
    QDialog(parent),
    ui(new Ui::RecentHistoryDialog)
{
    ui->setupUi(this);
    setMainRecentFileList(mrfl);//得到主窗口历史列表
    QFile file("recentFileList.txt");//打开配置文件
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "打开失败！";
        doOn_buttonConfirm_clicked();
    } else {
        if (!file.atEnd()) {//获得保存最近文件列表配置
            QByteArray line = file.readLine();
            QString str(line);
            str.remove("\n");
            if (str == "1") {
                ui->checkBoxSaveFileList->setChecked(true);
            } else if (str == "0")
                ui->checkBoxSaveFileList->setChecked(false);
        }
        if (!file.atEnd()) {//获得启动时打开最后文件配置
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
            QByteArray line = file.readLine();//获得最后打开文件
            QString str(line);
            str.remove("\n");
            firstFileName = str;

        }
        while (!file.atEnd()) {//获得最近打开文件历史列表
            QByteArray line = file.readLine();
            QString str(line);
            str.remove("\n");
            if (!str.isEmpty()) {
                recentFileList.append(str);
            }

        }
    }
    if (!mainRecentFileList->isEmpty()) {//将主窗口最近历史列表添加到子窗口列表
        for (int i = mainRecentFileList->length() - 1; i >= 0 ; i-- ) {
            while (recentFileList.contains(mainRecentFileList->at(i))) {
                recentFileList.removeOne(mainRecentFileList->at(i));
            }
            recentFileList.prepend(mainRecentFileList->at(i));
        }
    }
    *mainRecentFileList = recentFileList;//添加后更新主窗口列表
    if (!recentFileList.isEmpty()) {
        firstFileName = recentFileList.at(0);//设置最后打开文件
    }

    updateList();//更新listwidget
}

RecentHistoryDialog::~RecentHistoryDialog()
{
    delete ui;
}

void RecentHistoryDialog::updateList()//更新listwidget
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

void RecentHistoryDialog::setMainRecentFileList(QStringList *mrfl)//得到主窗口历史列表
{
    mainRecentFileList = mrfl;
}

void RecentHistoryDialog::doOn_buttonConfirm_clicked()//执行确定按钮
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
    if (ui->checkBoxSaveFileList->isChecked()) {//配置相关配置
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
    QFile file("recentFileList.txt");//打开该文件进入编辑模式
    if (file.open(QIODevice::WriteOnly)) { //如果被打开
        file.resize("tate.txt", 0); //清空内容

        QTextStream stream( &file );//开始写入文本
        stream << sfl << "\r\n" << lff << "\r\n" << firstFileName <<
               "\r\n";//将相关配置写入配置文件
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
    QListWidgetItem *selectedItem =
        ui->listWidgetRecentFile->currentItem();//如果有选择文件名，创建子窗口
    if (selectedItem) {
        MainWindow *mainWindow = (MainWindow *)parentWidget();
        QString text = selectedItem->text();
        mainWindow->createMdiChildByFileName(text);
    } else {

    }
    close();
}


void RecentHistoryDialog::on_buttonClose_clicked()
{
    close();
}


void RecentHistoryDialog::on_buttonDeleteAll_clicked()
{
    recentFileList.clear();//清空历史列表
    updateList();
}


void RecentHistoryDialog::on_buttonDelete_clicked()
{

    QListWidgetItem *selectedItem =
        ui->listWidgetRecentFile->currentItem();//如果有选择文件名，删除相关项目
    if (selectedItem) {
        QString text = selectedItem->text();
        recentFileList.removeOne(text);
    } else {

    }
    updateList();
}

