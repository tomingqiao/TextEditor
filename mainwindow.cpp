#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mdichild.h"
#include <QMdiSubWindow>
#include <QFileDialog>
#include <QSignalMapper>
#include <QMessageBox>
#include <QSettings>
#include <QCloseEvent>
#include <QLabel>
#include <qtextcodec.h>
#include "aboutdialog.h"
#include "searchdialog.h"
#include "replacedialog.h"
#include <QFontDialog>
#include <QColorDialog>
#include <QDebug>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    actionSeparator = new QAction(this); // 创建间隔器动作
    actionSeparator->setSeparator(true); // 在其中设置间隔器
    updateMenus();   // 更新菜单
    connect(ui->mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow *)), this,
            SLOT(updateMenus()));  // 当有活动窗口时更新菜单

    windowMapper = new QSignalMapper(this); // 创建信号映射器
    connect(windowMapper, SIGNAL(mapped(QWidget *)), // 映射器重新发射信号
            this, SLOT(setActiveSubWindow(QWidget *))); // 设置活动窗口

    // 更新窗口菜单，并且设置当窗口菜单将要显示的时候更新窗口菜单
    updateWindowMenu();
    connect(ui->menuW, SIGNAL(aboutToShow()), this, SLOT(updateWindowMenu()));


    readSettings(); // 初始窗口时读取窗口设置信息
    initWindow(); // 初始化窗口

    ui->actionToolar->setChecked(true);
    ui->actionStatusBar->setChecked(true);


}



QTextCodec *codec = QTextCodec::codecForName("UTF-8");
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionNew_triggered()  // 新建文件菜单
{
    MdiChild *child = createMdiChild(); // 创建MdiChild
    child->newFile(); // 新建文件
    child->show(); // 显示子窗口
}

MdiChild *MainWindow::createMdiChild()  // 创建子窗口部件
{
    MdiChild *child = new MdiChild; // 创建MdiChild部件
    ui->mdiArea->addSubWindow(child); // 向多文档区域添加子窗口，child为中心部件

    // 根据QTextEdit类的是否可以复制信号设置剪切复制动作是否可用
    connect(child, SIGNAL(copyAvailable(bool)), ui->actionCut,
            SLOT(setEnabled(bool)));
    connect(child, SIGNAL(copyAvailable(bool)), ui->actionCopy,
            SLOT(setEnabled(bool)));

    // 根据QTextDocument类的是否可以撤销恢复信号设置撤销恢复动作是否可用
    connect(child->document(), SIGNAL(undoAvailable(bool)),
            ui->actionUndo, SLOT(setEnabled(bool)));
    connect(child->document(), SIGNAL(redoAvailable(bool)),
            ui->actionRedo, SLOT(setEnabled(bool)));

    // 每当编辑器中的光标位置改变，就重新显示行号和列号
    connect(child, SIGNAL(cursorPositionChanged()), this, SLOT(showTextRowAndCol()));

    return child;
}

void MainWindow::updateMenus() // 更新菜单
{
    bool hasMdiChild = (activeMdiChild() != 0); // 是否有活动窗口
    ui->actionSave->setEnabled(hasMdiChild);   // 设置各个动作是否可用
    ui->actionSaveAs->setEnabled(hasMdiChild);
    ui->actionPaste->setEnabled(hasMdiChild);
    ui->actionClose->setEnabled(hasMdiChild);
    ui->actionCloseAll->setEnabled(hasMdiChild);
    ui->actionTile->setEnabled(hasMdiChild);
    ui->actionCascade->setEnabled(hasMdiChild);
    ui->actionNext->setEnabled(hasMdiChild);
    ui->actionPrevious->setEnabled(hasMdiChild);
    ui->actionDisplayLine->setEnabled(hasMdiChild);
    ui->actionFind->setEnabled(hasMdiChild);
    ui->actionFont->setEnabled(hasMdiChild);
    ui->actionReplace->setEnabled(hasMdiChild);
    ui->action_AutoWrop->setEnabled(hasMdiChild);
    ui->actionFontColor->setEnabled(hasMdiChild);
    ui->actionFontBackgroundColor->setEnabled(hasMdiChild);


    actionSeparator->setVisible(hasMdiChild); // 设置间隔器是否显示

    bool hasSelection = (activeMdiChild()
                         && activeMdiChild()->textCursor().hasSelection());

    // 有活动窗口且有被选择的文本，剪切复制才可用
    ui->actionCut->setEnabled(hasSelection);
    ui->actionCopy->setEnabled(hasSelection);

    ui->actionUndo->setEnabled(activeMdiChild() // 有活动窗口且文档有撤销操作
                               && activeMdiChild()->document()->isUndoAvailable());
    ui->actionRedo->setEnabled(activeMdiChild() // 有活动窗口且文档有恢复操作
                               && activeMdiChild()->document()->isRedoAvailable());

    AutoWropDetection();
    DisplayLineDetection();

    if (activeMdiChild()) {
        QString filename = activeMdiChild()->userFriendlyCurrentFile();
        int iPos = filename.lastIndexOf(".");
        QString fileExtension = filename.mid(iPos);
        highlighter = new Highlighter(activeMdiChild()->document(), fileExtension);
    }

}

MdiChild *MainWindow::activeMdiChild()  // 活动窗口
{
    // 如果有活动窗口，则将其内的中心部件转换为MdiChild类型
    if (QMdiSubWindow *activeSubWindow = ui->mdiArea->activeSubWindow())
        return qobject_cast<MdiChild *>(activeSubWindow->widget());
    return 0; // 没有活动窗口，直接返回0
}

void MainWindow::on_actionOpen_triggered() // 打开文件菜单
{
    QString fileName = QFileDialog::getOpenFileName(this); // 获取文件路径
    if (!fileName.isEmpty()) { // 如果路径不为空，则查看该文件是否已经打开
        QMdiSubWindow *existing = findMdiChild(fileName);
        if (existing) { // 如果已经存在，则将对应的子窗口设置为活动窗口
            ui->mdiArea->setActiveSubWindow(existing);
            return;
        }

        MdiChild *child = createMdiChild(); // 如果没有打开，则新建子窗口
        if (child->loadFile(fileName)) {
            ui->statusBar->showMessage(codec->toUnicode("打开文件成功"), 2000);
            child->show();
        } else {
            child->close();
        }
    }
}

QMdiSubWindow *MainWindow::findMdiChild(const QString &fileName)  // 查找子窗口
{

    QString canonicalFilePath = QFileInfo(fileName).canonicalFilePath();

    // 利用foreach语句遍历子窗口列表，如果其文件路径和要查找的路径相同，则返回该窗口
    foreach (QMdiSubWindow *window, ui->mdiArea->subWindowList()) {
        MdiChild *mdiChild = qobject_cast<MdiChild *>(window->widget());
        if (mdiChild->currentFile() == canonicalFilePath)
            return window;
    }
    return 0;
}

void MainWindow::setActiveSubWindow(QWidget *window) // 设置活动子窗口
{
    if (!window) // 如果传递了窗口部件，则将其设置为活动窗口
        return;
    ui->mdiArea->setActiveSubWindow(qobject_cast<QMdiSubWindow *>(window));
}

void MainWindow::updateWindowMenu() // 更新窗口菜单
{
    ui->menuW->clear(); // 先清空菜单，然后再添加各个菜单动作
    ui->menuW->addAction(ui->actionClose);
    ui->menuW->addAction(ui->actionCloseAll);
    ui->menuW->addSeparator();
    ui->menuW->addAction(ui->actionTile);
    ui->menuW->addAction(ui->actionCascade);
    ui->menuW->addSeparator();
    ui->menuW->addAction(ui->actionNext);
    ui->menuW->addAction(ui->actionPrevious);
    ui->menuW->addAction(actionSeparator);

    QList<QMdiSubWindow *> windows = ui->mdiArea->subWindowList();
    actionSeparator->setVisible(!windows.isEmpty());
    // 如果有活动窗口，则显示间隔器
    for (int i = 0; i < windows.size(); ++i) { // 遍历各个子窗口
        MdiChild *child = qobject_cast<MdiChild *>(windows.at(i)->widget());

        QString text;
        if (i < 9) { // 如果窗口数小于9，则设置编号为快捷键
            text = tr("&%1 %2").arg(i + 1)
                   .arg(child->userFriendlyCurrentFile());
        } else {
            text = tr("%1 %2").arg(i + 1)
                   .arg(child->userFriendlyCurrentFile());
        }
        QAction *action  = ui->menuW->addAction(text); // 添加动作到菜单
        action->setCheckable(true); // 设置动作可以选择

        // 设置当前活动窗口动作为选中状态
        action ->setChecked(child == activeMdiChild());

        // 关联动作的触发信号到信号映射器的map()槽函数上，这个函数会发射mapped()信号
        connect(action, SIGNAL(triggered()), windowMapper, SLOT(map()));

        // 将动作与相应的窗口部件进行映射，在发射mapped()信号时就会以这个窗口部件为参数
        windowMapper->setMapping(action, windows.at(i));
    }
}

/********************1.3.3小节对下面这些代码进行了部分省略****************************************/

void MainWindow::on_actionSave_triggered() // 保存菜单
{
    if (activeMdiChild() && activeMdiChild()->save())
        ui->statusBar->showMessage(codec->toUnicode("文件保存成功"), 2000);
}

void MainWindow::on_actionSaveAs_triggered()  // 另存为菜单
{
    if (activeMdiChild() && activeMdiChild()->saveAs())
        ui->statusBar->showMessage(codec->toUnicode("文件保存成功"), 2000);
}

void MainWindow::on_actionUndo_triggered() // 撤销菜单
{
    if (activeMdiChild()) activeMdiChild()->undo();
}

void MainWindow::on_actionRedo_triggered() // 恢复菜单
{
    if (activeMdiChild()) activeMdiChild()->redo();
}

void MainWindow::on_actionCut_triggered() // 剪切菜单
{
    if (activeMdiChild()) activeMdiChild()->cut();
}

void MainWindow::on_actionCopy_triggered() // 复制菜单
{
    if (activeMdiChild()) activeMdiChild()->copy();
}

void MainWindow::on_actionPaste_triggered() // 粘贴菜单
{
    if (activeMdiChild()) activeMdiChild()->paste();
}

void MainWindow::on_actionClose_triggered() // 关闭菜单
{
    ui->mdiArea->closeActiveSubWindow();
}

void MainWindow::on_actionCloseAll_triggered() // 关闭所有窗口菜单
{
    ui->mdiArea->closeAllSubWindows();
}

void MainWindow::on_actionTile_triggered() // 平铺菜单
{
    ui->mdiArea->tileSubWindows();
}

void MainWindow::on_actionCascade_triggered() // 层叠菜单
{
    ui->mdiArea->cascadeSubWindows();
}

void MainWindow::on_actionNext_triggered() // 下一个菜单
{
    ui->mdiArea->activateNextSubWindow();
}

void MainWindow::on_actionPrevious_triggered() // 前一个菜单
{
    ui->mdiArea->activatePreviousSubWindow();
}

void MainWindow::on_actionAbout_triggered() // 关于菜单
{
    AboutDialog dlg;
    dlg.exec();
}

void MainWindow::on_actionAboutQt_triggered() // 关于Qt菜单
{
    qApp->aboutQt(); // 这里的qApp是QApplication对象的全局指针，
    // 这行代码相当于QApplication::aboutQt();
}



void MainWindow::on_actionExit_triggered() // 退出菜单
{
    qApp->closeAllWindows(); // 等价于QApplication::closeAllWindows();
}

void MainWindow::closeEvent(QCloseEvent *event) // 关闭事件
{
    ui->mdiArea->closeAllSubWindows(); // 先执行多文档区域的关闭操作
    if (ui->mdiArea->currentSubWindow()) {
        event->ignore(); // 如果还有窗口没有关闭，则忽略该事件
    } else {
        writeSettings(); // 在关闭前写入窗口设置
        event->accept();
    }
}

void MainWindow::writeSettings() // 写入窗口设置
{
    QSettings settings("yafeilinux", "myMdi");
    settings.setValue("pos", pos());   // 写入位置信息
    settings.setValue("size", size()); // 写入大小信息
}

void MainWindow::readSettings() // 读取窗口设置
{
    QSettings settings("yafeilinux", "myMdi");
    QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
    QSize size = settings.value("size", QSize(400, 400)).toSize();
    move(pos);
    resize(size);
}

void MainWindow::showTextRowAndCol() // 显示文本的行号和列号
{
    // 如果有活动窗口，则显示其中光标所在的位置
    if (activeMdiChild()) {

        // 因为获取的行号和列号都是从0开始的，所以我们这里进行了加1
        int rowNum = activeMdiChild()->textCursor().blockNumber() + 1;
        int colNum = activeMdiChild()->textCursor().columnNumber() + 1;

        ui->statusBar->showMessage(tr("%1行,%2列 | 文本长度：%3,文本行数：%4")
                                   .arg(rowNum).arg(colNum)
                                   .arg(activeMdiChild()->toPlainText().length())
                                   .arg(activeMdiChild()->document()->lineCount()), 2000);
    }
}

void MainWindow::initWindow() // 初始化窗口
{
    setWindowTitle(codec->toUnicode("筱忆多文档编辑器"));

    // 我们在工具栏上单击鼠标右键时，可以关闭工具栏
    ui->mainToolBar->setWindowTitle(codec->toUnicode("工具栏"));

    // 当多文档区域的内容超出可视区域后，出现滚动条
    ui->mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    ui->statusBar->showMessage(codec->toUnicode("欢迎使用筱忆多文档编辑器"));

    QLabel *label = new QLabel(this);
    label->setFrameStyle(QFrame::Box | QFrame::Sunken);
    label->setText(
        tr("<a href=\"https://blog.csdn.net/qq_46424406?spm=1003.2020.3001.5343\">xiaoyi.com</a>"));
    label->setTextFormat(Qt::RichText); // 标签文本为富文本
    label->setOpenExternalLinks(true);  // 可以打开外部链接
    ui->statusBar->addPermanentWidget(label);

    ui->actionNew->setStatusTip(codec->toUnicode("创建一个文件"));

    ui->actionOpen->setStatusTip(codec->toUnicode("打开一个已经存在的文件"));
    ui->actionSave->setStatusTip(codec->toUnicode("保存文档到硬盘"));
    ui->actionSaveAs->setStatusTip(codec->toUnicode("以新的名称保存文档"));
    ui->actionExit->setStatusTip(codec->toUnicode("退出应用程序"));
    ui->actionUndo->setStatusTip(codec->toUnicode("撤销先前的操作"));
    ui->actionRedo->setStatusTip(codec->toUnicode("恢复先前的操作"));
    ui->actionCut->setStatusTip(codec->toUnicode("剪切选中的内容到剪贴板"));
    ui->actionCopy->setStatusTip(codec->toUnicode("复制选中的内容到剪贴板"));
    ui->actionPaste->setStatusTip(codec->toUnicode("粘贴剪贴板的内容到当前位置"));
    ui->actionClose->setStatusTip(codec->toUnicode("关闭活动窗口"));
    ui->actionCloseAll->setStatusTip(codec->toUnicode("关闭所有窗口"));
    ui->actionTile->setStatusTip(codec->toUnicode("平铺所有窗口"));
    ui->actionCascade->setStatusTip(codec->toUnicode("层叠所有窗口"));
    ui->actionNext->setStatusTip(codec->toUnicode("将焦点移动到下一个窗口"));
    ui->actionPrevious->setStatusTip(codec->toUnicode("将焦点移动到前一个窗口"));
    ui->actionAbout->setStatusTip(codec->toUnicode("显示本软件的介绍"));
    ui->actionFind->setStatusTip(codec->toUnicode("查找"));
    ui->actionReplace->setStatusTip(codec->toUnicode("替换"));
}


void MainWindow::on_actionFind_triggered()
{
    if (activeMdiChild()) {
        SearchDialog dlg(this, activeMdiChild());
        dlg.exec();
    }
}


void MainWindow::on_actionReplace_triggered()
{
    if (activeMdiChild()) {
        ReplaceDialog dlg(this, activeMdiChild());
        dlg.exec();
    }
}


void MainWindow::on_actionFont_triggered()
{
    if (activeMdiChild()) {
        bool ok = false;
        QFont font = QFontDialog::getFont(&ok, this);
        if (ok) {
            activeMdiChild()->setFont(font);
        }
    }
}


void MainWindow::on_actionFontColor_triggered()
{
    if (activeMdiChild()) {
        QColor color = QColorDialog::getColor(Qt::black, this, "选择颜色");
        if (color.isValid()) {
            activeMdiChild()->setStyleSheet(QString("MdiChild {color: %1}").arg(color.name()));
        }
    }
}


void MainWindow::on_actionFontBackgroundColor_triggered()
{
    if (activeMdiChild()) {
        QColor color = QColorDialog::getColor(Qt::black, this, "选择颜色");
        if (color.isValid()) {
            activeMdiChild()->setStyleSheet(QString("MdiChild {background-color: %1}").arg(color.name()));
        }
    }
}


void MainWindow::on_actionToolar_triggered()
{
    bool visible = ui->mainToolBar->isVisible();
    ui->mainToolBar->setVisible(!visible);
    ui->actionToolar->setChecked(!visible);
}


void MainWindow::on_actionStatusBar_triggered()
{
    bool visible = ui->statusBar->isVisible();
    ui->statusBar->setVisible(!visible);
    ui->actionStatusBar->setChecked(!visible);
}

void MainWindow::on_action_AutoWrop_triggered(bool checked)
{
    if (checked) {

    }
}

void MainWindow::on_action_AutoWrop_triggered()
{
    if (activeMdiChild()) {
        QPlainTextEdit::LineWrapMode mode = activeMdiChild()->lineWrapMode();
        if (mode == QPlainTextEdit::NoWrap) {
            activeMdiChild()->setLineWrapMode(QPlainTextEdit::WidgetWidth);
            ui->action_AutoWrop->setChecked(true);
        } else {
            activeMdiChild()->setLineWrapMode(QPlainTextEdit::NoWrap);
            ui->action_AutoWrop->setChecked(false);
        }
    }
}

void MainWindow::AutoWropDetection()
{
    if (activeMdiChild()) {
        QPlainTextEdit::LineWrapMode mode = activeMdiChild()->lineWrapMode();
        if (mode == QPlainTextEdit::WidgetWidth) {
            ui->action_AutoWrop->setChecked(true);
        } else {
            ui->action_AutoWrop->setChecked(false);
        }
    }
}


void MainWindow::on_actionDisplayLine_triggered(bool checked)
{
    if (activeMdiChild()) {
        activeMdiChild()->hideLineNumberArea(checked);
    }
}

void MainWindow::DisplayLineDetection()
{
    if (activeMdiChild()) {
        if (activeMdiChild()->hideLineNumberAreaShow) {
            ui->actionDisplayLine->setChecked(true);
        } else {
            ui->actionDisplayLine->setChecked(false);
        }
    }
}

