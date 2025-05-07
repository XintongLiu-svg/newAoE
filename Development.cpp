#include "Development.h"

// 科技系统默认构造函数
Development::Development()
{
    init_DevelopLab();  // 初始化科技树
}

// 科技系统构造函数
// 参数说明：
// represent: 玩家标识
Development::Development(int represent)
{
    playerRepresent = represent;  // 设置玩家标识
    init_DevelopLab();  // 初始化科技树
}

// 获取移动速度倍率
// 参数说明：
// sort: 单位类型
// type: 具体单位编号
double Development::get_rate_Move(int sort , int type)
{
    double rate = 1;  // 基础移动速度倍率
    if(sort == SORT_FARMER) rate += rate_FarmerMove;  // 农民获得额外移动速度加成

    return rate;
}

// 获取生命值倍率
// 参数说明：
// sort: 单位类型
// type: 具体单位编号
double Development::get_rate_Blood(int sort , int type)
{
    double rate = 1;  // 基础生命值倍率
    if(sort == SORT_FARMER) rate+= rate_FarmerBlood;  // 农民获得额外生命值加成

    return rate;
}

// 获取生命值加成
// 参数说明：
// sort: 单位类型
// type: 具体单位编号
int Development::get_addition_Blood( int sort , int type )
{
    int addition = 0;  // 基础生命值加成
    return addition;
}

// 获取攻击力倍率
// 参数说明：
// sort: 单位类型
// type: 具体单位编号
// armyClass: 军队类型
// attackType: 攻击类型
// interSort: 目标类型
// interNum: 目标编号
double Development::get_rate_Attack( int sort , int type , int armyClass , int attackType, int interSort, int interNum )
{
    double rate = 1;  // 基础攻击力倍率

    // 对建筑的特殊攻击加成
    if(interSort!=-1 && interNum!=-1)
    {
        if(interSort == SORT_BUILDING && sort == SORT_ARMY && (type == AT_SWORDSMAN || type == AT_CAVALRY || type == AT_IMPROVED))
            rate = 2;  // 剑士、骑兵和改良单位对建筑造成双倍伤害
    }

    return rate;
}

// 获取攻击力加成
// 参数说明：
// sort: 单位类型
// type: 具体单位编号
// armyClass: 军队类型
// attackType: 攻击类型
int Development::get_addition_Attack( int sort , int type , int armyClass , int attackType )
{
    int level = 0;
    int addition = 0;
    
    // 近战攻击加成
    if(attackType == ATTACKTYPE_CLOSE)
    {
        level = getActLevel(BUILDING_STOCK,BUILDING_STOCK_UPGRADE_USETOOL);
        switch (level) {
        case 1:
            addition+=BUILDING_STOCK_UPGRADE_USETOOL_ADDITION_ATKCLOSE;  // 工具利用升级提供近战攻击加成
        default:
            break;
        }
    }
    
    // 投石兵特殊攻击加成
    if(sort == SORT_ARMY && type == AT_SLINGER)
    {
        level = getActLevel(BUILDING_MARKET,BUILDING_MARKET_STONE_UPGRADE);
        switch (level) {
        case 1:
            addition+=BUILDING_MARKET_STONE_UPGRADE_ADDITION_SILNGERATK;  // 石头升级提供投石兵攻击加成
        default:
            break;
        }
    }

    return addition;
}

// 获取攻击距离加成
// 参数说明：
// sort: 单位类型
// type: 具体单位编号
// armyClass: 军队类型
// attackType: 攻击类型
int Development::get_addition_DisAttack( int sort, int type , int armyClass , int attackType )
{
    int addition = 0;
    int level = 0;

    // 投石兵特殊攻击距离加成
    if(sort == SORT_ARMY && type == AT_SLINGER)
    {
        level = getActLevel(BUILDING_MARKET,BUILDING_MARKET_STONE_UPGRADE);
        switch (level) {
        case 1:
            addition+=BUILDING_MARKET_STONE_UPGRADE_ADDITION_SILNGERDIS;  // 石头升级提供投石兵攻击距离加成
        default:
            break;
        }
    }
    // 远程攻击距离加成
    else if(attackType == ATTACKTYPE_SHOOT)
    {
        level = getActLevel(BUILDING_MARKET,BUILDING_MARKET_WOOD_UPGRADE);
        switch (level) {
        case 1:
            addition+=BUILDING_MARKET_WOOD_UPGRADE_ADDITION_DISSHOOT;  // 木材升级提供远程攻击距离加成
        default:
            break;
        }
    }

    return addition;
}

// 获取防御力倍率
// 参数说明：
// sort: 单位类型
// type: 具体单位编号
// armyClass: 军队类型
// attackType_got: 受到的攻击类型
double Development::get_rate_Defence( int sort , int type , int armyClass , int attackType_got )
{
    double rate = 1;  // 基础防御力倍率
    return rate;
}

// 获取防御力加成
// 参数说明：
// sort: 单位类型
// type: 具体单位编号
// armyClass: 军队类型
// attackType_got: 受到的攻击类型
int Development::get_addition_Defence( int sort , int type , int armyClass , int attackType_got )
{
    double addition = 0;
    int level = 0;

    // 军队单位防御加成
    if( sort == SORT_ARMY )
    {
        // 近战和动物攻击防御加成
        if(attackType_got == ATTACKTYPE_ANIMAL || attackType_got == ATTACKTYPE_CLOSE)
        {
            // 步兵护甲加成
            if(armyClass == ARMY_INFANTRY)
            {
                level = getActLevel(BUILDING_STOCK , BUILDING_STOCK_UPGRADE_DEFENSE_INFANTRY);
                switch (level) {
                case 1:
                    addition+=BUILDING_STOCK_UPGRADE_DEFENSE_INFANTRY_ADDITION_DEFENSE_INFANTRY;  // 步兵护甲升级提供防御加成
                default:
                    break;
                }
            }
            // 弓兵护甲加成
            if(armyClass == ARMY_ARCHER)
            {
                level = getActLevel(BUILDING_STOCK , BUILDING_STOCK_UPGRADE_DEFENSE_ARCHER);
                switch (level) {
                case 1:
                    addition+=BUILDING_STOCK_UPGRADE_DEFENSE_ARCHER_ADDITION_DEFENSE_ARCHER;  // 弓兵护甲升级提供防御加成
                default:
                    break;
                }
            }
            // 骑兵护甲加成
            if(armyClass == ARMY_RIDER)
            {
                level = getActLevel(BUILDING_STOCK , BUILDING_STOCK_UPGRADE_DEFENSE_RIDER);
                switch (level) {
                case 1:
                    addition+=BUILDING_STOCK_UPGRADE_DEFENSE_RIDER_ADDITION_DEFENSE_RIDER;  // 骑兵护甲升级提供防御加成
                default:
                    break;
                }
            }
        }
        // 远程攻击防御加成（铜器时代实现）
        else if(attackType_got == ATTACKTYPE_SHOOT)
        {
            // 铜器时代再编写
        }
    }
    return addition;
}

// 获取资源携带量加成
// 参数说明：
// resourceSort: 资源类型
int Development::get_addition_ResourceSort( int resourceSort )
{
    int addition = 0;
    int level = 0;
    
    // 木材携带量加成
    if(resourceSort == HUMAN_WOOD)
    {
        level = getActLevel(BUILDING_MARKET , BUILDING_MARKET_WOOD_UPGRADE);
        switch (level) {
        case 1:
            addition+=BUILDING_MARKET_WOOD_UPGRADE_ADDITION_CARRY;  // 木材升级提供携带量加成
        default:
            break;
        }
    }
    // 石头携带量加成
    else if(resourceSort == HUMAN_STONE)
    {
        level = getActLevel(BUILDING_MARKET , BUILDING_MARKET_STONE_UPGRADE);
        switch (level) {
        case 1:
            addition+=BUILDING_MARKET_STONE_UPGRADE_ADDITION_CARRY;  // 石头升级提供携带量加成
        default:
            break;
        }
    }
    // 黄金携带量加成
    else if(resourceSort == HUMAN_GOLD)
    {
        level = getActLevel(BUILDING_MARKET , BUILDING_MARKET_GOLD_UPGRADE);
        switch (level) {
        case 1:
            addition+=BUILDING_MARKET_GOLD_UPGRADE_ADDITION_CARRY;  // 黄金升级提供携带量加成
        default:
            break;
        }
    }

    return addition;
}

// 获取资源采集速度倍率
// 参数说明：
// resourceSort: 资源类型
double Development::get_rate_ResorceGather( int resourceSort )
{
    int rate = 1;  // 基础采集速度倍率
    int level = 0;
    
    // 木材采集速度加成
    if(resourceSort == HUMAN_WOOD)
    {
        level = getActLevel(BUILDING_MARKET , BUILDING_MARKET_WOOD_UPGRADE);
        switch (level) {
        case 1:
            rate+=BUILDING_MARKET_WOOD_UPGRADE_ADDITION_GATHERRATE;  // 木材升级提供采集速度加成
        default:
            break;
        }
    }
    // 石头采集速度加成
    else if(resourceSort == HUMAN_STONE)
    {
        level = getActLevel(BUILDING_MARKET , BUILDING_MARKET_STONE_UPGRADE);
        switch (level) {
        case 1:
            rate+=BUILDING_MARKET_STONE_UPGRADE_ADDITION_GATHERRATE;  // 石头升级提供采集速度加成
        default:
            break;
        }
    }
    // 黄金采集速度加成
    else if(resourceSort == HUMAN_GOLD)
    {
        level = getActLevel(BUILDING_MARKET , BUILDING_MARKET_GOLD_UPGRADE);
        switch (level) {
        case 1:
            rate+=BUILDING_MARKET_GOLD_UPGRADE_ADDITION_GATHERRATE;  // 黄金升级提供采集速度加成
        default:
            break;
        }
    }

    return rate;
}

// 获取最大数量加成
// 参数说明：
// sort: 单位类型
// type: 具体单位编号
int Development::get_addition_MaxCnt( int sort , int type )
{
    int addition = 0 , level = 0;

    // 农田食物产量加成
    if(sort == SORT_Building_Resource && type == BUILDING_FARM)
    {
        level = getActLevel(BUILDING_MARKET , BUILDING_MARKET_FARM_UPGRADE);
        switch (level) {
        case 1:
            addition+=BUILDING_MARKET_FARM_UPGRADE_ADDITION_FOOD;  // 农田升级提供食物产量加成
        default:
            break;
        }
    }

    return addition;
}

// 完成建筑动作
// 参数说明：
// buildingType: 建筑类型
// buildact: 建筑动作
void Development::finishAction(int buildingType , int buildact)
{
    // 如果是市镇中心升级，则进入新时代
    if(buildingType == BUILDING_CENTER && buildact == BUILDING_CENTER_UPGRADE) 
        civiChange();

    developLab[buildingType].finishAction(buildact);  // 完成建筑动作
}

// 检查建筑动作是否可执行
// 参数说明：
// buildingNum: 建筑编号
// actNum: 动作编号
// civilization: 文明等级
// wood,food,stone,gold: 资源数量
// oper: 操作结果指针
bool Development::get_isBuildActionAble( int buildingNum, int actNum, int civilization ,int wood, int food , int stone, int gold , int* oper )
{
    // 检查是否需要创建人口且是否有足够空间
    if(developLab[buildingNum].actCon[actNum].isNeedCreatObject() && !get_isHumanHaveSpace())
    {
        if(oper !=NULL) *oper = 1;
        return false;
    }
    return developLab[buildingNum].actCon[actNum].executable(civilization,wood,food,stone,gold);
}

// 设置文明等级
// 参数说明：
// civ: 文明等级
void Development::set_civilization( int civ )
{
    civilization = civ;
    // 检查市镇中心升级节点
    while(developLab[BUILDING_CENTER].actCon[BUILDING_CENTER_UPGRADE].nowExecuteNode!=NULL &&\
          developLab[BUILDING_CENTER].actCon[BUILDING_CENTER_UPGRADE].nowExecuteNode->civilization<civilization)
        developLab[BUILDING_CENTER].actCon[BUILDING_CENTER_UPGRADE].shift();
}

// 时代升级
void Development::civiChange()
{
    civilization++;  // 提升文明等级
    if(playerRepresent == NOWPLAYERREPRESENT)
        soundQueue.push("Age_Level_Up");  // 播放升级音效
}

// 解锁所有科技
void Development::all_technology_tree()
{
    map< int , st_buildAction >::iterator iter = developLab.begin(),itere = developLab.end();
    map<int , st_upgradeLab>::iterator iter1,iter1e;

    // 遍历所有建筑和动作
    while(iter != itere)
    {
        for(iter1 = iter->second.actCon.begin(),iter1e = iter->second.actCon.end(); iter1 != iter1e; iter1++)
        {
            // 解锁所有动作节点
            while(iter1->second.nowExecuteNode != NULL)
            {
                if(iter1->second.nowExecuteNode == iter1->second.endNode && iter1->second.nowExecuteNode->nextDevAction == iter1->second.endNode )
                    break;

                iter1->second.shift();
            }
        }
        iter++;
    }
}

// 初始化科技树
void Development::init_DevelopLab()
{
    conditionDevelop* newNode;

    // 市镇中心
    {
        // 创建市镇中心建筑条件
        developLab[BUILDING_CENTER].buildCon = new conditionDevelop( CIVILIZATION_IRONAGE , BUILDING_CENTER , TIME_BUILD_CENTER, BUILD_CENTER_WOOD );
        
        // 创建村民
        newNode = new conditionDevelop(CIVILIZATION_STONEAGE , BUILDING_CENTER , TIME_BUILDING_CENTER_CREATEFARMER,\
                                       0 ,BUILDING_CENTER_CREATEFARMER_FOOD );
        newNode->setCreatObjectAfterAction(SORT_FARMER );  // 设置创建农民
        developLab[BUILDING_CENTER].actCon[BUILDING_CENTER_CREATEFARMER].setHead(newNode);
        developLab[BUILDING_CENTER].actCon[BUILDING_CENTER_CREATEFARMER].endNodeAsOver();

        // 升级时代
        newNode = new conditionDevelop(CIVILIZATION_STONEAGE , BUILDING_CENTER , TIME_BUILDING_CENTER_UPGRADE,\
                                       0, BUILDING_CENTER_UPGRADE_FOOD);
        developLab[BUILDING_CENTER].actCon[BUILDING_CENTER_UPGRADE].setHead(newNode);
    }

    // 房屋
    developLab[BUILDING_HOME].buildCon =new conditionDevelop( CIVILIZATION_STONEAGE , BUILDING_HOME , TIME_BUILD_HOME , BUILD_HOUSE_WOOD );

    // 仓库
    {
        // 创建仓库建筑条件
        developLab[BUILDING_STOCK].buildCon = new conditionDevelop( CIVILIZATION_STONEAGE , BUILDING_STOCK , TIME_BUILD_STOCK , BUILD_STOCK_WOOD );

        // 升级工具利用
        newNode = new conditionDevelop( CIVILIZATION_TOOLAGE , BUILDING_STOCK , TIME_BUILDING_STOCK_UPGRADE_USETOOL , \
                                        0 ,BUILDING_STOCK_UPGRADE_USETOOL_FOOD);
        developLab[BUILDING_STOCK].actCon[BUILDING_STOCK_UPGRADE_USETOOL].setHead(newNode);
        
        // 升级步兵护甲
        newNode = new conditionDevelop(CIVILIZATION_TOOLAGE  , BUILDING_STOCK , TIME_BUILDING_STOCK_UPGRADE_DEFENSE_INFANTRY,\
                                       0, BUILDING_STOCK_UPGRADE_DEFENSE_INFANTRY_FOOD);
        developLab[BUILDING_STOCK].actCon[BUILDING_STOCK_UPGRADE_DEFENSE_INFANTRY].setHead(newNode);
        
        // 升级弓兵护甲
        newNode = new conditionDevelop(CIVILIZATION_TOOLAGE , BUILDING_STOCK , TIME_BUILDING_STOCK_UPGRADE_DEFENSE_ARCHER , \
                                       0, BUILDING_STOCK_UPGRADE_DEFENSE_ARCHER_FOOD);
        developLab[BUILDING_STOCK].actCon[BUILDING_STOCK_UPGRADE_DEFENSE_ARCHER].setHead(newNode);
        
        // 升级骑兵护甲
        newNode = new conditionDevelop(CIVILIZATION_TOOLAGE , BUILDING_STOCK , BUILDING_STOCK_UPGRADE_DEFENSE_RIDER , \
                                       0 , BUILDING_STOCK_UPGRADE_DEFENSE_RIDER_FOOD);
        developLab[BUILDING_STOCK].actCon[BUILDING_STOCK_UPGRADE_DEFENSE_RIDER].setHead(newNode);
    }

    // 谷仓
    {
        // 创建谷仓建筑条件
        developLab[BUILDING_GRANARY].buildCon = new conditionDevelop( CIVILIZATION_STONEAGE , BUILDING_GRANARY , TIME_BUILD_GRANARY , BUILD_GRANARY_WOOD );

        // 研发箭塔
        newNode = new conditionDevelop(CIVILIZATION_TOOLAGE , BUILDING_GRANARY , TIME_BUILDING_GRANARY_RESEARCH_ARROWTOWER , 0 , BUILDING_GRANARY_ARROWTOWER_FOOD );
        developLab[BUILDING_GRANARY].actCon[BUILDING_GRANARY_ARROWTOWER].setHead(newNode);
    }

    // 兵营
    {
        // 创建兵营建筑条件
        developLab[BUILDING_ARMYCAMP].buildCon = new conditionDevelop( CIVILIZATION_STONEAGE , BUILDING_ARMYCAMP , TIME_BUILD_ARMYCAMP , BUILD_ARMYCAMP_WOOD );

        // 创建棍棒兵
        newNode = new conditionDevelop(CIVILIZATION_STONEAGE , BUILDING_ARMYCAMP , TIME_BUILDING_ARMYCAMP_CREATE_CLUBMAN , 0 , BUILDING_ARMYCAMP_CREATE_CLUBMAN_FOOD);
        newNode->setCreatObjectAfterAction(SORT_ARMY , AT_CLUBMAN);  // 设置创建棍棒兵
        developLab[BUILDING_ARMYCAMP].actCon[BUILDING_ARMYCAMP_CREATE_CLUBMAN].setHead(newNode);
        developLab[BUILDING_ARMYCAMP].actCon[BUILDING_ARMYCAMP_CREATE_CLUBMAN].endNodeAsOver();
        
        // 升级棍棒兵为斧头兵
        newNode = new conditionDevelop(CIVILIZATION_TOOLAGE , BUILDING_ARMYCAMP , TIME_BUILDING_ARMYCAMP_UPGRADE_CLUBMAN , 0 , BUILDING_ARMYCAMP_UPGRADE_CLUBMAN_FOOD);
        developLab[BUILDING_ARMYCAMP].actCon[BUILDING_ARMYCAMP_UPGRADE_CLUBMAN].setHead(newNode);
        
        // 创建投石兵
        newNode = new conditionDevelop(CIVILIZATION_TOOLAGE , BUILDING_ARMYCAMP , TIME_BUILDING_ARMYCAMP_CREATE_SLINGER ,\
                                       0 , BUILDING_ARMYCAMP_CREATE_SLINGER_FOOD , BUILDING_ARMYCAMP_CREATE_SLINGER_STONE);
        newNode->setCreatObjectAfterAction(SORT_ARMY,AT_SLINGER);  // 设置创建投石兵
        developLab[BUILDING_ARMYCAMP].actCon[BUILDING_ARMYCAMP_CREATE_SLINGER].setHead(newNode);
        developLab[BUILDING_ARMYCAMP].actCon[BUILDING_ARMYCAMP_CREATE_SLINGER].endNodeAsOver();
    }

    // 市场
    {
        // 创建市场建筑条件
        developLab[BUILDING_MARKET].buildCon = new conditionDevelop( CIVILIZATION_TOOLAGE , BUILDING_MARKET , TIME_BUILD_MARKET , BUILD_MARKET_WOOD );
        developLab[BUILDING_MARKET].buildCon->addPreCondition(developLab[BUILDING_GRANARY].buildCon);  // 添加谷仓前置条件

        // 升级伐木
        {
            newNode = new conditionDevelop( CIVILIZATION_TOOLAGE , BUILDING_MARKET , TIME_BUILDING_MARKET_UPGRADE_CUTTING ,\
                                            BUILDING_MARKET_WOOD_UPGRADE_WOOD , BUILDING_MARKET_WOOD_UPGRADE_FOOD);
            developLab[BUILDING_MARKET].actCon[BUILDING_MARKET_WOOD_UPGRADE].setHead(newNode);
        }

        // 升级挖石头
        {
            newNode = new conditionDevelop( CIVILIZATION_TOOLAGE , BUILDING_MARKET , TIME_BUILDING_MARKET_UPGRADE_DIGGINGSOTNE ,\
                                            0, BUILDING_MARKET_STONE_UPGRADE_FOOD , BUILDING_MARKET_STONE_UPGRADE_STONE);
            developLab[BUILDING_MARKET].actCon[BUILDING_MARKET_STONE_UPGRADE].setHead(newNode);
        }

        // 升级农田
        {
            newNode = new conditionDevelop( CIVILIZATION_TOOLAGE , BUILDING_MARKET , TIME_BUILDING_MARKET_UPGRADE_FARM ,\
                                            BUILDING_MARKET_FARM_UPGRADE_WOOD , BUILDING_MARKET_FARM_UPGRADE_FOOD);
            developLab[BUILDING_MARKET].actCon[BUILDING_MARKET_FARM_UPGRADE].setHead(newNode);
        }
    }

    // 马厩
    {
        // 创建马厩建筑条件
        developLab[BUILDING_STABLE].buildCon = new conditionDevelop( CIVILIZATION_TOOLAGE , BUILDING_STABLE , TIME_BUILD_STABLE , BUILD_STABLE_WOOD );
        developLab[BUILDING_STABLE].buildCon->addPreCondition(developLab[BUILDING_ARMYCAMP].buildCon);  // 添加兵营前置条件

        // 创建侦察骑兵
        newNode = new conditionDevelop( CIVILIZATION_TOOLAGE , BUILDING_STABLE , TIME_BUILDING_STABLE_CREATE_SCOUT , 0 ,BUILDING_STABLE_CREATE_SCOUT_FOOD );
        newNode->setCreatObjectAfterAction(SORT_ARMY , AT_SCOUT);  // 设置创建侦察骑兵
        developLab[BUILDING_STABLE].actCon[BUILDING_STABLE_CREATE_SCOUT].setHead(newNode);
        developLab[BUILDING_STABLE].actCon[BUILDING_STABLE_CREATE_SCOUT].endNodeAsOver();
    }

    // 靶场
    {
        // 创建靶场建筑条件
        developLab[BUILDING_RANGE].buildCon = new conditionDevelop(CIVILIZATION_TOOLAGE , BUILDING_RANGE , TIME_BUILD_RANGE , BUILD_RANGE_WOOD);
        developLab[BUILDING_RANGE].buildCon->addPreCondition(developLab[BUILDING_ARMYCAMP].buildCon);  // 添加兵营前置条件

        // 创建弓箭手
        newNode = new conditionDevelop(CIVILIZATION_TOOLAGE , BUILDING_RANGE , TIME_BUILDING_RANGE_CREATE_BOWMAN ,\
                                       BUILDING_RANGE_CREATE_BOWMAN_WOOD , BUILDING_RANGE_CREATE_BOWMAN_FOOD);
        newNode->setCreatObjectAfterAction(SORT_ARMY,AT_BOWMAN);  // 设置创建弓箭手
        developLab[BUILDING_RANGE].actCon[BUILDING_RANGE_CREATE_BOWMAN].setHead(newNode);
        developLab[BUILDING_RANGE].actCon[BUILDING_RANGE_CREATE_BOWMAN].endNodeAsOver();
    }

    // 农田
    developLab[BUILDING_FARM].buildCon = new conditionDevelop(CIVILIZATION_TOOLAGE , BUILDING_FARM , TIME_BUILD_FARM , BUILD_FARM_WOOD);
    developLab[BUILDING_FARM].buildCon->addPreCondition(developLab[BUILDING_MARKET].buildCon);  // 添加市场前置条件

    // 箭塔
    developLab[BUILDING_ARROWTOWER].buildCon = new conditionDevelop( CIVILIZATION_TOOLAGE , BUILDING_ARROWTOWER , TIME_BUILD_ARROWTOWER ,0 ,0 , BUILD_ARROWTOWER_STONE );
    developLab[BUILDING_ARROWTOWER].buildCon->addPreCondition(developLab[BUILDING_GRANARY].actCon[BUILDING_GRANARY_ARROWTOWER].headAct);  // 添加箭塔研究前置条件
}
