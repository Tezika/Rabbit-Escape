#include "HelloWorldScene.h"
#include <sstream>

const int NUM_INITSTONE = 8;
const int NUM_DEATALSTONE = 4;

const string STR_STEP = "Step:";
const string STR_BEST = "Best:";

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    //load ctrl
    m_ctrl = new GameCtrl();
    
    //load studio resources/Users/Tezika/Projects/LearnDemos/Rabbit Escape/Rabbit/Resources/res
    auto pScene = CSLoader::createNode("MainScene.csb");
    this->addChild(pScene);
    
    //获取所有节点
    m_pNodes= pScene->getChildByName("Sprite_bg");
    
    //分别的到兔子和石头
    for (auto it = m_pNodes->getChildren().begin(); it!=m_pNodes->getChildren().end(); it++) {
          if ((*it)->getName()=="Sprite_Rabbit") {
              m_pNode_rabbit = (*it);
          }
        
          else{
              m_pNode_stones.pushBack((*it));
        }
            
    }
    
    
    //add listener;
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBeganCallBack,this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    //获取UITEXT
    m_pText_curStep = m_pNodes->getChildByName<Text*>("Text_curStep");
    m_pText_bestStep =  m_pNodes->getChildByName<Text*>("Text_bestStep");
 
    
    return true;
}

void HelloWorld::setRandStones(int num){
    int curNum = 0;
    //随机产生石头
    while (num!=curNum) {
     int ver,hor,tag;
        ver = 1+rand_0_1()*(NUM_MAPROWANDCOW-2);
        hor = 1+rand_0_1()*(NUM_MAPROWANDCOW-2);
        tag = ver*NUM_MAPROWANDCOW+hor;
        if(!m_pNodes->getChildByTag(tag)->isVisible()&&!(ver==NUM_MAPROWANDCOW/2&&hor==NUM_MAPROWANDCOW/2)){
            m_pNodes->getChildByTag(tag)->setVisible(true);
            m_ctrl->addStone(tag);
            curNum++;
        }
        else{
            continue;
        }

    }
}
void HelloWorld::onEnter(){
    Layer::onEnter();
    //设置随机的障碍
    this->setRandStones(NUM_INITSTONE+CCRANDOM_0_1()*NUM_DEATALSTONE);
    //设置UI中的Text
    this->setText(m_pText_curStep,m_ctrl->getStep());
    this->setText(m_pText_bestStep,UserDefault::getInstance()->getIntegerForKey(STR_BEST.c_str()));
    
}
void HelloWorld::onExit(){
    Layer::onExit();
    delete m_ctrl;
    
}
void HelloWorld::setText(Text* pTxt,int num){
    std::stringstream stream;
    stream<<num;
    string str;
    stream>>str;
    if(m_pText_curStep==pTxt){
       pTxt->setString(STR_STEP+str);
    }
    else{
        //如果不是第一次设置
        if(num!=VAL_MAX){
          pTxt->setString(STR_BEST+str);
        }
        else{
            pTxt->setString(STR_BEST);
        }
    }
    
}
bool HelloWorld::onTouchBeganCallBack(Touch *t, Event* event){
    for (auto it =m_pNode_stones.begin();it!=m_pNode_stones.end() ; it++) {
        if((*it)->getBoundingBox().containsPoint(t->getLocation())&&(!(*it)->isVisible())){
            
            //add stone
            auto stone = (*it);
            stone->setVisible(true);
            m_ctrl->addStone(stone->getTag());
            
            //判断是否失败
            if (m_ctrl->judgeFailed()) {
                MessageBox("没办法，AI就是任性", " =.=,兔子跑了!!");
                Director::getInstance()->replaceScene(HelloWorld::createScene());

            }
            
            //判断是否胜利
            else if (m_ctrl->judgeVictory()) {
                
                //判断是否超过最高分数
                if (UserDefault::getInstance()->getIntegerForKey(STR_BEST.c_str())>m_ctrl->getStep()) {
                    UserDefault::getInstance()->setIntegerForKey(STR_BEST.c_str(), m_ctrl->getStep());
                }
                
                //设置messageBox上面显示的信息
                char str_info[60];
                sprintf(str_info, "当前成绩为: %d,最好成绩为: %d",m_ctrl->getStep(),UserDefault::getInstance()->getIntegerForKey(STR_BEST.c_str()));
                MessageBox(str_info, "orz,兔子被围!!");
                Director::getInstance()->replaceScene(HelloWorld::createScene());
            }
            
            //move rabbit
            else{
                 int moveTag = m_ctrl->rabbitMove();
                 this->setText(m_pText_curStep,m_ctrl->getStep());
                 m_pNode_rabbit->runAction(MoveTo::create(0.1, m_pNodes->getChildByTag(moveTag)->getPosition()));
                 break;
            }
        }
    }
    return false;
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
