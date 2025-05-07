#include "Bloodhaver.h" 

/**
 * 获取防御值 - 根据攻击类型返回相应的防御值
 */
int BloodHaver::getDEF(int attackType_got)
{
    // 根据不同的攻击类型返回相应的防御值
    if(attackType_got == ATTACKTYPE_CLOSE)  // 近战攻击
        return defence_close;  // 返回近战防御值
    else if(attackType_got == ATTACKTYPE_SHOOT)  // 远程攻击
        return defence_shoot;  // 返回远程防御值
    else  
        return 0;  
}