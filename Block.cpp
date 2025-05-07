#include "Block.h"

// 地图块图像资源数组 - 存储不同类型地形的图像资源
std::list<ImageResource>* Block::block[BLOCK_COUNT]; // 普通显示模式下的地形块图像
std::list<ImageResource>* Block::grayblock[BLOCK_COUNT]; // 灰色显示模式下的地形块图像（可能用于未探索区域）
std::list<ImageResource>* Block::blackblock[BLOCK_COUNT]; // 黑色显示模式下的地形块图像（完全未探索区域）

// 地图块类型名称数组 - 存储不同地形类型的名称
std::string Block::Blockname[BLOCK_COUNT]={"Grass","Desert", // 基本地形：草地、沙漠
                                  "Grass_Border_111","Grass_Border_112","Grass_Border_113","Grass_Border_114", // 草地边界变种111-114
                                  "Grass_Border_121","Grass_Border_122","Grass_Border_123","Grass_Border_124","Grass_Border_125","Grass_Border_126", // 草地边界变种121-126
                                  "Grass_Border_131","Grass_Border_132","Grass_Border_133","Grass_Border_134", // 草地边界变种131-134
                                  "Grass_Border_140", // 草地边界变种140
                                  "Grass_002","Grass_003","Grass_004","Grass_005","Grass_006","Grass_007", // 草地变种002-007
                                  "Grass_008","Grass_009","Grass_010","Grass_011","Grass_012","Grass_013"}; // 草地变种008-013

// 默认构造函数 - 创建基本地形块（平坦草地）
Block::Block()
{
    this->Num = 0; // 设置默认地形块编号为0（草地）
    this->nowres = block[this->Num]->begin(); // 设置当前显示资源为地形块的第一帧
    this->OffsetX = 0; // X轴偏移量为0
    this->OffsetY = 0; // Y轴偏移量为0
    this->Type = MAPTYPE_FLAT; // 地形类型为平坦
    this->Pattern = MAPPATTERN_GRASS; // 地形图案为草地
    this->Height = MAPHEIGHT_FLAT; // 地形高度为平坦
    this->Resource = RESOURCE_EMPTY; // 地形资源为空（无资源）
}

// 更新地形块的下一帧动画（当地形有动画效果时使用）
// 目前为空实现，说明当前地形块没有动画效果
void Block::nextframe()
{

}

// 获取地形高度
// 返回值: 地形块的高度级别
int Block::getMapHeight() const
{
    return Height;
}

// 设置地形高度
// 参数: value - 新的高度级别
void Block::setMapHeight(int value)
{
    Height = value;
}

// 获取地形图案
// 返回值: 地形块的图案类型（如草地、沙漠等）
int Block::getMapPattern() const
{
    return Pattern;
}

// 设置地形图案
// 参数: value - 新的图案类型
void Block::setMapPattern(int value)
{
    Pattern = value;
}

// 获取地形资源类型
// 返回值: 地形块上的资源类型（如木材、石头、金矿等）
int Block::getMapResource() const
{
    return Resource;
}

// 设置地形资源类型
// 参数: value - 新的资源类型
void Block::setMapResource(int value)
{
    Resource = value;
}

// 获取X轴偏移量
// 返回值: 地形块在X轴上的偏移像素
int Block::getOffsetX() const
{
    return OffsetX;
}

// 设置X轴偏移量
// 参数: value - 新的X轴偏移像素
void Block::setOffsetX(int value)
{
    OffsetX = value;
}

// 获取Y轴偏移量
// 返回值: 地形块在Y轴上的偏移像素
int Block::getOffsetY() const
{
    return OffsetY;
}

// 设置Y轴偏移量
// 参数: value - 新的Y轴偏移像素
void Block::setOffsetY(int value)
{
    OffsetY = value;
}

// 获取地形类型
// 返回值: 地形块的类型（如平坦、山丘、水域等）
int Block::getMapType() const
{
    return Type;
}

// 设置地形类型
// 参数: value - 新的地形类型
void Block::setMapType(int value)
{
    Type = value;
}