#include "AboutDialog.h"
#include "ui_AboutDialog.h"

/**
 * @brief AboutDialog构造函数 - 初始化"关于游戏"对话框
 * @param parent 父窗口指针
 * 
 * 设置UI界面、固定窗口大小、加载背景图片
 * 设置窗口标题和显示项目名称及版本信息
 */
AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    setFixedSize(600,500);  // 固定对话框大小600x500像素
    pix = QPixmap(":/new/prefix1/res/About_Background_001.png");  // 加载背景图片

    setWindowTitle("关于游戏");  // 设置窗口标题
    ui->nameLabel->setText("项目名称： newAOE");  // 设置项目名称标签
    ui->versionLabel->setText("当前版本： " + QString::fromStdString(GAME_VERSION));  // 显示游戏版本号
}

/**
 * @brief AboutDialog析构函数 - 清理资源
 * 
 * 释放UI指针，防止内存泄漏
 */
AboutDialog::~AboutDialog()
{
    delete ui;
//    delete pix;  //QPixmap由Qt自动管理，不需要手动删除
}

/**
 * @brief 重写绘图事件处理函数
 * @param event 绘图事件
 * 
 * 在对话框上绘制背景图片，覆盖整个对话框区域
 */
void AboutDialog::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(0, 0, 600, 500, pix);  // 在坐标(0,0)处绘制大小为600x500的背景图片
}