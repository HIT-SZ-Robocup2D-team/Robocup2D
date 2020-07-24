// -*-c++-*-

/*!
  \file strategy.h
  \brief team strategy manager Header File
*/

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

#ifndef STRATEGY_H
#define STRATEGY_H

#include "soccer_role.h"

#include <rcsc/formation/formation.h>
#include <rcsc/geom/vector_2d.h>

#include <boost/shared_ptr.hpp>
#include <map>
#include <vector>
#include <string>

// # define USE_GENERIC_FACTORY 1

namespace rcsc {                                                        //两个基本类
class CmdLineParser;
class WorldModel;
}

enum PositionType {                                                     //三种position
    Position_Left = -1,
    Position_Center = 0,
    Position_Right = 1,
};

enum SituationType {                                                    //几种球员状态Situation
    Normal_Situation,
    Offense_Situation,
    Defense_Situation,
    OurSetPlay_Situation,
    OppSetPlay_Situation,
    PenaltyKick_Situation,
};


class Strategy {
public:
    static const std::string BEFORE_KICK_OFF_CONF;                      //定义几个字符串代表配置参数
    static const std::string NORMAL_FORMATION_CONF;
    static const std::string DEFENSE_FORMATION_CONF;
    static const std::string OFFENSE_FORMATION_CONF;
    static const std::string GOAL_KICK_OPP_FORMATION_CONF;
    static const std::string GOAL_KICK_OUR_FORMATION_CONF;
    static const std::string GOALIE_CATCH_OPP_FORMATION_CONF;
    static const std::string GOALIE_CATCH_OUR_FORMATION_CONF;
    static const std::string KICKIN_OUR_FORMATION_CONF;
    static const std::string SETPLAY_OPP_FORMATION_CONF;
    static const std::string SETPLAY_OUR_FORMATION_CONF;
    static const std::string INDIRECT_FREEKICK_OPP_FORMATION_CONF;
    static const std::string INDIRECT_FREEKICK_OUR_FORMATION_CONF;

    enum BallArea {
        BA_CrossBlock, BA_DribbleBlock, BA_DribbleAttack, BA_Cross,     //dribble带球盘球
        BA_Stopper,    BA_DefMidField,  BA_OffMidField,   BA_ShootChance,
        BA_Danger,

        BA_None
    };

private:
    //
    // factories
    //
#ifndef USE_GENERIC_FACTORY
    typedef std::map< std::string, SoccerRole::Creator > RoleFactory;   //Hash表将RoleName和Creator指针联系在一起
    typedef std::map< std::string, rcsc::Formation::Creator > FormationFactory;  //同上将FormationName和Creator指针联系在一起

    RoleFactory M_role_factory;
    FormationFactory M_formation_factory;
#endif


    //
    // formations
    //

    rcsc::Formation::Ptr M_before_kick_off_formation;                   //针对不同情况的阵型指针

    rcsc::Formation::Ptr M_normal_formation;
    rcsc::Formation::Ptr M_defense_formation;
    rcsc::Formation::Ptr M_offense_formation;

    rcsc::Formation::Ptr M_goal_kick_opp_formation;
    rcsc::Formation::Ptr M_goal_kick_our_formation;
    rcsc::Formation::Ptr M_goalie_catch_opp_formation;
    rcsc::Formation::Ptr M_goalie_catch_our_formation;
    rcsc::Formation::Ptr M_kickin_our_formation;
    rcsc::Formation::Ptr M_setplay_opp_formation;
    rcsc::Formation::Ptr M_setplay_our_formation;
    rcsc::Formation::Ptr M_indirect_freekick_opp_formation;
    rcsc::Formation::Ptr M_indirect_freekick_our_formation;


    int M_goalie_unum;


    // situation type
    SituationType M_current_situation;

    // role assignment
    std::vector< int > M_role_number;

    // current home positions
    std::vector< PositionType > M_position_types;
    std::vector< rcsc::Vector2D > M_positions;

    // private for singleton
    Strategy();

    // not used
    Strategy( const Strategy & );
    const Strategy & operator=( const Strategy & );
public:

    static
    Strategy & instance();                                              //访问Strategy然后返回指针

    static
    const
    Strategy & i()                                                      //功能和上面一样但是返回的指针不能修改
      {
          return instance();                              
      }

    //
    // initialization
    //

    bool init( rcsc::CmdLineParser & cmd_parser );                      //创建param_map并判断是否传回help，若没有就调用param_map的init函数，若返回ture就返回true
    bool read( const std::string & config_dir );                        //读取阵型文件的配置文件


    //
    // update
    //

    void update( const rcsc::WorldModel & wm );                         //从传入的world_model中更新时间位置等数据


    void exchangeRole( const int unum0,                                 //将两个球员的role进行调换
                       const int unum1 );

    //
    // accessor to the current information
    //

    int goalieUnum() const { return M_goalie_unum; }                    //返回goalieUnum

    int roleNumber( const int unum ) const                              //根据球员num返回role
      {
          if ( unum < 1 || 11 < unum ) return unum;
          return M_role_number[unum - 1];
      }

    bool isMarkerType( const int unum ) const;                          //判断rolenum是否为2、3、4、5


    SoccerRole::Ptr createRole( const int unum,                         //给传入一个球员的num，为这个球员创造role
                                const rcsc::WorldModel & wm ) const;
    PositionType getPositionType( const int unum ) const;               //根据球员号码判断他是在side还是center
    rcsc::Vector2D getPosition( const int unum ) const;                 //根据球员号码获取球员应该处于的位置


private:
    void updateSituation( const rcsc::WorldModel & wm );                //根据worldoMdel更新time和gamemode
    // update the current position table
    void updatePosition( const rcsc::WorldModel & wm );                 //根据worldmodel更新位置，包括球和球员

    rcsc::Formation::Ptr readFormation( const std::string & filepath ); //通过文件路径来读取阵型文件
    rcsc::Formation::Ptr createFormation( const std::string & type_name ) const;  //根据传入的FormationTypeName创造formation指针，并赋值，返回该指针

    rcsc::Formation::Ptr getFormation( const rcsc::WorldModel & wm ) const;  //根据当前的gamemode，返回对应的formation指针

public:
    static
    BallArea get_ball_area( const rcsc::WorldModel & wm );              //获取球的位置区域
    static
    BallArea get_ball_area( const rcsc::Vector2D & ball_pos );          //在log中添加球的位置数据，和上面的函数连续使用

    static
    double get_normal_dash_power( const rcsc::WorldModel & wm );        //返回体力剩余
};

#endif
