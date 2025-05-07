#include "Farmer.h"

// 农民类静态成员变量初始化
// 携带资源时的图像资源列表，5种资源类型，8个方向
std::list<ImageResource>* Farmer::Carry[5][8];
// 行走时的图像资源列表，7种状态，8个方向
std::list<ImageResource>* Farmer::Walk[7][8];
// 工作时的图像资源列表，7种状态，8个方向
std::list<ImageResource>* Farmer::Work[7][8];
// 站立时的图像资源列表，7种状态，8个方向
std::list<ImageResource>* Farmer::Stand[7][8];
// 攻击时的图像资源列表，7种状态，8个方向
std::list<ImageResource>* Farmer::Attack[7][8];
// 死亡时的图像资源列表，7种状态，8个方向
std::list<ImageResource>* Farmer::Die[7][8];
// 消失时的图像资源列表，7种状态，8个方向
std::list<ImageResource>* Farmer::Disappear[7][8];

// 农民类型名称数组：村民、伐木工、采集者、矿工、猎人、农民、工人
std::string Farmer::FarmerName[7]={"Villager","Lumber","Gatherer","Miner","Hunter","Farmer","Worker"};
// 携带资源类型名称数组：空、木材、肉类、石头、黄金
std::string Farmer::FarmerCarry[5]={"","CarryWood","CarryMeat","CarryStone","CarryGold"};

// 点击音效
string Farmer::sound_click = "Click_Villager";

// 工作音效数组：空、砍伐、采集、采矿、弓箭攻击、耕地、建造
std::string Farmer::sound_work[7] = {\
    "", "Cut", "Gather", "Mine", "Archer_Attack", "Plow", "Build"\
};

// 构造函数：初始化农民对象
Farmer::Farmer(double DR, double UR , Development* playerScience, int playerRepresent )
{
    // 初始化玩家科技和玩家标识
    this->playerScience = playerScience;
    this->playerRepresent = playerRepresent;

    // 设置基础属性
    this->Blood=1;                    // 初始血量
    this->MaxBlood=BLOOD_FARMER;      // 最大血量
    speed = HUMAN_SPEED;              // 移动速度
    this->atk = 3;                    // 攻击力
    attackType = ATTACKTYPE_CLOSE;    // 攻击类型：近战
    phaseFromEnd_MissionAttack = THROWMISSION_FARMER;  // 攻击任务阶段
    vision = VISION_FARMER;           // 视野范围

    nowres_step = NOWRES_TIMER_FARMER;  // 图像更新计时器

    // 初始化位置和状态
    this->Angle=rand()%8;             // 随机初始朝向
    this->state=HUMAN_STATE_IDLE;     // 初始状态：空闲
    this->resource=0;                 // 初始资源：0
    this->DR=DR;                      // 设置坐标
    this->UR=UR;
    this->BlockDR=DR/BLOCKSIDELENGTH; // 计算块坐标
    this->BlockUR=UR/BLOCKSIDELENGTH;

    // 设置碰撞和移动相关属性
    setSideLenth();                   // 设置边长
    crashLength = CRASHBOX_SINGLEOB;  // 设置碰撞箱大小
    this->nextBlockDR=BlockDR;        // 下一个目标块坐标
    this->nextBlockUR=BlockUR;
    this->PredictedDR=DR;             // 预测位置
    this->PredictedUR=UR;
    this->PreviousDR=DR;              // 上一个位置
    this->PreviousUR=UR;
    this->DR0=DR;                     // 初始位置
    this->UR0=UR;
    this->nowstate=MOVEOBJECT_STATE_STAND;  // 当前状态：站立

    // 设置战斗相关属性
    isAttackable = true;              // 可被攻击
    type_Missile = Missile_Spear;     // 投射物类型：矛

    // 设置图像资源
    setNowRes();
    this->imageX=this->nowres->pix.width()/2.0;    // 图像X偏移
    this->imageY=this->nowres->pix.width()/4.0;    // 图像Y偏移
    this->imageH=DR-UR;                           // 图像高度

    // 设置全局编号并注册到全局对象列表
    this->globalNum=10000*getSort()+g_globalNum;
    g_Object.insert({this->globalNum,this});
    g_globalNum++;
}

// 更新帧：处理动画和状态更新
void Farmer::nextframe()
{
    if(isDie())  // 如果已死亡
    {
        if( !isDying() )  // 如果刚开始死亡
        {
            setPreDie();  // 设置预死亡状态
            requestSound_Die();  // 请求播放死亡音效
        }
        else if(!get_isActionEnd() && isNowresShift())  // 如果死亡动画未结束且需要切换图像
        {
            if(nowres == nowlist->begin() && nowstate == MOVEOBJECT_STATE_ATTACK)
            {
                if(state == FARMER_HUNTER && isInWidget())  // 如果是猎人且在视野内
                     soundQueue.push(sound_work[state]);    // 播放攻击音效
            }

            nowres++;  // 切换到下一帧
            if( !changeToDisappear && get_isActionEnd())  // 如果动画结束且未开始消失
            {
                changeToDisappear = true;  // 开始消失
                nowres_step = 1000;        // 设置消失计时器
                setNowRes();               // 更新图像资源
            }
        }
    }
    else  // 如果未死亡
    {
        updateState();  // 更新状态

        if(isNowresShift())  // 如果需要切换图像
        {
            nowres++;  // 切换到下一帧
            if(nowres==nowlist->end())  // 如果到达动画末尾
            {
                requestSound_Work();  // 请求播放工作音效
                nowres=nowlist->begin();  // 重置到动画开始
                initAttack_perCircle();   // 初始化攻击循环
            }            
        }

        updateMove();  // 更新移动
        setNowRes();   // 更新图像资源
    }

    updateImageXYByNowRes();  // 更新图像位置
}

// 设置当前图像资源
void Farmer::setNowRes()
{
    std::list<ImageResource> *templist = NULL;

    // 根据当前状态选择对应的图像资源列表
    switch (this->nowstate) {
    case MOVEOBJECT_STATE_STAND:  // 站立状态
        templist=this->Stand[this->state][this->Angle];
        break;
    case MOVEOBJECT_STATE_WALK:   // 行走状态
        if(get_MatchingOfResourceAndCarry() && resource > 0 && resourceSort!=HUMAN_GRANARYFOOD)
            templist = this->Carry[this->resourceSort][this->Angle];  // 携带资源时的图像
        else
            templist=this->Walk[this->state][this->Angle];  // 普通行走图像
        break;
    case MOVEOBJECT_STATE_ATTACK:  // 攻击状态
        templist=this->Attack[this->state][this->Angle];
        break;
    case MOVEOBJECT_STATE_WORK:    // 工作状态
        templist=this->Work[this->state][this->Angle];
        break;
    case MOVEOBJECT_STATE_DIE:     // 死亡状态
        if(changeToDisappear) templist = this->Disappear[this->state][this->Angle];  // 消失动画
        else templist = this->Die[this->state][this->Angle];  // 死亡动画
        break;
    default:
        break;
    }
    // 如果图像资源列表发生变化，更新当前列表和迭代器
    if(templist!=nowlist && templist)
    {
        nowlist = templist;
        nowres = nowlist->begin();
        initNowresTimer();  // 初始化图像更新计时器
        initAttack_perCircle();  // 初始化攻击循环
    }
}

// 获取攻击距离
double Farmer::getDis_attack()
{
    double dis;

    // 如果是远程攻击，基础距离为3
    if(get_AttackType() == ATTACKTYPE_SHOOT) dis = 3;
    else dis = 0;

    if(dis == 0)  // 如果是近战攻击
    {
        dis = DISTANCE_ATTACK_CLOSE;  // 基础近战距离

        if(attackObject != NULL)
            dis += (attackObject->getSideLength())/2.0;  // 加上目标对象的一半边长
    }
    else dis = ( dis + playerScience->get_addition_DisAttack(getSort(), Num , 0 ,get_AttackType() ) )*BLOCKSIDELENGTH;  // 计算实际攻击距离

    return dis;
}

// 获取攻击类型
int Farmer::get_AttackType()
{
    if(interactSort== SORT_ANIMAL)  // 如果交互对象是动物
    {
        if(interactNum == ANIMAL_TREE || interactNum == ANIMAL_FOREST) 
            return ATTACKTYPE_CLOSE_TOTREE;  // 对树木使用近战攻击
        else 
            return ATTACKTYPE_SHOOT;  // 对其他动物使用远程攻击
    }
    return ATTACKTYPE_CLOSE;  // 默认使用近战攻击
}

// 获取特殊攻击加成
int Farmer::get_add_specialAttack()
{
    if(interactSort == SORT_ANIMAL) return 1;  // 对动物有特殊攻击加成
    else return 0;
}

// 更新农民状态
void Farmer::updateState()
{
    switch (interactSort) {
    case SORT_ANIMAL:  // 与动物交互
        if(interactNum ==ANIMAL_TREE || interactNum==ANIMAL_FOREST) 
            setState(1);  // 伐木工状态
        else 
            setState(4);  // 猎人状态
        break;
    case SORT_BUILDING:  // 与建筑交互
        setState(6);  // 工人状态
        break;
    case SORT_STATICRES:  // 与静态资源交互
        if(interactNum == NUM_STATICRES_Bush) 
            setState(2);  // 采集者状态
        else 
            setState(3);  // 矿工状态
        break;
    case SORT_Building_Resource:  // 与资源建筑交互
        if(interactBui_builtUp)  // 如果建筑已建成
        {
            if(interactNum == BUILDING_FARM)
                setState(5);  // 农民状态
        }
        else 
            setState(6);  // 工人状态
        break;
    default:
        setState(0);  // 默认村民状态
        break;
    }
}

// 请求播放工作音效
void Farmer::requestSound_Work()
{
    if(!isInWidget())  // 如果不在视野内
        return;

    // 在工作状态或攻击状态时播放对应音效
    if( nowstate == MOVEOBJECT_STATE_WORK &&\
        ( state == FARMER_LUMBER || state == FARMER_MINER || state == FARMER_WORKER|| state == FARMER_FARMER || state == FARMER_GATHERER )\
        || nowstate == MOVEOBJECT_STATE_ATTACK && state == FARMER_LUMBER
    )
        soundQueue.push(sound_work[state]);

    return;
}
