
//
//  BWBizerActionAppDelegate.h
//  BWBizerAction
//
//  Created by wangbin on 5/25/13.
//  Copyright __MyCompanyName__ 2013. All rights reserved.
//

#include "BWActionInterval.h"
#include "sprite_nodes/CCSprite.h"
#include "base_nodes/CCNode.h"
#include "support/CCPointExtension.h"
#include "CCStdC.h"
#include "CCActionInstant.h"
#include "cocoa/CCZone.h"
#include <stdarg.h>

USING_NS_CC;

BWActionInterval* BWActionInterval::create(float d)
{
    BWActionInterval *pAction = new BWActionInterval();
    pAction->initWithDuration(d);
    pAction->autorelease();

    return pAction;
}

bool BWActionInterval::initWithDuration(float d)
{
    m_fDuration = d;

    // prevent division by 0
    // This comparison could be in step:, but it might decrease the performance
    // by 3% in heavy based action games.
    if (m_fDuration == 0)
    {
        m_fDuration = FLT_EPSILON;
    }

    m_elapsed = 0;
    m_bFirstTick = true;
    m_bRotHead = true;

    return true;
}

CCObject* BWActionInterval::copyWithZone(CCZone *pZone)
{
    CCZone* pNewZone = NULL;
    BWActionInterval* pCopy = NULL;
    if(pZone && pZone->m_pCopyObject) 
    {
        //in case of being called at sub class
        pCopy = (BWActionInterval*)(pZone->m_pCopyObject);
    }
    else
    {
        pCopy = new BWActionInterval();
        pZone = pNewZone = new CCZone(pCopy);
    }
    CCFiniteTimeAction::copyWithZone(pZone);
    CC_SAFE_DELETE(pNewZone);
    pCopy->initWithDuration(m_fDuration);
    return pCopy;
}

bool BWActionInterval::isDone(void)
{
    return m_elapsed >= m_fDuration;
}

void BWActionInterval::step(float dt)
{
    if (m_bFirstTick)
    {
        m_bFirstTick = false;
        m_elapsed = 0;
    }
    else
    {
        m_elapsed += dt;
    }
    
    this->update(MAX (0,                                  // needed for rewind. elapsed could be negative
                      MIN(1, m_elapsed /
                          MAX(m_fDuration, FLT_EPSILON)   // division by 0
                          )
                      )
                 );
}

void BWActionInterval::setAmplitudeRate(float amp)
{
    CC_UNUSED_PARAM(amp);
    // Abstract class needs implementation
    CCAssert(0, "");
}

float BWActionInterval::getAmplitudeRate(void)
{
    // Abstract class needs implementation
    CCAssert(0, "");
    return 0;
}

void BWActionInterval::startWithTarget(CCNode *pTarget)
{
    CCFiniteTimeAction::startWithTarget(pTarget);
    m_elapsed = 0.0f;
    m_bFirstTick = true;
}

BWActionInterval* BWActionInterval::reverse(void)
{
    CCAssert(false, "CCIntervalAction: reverse not implemented.");
    return NULL;
}

//
// MoveBy
//

BWMoveBy* BWMoveBy::create(float duration, const CCPoint& deltaPosition)
{
    BWMoveBy *pRet = new BWMoveBy();
    pRet->initWithDuration(duration, deltaPosition);
    pRet->autorelease();

    return pRet;
}

bool BWMoveBy::initWithDuration(float duration, const CCPoint& deltaPosition)
{
    if (BWActionInterval::initWithDuration(duration))
    {
        m_positionDelta = deltaPosition;
        return true;
    }

    return false;
}

CCObject* BWMoveBy::copyWithZone(CCZone *pZone)
{
    CCZone* pNewZone = NULL;
    BWMoveBy* pCopy = NULL;
    if(pZone && pZone->m_pCopyObject) 
    {
        //in case of being called at sub class
        pCopy = (BWMoveBy*)(pZone->m_pCopyObject);
    }
    else
    {
        pCopy = new BWMoveBy();
        pZone = pNewZone = new CCZone(pCopy);
    }

    BWActionInterval::copyWithZone(pZone);

    pCopy->initWithDuration(m_fDuration, m_positionDelta);

    CC_SAFE_DELETE(pNewZone);
    return pCopy;
}

void BWMoveBy::startWithTarget(CCNode *pTarget)
{
    BWActionInterval::startWithTarget(pTarget);
    m_previousPosition = m_startPosition = pTarget->getPosition();
    
    float fRadian = atanf(m_positionDelta.y/m_positionDelta.x);
    if((m_positionDelta.y < 0 && m_positionDelta.x < 0)
       || (m_positionDelta.y < 0 && m_positionDelta.x > 0))
    {
        fRadian += M_PI;
    }
    m_fRotation = fRadian*180.0/M_PI;
}

BWActionInterval* BWMoveBy::reverse(void)
{
    return BWMoveBy::create(m_fDuration, ccp( -m_positionDelta.x, -m_positionDelta.y));
}


void BWMoveBy::update(float t)
{
    if (m_pTarget)
    {
#if CC_ENABLE_STACKABLE_ACTIONS
        CCPoint currentPos = m_pTarget->getPosition();
        CCPoint diff = ccpSub(currentPos, m_previousPosition);
        m_startPosition = ccpAdd( m_startPosition, diff);
        CCPoint newPos =  ccpAdd( m_startPosition, ccpMult(m_positionDelta, t) );
        m_pTarget->setPosition(newPos);
        m_previousPosition = newPos;
        
        if(m_bRotHead)
        m_pTarget->setRotation(m_fRotation);
#else
        m_pTarget->setPosition(ccpAdd( m_startPosition, ccpMult(m_positionDelta, t)));
#endif // CC_ENABLE_STACKABLE_ACTIONS
    }
}

//
// MoveTo
//

BWMoveTo* BWMoveTo::create(float duration, const CCPoint& position)
{
    BWMoveTo *pRet = new BWMoveTo();
    pRet->initWithDuration(duration, position);
    pRet->autorelease();

    return pRet;
}

bool BWMoveTo::initWithDuration(float duration, const CCPoint& position)
{
    if (BWActionInterval::initWithDuration(duration))
    {
        m_endPosition = position;
        return true;
    }

    return false;
}

CCObject* BWMoveTo::copyWithZone(CCZone *pZone)
{
    CCZone* pNewZone = NULL;
    BWMoveTo* pCopy = NULL;
    if(pZone && pZone->m_pCopyObject) 
    {
        //in case of being called at sub class
        pCopy = (BWMoveTo*)(pZone->m_pCopyObject);
    }
    else
    {
        pCopy = new BWMoveTo();
        pZone = pNewZone = new CCZone(pCopy);
    }

    BWMoveBy::copyWithZone(pZone);

    pCopy->initWithDuration(m_fDuration, m_endPosition);
    
    CC_SAFE_DELETE(pNewZone);
    return pCopy;
}

void BWMoveTo::startWithTarget(CCNode *pTarget)
{
    BWMoveBy::startWithTarget(pTarget);
    m_positionDelta = ccpSub( m_endPosition, pTarget->getPosition() );
    
    float fRadian = atanf(m_positionDelta.y/m_positionDelta.x);
    m_fRotation = fRadian*180.0/M_PI;
}


BWJumpBy* BWJumpBy::create(float duration, const CCPoint& position, float height, unsigned int jumps)
{
    BWJumpBy *pJumpBy = new BWJumpBy();
    pJumpBy->initWithDuration(duration, position, height, jumps);
    pJumpBy->autorelease();

    return pJumpBy;
}

bool BWJumpBy::initWithDuration(float duration, const CCPoint& position, float height, unsigned int jumps)
{
    if (BWActionInterval::initWithDuration(duration))
    {
        m_delta = position;
        m_height = height;
        m_nJumps = jumps;

        return true;
    }

    return false;
}

CCObject* BWJumpBy::copyWithZone(CCZone *pZone)
{
    CCZone* pNewZone = NULL;
    BWJumpBy* pCopy = NULL;
    if(pZone && pZone->m_pCopyObject) 
    {
        //in case of being called at sub class
        pCopy = (BWJumpBy*)(pZone->m_pCopyObject);
    }
    else
    {
        pCopy = new BWJumpBy();
        pZone = pNewZone = new CCZone(pCopy);
    }

    BWActionInterval::copyWithZone(pZone);

    pCopy->initWithDuration(m_fDuration, m_delta, m_height, m_nJumps);
    
    CC_SAFE_DELETE(pNewZone);
    return pCopy;
}

void BWJumpBy::startWithTarget(CCNode *pTarget)
{
    BWActionInterval::startWithTarget(pTarget);
    m_previousPos = m_startPosition = pTarget->getPosition();
}

void BWJumpBy::update(float t)
{
    // parabolic jump (since v0.8.2)
    if (m_pTarget)
    {
        float frac = fmodf( t * m_nJumps, 1.0f );
        float y = m_height * 4 * frac * (1 - frac);
        
       // float fA = m_height * 4 ;
       // float y =   - frac*frac * fA + fA * frac + 5;
        

        y += m_delta.y * t;
        float x = m_delta.x * t;
#if CC_ENABLE_STACKABLE_ACTIONS
        CCPoint currentPos = m_pTarget->getPosition();

        CCPoint diff = ccpSub( currentPos, m_previousPos );
        m_startPosition = ccpAdd( diff, m_startPosition);

        CCPoint newPos = ccpAdd( m_startPosition, ccp(x,y));
        m_pTarget->setPosition(newPos);
        m_previousPos = newPos;
        
        if(m_bRotHead)
        {
            CCPoint dirPoint = ccpSub( currentPos,newPos );
            float fRadian = atanf(dirPoint.y/dirPoint.x);
            if((dirPoint.y > 0 && dirPoint.x > 0)
               || (dirPoint.y < 0 && dirPoint.x > 0))
            {
                fRadian = M_PI_2 - fRadian;
            }
            else if((dirPoint.y < 0 && dirPoint.x < 0)||
                    (dirPoint.y > 0 && dirPoint.x < 0))
            {
                fRadian = -M_PI_2 - fRadian;
            }
            float m_fRotation = fRadian*180.0/M_PI;
            m_pTarget->setRotation(m_fRotation);
        }
       
#else
        m_pTarget->setPosition(ccpAdd( m_startPosition, ccp(x,y)));
#endif // !CC_ENABLE_STACKABLE_ACTIONS
    }
}

BWActionInterval* BWJumpBy::reverse(void)
{
    return BWJumpBy::create(m_fDuration, ccp(-m_delta.x, -m_delta.y),
        m_height, m_nJumps);
}

//
// JumpTo
//

BWJumpTo* BWJumpTo::create(float duration, const CCPoint& position, float height, int jumps)
{
    BWJumpTo *pJumpTo = new BWJumpTo();
    pJumpTo->initWithDuration(duration, position, height, jumps);
    pJumpTo->autorelease();

    return pJumpTo;
}

CCObject* BWJumpTo::copyWithZone(CCZone* pZone)
{
    CCZone* pNewZone = NULL;
    BWJumpTo* pCopy = NULL;
    if(pZone && pZone->m_pCopyObject)
    {
        //in case of being called at sub class
        pCopy = (BWJumpTo*)(pZone->m_pCopyObject);
    }
    else
    {
        pCopy = new BWJumpTo();
        pZone = pNewZone = new CCZone(pCopy);
    }

    BWJumpBy::copyWithZone(pZone);

    pCopy->initWithDuration(m_fDuration, m_delta, m_height, m_nJumps);
    
    CC_SAFE_DELETE(pNewZone);
    return pCopy;
}

void BWJumpTo::startWithTarget(CCNode *pTarget)
{
    BWJumpBy::startWithTarget(pTarget);
    m_delta = ccp(m_delta.x - m_startPosition.x, m_delta.y - m_startPosition.y);
}

// Bezier cubic formula:
//    ((1 - t) + t)3 = 1 
// Expands to¡­ 
//   (1 - t)3 + 3t(1-t)2 + 3t2(1 - t) + t3 = 1 
static inline float bezierat( float a, float b, float c, float d, float t )
{
    return (powf(1-t,3) * a + 
            3*t*(powf(1-t,2))*b + 
            3*powf(t,2)*(1-t)*c +
            powf(t,3)*d );
}

//
// BezierBy
//

BWBezierBy* BWBezierBy::create(float t, const BWBezierConfig& c)
{
    BWBezierBy *pBezierBy = new BWBezierBy();
    pBezierBy->initWithDuration(t, c);
    pBezierBy->autorelease();

    return pBezierBy;
}

bool BWBezierBy::initWithDuration(float t, const BWBezierConfig& c)
{
    if (BWActionInterval::initWithDuration(t))
    {
        m_sConfig = c;
        return true;
    }

    return false;
}

void BWBezierBy::startWithTarget(CCNode *pTarget)
{
    BWActionInterval::startWithTarget(pTarget);
    m_previousPosition = m_startPosition = pTarget->getPosition();
}

CCObject* BWBezierBy::copyWithZone(CCZone *pZone)
{
    CCZone* pNewZone = NULL;
    BWBezierBy* pCopy = NULL;
    if(pZone && pZone->m_pCopyObject) 
    {
        //in case of being called at sub class
        pCopy = (BWBezierBy*)(pZone->m_pCopyObject);
    }
    else
    {
        pCopy = new BWBezierBy();
        pZone = pNewZone = new CCZone(pCopy);
    }

    BWActionInterval::copyWithZone(pZone);

    pCopy->initWithDuration(m_fDuration, m_sConfig);
    
    CC_SAFE_DELETE(pNewZone);
    return pCopy;
}

void BWBezierBy::update(float time)
{
    if (m_pTarget)
    {
        float xa = 0;
        float xb = m_sConfig.controlPoint_1.x;
        float xc = m_sConfig.controlPoint_2.x;
        float xd = m_sConfig.endPosition.x;

        float ya = 0;
        float yb = m_sConfig.controlPoint_1.y;
        float yc = m_sConfig.controlPoint_2.y;
        float yd = m_sConfig.endPosition.y;

        float x = bezierat(xa, xb, xc, xd, time);
        float y = bezierat(ya, yb, yc, yd, time);

#if CC_ENABLE_STACKABLE_ACTIONS
        CCPoint currentPos = m_pTarget->getPosition();
        CCPoint diff = ccpSub(currentPos, m_previousPosition);
        m_startPosition = ccpAdd( m_startPosition, diff);

        CCPoint newPos = ccpAdd( m_startPosition, ccp(x,y));
        m_pTarget->setPosition(newPos);

        m_previousPosition = newPos;
        
        if(m_bRotHead)
        {
            CCPoint dirPoint = ccpSub(newPos, currentPos);
            float fRadian = atanf(dirPoint.y/dirPoint.x);
            
            
            if((dirPoint.y > 0 && dirPoint.x > 0)
               || (dirPoint.y < 0 && dirPoint.x > 0))
            {
                fRadian = M_PI_2 - fRadian;
            }
            else if((dirPoint.y < 0 && dirPoint.x < 0)||
                    (dirPoint.y > 0 && dirPoint.x < 0))
            {
                fRadian = -M_PI_2 - fRadian;
            }
            
            float m_fRotation = fRadian*180.0/M_PI;
            m_pTarget->setRotation(m_fRotation);
        }        
#else
        m_pTarget->setPosition(ccpAdd( m_startPosition, ccp(x,y)));
#endif // !CC_ENABLE_STACKABLE_ACTIONS
    }
}

BWActionInterval* BWBezierBy::reverse(void)
{
    BWBezierConfig r;

    r.endPosition = ccpNeg(m_sConfig.endPosition);
    r.controlPoint_1 = ccpAdd(m_sConfig.controlPoint_2, ccpNeg(m_sConfig.endPosition));
    r.controlPoint_2 = ccpAdd(m_sConfig.controlPoint_1, ccpNeg(m_sConfig.endPosition));

    BWBezierBy *pAction = BWBezierBy::create(m_fDuration, r);
    return pAction;
}

//
// BezierTo
//

BWBezierTo* BWBezierTo::create(float t, const BWBezierConfig& c)
{
    BWBezierTo *pBezierTo = new BWBezierTo();
    pBezierTo->initWithDuration(t, c);
    pBezierTo->autorelease();

    return pBezierTo;
}

bool BWBezierTo::initWithDuration(float t, const BWBezierConfig &c)
{
    bool bRet = false;
    
    if (BWActionInterval::initWithDuration(t))
    {
        m_sToConfig = c;
    }
    
    return bRet;
}

CCObject* BWBezierTo::copyWithZone(CCZone *pZone)
{
    CCZone* pNewZone = NULL;
    BWBezierBy* pCopy = NULL;
    if(pZone && pZone->m_pCopyObject) 
    {
        //in case of being called at sub class
        pCopy = (BWBezierTo*)(pZone->m_pCopyObject);
    }
    else
    {
        pCopy = new BWBezierTo();
        pZone = pNewZone = new CCZone(pCopy);
    }

    BWBezierBy::copyWithZone(pZone);

    pCopy->initWithDuration(m_fDuration, m_sConfig);
    
    CC_SAFE_DELETE(pNewZone);
    return pCopy;
}

void BWBezierTo::startWithTarget(CCNode *pTarget)
{
    BWBezierBy::startWithTarget(pTarget);
    m_sConfig.controlPoint_1 = ccpSub(m_sToConfig.controlPoint_1, m_startPosition);
    m_sConfig.controlPoint_2 = ccpSub(m_sToConfig.controlPoint_2, m_startPosition);
    m_sConfig.endPosition = ccpSub(m_sToConfig.endPosition, m_startPosition);
}

//
// BWSineBy
//

BWSineBy* BWSineBy::create(float t, CCPoint endPoint,float fA)
{
    BWSineBy *pSineBy = new BWSineBy();
    pSineBy->initWithDuration(t, endPoint,fA);
    pSineBy->autorelease();
    
    return pSineBy;
}

bool BWSineBy::initWithDuration(float t, CCPoint endPoint,float fA)
{
    if (BWActionInterval::initWithDuration(t))
    {
        m_endPosition = endPoint;
        m_fA = fA;
         return true;
    }
    
    return false;
}

void BWSineBy::startWithTarget(CCNode *pTarget)
{
    BWActionInterval::startWithTarget(pTarget);
    m_previousPosition = m_startPosition = pTarget->getPosition();
    float fDis =  ccpLength(m_endPosition);
    
    m_fW = 2*M_PI/fDis;
    

}

CCObject* BWSineBy::copyWithZone(CCZone *pZone)
{
    CCZone* pNewZone = NULL;
    BWSineBy* pCopy = NULL;
    if(pZone && pZone->m_pCopyObject)
    {
        //in case of being called at sub class
        pCopy = (BWSineBy*)(pZone->m_pCopyObject);
    }
    else
    {
        pCopy = new BWSineBy();
        pZone = pNewZone = new CCZone(pCopy);
    }
    
    BWActionInterval::copyWithZone(pZone);
    
    pCopy->initWithDuration(m_fDuration, m_endPosition,m_fA);
    
    CC_SAFE_DELETE(pNewZone);
    return pCopy;
}

void BWSineBy::update(float time)
{
    if (m_pTarget)
    {
        //CCLog("%f",time);
        CCPoint dir =  m_endPosition ;
        float fDis = ccpLength(m_endPosition);

        float fOldX = fDis * time;
        float fOldY = m_fA * sinf(m_fW * fOldX);
        float fOldC = sqrtf(powf(fOldX, 2)+powf(fOldY, 2));
        
        float fRotRadian = atanf(fOldY/fOldX) + atanf(dir.y/dir.x);
        if(m_endPosition.x < 0)
            fRotRadian += M_PI;
        float fRotOldX = fOldC * cos(fRotRadian);
        float fRotOldY = fOldC * sin(fRotRadian);
        
        float fNewX = fRotOldX + m_startPosition.x;
        float fNewY = fRotOldY + m_startPosition.y;
        
        CCPoint currentPos = m_pTarget->getPosition();
        CCPoint newPos = ccp( fNewX, fNewY);
        m_pTarget->setPosition(newPos);
        
        
        if(m_bRotHead)
        {
            CCPoint dirPoint = ccpSub(newPos, currentPos);
            float fRadian = atanf(dirPoint.y/dirPoint.x);
            if((dirPoint.y > 0 && dirPoint.x > 0)
               || (dirPoint.y < 0 && dirPoint.x > 0))
            {
                fRadian = M_PI_2 - fRadian;
            }
            else if((dirPoint.y < 0 && dirPoint.x < 0)||
                    (dirPoint.y > 0 && dirPoint.x < 0))
            {
                fRadian = -M_PI_2 - fRadian;
            }
            
            float m_fRotation = fRadian*180.0/M_PI;
            m_pTarget->setRotation(m_fRotation);
        }
    }
}

BWActionInterval* BWSineBy::reverse(void)
{
    BWBezierConfig r;
    BWSineBy *pAction = BWSineBy::create(m_fDuration, m_endPosition,m_fA);
    return pAction;
}


//
// BWSineBy
//

BWCircleBy* BWCircleBy::create(float t, CCPoint dirPoint,float fB)
{
    BWCircleBy *pCircleBy = new BWCircleBy();
    pCircleBy->initWithDuration(t, dirPoint,fB);
    pCircleBy->autorelease();
    
    return pCircleBy;
}

bool BWCircleBy::initWithDuration(float t, CCPoint dirPoint,float fB)
{
    if (BWActionInterval::initWithDuration(t))
    {
        m_dirPoint = dirPoint;
        m_fA = ccpLength(dirPoint);
        m_fB = fB;
        m_bClock = false;
        return true;
    }
    
    return false;
}

void BWCircleBy::startWithTarget(CCNode *pTarget)
{
    BWActionInterval::startWithTarget(pTarget);
    m_previousPosition = m_startPosition = pTarget->getPosition();
    float fDis =  ccpLength(m_dirPoint);
    m_fA = fDis*0.5;
}

CCObject* BWCircleBy::copyWithZone(CCZone *pZone)
{
    CCZone* pNewZone = NULL;
    BWCircleBy* pCopy = NULL;
    if(pZone && pZone->m_pCopyObject)
    {
        //in case of being called at sub class
        pCopy = (BWCircleBy*)(pZone->m_pCopyObject);
    }
    else
    {
        pCopy = new BWCircleBy();
        pZone = pNewZone = new CCZone(pCopy);
    }
    
    BWActionInterval::copyWithZone(pZone);
    
    pCopy->initWithDuration(m_fDuration, m_dirPoint,m_fB);
    
    CC_SAFE_DELETE(pNewZone);
    return pCopy;
}

void BWCircleBy::update(float time)
{
    if (m_pTarget)
    {
        //CCLog("%f",time);
        CCPoint dir =  m_dirPoint ;
        
        float fRadianMin = 0;
        float fRadianMax = 2*M_PI;
        
        if(m_dirPoint.x > 0)
        {
            fRadianMin = -M_PI;
            if(m_bClock)
            {
                time = 1-time;
            }
        }
        else
        {
            if(m_bClock)
            {
                time = 1-time;
            }
        }
        float fRadian = fRadianMin + fRadianMax * time;
        float fOldX = m_fA * cosf(fRadian);
        float fOldY = m_fB * sinf(fRadian);
        
        
        if(m_dirPoint.x > 0)
        {
            fOldX += m_fA;
        }
        else
        {
            fOldX -= m_fA;
        }
        float fOldC = sqrtf(powf(fOldX, 2)+powf(fOldY, 2));
        
        float fRotRadian = atanf(fOldY/fOldX) + atanf(dir.y/dir.x);
        if(fOldX < 0 )
            fRotRadian += M_PI;
        
        
        float fRotOldX = fOldC * cos(fRotRadian);
        float fRotOldY = fOldC * sin(fRotRadian);

        
        float fNewX = fRotOldX + m_startPosition.x;
        float fNewY = fRotOldY + m_startPosition.y;
        
        CCPoint currentPos = m_pTarget->getPosition();
        CCPoint newPos = ccp( fNewX, fNewY);
        m_pTarget->setPosition(newPos);
        
        
        if(m_bRotHead)
        {
            CCPoint dirPoint = ccpSub(newPos, currentPos);
            float fRadian = atanf(dirPoint.y/dirPoint.x);
            if((dirPoint.y > 0 && dirPoint.x > 0)
               || (dirPoint.y < 0 && dirPoint.x > 0))
            {
                fRadian = M_PI_2 - fRadian;
            }
            else if((dirPoint.y < 0 && dirPoint.x < 0)||
                    (dirPoint.y > 0 && dirPoint.x < 0))
            {
                fRadian = -M_PI_2 - fRadian;
            }
            
            float m_fRotation = fRadian*180.0/M_PI;
            m_pTarget->setRotation(m_fRotation);
        }
    }
}

BWActionInterval* BWCircleBy::reverse(void)
{
    BWBezierConfig r;
    BWCircleBy *pAction = BWCircleBy::create(m_fDuration, m_dirPoint,m_fB);
    return pAction;
}










