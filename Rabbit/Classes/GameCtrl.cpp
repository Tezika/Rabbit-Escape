//
//  GameCtrl.cpp
//  StudioStudy
//
//  Created by TonyZefkia on 1/22/15.
//
//

#include "GameCtrl.h"



GameCtrl::GameCtrl(){
    m_step = 0;
    this->initMoveVec();
    this->initMapInfo();
}


void GameCtrl::initMoveVec(){
    //创建移动HASH
    m_moveVec[(int)Dir::up] = new Vec(-1,0);
    m_moveVec[(int)Dir::down] = new Vec(1,0);
    m_moveVec[(int)Dir::left] = new Vec(0,-1);
    m_moveVec[(int)Dir::right] = new Vec(0,1);
    m_moveVec[(int)Dir::upLeft] = new Vec(-1,-1);
    m_moveVec[(int)Dir::upRight] = new Vec(-1,1);
    m_moveVec[(int)Dir::downLeft] = new Vec(1,-1);
    m_moveVec[(int)Dir::downRight]= new Vec(1,1);
    
    //创建映射hash
    m_moveDirs[m_moveVec[(int)Dir::up]]=(int)Dir::up;
    m_moveDirs[m_moveVec[(int)Dir::down]]=(int)Dir::down;
    m_moveDirs[m_moveVec[(int)Dir::left]]=(int)Dir::left;
    m_moveDirs[m_moveVec[(int)Dir::right]]=(int)Dir::right;
    m_moveDirs[m_moveVec[(int)Dir::upLeft]]=(int)Dir::upLeft;
    m_moveDirs[m_moveVec[(int)Dir::upRight]]=(int)Dir::upRight;
    m_moveDirs[m_moveVec[(int)Dir::downLeft]]=(int)Dir::downLeft;
    m_moveDirs[m_moveVec[(int)Dir::downRight]]=(int)Dir::downRight;


}
void GameCtrl::initMapInfo(){
    
    //初始化地图
    for (int i = 0; i<NUM_MAPROWANDCOW; i++) {
        vector<Pos *> curRow;
        for (int j = 0; j<NUM_MAPROWANDCOW; j++) {
            auto pNewPos = new Pos(i,j);
            curRow.push_back(pNewPos);
           
        }
        m_gameMap.push_back(curRow);
    }
    
    //初始化兔子信息
    m_rabbitPos.hor=m_rabbitPos.ver = NUM_MAPROWANDCOW/2;
}

void GameCtrl::addStone(int tag){
    m_gameMap[tag/NUM_MAPROWANDCOW][tag%NUM_MAPROWANDCOW]->isStone = true;
}

int GameCtrl::rabbitMove() {
    Dir d = this->getRabbitMoveDir();
    if (Dir::none!=d) {
     //步数增加
      m_step++;
      m_rabbitPos=(*m_moveVec[(int)d])+m_rabbitPos;
      return m_rabbitPos.ver*NUM_MAPROWANDCOW+m_rabbitPos.hor;
    }
    else{
        return VAL_MAX;
    }
}

Dir GameCtrl::getRabbitMoveDir(){
    Dir minDir = Dir::none;
    int minFVal = VAL_MAX;
    //遍历可走方向,使用A*算法
    vector<Vec* > moveDirs;
    this->getCurDirs(moveDirs,m_rabbitPos);
    for(auto it:moveDirs){
        if(!m_gameMap[m_rabbitPos.ver+it->ver][m_rabbitPos.hor+it->hor]->isStone) {
            this->setTargetPos((Dir)m_moveDirs[it]);
            AStart();
            //如果小于最小价值
            if (minFVal>m_targetPos->fVal) {
                minFVal =m_targetPos->fVal;
                minDir  = static_cast<Dir>(m_moveDirs[it]);
            }
            this->resetMapFVal(); 
        }
        
    }
    //如果已经被围住,采用最大通路算法
    if (Dir::none==minDir) {
        minDir  = maxRoadMove();
    }
    return minDir;
}
void GameCtrl::setTargetPos(const Dir& d){
    int size = NUM_MAPROWANDCOW-1;
    switch(d){
        case  Dir::up:
            m_targetPos = m_gameMap[0][m_rabbitPos.hor];
            break;
        case Dir::down:
            m_targetPos = m_gameMap[size][m_rabbitPos.hor];
            break;
        case Dir::left:
             m_targetPos = m_gameMap[m_rabbitPos.ver][0];
            break;
        case Dir::right:
             m_targetPos = m_gameMap[m_rabbitPos.ver][size];
             break;
        case Dir::upLeft:
             m_targetPos = m_rabbitPos.ver<m_rabbitPos.hor?m_gameMap[0][m_rabbitPos.hor-m_rabbitPos.ver]:m_gameMap[m_rabbitPos.ver-m_rabbitPos.hor][0];
             break;
        case Dir::downLeft:
            m_targetPos = (size-m_rabbitPos.ver)<m_rabbitPos.hor?m_gameMap[size][m_rabbitPos.hor+m_rabbitPos.ver-size]:m_gameMap[m_rabbitPos.ver+m_rabbitPos.hor][0];
             break;
        case Dir::upRight:
            m_targetPos = m_rabbitPos.ver<(size-m_rabbitPos.hor)?m_gameMap[0][m_rabbitPos.ver+m_rabbitPos.hor]:m_gameMap[m_rabbitPos.ver+m_rabbitPos.hor-size][size];
            break;
        case Dir::downRight:
            m_targetPos = (size-m_rabbitPos.ver)<(size-m_rabbitPos.hor)?m_gameMap[size][m_rabbitPos.hor+size-m_rabbitPos.ver]:m_gameMap[m_rabbitPos.ver+size-m_rabbitPos.hor][size];
            break;
        default:
            break;
    }
}
void GameCtrl::AStart(){
    //清空上次的数据
    m_openPos.clear();
    m_closePos.clear();
    
    //将兔子的位置放进open
    m_openPos.push_back(m_gameMap[m_rabbitPos.ver][m_rabbitPos.hor]);
    
    //初始化当前的花费
    int curCost = 0;
    
    while (!m_openPos.empty()) {
        //找出open中最小的S
        Pos* minPos = NULL;
        int minFval = VAL_MAX;
        int minIndex =  0;
        for (int i =0 ; i<m_openPos.size(); i++) {
            if (minFval>=m_openPos.at(i)->fVal) {
                minFval = m_openPos.at(i)->fVal;
                minPos = m_openPos.at(i);
                minIndex  = i;
            }
        }
        //open中剔除
        m_openPos.erase(m_openPos.begin()+minIndex);

        //加入close
        m_closePos.push_back(minPos);
        
        if (NULL!=minPos){
        //如果找到目标就退出
          if(m_targetPos==minPos) {
                break;
             }
            curCost++;
            vector<Vec*> moveDirs;
            this->getCurDirs(moveDirs,minPos->vec);
            for (auto it: moveDirs) {
                this->updateDirsFVal(m_gameMap[minPos->vec.ver+it->ver][minPos->vec.hor+it->hor],curCost);
             }
            }
        }
}

void GameCtrl::updateDirsFVal(Pos* newPos,int curCost){
//如果该点不是障碍
  if(!newPos->isStone){
    //如果该店不在close中
      if (std::find(m_closePos.begin(),m_closePos.end(), newPos)==m_closePos.end()) {
        //如果该点也不再open中
              if (std::find(m_openPos.begin(),m_openPos.end(), newPos)==m_openPos.end()) {
                newPos->cost=curCost;
                //计算H姑凉值
                newPos->hEstimate = this->funcHEstimate(newPos,m_targetPos);
                newPos->fVal = newPos->cost+newPos->hEstimate;
                //放进open
                m_openPos.push_back(newPos);
              }
            //如果在则刷新
              else{
                  int tfVal  = curCost+this->funcHEstimate(newPos,m_targetPos);
                  if (newPos->fVal>tfVal) {
                    newPos->cost= curCost;
                    //计算H姑凉值
                    newPos->hEstimate = this->funcHEstimate(newPos,m_targetPos);
                    newPos->fVal = newPos->cost+newPos->hEstimate;
                    
                  }
            }
          }
      }
}

void GameCtrl::getCurDirs(vector<Vec*>& moveDirs,const Vec& curVec){
    if (curVec.ver!=0) {
        moveDirs.push_back(m_moveVec[(int)Dir::up]);
    }
    if (curVec.ver!=NUM_MAPROWANDCOW-1) {
         moveDirs.push_back(m_moveVec[(int)Dir::down]);
    }
    if (curVec.hor!=0) {
         moveDirs.push_back(m_moveVec[(int)Dir::left]);
        //如果当前行数为偶数，增加上左和下左方向
        if (curVec.ver%2==0){
            if(curVec.ver!=0){
                moveDirs.push_back(m_moveVec[(int)Dir::upLeft]);
            }
            if (curVec.ver!=NUM_MAPROWANDCOW-1) {
                moveDirs.push_back(m_moveVec[(int)Dir::downLeft]);
            }
        }
    }
    //如果当前为计数,增加上右和下右方向
    if (curVec.hor!=NUM_MAPROWANDCOW-1) {
         moveDirs.push_back(m_moveVec[(int)Dir::right]);
        if (curVec.ver%2!=0){
            if(curVec.ver!=0){
                moveDirs.push_back(m_moveVec[(int)Dir::upRight]);
            }
            if (curVec.ver!=NUM_MAPROWANDCOW-1) {
                moveDirs.push_back(m_moveVec[(int)Dir::downRight]);
            }
        }

    }
    
}

int GameCtrl::funcHEstimate(Pos* p1,Pos* p2){
    return abs(p1->vec.ver-p2->vec.ver)+abs(p1->vec.hor-p2->vec.hor);
}

void GameCtrl::resetMapFVal(){
    for (int i = 0; i<NUM_MAPROWANDCOW; i++) {
        for (int j = 0; j<NUM_MAPROWANDCOW; j++) {
            m_gameMap[i][j]->fVal = VAL_MAX;
        }
    }

}

Dir GameCtrl::maxRoadMove(){
    //定义规格
    int size = NUM_MAPROWANDCOW-1;
   //记录最长距离
    int maxL =  0;
   //记录返回的方向
    Dir maxDir = Dir::none;
    vector<Vec*> moveDirs;
    this->getCurDirs(moveDirs,m_rabbitPos);
    for (auto it:moveDirs) {
        int curL = 0;
         Vec curVec = m_rabbitPos+(*it);
        //如果当前不是石头并且不是边界
        while (!m_gameMap[curVec.ver][curVec.hor]->isStone&&curVec.ver>0&&curVec.ver<size&&curVec.hor>0&&curVec.hor<size){
            curL++;
            curVec = curVec +(*it);
        }
        if (maxL<curL) {
            maxL = curL;
            maxDir = (Dir)m_moveDirs[it];
        }
    }
    return maxDir;
}
bool GameCtrl::judgeVictory(){
    //获取当前可遍历的方向
    vector<Vec*> moveDirs;
    this->getCurDirs(moveDirs,m_rabbitPos);
    for (auto it:moveDirs) {
        if (!m_gameMap[m_rabbitPos.ver+it->ver][m_rabbitPos.hor+it->hor]->isStone){
            return false;
        }
    }
    return true;
}

bool GameCtrl::judgeFailed(){
    if (m_rabbitPos.hor==0||m_rabbitPos.ver==0||m_rabbitPos.hor==NUM_MAPROWANDCOW-1||m_rabbitPos.ver==NUM_MAPROWANDCOW-1) {
         return  true;
    }
    return  false;
}
GameCtrl::~GameCtrl(){
    //析构地图信息
    for (int i  = 0; i<NUM_MAPROWANDCOW; i++) {
        for (int j = 0; j<NUM_MAPROWANDCOW; j++) {
            delete m_gameMap[i][j];
        }
    }
    
    
    //析构移动矢量
    for (auto& it:m_moveVec) {
        delete it.second;
    }
    
}




