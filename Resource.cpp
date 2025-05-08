#include "Resource.h"

// 资源类构造函数
Resource::Resource()
{
}

// 根据资源类型返回应该存放在哪种建筑中
int Resource::get_ReturnBuildingType()
{
    int buildingType;
    switch (resourceSort)
    {
    case HUMAN_WOOD:      // 木材资源
    case HUMAN_GOLD:      // 黄金资源
    case HUMAN_STONE:     // 石头资源
    case HUMAN_STOCKFOOD: // 食物资源
        buildingType = BUILDING_STOCK;  // 存放在仓库中
        break;
    case HUMAN_GRANARYFOOD:  // 谷物资源
        buildingType = BUILDING_GRANARY;  // 存放在粮仓中
        break;
    default:
        buildingType = BUILDING_CENTER;  // 默认存放在城镇中心
        break;
    }

    return buildingType;
}
