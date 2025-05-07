#include "Animal.h"



// 各种动物状态的图像资源

std::string Animal::Animalname[5]={"Tree","Gazelle","Elephant","Lion","Forest"};

std::string Animal::Animalcarcassname[5]={"Fallen_Tree","Gazelle","Elephant","Lion","Forest_Stool"};

std::string Animal::AnimalDisplayName[5]={"树","羚羊","大象","狮子","森林"};

std::list<ImageResource>* Animal::Walk[5][8];      // 行走

std::list<ImageResource>* Animal::Stand[5][8];     // 站立

std::list<ImageResource>* Animal::Attack[5][8];    // 攻击

std::list<ImageResource>* Animal::Die[5][8];       // 死亡

std::list<ImageResource>* Animal::Run[5][8];       // 奔跑

std::list<ImageResource>* Animal::Disappear[5][8];  // 消失





// 音效

std::string Animal::sound_click[5] = {\

    "", "", "Elephant_Stand", "Lion_Stand", ""

};





// 动物属性

// 树木 羚羊， 大象 狮子， 森林

// 最大血量

int Animal::AnimalMaxBlood[5] = { BLOOD_TREE, BLOOD_GAZELLE, BLOOD_ELEPHANT, BLOOD_LION, BLOOD_FARMER };

// 资源类型

int Animal::AnimalResouceSort[5] = { HUMAN_WOOD, HUMAN_STOCKFOOD, HUMAN_STOCKFOOD, HUMAN_STOCKFOOD, HUMAN_WOOD };

// 资源数量

int Animal::AnimalCnt[5] = { CNT_TREE, CNT_GAZELLE, CNT_ELEPHANT, CNT_LION, CNT_FOREST };

// 动画帧间隔

int Animal::AnimalNowresStep[5] = { 0, 0, NOWRES_TIMER_ELEPHANT, NOWRES_TIMER_LION, 0 };



// 视野范围

int Animal::AnimalVision[5] = { 0, VISION_GAZELLE, VISION_ELEPHANT, VISION_LION, 0};

// 碰撞长度

double Animal::AnimalCrashLen[5] = { CRASHBOX_SINGLEBLOCK, CRASHBOX_SINGLEOB, CRASHBOX_BIGOB, CRASHBOX_SMALLOB, CRASHBOX_SMALLBLOCK };

// 移动速度

double Animal::AnimalSpeed[5] = { 0, ANIMAL_SPEED, SPEED_ELEPHANT, ANIMAL_SPEED, 0 };



// 友好关系类型

int Animal::AnimalFriendly[5] = { FRIENDLY_NULL, FRIENDLY_FRI, FRIENDLY_FENCY, FRIENDLY_ENEMY, FRIENDLY_NULL };

// 是否可攻击

bool Animal::AnimalAttackable[5] = { false, false, true, true, false };

// 基础攻击力

int Animal::AnimalAtk[5] = { 0, 0, 10, 2, 0 };





// - Num:0:树 1:羚羊 2:大象 3:狮子 4:森林

// - DR: 初始位置的DR坐标

// - UR: 初始位置的UR坐标

Animal::Animal(int Num, double DR, double UR)

{

    this->Num=Num;



    setDRUR(DR, UR);                // 设置当前位置坐标

    setDR0UR0(DR, UR);              // 设置目标位置坐标

    setPredictedDRUR(DR, UR);        // 设置预测位置坐标

    setPreviousDRUR(DR, UR);         // 设置上一帧位置坐标



    updateBlockByDetail();           // 根据细节坐标更新所在地图块

    this->visible = 0;               // 初始可见性0



    setAttribute();                  // 设置属性



    this->state = ANIMAL_STATE_IDLE;  // 初始状态为闲置

    setNowRes();                     // 显示资源

    updateImageXYByNowRes();          // 更新图像位置



    // 全局唯一编号并加入全局对象表

    this->globalNum=10000*getSort()+g_globalNum;

    g_Object.insert({this->globalNum,this});

    g_globalNum++;

}



// 更新动物的下一帧状态和图像

// 根据当前状态(闲置/行走/攻击等)选择适当的动画帧并显示

void Animal::nextframe()

{

    if(isDie())  // 死亡状态

    {

        if( !isDying() )  // 未开始死亡动画

        {

            setPreDie();

            changeToGatherAble();  // 可采集

            requestSound_Die();    // 死亡音效

        }

        else if(!get_isActionEnd() && isNowresShift()) nowres++;  // 正在死亡中，更新动画帧

        else if(!changeToDisappear && !is_Surplus())  // 死亡动画结束且资源已耗尽

        {

            changeToDisappear = true;  // 消失

            if(!isTree())

            {

                nowres_step = 1000;

                setNowRes();

            }

        }

    }

    else  // 存活状态

    {

        if(Num!=ANIMAL_TREE && Num!=ANIMAL_FOREST)  // 除了木材和森林外可以移动

        {



            if(isNowresShift())  // 切换动画帧

            {

                if(nowres == nowlist->begin() && nowstate == MOVEOBJECT_STATE_ATTACK)

                    requestSound_Attack();  // 播放攻击音效



                this->nowres++;  // 前进到下一帧

                if(this->nowres==nowlist->end())

                {

                    nowres=nowlist->begin();  // 循环播放

                    initAttack_perCircle();     // 重置每次攻击循环参数

                }

            }



            updateMove();  // 更新移动状态

        }

        this->imageX=this->nowres->pix.width()/2.0;   // 设置图像中心点X

        this->imageY=this->nowres->pix.width()/4.0;   // 设置图像中心点Y

    }

}



// 根据动物类型、当前状态和朝向选择适当的图像资源

void Animal::setNowRes()

{

    std::list<ImageResource> *templist = NULL;



    if(isTree())  

    {

        if(nowstate == MOVEOBJECT_STATE_DIE)  // 被砍伐

        {

            nowlist=this->Die[this->Num][this->Angle];

            this->nowres = nowlist->begin();

        }

        else  // 正常状态

        {

            nowlist=this->Stand[this->Num][this->Angle];

            nowres = next(nowlist->begin(),treeState);  // 多种外观

        }

    }else  

    {

        switch (this->nowstate) {

        case 0:  // 站立

            templist=this->Stand[this->Num][this->Angle];

            break;

        case MOVEOBJECT_STATE_WALK:  // 行走

            if(changeToRun && Num != ANIMAL_ELEPHANT) templist=this->Run[this->Num][this->Angle];  // 奔跑状态(大象不会跑)

            else  templist=this->Walk[this->Num][this->Angle];  // 普通行走

            break;

        case MOVEOBJECT_STATE_ATTACK:  // 攻击

            templist=this->Attack[this->Num][this->Angle];

            break;

        case MOVEOBJECT_STATE_DIE:  // 死亡

            if(changeToDisappear)templist = this->Disappear[this->Num][this->Angle];  // 尸体消失

            else templist=this->Die[this->Num][this->Angle];  // 死亡

            break;

        default:

            break;

        }



        if(templist!=nowlist && templist)  // 切换动画序列

        {

            nowlist = templist;

            nowres = nowlist->begin();  // 第一帧

            initNowresTimer();      // 初始化动画计时器

            initAttack_perCircle();  // 初始化攻击循环参数

        }

    }

}




// 设置动物属性

void Animal::setAttribute()

{

    // 检查类型是否有效

    if(Num < 0 || Num >= 5)

    {

        incorrectNum = true;

        return;

    }



    // 从静态属性数组获取基本属性

    MaxBlood = AnimalMaxBlood[Num];         // 最大血量

    resourceSort = AnimalResouceSort[Num];   // 资源类型

    MaxCnt = AnimalCnt[Num];               // 资源数量

    nowres_step = AnimalNowresStep[Num];   // 动画帧间隔



    vision = AnimalVision[Num];            // 视野范围

    crashLength = AnimalCrashLen[Num];     // 碰撞长度

    speed = AnimalSpeed[Num];             // 移动速度

    Friendly = AnimalFriendly[Num];        // 友好关系

    isAttackable = AnimalAttackable[Num];  // 是否可攻击

    atk = AnimalAtk[Num];                 // 攻击力

    this->Cnt = this->MaxCnt;             // 资源数量最大值

    this->gatherable = false;              // 不可采集

    this->Blood = 1;                      // 血量满



    if(Num == ANIMAL_FOREST)

        BlockSizeLen = SIZELEN_SMALL;



    // 树木和森林的固定属性

    if(Num == ANIMAL_FOREST || Num == ANIMAL_TREE)

    {

        this->Angle = 0;                  // 固定角度



        moveAble = false;                   // 不能移动

        treeState = rand()%Stand[this->Num][this->Angle]->size();  // 随机树

    }

    else

        this->Angle = rand()%8;           // 随机8个方向



    // 大象属性

    if(Num == ANIMAL_ELEPHANT)

    {

        dis_Attack = DISTANCE_ATTACK_CLOSE_BIGOB;  // 攻击距离

        isRangeAttack = true;                     // 可以范围攻击

    }



    // 大象和狮子的攻击类型

    if(Num == ANIMAL_ELEPHANT || Num == ANIMAL_LION)

        attackType = ATTACKTYPE_ANIMAL;



    setSideLenth();  // 碰撞盒子

}



// 判断是否监视特定对象,狮子会监视敌军、农民和羚羊(想吃)

// 羚羊会监视敌军、农民和狮子(以便逃跑)

bool Animal::isMonitorObject(Coordinate* judOb)

{

    int judNum = judOb->getNum(), judSort = judOb->getSort();

    // 狮子监视军事单位和羚羊

    if(Num == ANIMAL_LION)

        return judSort == SORT_ARMY || judSort == SORT_FARMER || (judSort == SORT_ANIMAL && judNum == ANIMAL_GAZELLE);

    // 羚羊监视军事单位和狮子

    if(Num == ANIMAL_GAZELLE)

        return judSort == SORT_ARMY || judSort == SORT_FARMER || (judSort == SORT_ANIMAL && judNum == ANIMAL_LION);

    return false;

}





/*******战斗相关*******/

// 获取额外攻击加成

int Animal::get_add_specialAttack()

{

    int addition = 0;

    // 大象对箭塔和城墙有额外伤害

    if(Num == ANIMAL_ELEPHANT)

    {

        if(interactSort == SORT_BUILDING )

        {

            if(interactNum == BUILDING_ARROWTOWER) addition +=40;      // 对箭塔+40攻击

            else if(interactNum == BUILDING_WALL) addition +=80;      // 对城墙+80攻击

        }

    }



    return addition;

}

// 树木和森林死亡时有砍伐声，动物有死亡音效

void Animal::requestSound_Die()

{

    if(!isInWidget())  // 不在当前视图

        return;



    if(!isTree()) 

        soundQueue.push("Animal_Die");

}


// 根据动物类型播放不同的攻击音效

void Animal::requestSound_Attack()

{

    if(!isInWidget())  

        return;

    // 根据动物类型播放对应的攻击音效

    if(Num == ANIMAL_LION)

        soundQueue.push("Lion_Attack");

    else if(Num == ANIMAL_ELEPHANT)

        soundQueue.push("Elephant_Attack");

}

// 释放行走动画资源

void Animal::deallocateWalk(int i, int j)

{

    delete Walk[i][j];

    Walk[i][j] = nullptr;

}



// 释放站立动画资源

void Animal::deallocateStand(int i, int j)

{

    delete Stand[i][j];

    Stand[i][j] = nullptr;

}



// 释放攻击动画资源

void Animal::deallocateAttack(int i, int j)

{

    delete Attack[i][j];

    Attack[i][j] = nullptr;

}



// 释放死亡动画资源

void Animal::deallocateDie(int i, int j)

{

    delete Die[i][j];

    Die[i][j] = nullptr;

}



// 释放奔跑动画资源

void Animal::deallocateRun(int i, int j)

{

    delete Run[i][j];

    Run[i][j] = nullptr;

}



// 释放消失动画资源

void Animal::deallocateDisappear(int num, int angle)

{

    delete Disappear[num][angle];

    Disappear[num][angle] = nullptr;

}
