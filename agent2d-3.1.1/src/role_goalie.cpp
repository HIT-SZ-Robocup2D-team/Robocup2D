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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <math.h>

#include "role_goalie.h"

#include "bhv_goalie_chase_ball.h"
#include "bhv_goalie_basic_move.h"
#include "bhv_goalie_free_kick.h"

#include <rcsc/action/basic_actions.h>
#include <rcsc/action/neck_scan_field.h>
#include <rcsc/action/body_clear_ball.h>

#include <rcsc/player/player_agent.h>
#include <rcsc/player/debug_client.h>
#include <rcsc/player/world_model.h>
#include <rcsc/player/player_object.h>

#include <rcsc/common/logger.h>
#include <rcsc/common/server_param.h>
#include <rcsc/geom/rect_2d.h>

#include <vector>
#include <utility>


using namespace rcsc;

const std::string RoleGoalie::NAME( "Goalie" );
const double EPS = 1e-6;           //用于比较大小

/*-------------------------------------------------------------------*/
/*!

 */
namespace {
rcss::RegHolder role = SoccerRole::creators().autoReg( &RoleGoalie::create,
                                                       RoleGoalie::name() );
                                                       
}

//寻找最大匹配函数
void findMax( const std::vector <std:vector <double> > & vec, int i, int length, std::vector <int> & maxLoc, int option )
{
	if( option == 0 )
	{
		int row = i;
		int height = length;
		double max = vec[row][0];
		maxLoc[row] = 0
		for( int i = 1; i < height; i++ )
		{
			if( vec[row][i] > max )
			{
				max = vec[row][i];
				maxLoc[row] = i;
			}
		}
	
		for( int j = 0; j < row; j++ )
		{
			if( fabs(maxLoc[j] - maxLoc[row]) < EPS )
			{
				int temp = (vec[j][maxLoc[j]] - vec[row][maxLoc[row]] > EPS ）？row:j;   //temp存储将要调整的行号
				vec[temp][maxLoc[temp]] = -1e+6;                                         //将该值变成无穷小
				findMax( vec, temp, height, maxLoc, 0 );
			}
		}
	}
	
	else
	{
		int col = i;
		int width = legth;
		double max = vec[0][col];
		maxLoc[col] = 0
		for( int i = 1; i < width; i++ )
		{
			if( vec[i][col] > max )
			{
				max = vec[i][col];
				maxLoc[col] = i;
			}
		}
	
		for( int j = 0; j < col; j++ )
		{
			if( fabs(maxLoc[j] - maxLoc[col]) < EPS )
			{
				int temp = (vec[maxLoc[j]][j] - vec[maxLoc[col]][col] > EPS ）？col:j;   //temp存储将要调整的行号
				vec[maxLoc[temp]][temp] = -1e+6;                                         //将该值变成无穷小
				findMax( vec, temp, width, maxLoc, 1 );
			}
		}
	}
					
}

/*-------------------------------------------------------------------*/
/*!

 */
bool
RoleGoalie::execute( PlayerAgent * agent )
{
    static const Rect2D our_penalty( Vector2D( -ServerParam::i().pitchHalfLength(),
                                               -ServerParam::i().penaltyAreaHalfWidth() + 1.0 ),
                                     Size2D( ServerParam::i().penaltyAreaLength() - 1.0,
                                             ServerParam::i().penaltyAreaWidth() - 2.0 ) );

    //////////////////////////////////////////////////////////////
    // play_on play goalie安排盯防
    PlayerPtrCont markedOpp；
    PlayerPtrCont markingTeammate;
    for( std::vector< PlayerObject * >::iterator it = agent->world().opponentsFromSelf().begin();  //遍历对手球员的位置，寻找在禁区内内的球员进行盯防
		it != agent->world().opponentsFromSelf().end(); 
		it++)
	{
		if ( our_penalty.contains ( it -> M_pos, 3);
			markedOpp.push_back(it);
	}
	
	 for( std::vector< PlayerObject * >::iterator it = agent->world().teammatesFromSelf().begin();  //遍历队友的位置，寻找在禁区内内的队友进行盯防
		it1 != agent->world().teammatesFromSelf().end(); 
		it1++)
	{
		if ( our_penalty.contains ( it -> M_pos, 3);
			markingTeammate.push_back(it);
	}
	
	/**计算每个盯防队友到每个盯防对手之间的距离,并存储在矩阵中*/
	//设置元素类型为double的矩阵,实现方法为std：：vector<vector>
	doubleMatrix *distance = new doubleMatrix; 
	int height =  markingTeammate.size();
	int width =  markedOpp.size();
	std::vector <pair <int>, <int> > markPairs = agent->world().getMarkPairs;
	distance->resize(height);    //设置数组行数
	for ( int i = 0; i < height; i++ )	//逐行设置列数
		(*distance)[i].resize(width);      
	
	//计算距离并存储在矩阵中	     
	for( std::vector< PlayerObject * >::iterator it1 = markingTeammate.begin(), int i = 0;
		 it != markingTeammate.end();
		 it++, i++ )
	{
		for( std::vector< PlayerObject * >::iterator it2 = markedOpp.begin(), int j = 0;
			 it2 != markedOpp.end();
			 it2++, j++ )
		{
			(*distance)[i][j] = -(it1-> pos().dis2(it2->pos()));        //因为要寻找最小距离匹配，所以全部取相反数
		}
	}
	
	//寻找最优匹配
	std::vector <int> maxLoc;                              //使用maxLoc数组来记录最优匹配
	if( height > width )                                   //如果己方队友比较多，就从对手角度出发匹配
	{
		for( int i = 0; i < width; i++ )
			findMax( *distance, i, width, maxLoc, 1 );
		//写入worldModel中的markPairs
		for( int i = 0; i < 10; i++ )
		{
			for( int j = 0; j < 10; j++ )
			{
				if( markingTeammate[maxLoc[i]] == markPairs[j].first() )    //这个时候maxLoc[i]记录盯防对手i的队友号码
					markPairs[j].second() = markedOpp[i];
			}
		}
	}
	
	else
	{
		for( int i = 0; i < height; i++ )                      
			findMax( *distance, i, height, maxLoc, 0 );
		//写入worldModel中的markPairs
		for( int i = 0; i < 10; i++ )
		{
			for( int j = 0; j < 10; j++ )
			{
				if( markingTeammate[i] == markPairs[j].first() )    //这个时候maxLoc[i]记录队友i盯防的对手号
					markPairs[j].second() = markedOpp[maxLoc[i]];
			}
		}
	}
	
    // catchable
    if ( agent->world().time().cycle()
         > agent->world().self().catchTime().cycle() + ServerParam::i().catchBanCycle()
         && agent->world().ball().distFromSelf() < ServerParam::i().catchableArea() - 0.05
         && our_penalty.contains( agent->world().ball().pos() ) )
    {
        dlog.addText( Logger::ROLE,
                      __FILE__": catchable. ball dist=%.1f, my_catchable=%.1f",
                      agent->world().ball().distFromSelf(),
                      ServerParam::i().catchableArea() );
        agent->doCatch();
        agent->setNeckAction( new Neck_TurnToBall() );
    }
    else if ( agent->world().self().isKickable() )
    {
        doKick( agent );
    }
    else
    {
        doMove( agent );
    }

    return true;
}

/*-------------------------------------------------------------------*/
/*!

 */
void
RoleGoalie::doKick( PlayerAgent * agent )
{
    Body_ClearBall().execute( agent );
    agent->setNeckAction( new Neck_ScanField() );
}

/*-------------------------------------------------------------------*/
/*!

 */
void
RoleGoalie::doMove( PlayerAgent * agent )
{
    if ( Bhv_GoalieChaseBall::is_ball_chase_situation( agent ) )        //能冲到球旁边就冲
    {
        Bhv_GoalieChaseBall().execute( agent );
    }
    else                                                                //不然就BasicMove
    {
        Bhv_GoalieBasicMove().execute( agent );
    }
}
