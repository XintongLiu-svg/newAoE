#include "Human.h"

// 人类单位默认构造函数
Human::Human()
{
}

// 人类单位构造函数
// 参数说明：
// Num: 单位类型编号
// DR, UR: 详细坐标（下右、上右）
// playerScience: 玩家科技系统指针
// playerRepresent: 玩家标识
Human::Human(int Num, double DR, double UR,Development* playerScience, int playerRepresent)
{
    // 初始化玩家科技和玩家标识
    this->playerScience = playerScience;
    this->playerRepresent = playerRepresent;
    
    // 设置单位基本属性
    this->Num=Num;  // 单位类型编号
    this->DR=DR;    // 下右坐标
    this->UR=UR;    // 上右坐标

    speed = HUMAN_SPEED;  // 设置移动速度
}

// 更新帧：处理单位动画和状态
void Human::nextframe()
{
}

// 获取防御力
// 参数说明：
// attackType_got: 受到的攻击类型（近战/远程/动物）
// 返回值：计算后的防御力
int Human::getDEF(int attackType_got)
{
    int def = 0;  // 基础防御力

    // 根据攻击类型选择对应的防御属性
    if( attackType_got == ATTACKTYPE_CLOSE||attackType_got == ATTACKTYPE_ANIMAL ) 
        def = defence_close;  // 近战/动物攻击使用近战防御
    else if(attackType_got == ATTACKTYPE_SHOOT ) 
        def = defence_shoot;  // 远程攻击使用远程防御

    // 计算最终防御力：
    // 1. 基础防御力 * 科技防御加成系数
    // 2. 加上科技提供的额外防御力
    return (int)(def * playerScience->get_rate_Defence(getSort(),Num,ARMY_INFANTRY , attackType_got) ) +\
            playerScience->get_addition_Defence(getSort() , Num , ARMY_INFANTRY , attackType_got);
}
