// -*-c++-*-

/*!
  \file formation_sbsp.h
  \brief simple SBSP formation Header File.
*/
/**相比较之下，这个文件和formation文件的功能基本类似，很多函数是formation里面有过的，
 * 但这里进行了重新定义和写，也加入了一些新的函数，train函数没有函数体
 * 使用sampledata中定义的函数来访问球员数据
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

#ifndef RCSC_FORMATION_FORMATION_SBSP_H
#define RCSC_FORMATION_FORMATION_SBSP_H

#include <rcsc/formation/formation.h>
#include <rcsc/geom/rect_2d.h>
#include <boost/array.hpp>

namespace rcsc {

/*!
  \class FormationSBSP
  \brief formation implementation using SBSP method
*/
class FormationSBSP
    : public Formation {
public:

    static const std::string NAME; //!< type name阵型名称

    /*!
      \struct Role
      \brief role parameter
     */
    struct Role {                                                       //独有的role结构体，用来存储一个球员的各种信息
        int number_; //!< player number
        int symmetry_; //!< mirror reference nuber. =0:center, -1:side, >0:refered number
        std::string name_; //!< role name string
        Vector2D pos_; //!< basic position
        Vector2D attract_; //!< attraction parameter
        Rect2D region_; //!< movable area                                在rect_2d.h中，是矩形的定义
        bool behind_ball_; //!< defensive flag

        /*!
          \brief default constructor
         */
        Role();                                                         //非常规的构造函数，很多参数是随机生成

        /*!
          \brief construct with parameters
          \param attract attraction parameter vector
          \param region movable region
          \param behind_ball behind flag
         */
        Role( const Vector2D & attract,                                 //正常情况的构造函数，根据传入的信息进行构造
              const Rect2D & region,
              const bool behind_ball );

        /*!
          \brief create random parameters
         */
        void randomize();                                               //随即生成参数，被非正常的Role（）函数调用
 
        /*!
          \brief read data from an input stream
          \return parsing result
         */
        bool read( std::istream & is );                                 //从输入流中读取数据，并赋值给对应的参数

        /*!
          \brief write data to an output stream
          \param os reference to the output stream
          \return reference to the output stream
         */
        std::ostream & print( std::ostream & os ) const;                //输出各种参数
    };

    /*!
      \class Param
      \brief the set of role
     */
    class Param {
    private:
        std::string M_name; //!< formation name
        boost::array< Role, 11 > M_roles; //!< role set                 固定长度的数组，但可以调用已有函数进行排序等功能的实现

        Param(); // not used
    public:

        /*!
          \brief construct with formation name
          \param name formation name
         */
        explicit                                                        //通常情况下只有一个参数的构造函数为隐式的，加了explicit之后变成显式且不会被转换为隐式，可以直接调用
        Param( const std::string & name )
            : M_name( name )                                            //函数功能就是把传入的string赋值给M_name
          { }

        /*!
          \brief get the symmetry information of the specified player
          \param unum target player number
         */
        int getSymmetry( const int unum ) const                         //返回symmetr类型的值（=0:center, -1:side, >0:refered number）
          {
              return M_roles.at( unum - 1 ).symmetry_;
          }

        /*!
          \brief get the role of the specified number
          \param unum target player number
         */
        Role & getRole( const int unum )                                //返回role结构体
          {
              return M_roles.at( unum - 1 );
          }

        /*!
          \brief get the role of the specified number
          \param unum target player number
         */
        const
        Role & getRole( const int unum ) const                          //功能同上，但是不可修改
          {
              return M_roles.at( unum - 1 );
          }

        /*!
          \brief get the position for the input
          \param unum target player number
          \param ball_pos input ball position
          \return player's move position
         */
        Vector2D getPosition( const int unum,                           //获取position，但里面涉及到很复杂的处理，不知道返回的到底是什么position
                              const Vector2D & ball_pos ) const;

        /*!
          \brief check the validity of this formation
          \return true if this is valid formation.
         */
        bool check();                                                   //检查各种球员的数据是否正常
 
        /*!
          \brief set synmetric parameters
         */
        void createSymmetryParam();                                     //设置symmetry的各项参数，从symmetry_role赋值给M_roles[i]

        /*!
          \brief read data from an input stream
          \return parsing result
         */
        bool read( std::istream & is );                                 //在读取万之后添加了check判断和createsymmetry（）操作，但没有将read细分出readconf等函数下一步操作

        /*!
          \brief write data to an output stream
          \param os reference to the output stream
          \return reference to the output stream
         */
        std::ostream & print( std::ostream & os ) const;                //调用M_roles[i].print打印信息,添加了刷新缓冲区的操作

    };


private:

    Param M_param;

public:

    /*!
      \brief just call the base class constructor
    */
    FormationSBSP();                                                    //构造函数，会将param设置为default

    /*!
      \brief static method. get the type name of this formation
      \return type name string
    */
    static
    std::string name()                                                  //返回阵型名称
      {
          //return std::string( "SBSP" );
          return NAME;
      }

    /*!
      \brief static method. factory of this class
      \return pointer to the new object
    */
    static
    Formation::Ptr create()                                             //返回指向这个阵型的指针
      {
          return Formation::Ptr( new FormationSBSP() );
      }

    //--------------------------------------------------------------

    /*!
      \brief create default formation. assign role and initial positions.
      \return snapshow variable for the initial stat(ball pos=(0,0)).
    */
    virtual
    void createDefaultData();                                           //创建球员并把球员放到球场上

    /*!
      \brief get the name of this formation
      \return name string
    */
    virtual
    std::string methodName() const                                      //返回阵型名称，但不可修改
      {
          return FormationSBSP::name();
      }

protected:

    /*!
      \brief set the role name of the specified player
      \param unum target player's number
      \param name role name string.
    */
    virtual
    void setRoleName( const int unum,                                   //根据球员号设定roleName
                      const std::string & name );

    /*!
      \brief create new role parameter.
      \param unum target player's number
      \param role_name new role name
      \param type side type of this parameter
    */
    virtual
    void createNewRole( const int unum,                                 //给某个球员设置role，但不能是symmetry类型，然后调用randomize初始化
                        const std::string & role_name,
                        const SideType type );

public:                                                                 //四个虚函数，派生类中可以重新定义
 
    /*!
      \brief get the role name of the specified player
      \param unum target player's number
      \return role name string. if empty string is returned,
      that means no role parameter is assigned for unum.
    */
    virtual
    std::string getRoleName( const int unum ) const;                    //返回rolename，不可修改

    /*!
      \brief get position for the current focus point
      \param unum player number
      \param ball_pos current focus point, usually ball position.
    */
    virtual
    Vector2D getPosition( const int unum,                               //虚函数，派生类中可以重新再定义这个函数
                          const Vector2D & ball_pos ) const;

    /*!
      \brief get all positions for the current focus point
      \param focus_point current focus point, usually ball position
      \param positions contaner to store the result
     */
    virtual
    void getPositions( const Vector2D & focus_point,
                       std::vector< Vector2D > & positions ) const;

    /*!
      \brief update formation paramter using training data set
    */
    virtual
    void train();                                                       //这里train函数没有函数体

protected:

    /*!
      \brief restore conf data from the input stream.
      \param is reference to the input stream.
      \return pasing result.
    */
    virtual
    bool readConf( std::istream & is );                                 //读取配置文件并设置球员

    /*!
      \brief put data to the output stream.
      \param os reference to the output stream
      \return reference to the output stream
    */
    virtual
    std::ostream & printConf( std::ostream & os ) const;                //打印球员信息

private:

    /*!
      \brief get the current formation parameter
      \return const reference to the current formation parameter
    */
    const
    Param & param() const
      {
          return M_param;
      }
};

}

#endif
