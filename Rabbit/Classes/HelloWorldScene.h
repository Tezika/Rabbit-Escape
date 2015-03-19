#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "GameCtrl.h"

#include "editor-support/cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"


USING_NS_CC;
USING_NS_TIMELINE;
using namespace cocos2d::ui;

class HelloWorld : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
public:
    virtual void onEnter();
    virtual void onExit();
public:
    bool onTouchBeganCallBack(cocos2d::Touch* t,Event* event);
private:
    //设置随机石块
     void setRandStones(int num);
    //设置Text
    void setText(Text* pTxt,int num);
private:
    Node* m_pNodes;
    Node* m_pNode_rabbit;
    
    Text* m_pText_curStep;
    Text* m_pText_bestStep;
    
    Vector<Node*> m_pNode_stones;
    GameCtrl* m_ctrl;
   
};

#endif // __HELLOWORLD_SCENE_H__
