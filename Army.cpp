#include "Army.h"

//军队单位动画资源 [阵营代表][兵种类型][等级][角度]
std::list<ImageResource>* Army::Walk[2][7][2][8];      // 行走
std::list<ImageResource>* Army::Disappear[2][7][2][8]; // 消失
std::list<ImageResource>* Army::Stand[2][7][2][8];     // 站立
std::list<ImageResource>* Army::Attack[2][7][2][8];    // 攻击
std::list<ImageResource>* Army::Die[2][7][2][8];       // 死亡

// 军队类型名称[兵种类型][等级]
std::string Army::ArmyName[7][2]={{"Clubman","Axeman"},
                                  {"Slinger","Slinger"},
                                  {"Archer","Archer"},
                                  {"Scout","Scout"},
                                  {"Sworder","Sworder"},
                                  {"ImprovedArcher","ImprovedArcher"},
                                  {"Cavalry","Cavalry"}
                                 };

// 军队显示[兵种类型][等级]
std::string Army::ArmyDisplayName[7][2]={{"棍棒兵","刀斧兵"},
                                         {"投石兵","投石兵"},
                                         {"弓箭手","弓箭手"},
                                         {"侦察骑兵","侦察骑兵"},
                                         {"Prof.Yan","Prof.Yan"},
                                         {"Prof.Lou","Prof.Lou"},
                                         {"Prof.Lu","Prof.Lu"}
                                        };

// 点击音效
string Army::click_sound = "Click_Army";
Army::Army()
{

}

// - DR,UR: 初始位置坐标
// - Num: 兵种类型
// - playerScience: 玩家科技树指针(用于确定单位属性加成)
// - playerRepresent: 玩家阵营标识
Army::Army(double DR,double UR,int Num , Development* playerScience, int playerRepresent)
{
    //设置科技树和阵营
    this->playerScience = playerScience;
    this->playerRepresent = playerRepresent;

    this->Num = Num;

    setAttribute(); // 设置属性

    setDRUR(DR, UR); // 设置初始位置
    updateBlockByDetail(); // 更新地图块

    setSideLenth(); // 设置碰撞盒尺寸
    this->nextBlockDR=BlockDR;
    this->nextBlockUR=BlockUR;
    setPredictedDRUR(DR, UR); // 预测位置
    setPreviousDRUR(DR, UR); // 上一帧位置
    setDR0UR0(DR, UR); // 目标位置

    this->nowstate=MOVEOBJECT_STATE_STAND; // 初始状态站立
    isAttackable = true; // 可以攻击

    setNowRes(); // 设置资源
    updateImageXYByNowRes(); // 更新图像位置
    this->imageH=DR-UR;

    //SN信息
    this->globalNum=10000*getSort()+g_globalNum;
    g_Object.insert({this->globalNum,this}); // 添加到全局对象表
    g_globalNum++;
}

// 处理死亡、攻击、移动等各种状态的动画帧更新
void Army::nextframe()
{
    if(isDie()) // 死亡
    {
        if( !isDying() ) // 未开始死亡
        {
             setPreDie();
             requestSound_Die(); // 死亡音效
        }
        else if(!get_isActionEnd() && isNowresShift()) // 死亡动画未结束
        {
            nowres++; // 播放下一帧
            if( !changeToDisappear && get_isActionEnd()) // 动画结束但还未开始消失动画
            {
                changeToDisappear = true; 
                nowres_step = 1000; // 帧间隔
                setNowRes(); // 更新资源
            }
        }
    }
    else 
    {
        if(isNowresShift()) //切换动画帧
        {
            if(nowres == nowlist->begin()) // 如果是动画序列的第一帧
            {
                if(nowstate == MOVEOBJECT_STATE_ATTACK) // 攻击状态
                    requestSound_Attack(); // 播放攻击音效
                else if(nowobject == this && nowstate == MOVEOBJECT_STATE_WALK && playerRepresent == NOWPLAYERREPRESENT) // 如果是当前玩家的移动单位
                    requestSound_Walk(); // 行走音效
            }

            nowres++; // 下一帧
            if(nowres==nowlist->end()) // 到达动画序列末尾
            {
                nowres=nowlist->begin(); // 循环第一帧
                initAttack_perCircle(); // 重置攻击循环参数
            }
        }

        updateMove(); 
        setNowRes(); 
    }

    if(playerRepresent != 0 && timer_Visible>0) time_BeVisible(); //计时器

    updateImageXYByNowRes();
}

// 根据当前状态选择对应的动画资源
void Army::setNowRes()
{
    std::list<ImageResource> *templist = NULL;

    switch (this->nowstate) {
    case MOVEOBJECT_STATE_STAND: // 站立
        templist =this->Stand[playerRepresent][Num][getLevel()][Angle];
        break;
    case MOVEOBJECT_STATE_WALK: // 行走
        templist =this->Walk[playerRepresent][Num][getLevel()][Angle];
        break;
    case MOVEOBJECT_STATE_ATTACK: // 攻击
        templist =this->Attack[playerRepresent][Num][getLevel()][Angle];
        break;
    case MOVEOBJECT_STATE_DIE: // 死亡
        if(changeToDisappear) templist = this->Disappear[playerRepresent][Num][getLevel()][Angle]; // 消失
        else templist =this->Die[playerRepresent][Num][getLevel()][Angle]; // 死亡动画
        break;
    default:
        break;
    }
    if(templist!= nowlist) // 切换动画序列
    {
        nowlist = templist;
        nowres = nowlist->begin(); // 第一帧
        initAttack_perCircle(); // 重置攻击循环参数
            initNowresTimer(); // 初始化帧计时器
    }
}

//播放攻击音效
void Army::requestSound_Attack()
{
    if((Num == AT_IMPROVED|| Num == AT_BOWMAN) && isInWidget()) // 弓箭手类型且在当前视图内
        soundQueue.push("Archer_Attack"); // 播放弓箭手攻击音效
}

// 播放死亡音效
void Army::requestSound_Die()
{
    if(!isInWidget()) 
        return;

    if(Num == AT_SCOUT) // 侦察兵
        soundQueue.push("Scout_Die"); 
    else
        soundQueue.push("Army_Die"); // 播放一般军队死亡音效
}

// 播放行走音效
void Army::requestSound_Walk()
{
    if(Num == AT_SCOUT && isInWidget()) // 侦察兵
        soundQueue.push("Scout_Walk"); 
}


// 获取单位移动速度
// 考虑单位等级和科技加成
double Army::getSpeed()
{
    double moveSpeed;

    if( upgradable ) moveSpeed = speed_change[getLevel()]; // 可升级
    else moveSpeed = speed; 

    return moveSpeed*playerScience->get_rate_Move(getSort(),Num); // 科技加成
}

// 获取单位最大血量
int Army::getMaxBlood()
{
    int realmBlood;

    if(upgradable) realmBlood = MaxBlood_change[getLevel()]; // 可升级
    else realmBlood = MaxBlood; 

    return  realmBlood*playerScience->get_rate_Blood(getSort(),Num)+playerScience->get_addition_Blood(getSort(),Num); // 应用科技加成
}

// 获取单位视野范围
int Army::getVision()
{
    int realVision;
    if(upgradable) realVision = vision_change[getLevel()]; // 可升级
    else realVision = vision; 

    return realVision + playerScience->get_addition_DisAttack(getSort(),Num,armyClass,get_AttackType()); // 应用科技加成
}
// 获取单位攻击力
int Army::getATK()
{
    int atkValue; // 初始攻击力

    // 赋值初始攻击力,依据兵种是否能升级,划分两类赋值方式
    if(upgradable) atkValue = atk_change[getLevel()]; // 可升级
    else atkValue = atk; // 否则使用基础攻击值
//    return (int)( atkValue*playerScience->get_rate_Attack(getSort(),Num,armyClass,get_AttackType()), interactSort, interactNum) + \
//             get_add_specialAttack() + playerScience->get_addition_Attack(getSort(),Num,armyClass,get_AttackType());

    return (int)( atkValue*playerScience->get_rate_Attack(getSort(),Num,armyClass,get_AttackType(), interactSort, interactNum)) + \
            get_add_specialAttack() + playerScience->get_addition_Attack(getSort(),Num,armyClass,get_AttackType());
}

// 获取单位防御力
int Army::getDEF(int attackType_got)
{
    int defValue = 0; // 用于存储初始防御力

    // 赋值defValue;根据收到的伤害类型,选择肉盾防御或投射防御
    if(attackType_got == ATTACKTYPE_CLOSE || attackType_got == ATTACKTYPE_ANIMAL) // 近战攻击或动物攻击
    {
        if(upgradable) defValue = defence_close_change[getLevel()]; // 可升级
        else defValue = defence_close; // 否则使用基础近战防御值
    }
    else if(attackType_got == ATTACKTYPE_SHOOT) // 远程攻击
    {
        if(upgradable) defValue = defence_shoot_change[getLevel()]; // 可升级
        else defValue = defence_shoot; // 否则使用基础远程防御值
    }

    // 在defValue的基础上,计算player及科技带来的加成
    return (int)( defValue*playerScience->get_rate_Defence(getSort(),Num,armyClass,attackType_got) ) + \
            playerScience->get_addition_Defence(getSort(),Num,armyClass,attackType_got);
}

// 基础攻击力（不包含科技加成，只包含特殊加成）
int Army::showATK_Basic()
{
    int atkValue; // 初始攻击力

    // 赋值初始攻击力,依据兵种是否能升级,划分两类赋值方式
    if(upgradable) atkValue = atk_change[getLevel()]; // 可升级
    else atkValue = atk; // 否则使用基础攻击值

    return atkValue+get_add_specialAttack(); // 返回基础攻击力加特殊加成
}

// 基础近战防御力
int Army::showDEF_Close()
{
    int defValue = 0;

    if(upgradable) defValue = defence_close_change[getLevel()]; // 可升级
    else defValue = defence_close; // 否则使用基础近战防御值

    return defValue;
}

// 基础远程防御力
int Army::showDEF_Shoot()
{
    int defValue = 0;

    if(upgradable) defValue = defence_shoot_change[getLevel()]; // 可升级
    else defValue = defence_shoot; // 否则使用基础远程防御值

    return defValue;
}

// 获取单位攻击距离
double Army::getDis_attack()
{
    double dis;

    if(upgradable) dis = dis_Attack_change[getLevel()]; // 可升级
    else dis = dis_Attack; // 否则使用基础攻击距离值

    if(dis == 0) dis = DISTANCE_ATTACK_CLOSE + (attackObject->getSideLength())/2.0; // 近战单位攻击距离计算
    else dis = ( dis + playerScience->get_addition_DisAttack(getSort(),Num,armyClass,get_AttackType() ) )*BLOCKSIDELENGTH; // 远程单位攻击距离计算

    return dis;
}

// 获取特殊攻击加成
int Army::get_add_specialAttack()
{
    int addition = 0;

    if(Num == AT_SLINGER) // 投石兵
    {
       /* if(interactSort == SORT_ARMY)
        {
            if(interactNum == AT_BOWMAN || interactNum == AT_IMPROVED) addition+=2;
        }
        else */if( interactSort == SORT_BUILDING) // 如果目标是建筑
        {
            if(interactNum == BUILDING_ARROWTOWER || interactNum == BUILDING_WALL) // 如果目标是箭塔或城墙
                addition += DEFSHOOT_BUILD_ARROWTOWER; // 增加额外伤害
        }
    }
//    else if(Num == AT_CAVALRY)
//    {
//        if(interactSort == SORT_ARMY)
//        {
//            if(interactNum == AT_CLUBMAN || interactNum == AT_SWORDSMAN)
//                addition+=5;
//        }

//    }

    return addition;
}


// 设置单位的基础属性
void Army::setAttribute()
{
    this->Blood=1; // 初始血量设为满
    this->Angle=rand()%8; // 随机朝向(8个方向)
    
    switch (Num) {
    case AT_CLUBMAN:        // 棍棒兵,可升级1次
        upgradable = true; // 可升级
        dependBuildNum = BUILDING_ARMYCAMP; // 依赖兵营建筑
        dependBuildAct = BUILDING_ARMYCAMP_UPGRADE_CLUBMAN; // 升级动作
        armyClass = ARMY_INFANTRY; // 步兵类型
        attackType = ATTACKTYPE_CLOSE; // 近战攻击类型

        // 不同等级的属性
        MaxBlood_change = new int[2]{ BLOOD_CLUBMAN1,BLOOD_CLUBMAN2 }; // 最大血量
        speed_change = new double[2]{ SPEED_CLUBMAN1,SPEED_CLUBMAN2 }; // 移动速度
        vision_change = new int[2]{ VISION_CLUBMAN1,VISION_CLUBMAN2 }; // 视野范围
        atk_change  = new int[2]{ATK_CLUBMAN1,ATK_CLUBMAN2}; // 攻击力
        dis_Attack_change  = new double[2]{DIS_CLUBMAN1 , DIS_CLUBMAN2}; // 攻击距离
        inter_Attack_change = new double[2]{ INTERVAL_CLUBMAN1,INTERVAL_CLUBMAN2 }; // 攻击间隔
        defence_close_change  = new int[2]{ DEFCLOSE_CLUBMAN1,DEFCLOSE_CLUBMAN2 }; // 近战防御
        defence_shoot_change  = new int[2]{ DEFSHOOT_CLUBMAN1,DEFSHOOT_CLUBMAN2 }; // 远程防御

        crashLength = CRASHBOX_SINGLEOB; // 碰撞盒大小
        nowres_step = NOWRES_TIMER_CLUBMAN; // 动画帧间隔

        break;

//    case AT_SWORDSMAN:  //短剑兵,可升级3次
//        upgradable = true;
//        armyClass = ARMY_INFANTRY;
//        attackType = ATTACKTYPE_CLOSE;

//        MaxBlood_change = new int[4]{ BLOOD_SHORTSWORDSMAN1,BLOOD_SHORTSWORDSMAN2,BLOOD_SHORTSWORDSMAN3,BLOOD_SHORTSWORDSMAN4 };
//        speed_change = new double[4]{ SPEED_SHORTSWORDSMAN1,SPEED_SHORTSWORDSMAN2,SPEED_SHORTSWORDSMAN3,SPEED_SHORTSWORDSMAN4 };
//        vision_change = new int[4]{ VISION_SHORTSWORDSMAN1,VISION_SHORTSWORDSMAN2,VISION_SHORTSWORDSMAN3,VISION_SHORTSWORDSMAN4 };
//        atk_change  = new int[4]{ATK_SHORTSWORSMAN1,ATK_SHORTSWORSMAN2,ATK_SHORTSWORSMAN3,ATK_SHORTSWORSMAN4};
//        dis_Attack_change  = new double[4]{DIS_SHORTSWORDSMAN1 , DIS_SHORTSWORDSMAN2,DIS_SHORTSWORDSMAN3,DIS_SHORTSWORDSMAN4};
//        inter_Attack_change = new double[4]{ INTERVAL_SHORTSWORDSMAN1,INTERVAL_SHORTSWORDSMAN2,INTERVAL_SHORTSWORDSMAN3,INTERVAL_SHORTSWORDSMAN4 };
//        defence_close_change  = new int[4]{ DEFCLOSE_SHORTSWORSMAN1,DEFCLOSE_SHORTSWORSMAN2,DEFCLOSE_SHORTSWORSMAN3,DEFCLOSE_SHORTSWORSMAN4 };
//        defence_shoot_change  = new int[4]{ DEFSHOOT_SHORTSWORSMAN1,DEFSHOOT_SHORTSWORSMAN2,DEFSHOOT_SHORTSWORSMAN3,DEFSHOOT_SHORTSWORSMAN4 };
//        break;

    case AT_SWORDSMAN:    // 剑士
        upgradable = false; // 不可升级
        dependBuildNum = BUILDING_ARMYCAMP; // 依赖兵营建筑
        armyClass = ARMY_INFANTRY; // 步兵类型
        attackType = ATTACKTYPE_CLOSE; // 近战攻击类型

        // 基础属性
        MaxBlood = BLOOD_SHORTSWORDSMAN1; 
        speed = SPEED_SHORTSWORDSMAN1; 
        vision = VISION_SHORTSWORDSMAN1; 
        atk = ATK_SHORTSWORSMAN1; 
        dis_Attack = DIS_SHORTSWORDSMAN1; 
        inter_Attack = INTERVAL_SHORTSWORDSMAN1; 
        defence_close = DEFCLOSE_SHORTSWORSMAN1; 
        defence_shoot = DEFSHOOT_SHORTSWORSMAN1; 

        crashLength = CRASHBOX_SINGLEOB; 
        nowres_step = NOWRES_TIMER_SWORSMAN; 
        break;

    case AT_SLINGER:    // 投石兵
        upgradable = false; // 不可升级
        dependBuildNum = BUILDING_ARMYCAMP; // 依赖兵营建筑
        armyClass = ARMY_INFANTRY; // 步兵类型
        attackType = ATTACKTYPE_SHOOT; // 远程攻击类型

        // 基础属性
        MaxBlood = BLOOD_SLINGER; 
        speed = SPEED_SLINGER; 
        vision = VISION_SLINGER; 
        atk = ATK_SLINGER; 
        dis_Attack = DIS_SLINGER; 
        inter_Attack = INTERVAL_SLINGER; 
        defence_close = DEFCLOSE_SLINGER; 
        defence_shoot = DEFSHOOT_SLINGER; 

        crashLength = CRASHBOX_SINGLEOB; 

        type_Missile = Missile_Cobblestone; // 投射物类型为石块
        phaseFromEnd_MissionAttack = THROWMISSION_SLINGER; // 发射投射物的动画帧位置

        nowres_step = NOWRES_TIMER_SLINGER; 
        break;

    case AT_BOWMAN:     // 弓箭手
        upgradable = false; // 不可升级
        dependBuildNum = BUILDING_RANGE; // 依赖射箭场建筑
        armyClass = ARMY_ARCHER; // 弓箭手类型
        attackType = ATTACKTYPE_SHOOT; // 远程攻击类型

        // 基础属性
        MaxBlood = BLOOD_BOWMAN; 
        speed = SPEED_BOWMAN; 
        vision = VISION_BOWMAN; 
        atk = ATK_BOWMAN; 
        dis_Attack = DIS_BOWMAN; 
        inter_Attack = INTERVAL_BOWMAN; 
        defence_close = DEFCLOSE_BOWMAN; 
        defence_shoot = DEFSHOOT_BOWMAN; 

        crashLength = CRASHBOX_SINGLEOB; 

        type_Missile = Missile_Arrow; // 投射物类型为箭矢
        phaseFromEnd_MissionAttack = THROWMISSION_ARCHER; // 发射投射物的动画帧位置

        nowres_step = NOWRES_TIMER_BOWMAN; 
        break;

    case AT_IMPROVED:     // 强化弓箭手
        upgradable = false; // 不可升级
        dependBuildNum = BUILDING_RANGE; // 依赖射箭场建筑
        armyClass = ARMY_ARCHER; // 弓箭手类型
        attackType = ATTACKTYPE_SHOOT; // 远程攻击类型

        // 基础属性
        MaxBlood = BLOOD_IMPROVEDBOWMAN1; // 最大血量
        speed = SPEED_IMPROVEDBOWMAN1; // 移动速度
        vision = VISION_IMPROVEDBOWMAN1; // 视野范围
        atk = ATK_IMPROVEDBOWMAN1; 
        dis_Attack = DIS_IMPROVEDBOWMAN1; 
        inter_Attack = INTERVAL_IMPROVEDBOWMAN1; 
        defence_close = DEFCLOSE_IMPROVEDBOWMAN1; 
        defence_shoot = DEFSHOOT_IMPROVEDBOWMAN1; 

        crashLength = CRASHBOX_SINGLEOB; 

        type_Missile = Missile_Arrow; // 投射物类型为箭矢
        phaseFromEnd_MissionAttack = THROWMISSION_IMPROVEDBOWMAN1; // 发射投射物的动画帧位置

        nowres_step = NOWRES_TIMER_IMPROVEDBOWMAN1; 
        break;

    case AT_SCOUT:      // 侦察骑兵
        upgradable = false; // 不可升级
        dependBuildNum = BUILDING_STABLE; // 依赖马厩建筑
        armyClass = ARMY_RIDER; // 骑兵类型
        attackType = ATTACKTYPE_CLOSE; // 近战攻击类型

        // 基础属性
        MaxBlood = BLOOD_SCOUT; 
        speed = SPEED_SCOUT; 
        vision = VISION_SCOUT; 
        atk = ATK_SCOUT; 
        dis_Attack = DIS_SCOUT; 
        inter_Attack = INTERVAL_SCOUT; 
        defence_close = DEFCLOSE_SCOUT; 
        defence_shoot = DEFSHOOT_SCOUT; 

        crashLength = CRASHBOX_SMALLOB; 

        nowres_step = NOWRES_TIMER_SCOUT; 
        break;

    case AT_CAVALRY:      // 骑兵
        upgradable = false; // 不可升级
        dependBuildNum = BUILDING_STABLE; // 依赖马厩建筑
        armyClass = ARMY_RIDER; // 骑兵类型
        attackType = ATTACKTYPE_CLOSE; // 近战攻击类型

        // 基础属性
        MaxBlood = BLOOD_CAVALRY; 
        speed = SPEED_CAVALRY; 
        vision = VISION_CAVALRY; 
        atk = ATK_CAVALRY; 
        dis_Attack = DIS_CAVALRY; 
        inter_Attack = INTERVAL_CAVALRY; 
        defence_close = DEFCLOSE_CAVALRY; 
        defence_shoot = DEFSHOOT_CAVALRY; 

        crashLength = CRASHBOX_SMALLOB; 

        nowres_step = NOWRES_TIMER_CAVALRY; 
        break;
    default:
        incorrectNum = true; 
        break;
    }

}
// 通过查询玩家科技树，获取该兵种的当前等级
int Army::getLevel()
{
    /**
    *   通过查询player科技树表，得到当前player管控的该种类士兵的等级
    *   如果该种类士兵无法升级，则默认为0级
    */
    if(upgradable) return playerScience->getActLevel(dependBuildNum , dependBuildAct); // 如果可升级，返回科技树中的等级
    else return 0; // 否则返回0级
}

// 析构
Army::~Army()
{
    if(MaxBlood_change!=NULL)
    {
        delete MaxBlood_change;
        MaxBlood_change = NULL;
    }

    if(speed_change!=NULL)
    {
        delete speed_change;
        speed_change = NULL;
    }

    if(vision_change!=NULL)
    {
        delete vision_change;
        vision_change = NULL;
    }

    if(atk_change!=NULL)
    {
        delete atk_change;
        atk_change = NULL;
    }

    if(dis_Attack_change!=NULL)
    {
        delete dis_Attack_change;
        dis_Attack_change = NULL;
    }

    if(inter_Attack_change!=NULL)
    {
        delete inter_Attack_change;
        inter_Attack_change = NULL;
    }

    if(defence_close_change!=NULL)
    {
        delete defence_close_change;
        defence_close_change = NULL;
    }

    if(defence_shoot_change!=NULL)
    {
        delete defence_shoot_change;
        defence_shoot_change = NULL;
    }

}