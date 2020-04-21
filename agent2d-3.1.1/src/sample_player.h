// -*-c++-*-

/*
 *Copyright:

 Copyright (C) Hidehisa AKIYAMA

 This code is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 3, or (at your option)
 any later version.

 This code is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this code; see the file COPYING.  If not, write to
 the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.

 *EndCopyright:
 */

/////////////////////////////////////////////////////////////////////
                                     //这个文件主要是定义了一个基于librcsc库的sampleplayer，功能还非常简略，不过已经有基本操作，在main_player中会有调用的时候
#ifndef SAMPLE_PLAYER_H              //=if not define 只有在没有deffine过SAMPLE_PLAYER_H的情况下才会执行下面的语句直到#endif
#define SAMPLE_PLAYER_H

#include "action_generator.h"
#include "field_evaluator.h"
#include "communication.h"

#include <rcsc/player/player_agent.h>
#include <vector>

class SamplePlayer

    : public rcsc::PlayerAgent {                    //定义一个SamplePlayer的类，这个类继承于rcsc这个namespace中的playerAgent这个类

private:

    Communication::Ptr M_communication;

    FieldEvaluator::ConstPtr M_field_evaluator;
    ActionGenerator::ConstPtr M_action_generator;

public:

    SamplePlayer();

    virtual
    ~SamplePlayer();

protected:

    /*!
      You can override this method.
      But you must call PlayerAgent::initImpl() in this method.
    */
    virtual

    bool initImpl( rcsc::CmdLineParser & cmd_parser );                  //初始化接口

    //! main decision
    virtual
    void actionImpl();                                                  //动作接口

    //! communication decision
    virtual
    void communicationImpl();                                           //交流接口

    virtual
    void handleActionStart();                                           //处理动作的开始函数
    virtual
    void handleActionEnd();                                             //结束函数

    virtual
    void handleServerParam();                                           //处理服务器参数

    virtual
    void handlePlayerParam();
    virtual
    void handlePlayerType();

    virtual

    FieldEvaluator::ConstPtr createFieldEvaluator() const;              //评估球场函数，加const隐含传入的指针不能被修改


    virtual
    ActionGenerator::ConstPtr createActionGenerator() const;

private:

    bool doPreprocess();
    bool doShoot();
    bool doForceKick();
    bool doHeardPassReceive();

public:
    virtual
    FieldEvaluator::ConstPtr getFieldEvaluator() const;
};

#endif
