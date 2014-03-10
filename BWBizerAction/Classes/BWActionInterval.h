//
//  BWBizerActionAppDelegate.h
//  BWBizerAction
//
//  Created by wangbin on 5/25/13.
//  Copyright __MyCompanyName__ 2013. All rights reserved.
//

#ifndef __ACTION_BWINTERVAL_ACTION_H__
#define __ACTION_BWINTERVAL_ACTION_H__

#include "base_nodes/CCNode.h"
#include "CCAction.h"
#include "CCProtocols.h"
#include "sprite_nodes/CCSpriteFrame.h"
#include "sprite_nodes/CCAnimation.h"
#include <vector>

USING_NS_CC;

class BWActionInterval : public CCFiniteTimeAction
{
public:
    /** how many seconds had elapsed since the actions started to run. */
    inline float getElapsed(void) { return m_elapsed; }

    /** initializes the action */
    bool initWithDuration(float d);

    /** returns true if the action has finished */
    virtual bool isDone(void);

    virtual CCObject* copyWithZone(CCZone* pZone);
    virtual void step(float dt);
    virtual void startWithTarget(CCNode *pTarget);
    /** returns a reversed action */
    virtual BWActionInterval* reverse(void);
    void  setRotHead(bool bRotHead){m_bRotHead = bRotHead;}
    

public:

    /** creates the action */
    static BWActionInterval* create(float d);

public:
    //extension in CCGridAction 
    void setAmplitudeRate(float amp);
    float getAmplitudeRate(void);

protected:
    float  m_elapsed;
    bool   m_bFirstTick;
    bool   m_bRotHead;
};

class  BWMoveBy : public BWActionInterval
{
public:
    /** initializes the action */
    bool initWithDuration(float duration, const CCPoint& deltaPosition);

    virtual CCObject* copyWithZone(CCZone* pZone);
    virtual void startWithTarget(CCNode *pTarget);
    virtual BWActionInterval* reverse(void);
    virtual void update(float time);

public:
    /** creates the action */
    static BWMoveBy* create(float duration, const CCPoint& deltaPosition);
protected:
    CCPoint m_positionDelta;
    CCPoint m_startPosition;
    CCPoint m_previousPosition;
    
    float m_fRotation;
 
};


class  BWMoveTo : public BWMoveBy
{
public:
    /** initializes the action */
    bool initWithDuration(float duration, const CCPoint& position);

    virtual CCObject* copyWithZone(CCZone* pZone);
    virtual void startWithTarget(CCNode *pTarget);

public:
    /** creates the action */
    static BWMoveTo* create(float duration, const CCPoint& position);
protected:
    CCPoint m_endPosition;
};



/** @brief Moves a CCNode object simulating a parabolic jump movement by modifying it's position attribute.
*/
class BWJumpBy : public BWActionInterval
{
public:
    /** initializes the action */
    bool initWithDuration(float duration, const CCPoint& position, float height, unsigned int jumps);

    virtual CCObject* copyWithZone(CCZone* pZone);
    virtual void startWithTarget(CCNode *pTarget);
    virtual void update(float time);
    virtual BWActionInterval* reverse(void);

public:
    /** creates the action */
    static BWJumpBy* create(float duration, const CCPoint& position, float height, unsigned int jumps);
protected:
    CCPoint         m_startPosition;
    CCPoint         m_delta;
    float           m_height;
    unsigned int    m_nJumps;
    CCPoint         m_previousPos;
};

/** @brief Moves a CCNode object to a parabolic position simulating a jump movement by modifying it's position attribute.
*/ 
class BWJumpTo : public BWJumpBy
{
public:
    virtual void startWithTarget(CCNode *pTarget);
    virtual CCObject* copyWithZone(CCZone* pZone);

public:
    /** creates the action */
    static BWJumpTo* create(float duration, const CCPoint& position, float height, int jumps);
};

/** @typedef bezier configuration structure
 */
typedef struct _ccBezierConfig {
    //! end position of the bezier
    CCPoint endPosition;
    //! Bezier control point 1
    CCPoint controlPoint_1;
    //! Bezier control point 2
    CCPoint controlPoint_2;
} BWBezierConfig;

/** @brief An action that moves the target with a cubic Bezier curve by a certain distance.
 */
class BWBezierBy : public BWActionInterval
{
public:
    /** initializes the action with a duration and a bezier configuration */
    bool initWithDuration(float t, const BWBezierConfig& c);

    virtual CCObject* copyWithZone(CCZone* pZone);
    virtual void startWithTarget(CCNode *pTarget);
    virtual void update(float time);
    virtual BWActionInterval* reverse(void);

public:
    /** creates the action with a duration and a bezier configuration */
    static BWBezierBy* create(float t, const BWBezierConfig& c);
protected:
    BWBezierConfig m_sConfig;
    CCPoint m_startPosition;
    CCPoint m_previousPosition;
    
   // float m_fRotation;
};

class BWBezierTo : public BWBezierBy
{
public:
    virtual void startWithTarget(CCNode *pTarget);
    virtual CCObject* copyWithZone(CCZone* pZone);

public:

    /** creates the action with a duration and a bezier configuration */
    static BWBezierTo* create(float t, const BWBezierConfig& c);
    bool initWithDuration(float t, const BWBezierConfig &c);
    
protected:
    BWBezierConfig m_sToConfig;
};

class BWSineBy : public BWActionInterval
{
public:
    bool initWithDuration(float t, CCPoint endPoint,float fA);
    virtual CCObject* copyWithZone(CCZone* pZone);
    virtual void startWithTarget(CCNode *pTarget);
    virtual void update(float time);
    virtual BWActionInterval* reverse(void);
    
public:
    static BWSineBy* create(float t, CCPoint endPoint,float fA);
protected:
    CCPoint m_startPosition;
    CCPoint m_previousPosition;
    float   m_fW;
    CCPoint m_endPosition;
    float   m_fA;
};


class BWCircleBy : public BWActionInterval
{
public:
    bool initWithDuration(float t, CCPoint dirPoint,float fB);
    virtual CCObject* copyWithZone(CCZone* pZone);
    virtual void startWithTarget(CCNode *pTarget);
    virtual void update(float time);
    virtual BWActionInterval* reverse(void);
    void    setClock(bool bClock){m_bClock = bClock;}
    
public:
    static BWCircleBy* create(float t, CCPoint dirPoint,float fB);
protected:
    CCPoint m_startPosition;
    CCPoint m_previousPosition;
    CCPoint m_dirPoint;
    float   m_fA;
    float   m_fB;
    
    bool    m_bClock;
};

#endif //__ACTION_CCINTERVAL_ACTION_H__
