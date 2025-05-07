#include "Building.h"

/********************静态资源**************************/
// 建筑图像资源列表
std::list<ImageResource>* Building::build[4];  // 建造中的建筑图像
std::list<ImageResource>* Building::built[3][10];  // 已建成的建筑图像，3个文明，10种建筑
std::list<ImageResource>* Building::buildFire[3];  // 建筑着火时的图像

// 建筑名称数组
std::string Building::Buildingname[4]={"Small_Foundation","Foundation","Big_Foundation","Building_House1"};  // 地基和房屋名称
std::string Building::Builtname[3][10]={{},  // 已建成建筑名称，3个文明，10种建筑
                                       {"House1","Granary","Center1","Stock","Farm","Market","ArrowTower","ArmyCamp","Stable","Range"},
                                       {"House2","Granary","Center2","Stock","Farm","Market","ArrowTower","ArmyCamp","Stable","Range"}
                                      };
// 建筑显示名称
std::string Building::BuildDisplayName[10]={"房屋","谷仓","市镇中心","仓库","农场","市场","箭塔","兵营","马厩","靶场"};

// 建筑着火效果名称
std::string Building::BuildFireName[3] = { "S_Fire", "M_Fire", "B_Fire"};  // 小、中、大火

// 建筑点击音效
std::string Building::sound_click[10] = {\
    "Click_House","Click_Granary","Click_Center","Click_Stock","Click_Farm","Click_Market","Villager_ArrowTower","Click_ArmyCamp","Click_Stable","Click_Range"\
};

// 建筑可执行动作列表
int Building::actNames[BUILDING_TYPE_MAXNUM][ACT_WINDOW_NUM_FREE] = {ACT_NULL};

// 建筑最大生命值
int Building::BuildingMaxBlood[10] = { BLOOD_BUILD_HOUSE,  BLOOD_BUILD_GRANARY, BLOOD_BUILD_CENTER, BLOOD_BUILD_STOCK, BLOOD_BUILD_FARM,\
                                     BLOOD_BUILD_MARKET, BLOOD_BUILD_ARROWTOWER, BLOOD_BUILD_ARMYCAMP, BLOOD_BUILD_STABLE, BLOOD_BUILD_RANGE};

// 建筑地基类型
int Building::BuildingFundation[10] = { FOUNDATION_SMALL, FOUNDATION_MIDDLE, FOUNDATION_MIDDLE, FOUNDATION_MIDDLE, FOUNDATION_MIDDLE,\
                                      FOUNDATION_MIDDLE, FOUNDATION_SMALL, FOUNDATION_MIDDLE, FOUNDATION_MIDDLE, FOUNDATION_MIDDLE,};

// 建筑视野范围
int Building::BuildingVision[10] = { VISION_HOME, VISION_GRANARY, VISION_CENTER, VISION_STOCK, VISION_FARM,\
                                     VISION_MARKET, VISION_ARROWTOWER, VISION_ARMYCAMP, VISION_STABLE, VISION_RANGE};

/********************静态资源**************************/
bool is_cheatAction = false;  // 作弊模式标志
extern Score usrScore;  // 用户分数


/********************构造与析构**************************/
// 建筑构造函数
Building::Building(int Num, int BlockDR, int BlockUR, int civ, Development* playerScience, int playerRepresent, int Percent)
{
    // 初始化玩家科技和玩家标识
    this->playerScience = playerScience;
    this->playerRepresent = playerRepresent;

    // 设置建筑基本属性
    this->Num=Num;  // 建筑类型编号
    setBlockDRUR(BlockDR, BlockUR);  // 设置块坐标
    this->civ=civ;  // 文明类型
    this->visible=1;  // 可见性
    this->imageH=(BlockDR-BlockUR)*BLOCKSIDELENGTH;  // 图像高度
    this->Percent=Percent;  // 建造进度
    if(isFinish()) recordConstruct();  // 如果建造完成，记录建造
    setAttribute();  // 设置建筑属性
    init_Blood();  // 初始化生命值
    setFundation();  // 设置地基

    setDetailPointAttrb_FormBlock();  // 设置详细坐标属性
    setNowRes();  // 设置当前图像资源

    updateImageXYByNowRes();  // 更新图像位置
    // 设置全局编号并注册到全局对象列表
    this->globalNum=10000*SORT_BUILDING+g_globalNum;
    g_Object.insert({this->globalNum,this});
    g_globalNum++;
}


/********************虚函数**************************/
/***************状态与图像显示****************/
// 更新帧：处理建筑动画和状态
void Building::nextframe()
{
    setNowRes();  // 设置当前图像资源
    if(Percent<100)  // 如果正在建造
    {
        nowres = nowlist->begin();
        advance(nowres, (int)(Percent/25));  // 根据建造进度选择图像
    }
    else  // 如果建造完成
    {
        nowres++;
        if(nowres==nowlist->end())
        {
            nowres=nowlist->begin();  // 动画循环
            initAttack_perCircle();  // 初始化攻击循环
        }

        // 处理着火效果
        if(fireNowList != NULL)
        {
            fireNowRes++;

            if(fireNowRes == fireNowList->end())
                fireNowRes = fireNowList->begin();

            this->fireImageX = fireNowRes->pix.width()/2.0;
            this->fireImageY = fireNowRes->pix.width()/4.0;
        }

        // 处理防御状态
        if(defencing)
            missionThrowTimer = missionThrowTimer == missionThrowStep ? 0 : missionThrowTimer+1;
    }

    updateImageXYByNowRes();  // 更新图像位置
}

// 设置当前图像资源
void Building::setNowRes()
{
    std::list<ImageResource>* tempNowlist = NULL;
    if(Percent<100) tempNowlist = Building::build[Foundation];  // 建造中的图像
    else
    {
        tempNowlist = Building::built[get_civilization()][Num];  // 已建成的图像
        setFireNowRes();  // 设置着火效果
    }

    if(tempNowlist != nowlist)
    {
        nowlist = tempNowlist;
        nowres = nowlist->begin();
    }
}


/*******状态与属性设置、获取*******/
// 设置建筑属性
void Building::setAttribute()
{
    /*********特殊设定**********/
    switch (Num) {
    case BUILDING_CENTER:  // 市镇中心
    case BUILDING_HOME:    // 房屋
    case BUILDING_STOCK:   // 仓库
    case BUILDING_GRANARY: // 谷仓
    case BUILDING_ARMYCAMP:// 兵营
    case BUILDING_MARKET:  // 市场
    case BUILDING_RANGE:   // 靶场
    case BUILDING_STABLE:  // 马厩
    case BUILDING_WALL:    // 城墙
        break;
    case BUILDING_ARROWTOWER:  // 箭塔
        atk = ATK_BUILD_ARROWTOWER;  // 攻击力
        defence_shoot = DEFSHOOT_BUILD_ARROWTOWER;  // 远程防御
        dis_Attack = DIS_ARROWTOWER;  // 攻击距离
        attackType = ATTACKTYPE_SHOOT;  // 攻击类型：远程
        type_Missile = Missile_Arrow;  // 投射物类型：箭
        missionThrowStep = THROWMISSION_ARROWTOWN_TIMER;  // 攻击间隔

        isAttackable = true;  // 可被攻击
        break;
    default:
        incorrectNum = true;  // 无效的建筑类型
        Foundation=FOUNDATION_MIDDLE;  // 默认中型地基
        break;
    }

    if(incorrectNum)
        return;

    MaxBlood = BuildingMaxBlood[Num];  // 设置最大生命值
    Foundation = BuildingFundation[Num];  // 设置地基类型
    vision = BuildingVision[Num];  // 设置视野范围
}

// 获取建筑视野范围
int Building::getVision()
{
    if(getNum() == BUILDING_ARROWTOWER)  // 如果是箭塔
        return vision + playerScience->get_addition_DisAttack(getSort(), Num, 0, get_AttackType());  // 加上科技加成
    else
        return vision;
}

// 判断是否为监控对象（箭塔专用）
bool Building::isMonitorObject(Coordinate* judOb)
{
    if(Num == BUILDING_ARROWTOWER)  // 如果是箭塔
        return judOb->isPlayerControl() && judOb->getPlayerRepresent() != getPlayerRepresent();  // 判断是否为敌方单位

    return false;
}

// 获取文明类型
int Building::get_civilization()
{
    if(playerScience == NULL)
        return CIVILIZATION_STONEAGE;  // 默认石器时代
    else
        return playerScience->get_civilization();
}

// 初始化生命值
void Building::init_Blood()
{
    if(Percent == 100) Blood = 1;  // 建造完成时满血
    else Blood = 1.0/(double)getMaxBlood();  // 建造中时血量很低
}


/*******行动相关*******/
// 设置建筑动作
void Building::setAction( int actNum)
{
    this->actNum = actNum;  // 设置动作编号

    ActNumToActName();  // 将动作编号转换为动作名称
    initActionPersent();  // 初始化动作进度
    playerScience->BuildingActionExecuting(Num, actNum);  // 执行建筑动作
    actSpeed= get_retio_Action();  // 设置动作速度
}

// 初始化动作
void Building::initAction()
{
    if(actSpeed != 0 && actNum != ACT_NULL)
        playerScience->BuildingActionOverExecuting(Num, actNum);  // 执行动作结束回调

    actName = ACT_NULL;  // 清空动作名称
    actNum = ACT_NULL;   // 清空动作编号
    actSpeed = 0;        // 清空动作速度
}

// 将动作编号转换为动作名称
void Building::ActNumToActName()
{
    if(Num == BUILDING_CENTER)  // 市镇中心
    {
        if(actNum == BUILDING_CENTER_CREATEFARMER) actName = ACT_CREATEFARMER;  // 创建农民
        else if(actNum == BUILDING_CENTER_UPGRADE) actName = ACT_UPGRADE_AGE;   // 升级时代
    }
    else if( Num == BUILDING_GRANARY)  // 谷仓
    {
        if(actNum == BUILDING_GRANARY_ARROWTOWER) actName = ACT_UPGRADE_TOWERBUILD;  // 升级箭塔
    }
    else if(Num == BUILDING_STOCK)  // 仓库
    {
        if(actNum == BUILDING_STOCK_UPGRADE_USETOOL) actName = ACT_STOCK_UPGRADE_USETOOL;  // 升级工具
        else if(actNum == BUILDING_STOCK_UPGRADE_DEFENSE_INFANTRY) actName = ACT_STOCK_UPGRADE_DEFENSE_INFANTRY;  // 升级步兵防御
        else if(actNum == BUILDING_STOCK_UPGRADE_DEFENSE_ARCHER) actName = ACT_STOCK_UPGRADE_DEFENSE_ARCHER;  // 升级弓箭手防御
        else if(actNum == BUILDING_STOCK_UPGRADE_DEFENSE_RIDER) actName = ACT_STOCK_UPGRADE_DEFENSE_RIDER;  // 升级骑兵防御
    }
    else if(Num == BUILDING_ARMYCAMP)  // 兵营
    {
        if(actNum == BUILDING_ARMYCAMP_CREATE_CLUBMAN) actName = ACT_ARMYCAMP_CREATE_CLUBMAN;  // 创建棍棒兵
        else if(actNum == BUILDING_ARMYCAMP_UPGRADE_CLUBMAN) actName = ACT_ARMYCAMP_UPGRADE_CLUBMAN;  // 升级棍棒兵
        else if(actNum == BUILDING_ARMYCAMP_CREATE_SLINGER) actName = ACT_ARMYCAMP_CREATE_SLINGER;  // 创建投石兵
    }
    else if(Num == BUILDING_MARKET)  // 市场
    {
        if(actNum == BUILDING_MARKET_WOOD_UPGRADE) actName = ACT_UPGRADE_WOOD;  // 升级木材采集
        else if(actNum == BUILDING_MARKET_STONE_UPGRADE) actName = ACT_UPGRADE_STONE;  // 升级石头采集
        else if(actNum == BUILDING_MARKET_FARM_UPGRADE) actName = ACT_UPGRADE_FARM;  // 升级农场
        else if(actNum == BUILDING_MARKET_GOLD_UPGRADE) actName = ACT_UPGRADE_GOLD;  // 升级黄金采集
    }
    else if( Num == BUILDING_RANGE)  // 靶场
    {
        if(actNum == BUILDING_RANGE_CREATE_BOWMAN) actName = ACT_RANGE_CREATE_BOWMAN;  // 创建弓箭手
    }
    else if(Num == BUILDING_STABLE)  // 马厩
    {
        if(actNum == BUILDING_STABLE_CREATE_SCOUT) actName =ACT_STABLE_CREATE_SCOUT;  // 创建侦察兵
    }
}


/*******战斗相关*******/
// 获取攻击距离
double Building::getDis_attack()
{
    if(getNum() == BUILDING_ARROWTOWER)  // 如果是箭塔
        return ( dis_Attack + playerScience->get_addition_DisAttack(getSort(),Num,0,get_AttackType()) )*BLOCKSIDELENGTH;  // 计算实际攻击距离
    else return 0;
}

/********************虚函数**************************/


/********************静态函数**************************/
// 释放建造中建筑图像资源
void Building::deallocatebuild(int i)
{
    delete build[i];
    build[i] = nullptr;
}

// 释放已建成建筑图像资源
void Building::deallocatebuilt(int i,int j)
{
    delete built[i][j];
    built[i][j] = nullptr;
}

// 释放建筑着火效果图像资源
void Building::deallocatebuildFire(int type)
{
    if(buildFire[type]!=nullptr)
    {
        delete buildFire[type];
        buildFire[type] = nullptr;
    }
}

/********************静态函数**************************/


/********************************************/
/*****************act相关***************/
// 更新建筑建造进度
void Building::update_Build()
{
    double ratio = get_retio_Build();  // 获取建造速度

    if(!constructed)  // 如果未建造完成
    {
        Percent+=ratio;  // 增加建造进度
        if(Percent>100) Percent = 100;  // 限制最大进度
    }

    if(!isDie())  // 如果建筑未死亡
        Blood+=ratio/100;  // 增加生命值

    if(Blood>1) Blood = 1;  // 限制最大生命值
}

// 更新建筑动作进度
void Building::update_Action(){
    actPercent += actSpeed;  // 增加动作进度
    if(actPercent > 100) actPercent = 100;  // 限制最大进度
}

// 设置动作状态
void Building::setActStatus(int wood , int food , int stone , int gold)
{
    int actionName, actionNumber;

    for(int position = 0; position<ACT_WINDOW_NUM_FREE; position++)
    {
        actionName = getActNames(position);  // 获取动作名称
        actionNumber = ActNameToActNum(actionName);  // 获取动作编号

        // 检查资源是否足够执行动作
        if(actionNumber>-1 && !playerScience->get_isBuildActionAble(Num, actionNumber, get_civilization(), wood, food, stone, gold))
            actStatus[position] = ACT_STATUS_DISABLED;  // 禁用动作
        else actStatus[position] = ACT_STATUS_ENABLED;  // 启用动作
    }
}

// 获取建造速度
double Building::get_retio_Build()
{
    if(is_cheatAction) return 100.0;  // 作弊模式
    else return 100.0/playerScience->get_buildTime(Num)/FRAMES_PER_SECOND;  // 正常建造速度
}

// 获取动作速度
double Building::get_retio_Action()
{
    if(is_cheatAction) return 100.0;  // 作弊模式
    else return 100.0/playerScience->get_actTime(Num, actNum)/FRAMES_PER_SECOND;  // 正常动作速度
}


/*******状态与属性设置、获取*******/
// 检查资源类型是否匹配
bool Building::isMatchResourceType(int resourceType)
{
    if(Num == BUILDING_CENTER)  // 市镇中心可以存放所有资源
        return true;

    if(Num == BUILDING_STOCK && ( resourceType == HUMAN_WOOD || resourceType == HUMAN_GOLD || resourceType == HUMAN_STONE || resourceType == HUMAN_STOCKFOOD ))
        return true;  // 仓库可以存放木材、黄金、石头和食物

    if(Num == BUILDING_GRANARY &&  resourceType == HUMAN_GRANARYFOOD )
        return true;  // 谷仓只能存放谷物

    return false;
}

// 设置地基属性
void Building::setFundation()
{
    //设置地基大小
    switch (Foundation) {
    case FOUNDATION_SMALL:  // 小型地基
        BlockSizeLen = SIZELEN_SMALL;
        crashLength = CRASHBOX_SMALL;
        break;
    case FOUNDATION_MIDDLE:  // 中型地基
        BlockSizeLen = SIZELEN_MIDDLE;
        crashLength = CRASHBOX_MIDDLE;
        break;
    case FOUNDATION_BIG:  // 大型地基
        BlockSizeLen = SIZELEN_BIG;
        crashLength = CRASHBOX_BIG;
        break;
    default:
        break;
    }
}


/***************状态与图像显示****************/
// 设置建筑着火效果
void Building::setFireNowRes()
{
    std::list<ImageResource>* tempNowlist = NULL;

    // 根据生命值选择着火效果
    if(Blood <= BUILDING_BLOOD_FIRE_BIG)
        tempNowlist = buildFire[BUILDING_FIRE_BIG];  // 大火
    else if(Blood <= BUILDING_BLOOD_FIRE_MIDDLE)
        tempNowlist = buildFire[BUILDING_FIRE_MIDDLE];  // 中火
    else if(Blood <= BUILDING_BLOOD_FIRE_SMALL)
        tempNowlist = buildFire[BUILDING_FIRE_SMALL];  // 小火

    if(fireNowList != tempNowlist)
    {
        fireNowList = tempNowlist;
        if(fireNowList != NULL)
            fireNowRes = fireNowList->begin();
    }

    return;
}
