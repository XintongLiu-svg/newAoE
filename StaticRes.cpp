#include "StaticRes.h"

std::list<ImageResource>* StaticRes::staticResource[3];
// 静态资源：浆果丛、石头、金矿
std::string StaticRes::StaticResname[3]={"Bush","Stone","GoldOre"};
std::string StaticRes::StaticResDisplayName[3] = {"浆果丛","石头","金矿"};

// 通过详细坐标创建静态资源
StaticRes::StaticRes(int Num, double DR, double UR)
{
    this->Num=Num;  // 设置资源类型编号

    setDRUR(DR, UR);  // 设置详细坐标
    updateBlockByDetail();  // 根据详细坐标更新块坐标
    setNowRes();  // 设置当前图像资源
    setAttribute();  // 设置资源属性

    // 设置全局编号并注册到全局对象列表
    this->globalNum=g_globalNum;
    g_Object.insert({this->globalNum,this});
    g_globalNum++;
}

// 通过块坐标创建静态资源
StaticRes::StaticRes(int Num, int BlockDR, int BlockUR)
{
    this->Num=Num;  
    setBlockDRUR(BlockDR, BlockUR);  

    setNowRes();  
    setAttribute();  
    setDetailPointAttrb_FormBlock();  

    this->globalNum=10000*getSort()+g_globalNum;
    g_Object.insert({this->globalNum,this});
    g_globalNum++;
}

// 设置静态资源的属性
void StaticRes::setAttribute()
{
    switch(Num){
    case NUM_STATICRES_Bush:  // 浆果丛
        MaxCnt = CNT_BUSH;  // 最大采集量
        resourceSort = HUMAN_GRANARYFOOD;  // 资源类型：食物
        BlockSizeLen = SIZELEN_SINGEL;  // 占地面积：单个单位
        crashLength = CRASHBOX_SINGLEOB;  // 碰撞箱大小：单个单位
        break;
    case NUM_STATICRES_Stone:  // 石头
        MaxCnt = CNT_STONE;  // 最大采集量
        resourceSort = HUMAN_STONE;  // 资源类型：石头
        BlockSizeLen = SIZELEN_SMALL;  // 占地面积：小型
        crashLength = CRASHBOX_SMALLBLOCK;  // 碰撞箱大小：小型
        break;
    case NUM_STATICRES_GoldOre:  // 金矿
        MaxCnt = CNT_GOLDORE;  // 最大采集量
        resourceSort = HUMAN_GOLD;  // 资源类型：黄金
        BlockSizeLen = SIZELEN_SMALL;  // 占地面积：小型
        crashLength = CRASHBOX_SMALLBLOCK;  // 碰撞箱大小：小型
        break;
    default:
        incorrectNum = true;  
        break;
    }
    setSideLenth();  
    this->Cnt = MaxCnt;  
}

// 设置当前图像资源
void StaticRes::setNowRes()
{
    int listLen;
    nowlist = staticResource[Num];  // 获取对应资源类型的图像资源列表
    listLen = nowlist->size();  // 获取图像资源列表长度

    // 随机选择一个图像资源
    nowres = next(nowlist->begin(), rand()%listLen);

    updateImageXYByNowRes();  
    this->imageH=DR-UR;  
}
