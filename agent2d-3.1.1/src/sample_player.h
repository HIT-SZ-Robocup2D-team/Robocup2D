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

    bool initImpl( rcsc::CmdLineParser & cmd_parser );                  //初始化接口，调用父类的initImpl，加入判断

    //! main decision
    virtual
    void actionImpl();                                                  //动作决策，相当于球员的大脑，关键

    //! communication decision
    virtual
    void communicationImpl();                                           //判断M_communication是否为空，不为空就调用这个Communication类的对象的执行函数

    virtual
    void handleActionStart();                                           //处理动作的开始函数，还没写
    virtual
    void handleActionEnd();                                             //结束函数，给debugclient和log添加记录

    virtual
    void handleServerParam();                                           //处理服务器参数

    virtual
    void handlePlayerParam();                                           //处理player的参数，还没有写
    virtual
    void handlePlayerType();                                            //还没有写

    virtual

    FieldEvaluator::ConstPtr createFieldEvaluator() const;              //创造sampleEaluator并返回这个指针


    virtual
    ActionGenerator::ConstPtr createActionGenerator() const;            //创造一个CompositeActionGenerator，添加参数，然后返回这个指针

private:

    bool doPreprocess();                                                //判断一系列过程是否成功执行（包括但不限于下面的几个过程）
    bool doShoot();                                                     //判断是否可执行shoot，同时执行shoot是否成功，成功后就添加log
    bool doForceKick();                                                 //判断是否可执行，可执行就往下添加log，并执行一系列操作
    bool doHeardPassReceive();                                          //判断是否可行，可行就添加log，并执行一系列操作

public:
    virtual
    FieldEvaluator::ConstPtr getFieldEvaluator() const;                 //直接返回M_FieldEvaluator,给别类调用用的
};

#endif
