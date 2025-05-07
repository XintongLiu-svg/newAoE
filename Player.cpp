#include "Player.h"

//***************************************************************
//构造与析构
// 玩家默认构造函数
Player::Player()
{
    playerScience = new Development(represent);  // 创建玩家科技系统
}

// 玩家构造函数
// 参数说明：
// represent: 玩家标识
Player::Player(int represent)
{
    this->represent = represent;  // 设置玩家标识
    playerScience = new Development(represent);  // 创建玩家科技系统
}

// 玩家析构函数
Player::~Player()
{
    delete playerScience;  // 释放科技系统内存
    
    // 清空所有对象池
    list<Human*>::iterator iter_deleHuman = human.begin();
    list<Building*>::iterator iter_deleBuild = build.begin();
    list<Missile*>::iterator iter_deleMissile = missile.begin();

    // 删除所有人类单位
    while(iter_deleHuman!=human.end())iter_deleHuman = deleteHuman(iter_deleHuman);
    // 删除所有建筑
    while(iter_deleBuild!=build.end())iter_deleBuild = deleteBuilding(iter_deleBuild);
    // 删除所有投射物
    while(iter_deleMissile!=missile.end())iter_deleMissile = deleteMissile(iter_deleMissile);
}

//***************************************************************
//添加实例对象
// 添加建筑
// 参数说明：
// Num: 建筑类型编号
// BlockDR, BlockUR: 块坐标
// percent: 建造进度
Building* Player::addBuilding(int Num, int BlockDR, int BlockUR , double percent)
{
    Building *newbuilding = NULL;
    // 根据建筑类型创建不同的建筑对象
    if(Num == BUILDING_FARM) 
        newbuilding = new Building_Resource(Num,BlockDR,BlockUR,getCiv() , playerScience , represent , percent);  // 农场
    else 
        newbuilding=new Building(Num,BlockDR,BlockUR, getCiv(), playerScience , represent , percent);  // 其他建筑

    build.push_back(newbuilding);  // 添加到建筑列表
    return newbuilding;
}

// 添加人类单位
// 参数说明：
// Num: 单位类型编号
// DR, UR: 详细坐标
int Player::addHuman(int Num, double DR, double UR)
{
    Human *newhuman=new Human(Num,DR,UR , playerScience , represent);  // 创建人类单位

    // 输出调试信息
    call_debugText("blue"," 产生了新的"+newhuman->getChineseName()+"(编号:" + QString::number(newhuman->getglobalNum()) + ")",represent);

    human.push_back(newhuman);  // 添加到人类单位列表
    humanNumIncrease(newhuman);  // 增加人口计数
    return 0;
}

// 添加军队单位
// 参数说明：
// Num: 军队类型编号
// DR, UR: 详细坐标
Army* Player::addArmy(int Num , double DR , double UR)
{
    Army *newArmy = new Army(DR , UR, Num ,  playerScience , represent);  // 创建军队单位
    call_debugText("blue"," 产生了新的"+newArmy->getChineseName()+"(编号:" + QString::number(newArmy->getglobalNum()) + ")",represent);

    // 播放军队出生音效
    if(g_frame > 10 && represent == NOWPLAYERREPRESENT)
        soundQueue.push("Army_Born");

    human.push_back(newArmy);  // 添加到人类单位列表
    humanNumIncrease(newArmy);  // 增加人口计数
    return newArmy;
}

// 添加农民
// 参数说明：
// DR, UR: 详细坐标
int Player::addFarmer(double DR, double UR)
{
    Farmer *newfarmer=new Farmer(DR,UR , playerScience , represent);  // 创建农民
    call_debugText("blue"," 产生了新的村民(编号:" + QString::number(newfarmer->getglobalNum()) + ")",represent);

    // 播放农民出生音效
    if(g_frame > 10 && represent == NOWPLAYERREPRESENT)
        soundQueue.push("Villager_Born");

    human.push_back(newfarmer);  // 添加到人类单位列表
    humanNumIncrease(newfarmer);  // 增加人口计数
    return 0;
}

// 添加投射物
// 参数说明：
// attacker: 攻击者坐标
// attackee: 目标坐标
// beginHeight: 初始高度
Missile* Player::addMissile( Coordinate* attacker , Coordinate* attackee , int beginHeight)
{
    Missile* newMissile = NULL;
    BloodHaver* aterOb = NULL;

    // 获取攻击者信息
    attacker->printer_ToBloodHaver((void**)&aterOb);
    if(aterOb!=NULL)
    {
        // 创建投射物
        newMissile = new Missile(aterOb->get_type_Missile(), attacker , attackee, beginHeight , playerScience , represent);
        missile.push_back(newMissile);  // 添加到投射物列表
    }

    return newMissile;
}

//删除实例对象
// 删除人类单位
list<Human*>::iterator Player::deleteHuman( list<Human*>::iterator iterDele )
{
    delete *iterDele;  // 释放内存
    return human.erase(iterDele);  // 从列表中移除
}

// 删除建筑
list<Building*>::iterator Player::deleteBuilding( list<Building*>::iterator iterDele )
{
    // 根据建筑类型更新科技系统
    if((*iterDele)->getNum() == BUILDING_HOME)  // 房屋
    {
        if((*iterDele)->isConstructed())
            playerScience->subHome();  // 减少人口上限
    }
    else if((*iterDele)->getNum() == BUILDING_CENTER)  // 市镇中心
        playerScience->subCenter();
    else if((*iterDele)->getNum() != BUILDING_FARM)  // 其他时代建筑
        playerScience->sub_civiBuildNum((*iterDele)->getNum());

    delete *iterDele;  // 释放内存
    return build.erase(iterDele);  // 从列表中移除
}

// 删除投射物
list<Missile*>::iterator Player::deleteMissile( list<Missile*>::iterator iterDele )
{
    delete *iterDele;  // 释放内存
    return missile.erase(iterDele);  // 从列表中移除
}

// 删除投射物攻击者（已死亡）
void Player::deleteMissile_Attacker( Coordinate* attacker )
{
    // 遍历所有投射物，删除与已死亡攻击者的关联
    for(list<Missile*>::iterator iter = missile.begin();iter!=missile.end();iter++)
        (*iter)->deleteAttackerSponsor(attacker);
}

//***************************************************************
//控制资源
// 改变单一资源数量
// 参数说明：
// resourceSort: 资源类型
// num: 资源数量
// negative: 是否为减少操作
void Player::changeResource( int resourceSort , int num  , bool negative)
{
    if(negative) num *= -1;     // 如果是减少操作，将数量转为负数

    // 根据资源类型更新对应资源
    switch (resourceSort)
    {
        case HUMAN_GOLD:  // 黄金
            gold+=num;
            break;
        case HUMAN_WOOD:  // 木材
            wood+=num;
            break;
        case HUMAN_STONE:  // 石头
            stone+=num;
            break;
        case HUMAN_GRANARYFOOD:  // 谷物
        case HUMAN_STOCKFOOD:    // 食物
            food+=num;
            break;
        default:
            break;
    }
}

// 同时改变多种资源数量
void Player::changeResource(int wood,int food,int stone,int gold, bool negative)
{
    if(negative) wood*=-1,food*=-1,stone*=-1,gold*=-1;  // 如果是减少操作，将所有数量转为负数

    // 更新所有资源
    this->wood +=wood;
    this->food+=food;
    this->stone+=stone;
    this->gold+=gold;
}

// 根据建筑消耗改变资源
void Player::changeResource_byBuild(int buildNum)
{
    int wood,food,stone,gold;
    // 获取建筑消耗的资源
    playerScience->get_Resource_Consume(buildNum,wood,food,stone,gold);
    changeResource(wood,food,stone,gold,true);  // 减少资源
}

// 根据建筑动作消耗改变资源
void Player::changeResource_byBuildAction(Building* actbuilding , int buildact)
{
    int wood = 0,food = 0,stone = 0,gold = 0;
    // 获取建筑动作消耗的资源
    playerScience->get_Resource_Consume(actbuilding->getNum(),buildact,wood,food,stone,gold);
    changeResource(wood,food,stone,gold,true);  // 减少资源
    actbuilding->set_Resource_TS(wood,food,stone,gold);  // 记录资源消耗
}

//***************************************************************
//控制建筑行动
// 强制执行建筑动作
// 参数说明：
// actBuild: 执行动作的建筑
// Block_free: 建筑周围可用的格子
void Player::enforcementAction( Building* actBuild, vector<Point>Block_free  )
{
    bool isNeedCreatObject = false;  // 是否需要创建新对象
    int creatObjectSort , creatObjectNum;   // 需创建对象的类型和编号
    Point block;

    // 检查是否需要创建新对象
    isNeedCreatObject = playerScience->isNeedCreatObjectAfterAction(actBuild->getNum() , actBuild->getActNum() , creatObjectSort , creatObjectNum);

    // 获取建筑位置
    block.x = actBuild->getBlockDR();
    block.y = actBuild->getBlockUR();

    // 完成建筑动作
    playerScience->finishAction(actBuild->getNum() ,actBuild->getActNum());
    actBuild->init_Resouce_TS();    // 重置资源消耗记录

    if(isNeedCreatObject)   // 如果需要创建新对象
    {
        // 随机选择可用位置
        if(Block_free.size()) block = Block_free[ rand()%Block_free.size() ];

        // 创建相应对象
        if(creatObjectSort == SORT_FARMER)  // 创建农民
            addFarmer(trans_BlockPointToDetailCenter(block.x) , trans_BlockPointToDetailCenter(block.y));
        else if(creatObjectSort == SORT_ARMY)  // 创建军队
            addArmy(creatObjectNum , trans_BlockPointToDetailCenter(block.x) , trans_BlockPointToDetailCenter(block.y));
    }
}

// 返还建筑动作消耗的资源
void Player::back_Resource_TS( Building* actBuild )
{
    // 获取建筑记录的资源消耗
    int wood,food,stone,gold;
    actBuild->get_Resouce_TS(wood,food,stone,gold);
    this->changeResource(wood,food,stone,gold);  // 返还资源
    actBuild->init_Resouce_TS();  // 重置资源消耗记录
}

// 处理建筑建造完成
void Player::finishBuild( Building* buildBuilding ){
    playerScience->finishAction(buildBuilding->getNum());   // 记录建筑建造完成
    buildBuilding->recordConstruct();   // 标记建筑已建造完成

    // 根据建筑类型更新科技系统
    if(buildBuilding->getNum() == BUILDING_HOME) 
        playerScience->addHome();  // 增加人口上限
    else if(buildBuilding->getNum() != BUILDING_CENTER && buildBuilding->getNum()!= BUILDING_FARM)  
        playerScience->add_civiBuildNum(buildBuilding->getNum());  // 记录时代建筑

    // 输出调试信息
    call_debugText("blue"," "+buildBuilding->getChineseName()+"(编号:"+QString::number(buildBuilding->getglobalNum())+")建造完毕",represent);
}

// 开始攻击
void Player::beginAttack()
{
    // 播放攻击音效
    if(represent == NOWPLAYERREPRESENT && (g_frame - attackFrame > 1500|| attackFrame == 0))
        soundQueue.push("Beginning_Attack");

    attackFrame = g_frame;  // 记录攻击时间
}

//**********************************************************


