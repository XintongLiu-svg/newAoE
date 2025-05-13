#include "ActWidget.h"

/**
 * @brief ActWidget构造函数
 * @param parent 父窗口指针
 * 
 * 创建一个动作按钮部件，显示游戏UI中的各种操作按钮
 */
ActWidget::ActWidget(QWidget *parent) : QWidget(parent)
{

}

/*
 * 为该按钮部件设置对应的动作标识符，用于标识按钮功能类型
 */
void ActWidget::setActName(int actName)
{
    this->actName = actName;
}

/*
 * 为按钮设置要显示的图像
 */
void ActWidget::setPix(QPixmap pix)
{
    this->pix = pix;
}

/*
 * 设置按钮的唯一标识符，用于在点击事件中识别是哪个按钮被按下
 */
void ActWidget::setNum(int num)
{
    this->num = num;
}

/*
 * 设置按钮的当前阶段，可能用于控制动画或状态变化
 */
void ActWidget::setPhase(int phase)
{
    this->phase = phase;
}

/*
 * 设置按钮的显示状态，影响按钮的视觉效果和交互行为
 */
void ActWidget::setStatus(int status)
{
    this->status = status;
}

/*
 * @brief 获取动作名称
 */
int ActWidget::getActName()
{
    return actName;
}

/*
 * @return 返回按钮当前显示的图像
 */
QPixmap ActWidget::getPix()
{
    return pix;
}

/*
 * @return 返回按钮的唯一标识符
 */
int ActWidget::getNum()
{
    return num;
}

/*
 * @return 返回按钮当前的阶段值
 */
int ActWidget::getPhase()
{
    return phase;
}

/**
 * @brief 获取按钮状态
 * @return 返回按钮当前的状态值
 */
int ActWidget::getStatus()
{
    return status;
}

/*
 * - status=0: 普通状态，显示正常图标
 * - status=1: 按下状态，显示凹陷效果的图标
 * - status=2: 禁用状态，显示灰色图标
 */
void ActWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    if(status == 0)//普通状态
    {
        painter.drawPixmap(0,0,80,80,pix);
        if(this->num < ACT_WINDOW_NUM_FREE)
        {
            painter.fillRect(QRect(0, 0, 4, 80), QBrush(Qt::white));  // 左边白色边框
            painter.fillRect(QRect(4, 0, 76, 4), QBrush(Qt::gray));   // 上方灰色边框
            painter.fillRect(QRect(2, 0, 2, 2), QBrush(Qt::gray));    // 左上角灰色小方块
        }
        this->show();
    }
    else if(status == 1)//凹效果
    {
        if(this->num < ACT_WINDOW_NUM_FREE)
        {
            painter.drawPixmap(4,4,76,76,pix);  // 图像向右下偏移4像素产生凹陷效果
            painter.fillRect(QRect(0, 0, 4, 80), QBrush(Qt::white));  // 左边白色边框
            painter.fillRect(QRect(4, 0, 76, 4), QBrush(Qt::gray));   // 上方灰色边框
            painter.fillRect(QRect(2, 0, 2, 2), QBrush(Qt::gray));    // 左上角灰色小方块
        }
        else
        {
            painter.drawPixmap(0,0,120,120,pix);  // 特殊按钮使用更大尺寸
        }
        this->show();
    }
    else if(status == 2)//灰色图标
    {
        if(!pix.isNull())
        {
            QImage img = pix.toImage().scaled(80, 80);
            for(int i = 0; i < 80; i++)
            {
                for(int j = 0; j < 80; j++)
                {
                    QColor clr = img.pixelColor(i, j);
                    // 标准灰度转换公式: Gray = R*0.3 + G*0.59 + B*0.11
                    int gray = clr.red() * 0.3 + clr.green() * 0.59 + clr.blue() * 0.11;
                    img.setPixelColor(i, j, QColor(gray, gray, gray));
                }
            }
            pix = QPixmap::fromImage(img);
            painter.drawPixmap(0,0,80,80,pix);
        }
    }
}

/*
 * 释放鼠标按钮时，发出actPress信号通知监听者此按钮被点击
 * 信号包含按钮的编号，以便识别是哪个按钮触发了事件
 */
void ActWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if(this->isVisible())
    {
        emit actPress(num);  // 发送按钮点击信号，传递按钮编号
        this->update();      // 更新按钮显示
    }
}

/*
 * 当用户按下鼠标按钮时更新按钮显示，通常用于视觉反馈
 */
void ActWidget::mousePressEvent(QMouseEvent *event){
    this->update();  // 更新按钮显示以提供视觉反馈
}