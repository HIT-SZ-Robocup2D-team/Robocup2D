// -*-c++-*-

/*!
	\file formation.cpp
	\brief formation data classes Source File.
*/

/*
 *Copyright:

 Copyright (C) Hidehisa AKIYAMA

 This code is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 3 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

 *EndCopyright:
 */

/////////////////////////////////////////////////////////////////////

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "formation.h"

#include "formation_bpn.h"
#include "formation_cdt.h"
#include "formation_dt.h"
#include "formation_knn.h"
#include "formation_ngnet.h"
#include "formation_rbf.h"
#include "formation_sbsp.h"
#include "formation_static.h"
#include "formation_uva.h"

#include <sstream>

namespace rcsc {

using namespace formation;

/*-------------------------------------------------------------------*/
/*!

*/
Formation::Creators &                 //调用这个函数返回creator
Formation::creators()
{
    static Creators s_instance;
    return s_instance;
}

/*-------------------------------------------------------------------*/
/*!

*/
Formation::Ptr                                                           //根据name将对应的阵型指针赋给ptr
Formation::create( const std::string & name )
{
    Formation::Ptr ptr( static_cast< Formation * >( 0 ) );

    Formation::Creator creator;
    if ( Formation::creators().getCreator( creator, name ) )
    {
        ptr = creator();
    }
    else if ( name == FormationBPN::NAME ) ptr = FormationBPN::create();
    else if ( name == FormationCDT::NAME ) ptr = FormationCDT::create();
    else if ( name == FormationDT::NAME ) ptr = FormationDT::create();
    else if ( name == FormationKNN::NAME ) ptr = FormationKNN::create();
    else if ( name == FormationNGNet::NAME ) ptr = FormationNGNet::create();
    else if ( name == FormationRBF::NAME ) ptr = FormationRBF::create();
    else if ( name == FormationSBSP::NAME ) ptr = FormationSBSP::create();
    else if ( name == FormationStatic::NAME ) ptr = FormationStatic::create();
    else if ( name == FormationUvA::NAME ) ptr = FormationUvA::create();

    return ptr;
}

/*-------------------------------------------------------------------*/
/*!

*/
Formation::Ptr
Formation::create( std::istream & is )                                  //从输入流获得阵型名称，然后调用上一个create函数创建阵型
{
    std::string temp, type;
    is >> temp >> type;

    is.seekg( 0 );                                                      //在输入流中重新定位到起始位置（偏移量为0）
    return create( type );
}


/*-------------------------------------------------------------------*/
/*!

 */
Formation::Formation()                                                  //类的构造函数，先把M_symmetry_number全部初始化为-1，顺便把M_version和M_samples给赋值了
    : M_version( 0 )
    , M_samples( new SampleDataSet() )
{
    for ( int i = 0; i < 11; ++i )
    {
        M_symmetry_number[i] = -1;
    }
}


/*-------------------------------------------------------------------*/
/*!

 */
void
Formation::setCenterType( const int unum )                 //根据unum（球员号码）给数组赋值确定球员center类型
{
    if ( unum < 1 || 11 < unum )                            //球员号码只能是1-11
    {
        std::cerr << __FILE__ << ":" << __LINE__
                  << " *** ERROR *** invalid unum " << unum
                  << std::endl;
        return;
    }

    M_symmetry_number[unum - 1] = 0;                  //创建一个数组，通过数组的之来确定球员类型
                                                      //negative number means this player is original SIDE type.（边位置）
                                                      //positive numver means that this player is SYMMETRY type and referes other player.
                                                       //Zero means that this player is CENTER type（中间位置） 
}

/*-------------------------------------------------------------------*/
/*!

 */
void
Formation::setSideType( const int unum )             //将某个球员（号码传入）设置在球场边的位置
{
    if ( unum < 1 || 11 < unum )
    {
        std::cerr << __FILE__ << ":" << __LINE__
                  << " *** ERROR *** invalid unum " << unum
                  << std::endl;
        return;
    }

    M_symmetry_number[unum - 1] = -1;
}

/*-------------------------------------------------------------------*/
/*!

 */
bool
Formation::setSymmetryType( const int unum,                            //给球员设置symmetryType（意思是给将某个球员的类型设置为和symmetry_unum一样，相当于复制一个，一般用在SIDE位置上对称的左右两个人）
                            const int symmetry_unum,
                            const std::string & role_name )
{
    if ( unum < 1 || 11 < unum )                                          //判断球员号码
    {
        std::cerr << __FILE__ << ":" << __LINE__
                  << " *** ERROR *** Invalid unum " << unum
                  << std::endl;
        return false;
    }
    if ( symmetry_unum < 1 || 11 < symmetry_unum )                      //判断symmetry_unum
    {
        std::cerr << __FILE__ << ":" << __LINE__
                  << " *** ERROR *** Invalid symmetry unum " << unum
                  << std::endl;
        return false;
    }
    if ( symmetry_unum == unum )                                        //symmetry_unum不能和unum重合
    {
        std::cerr << __FILE__ << ":" << __LINE__
                  << " *** ERROR *** Never symmetry itself. unum=" << unum
                  << "  symmetry=" << symmetry_unum
                  << std::endl;
        return false;
    }
    if ( M_symmetry_number[symmetry_unum - 1] > 0 )                     //之前不能被设置过
    {
        std::cerr << __FILE__ << ":" << __LINE__
                  << " *** ERROR *** " << symmetry_unum << " is already a symmetrical player. "
                  << std::endl;
        return false;
    }
    if ( M_symmetry_number[symmetry_unum - 1] == 0 )                     //不能是center type
    {
        std::cerr << __FILE__ << ":" << __LINE__
                  << " *** ERROR *** " << symmetry_unum << " is a center type player. "
                  << std::endl;
        return false;
    }


    // check if unum is already assigned as original side type player.
    for ( int i = 0; i < 11; ++i )                                      //确定这个要设置的symmetry_unum不能被其他球员占了
    {
        if ( i + 1 == unum ) continue;
        if ( M_symmetry_number[i] == symmetry_unum )
        {
            // refered by other player.
            std::cerr << __FILE__ << ":" << __LINE__
                      << " *** ERROR *** player " << unum
                      << " has already refered by player " << i + 1
                      << std::endl;
            return false;
        }
    }

    M_symmetry_number[unum - 1] = symmetry_unum;

    if ( role_name.empty() )                                            //设置role
    {
        setRoleName( unum, getRoleName( symmetry_unum ) );
    }
    else
    {
        setRoleName( unum, role_name );
    }

    return true;
}

/*-------------------------------------------------------------------*/
/*!

 */
bool
Formation::updateRole( const int unum,                                  //更新role
                       const int symmetry_unum,
                       const std::string & role_name )
{
    if ( getSymmetryNumber( unum ) != symmetry_unum )                   //当前球员的symmentry_num和
    {
        if ( symmetry_unum == 0 )                                       //等于0设置为center
        {
            createNewRole( unum, role_name, Formation::CENTER );
            return true;
        }

        if ( symmetry_unum < 0 )                                        //小于0设置为side位置
        {
            createNewRole( unum, role_name, Formation::SIDE );
            return true;
        }

        // ( symmetry_unum > 0 )
        if ( ! isSideType( symmetry_unum ) )                            //判断symmetry_unum指向（refer）的球员是否是side类型（<0）
        {
            std::cerr << __FILE__ << ":" << __LINE__
                      << " You cannot use the player number " << symmetry_unum
                      << " as a symmetry number."
                      << std::endl;
            return false;
        }

        for ( int i = 1; i <= 11; ++i )                                 //遍历找到既不是unum又不是symmetry_unum的球员
        {
            if ( i == unum || i == symmetry_unum ) continue;
            if ( getSymmetryNumber( i ) == symmetry_unum )              //该球员的symmetry_unum不能等于symmetry_unum，否则输出错误
            {
                std::cerr << __FILE__ << ":" << __LINE__
                          << " player number " << symmetry_unum
                          << " has already refered by player " << i
                          << std::endl;
                return false;
            }
        }

        setSymmetryType( unum, symmetry_unum, role_name );              //设置symmetry_type
        return true;
    }

    if ( ! role_name.empty()                                            //如果这个球员的role_name为空或者和传入的role_name不等
         && getRoleName( unum ) != role_name )
    {
        setRoleName( unum, role_name );                                 //设置
        return true;
    }

    return false;
}

/*-------------------------------------------------------------------*/
/*!

 */
bool
Formation::read( std::istream & is )                                    //读取输入
{
    if ( ! readHeader( is ) ) return false;                             //必须Header，Conf，Samples三个都读取到
    if ( ! readConf( is ) ) return false;
    if ( ! readSamples( is ) ) return false;

    // check symmetry number circuration reference
    for ( int i = 0; i < 11; ++i )                                      //检查symmetry_unum有没有大于0
    {
        int refered_unum = M_symmetry_number[i];
        if ( refered_unum <= 0 ) continue;
        if ( M_symmetry_number[refered_unum - 1] > 0 )
        {
            std::cerr << __FILE__ << ":" << __LINE__
                      << " *** ERROR *** failed to read formation."
                      << " Bad symmetrying. player "
                      << i + 1
                      << " mirro = " << refered_unum
                      << " is already symmetrying player"
                      << std::endl;
            return false;
        }
    }

    return true;
}

/*-------------------------------------------------------------------*/
/*!

 */
std::ostream &
Formation::print( std::ostream & os ) const                             //输出Header、Conf、Samples
{
    if ( os ) printHeader( os );
    if ( os ) printConf( os );
    if ( os ) printSamples( os );

    return os;
}

/*-------------------------------------------------------------------*/
/*!

 */
bool
Formation::readHeader( std::istream & is )
{
    std::string line_buf;

    while ( std::getline( is, line_buf ) )                              //不断读取getline
    {
        if ( line_buf.empty()
             || line_buf[0] == '#'
             || ! line_buf.compare( 0, 2, "//" ) )                      //？？？
        {
            continue;
        }

        std::istringstream istr( line_buf );                            //从流中提取数据，将缓冲区定义出来istr

        std::string tag;                                                //string定义类为tag
        istr >> tag;
        if ( tag != "Formation" )                                       //如果tag不是"Formation"
        {
            std::cerr << __FILE__ << ":" << __LINE__
                      << " *** ERROR *** Found invalid tag ["
                      << tag << "]"
                      << std::endl;
            return false;
        }

        std::string name;                                               //定义string类name
        istr >> name;
        if ( name != methodName() )                                     //如果name不等于methodame，也就是不匹配
        {
            std::cerr << __FILE__ << ":" << __LINE__
                      << " *** ERROR *** Unsupported formation type name "
                      << " [" << name << "]."
                      << " The name has to be " << methodName()
                      << std::endl;
            return false;
        }

        int ver = 0;
        if ( istr >> ver )                                              //读取ersion并判断赋值
        {
            if ( ver < 0 )
            {
                std::cerr << __FILE__ << ":" << __LINE__
                          << " *** ERROR *** Illegas format version "
                          << ver
                          << std::endl;
                return false;
            }

            M_version = ver;
        }
        else                                                            //没读取到就直接设为0
        {
            M_version = 0;
        }

        return true;
    }

    return false;
}

/*-------------------------------------------------------------------*/
/*!

 */
bool
Formation::readSamples( std::istream & is )                             //读取Samples
{
    M_samples = SampleDataSet::Ptr( new SampleDataSet() );

    if ( ! M_samples->read( is ) )
    {
        M_samples.reset();
        return false;
    }

    return true;
}

/*-------------------------------------------------------------------*/
/*!

 */
std::ostream &
Formation::printHeader( std::ostream & os ) const                       //打印Header
{
    os << "Formation " << methodName() << ' ' << version() << '\n';
    return os;
}

/*-------------------------------------------------------------------*/
/*!

 */
std::ostream &
Formation::printSamples( std::ostream & os ) const                      //打印samples
{
    if ( M_samples )                                                    //只在samples不为NULL才打印
    {
        M_samples->print( os );
    }

    return os;
}

}
