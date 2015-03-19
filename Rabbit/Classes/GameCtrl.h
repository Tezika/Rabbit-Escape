//
//  GameCtrl.h
//  StudioStudy
//
//  Created by TonyZefkia on 1/22/15.
//
//

#ifndef __StudioStudy__GameCtrl__
#define __StudioStudy__GameCtrl__

#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

const int NUM_MAPROWANDCOW  = 9;
const int NUM_DIR = 4;
const int VAL_MAX =  0x0FFFFFFF;


//方向枚举
enum class Dir{
    up,
    down,
    left,
    right,
    upLeft,
    upRight,
    downLeft,
    downRight,
    none
};

//地图矢量
struct Vec{
    int hor;
    int ver;
    Vec(){
        hor =  0;
        ver = 0;
    }
    Vec(int _ver,int _hor){
        hor = _hor;
        ver =  _ver;
    }
    bool operator == (const Vec& vec ){
        return vec.hor==hor&&vec.ver==ver;
    }
    
    Vec operator + (const Vec& vec){
      return Vec(vec.ver+ver,vec.hor+hor);
    }

};

//地图位置
struct Pos{
    //cost代价
    int cost;
    //距离目标位置的的估计值
    int hEstimate;
    //A*中的F值
    int fVal;
    //是否是石头;
    bool isStone;
    //位置信息
    Vec vec;
    Pos(){
        cost = 0;
        hEstimate = 0;
        fVal = VAL_MAX;
        isStone = false;
    }
    Pos(int _ver,int _hor){
        vec.ver = _ver;
        vec.hor = _hor;
        cost = 0;
        hEstimate = 0;
        fVal = VAL_MAX;
        isStone = false;

    }
};



class GameCtrl{
public:
    GameCtrl();
    ~GameCtrl();
public:
    //添加石头
    void addStone(int tag);
    //移动兔子
    int rabbitMove();
    //判断胜利
    bool judgeVictory();
    //判断失败
    bool judgeFailed();
    //得到当前游戏步数
    int  getStep(){return m_step;};
private:
    //游戏地图
    vector<vector<Pos *>>m_gameMap;
    //移动距离
    std::unordered_map<int,Vec*> m_moveVec;
    std::unordered_map<Vec*,int> m_moveDirs;
    
    //初始化地图移动矢量
    void initMoveVec();
    //初始花地图信息
    void initMapInfo();
    
    /////////////A*////////////////
    //open集合
    vector<Pos *> m_openPos;
    //close
    vector<Pos *> m_closePos;
    //目标点
    Pos*  m_targetPos;
    //设置目标点
    void setTargetPos(const Dir& d);
     //A*寻路
    void AStart();
    //H估径函数
    int  funcHEstimate(Pos* p1, Pos* p2);
    //刷新周围的点
    void updateDirsFVal(Pos* newPos,int curCost);
    //重置地图F值
    void resetMapFVal();
    //获得当前的可遍历的方向
    void getCurDirs(vector<Vec*>& moveDirs,const Vec& curVec);
    
    /////////////最大通路////////////////
    //获得当前最大通路的方向
    Dir maxRoadMove();
    //兔子信息
    Vec m_rabbitPos;
    Dir getRabbitMoveDir();
    
    //当前移动步数
    int m_step;
    
};
#endif /* defined(__StudioStudy__GameCtrl__) */
