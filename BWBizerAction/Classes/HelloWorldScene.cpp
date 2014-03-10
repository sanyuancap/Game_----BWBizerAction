

#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"


using namespace cocos2d;
using namespace CocosDenshion;

#include "BWActionInterval.h"


CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}


CCSprite* pSprite = NULL;
// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                        "CloseNormal.png",
                                        "CloseSelected.png",
                                        this,
                                        menu_selector(HelloWorld::menuCloseCallback) );
    pCloseItem->setPosition( ccp(CCDirector::sharedDirector()->getWinSize().width - 20, 20) );

    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition( CCPointZero );
    this->addChild(pMenu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    CCLabelTTF* pLabel = CCLabelTTF::create("Hello World", "Thonburi", 34);

    // ask director the window size
    CCSize size = CCDirector::sharedDirector()->getWinSize();

    // position the label on the center of the screen
    pLabel->setPosition( ccp(size.width / 2, size.height - 20) );

    // add the label as a child to this layer
    this->addChild(pLabel, 1);
    
    
    pSprite = CCSprite::create("Icon-72.png");
    pSprite->setPosition( ccp(size.width/2, size.height/2) );
    //pSprite->setPosition( ccp(0, 0) );
    this->addChild(pSprite, 0);
    
    
//    BWJumpBy* pJump = BWJumpBy::create(5, ccp(600,0), 100, 5);
//    pSprite->runAction(pJump);
//    BWBezierConfig confi;
//    confi.controlPoint_1 = ccp(-100,150);
//    confi.controlPoint_2 = ccp(100,200);
//    confi.endPosition = ccp(0,250);
//    BWBezierBy* pBezier = BWBezierBy::create(5, confi);
//    CCRepeat* pRepeat = CCRepeat::create(pBezier, 2);
//    pSprite->runAction(pRepeat);
    
//    ccBezierConfig confi;
//    confi.controlPoint_1 = ccp(-100,150);
//    confi.controlPoint_2 = ccp(100,200);
//    confi.endPosition = ccp(0,250);
//    CCBezierBy* pBezier = CCBezierBy::create(5, confi);
//    CCRepeat* pRepeat = CCRepeat::create(pBezier, 2);
//    pSprite->runAction(pRepeat);
    
    
//    BWSineBy* pSineBy = BWSineBy::create(3, ccp(200,200), 100);
//    CCRepeat* pRepeat = CCRepeat::create(pSineBy, 3);
//    pSprite->runAction(pRepeat);
    
    BWCircleBy* pCircleBy1 = BWCircleBy::create(3, ccp(-200,0), 100);
    //pCircleBy->setClock(true);
    BWCircleBy* pCircleBy2 = BWCircleBy::create(3, ccp(200,0), 100);
    pCircleBy2->setClock(true);
    BWCircleBy* pCircleBy3 = BWCircleBy::create(3, ccp(0,200), 100);
    pCircleBy2->setClock(true);
    BWCircleBy* pCircleBy4 = BWCircleBy::create(3, ccp(0,-200), 100);
    pCircleBy2->setClock(true);
    CCSequence* pSeq = CCSequence::create(pCircleBy1,pCircleBy2,pCircleBy3,pCircleBy4,NULL);
    
    CCRepeat* pRepeat = CCRepeat::create(pSeq, 2);

    pSprite->runAction(pRepeat);

    
    schedule(schedule_selector(HelloWorld::pushPoint));
    return true;
}

void HelloWorld::pushPoint()
{
    m_vTouchPoint.push_back(pSprite->getPosition());
    
    return;
}

void HelloWorld::draw()
{
    ccPointSize(5);
	ccDrawColor4B(0,0,255,128);
    
    for(int i=0;i<m_vTouchPoint.size();i++)
    {
        CCPoint pos = m_vTouchPoint[i];
        ccDrawPoint(pos);
    }
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
