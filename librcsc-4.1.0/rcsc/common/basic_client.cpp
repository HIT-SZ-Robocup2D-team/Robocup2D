// -*-c++-*-

/*!
  \file basic_client.cpp
  \brief abstract soccer client class Source File.
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

#include "basic_client.h"

#include "soccer_agent.h"

#include <rcsc/gz/gzcompressor.h>
#include <rcsc/net/udp_socket.h>

#include <iostream>
#include <cstring>

#include <unistd.h> // select()
#include <sys/select.h> // select()
#include <sys/time.h> // select()
#include <sys/types.h> // select()

namespace rcsc {

/*-------------------------------------------------------------------*/
/*!

*/
<<<<<<< HEAD
BasicClient::BasicClient()                           //构造函数的一个例子，使用初始化列表来初始化字段，详见https://www.runoob.com/cplusplus/cpp-constructor-destructor.html
=======
BasicClient::BasicClient()
>>>>>>> master
    : M_client_mode( ONLINE ),
      M_server_alive( false ),
      M_interval_msec( 10 ),
      M_compression_level( 0 )
{
<<<<<<< HEAD
    std::memset( M_message, 0, MAX_MESG );          //初始化函数，可以将M_message指向的结构初始化，0表示初始化为0，MAX_MESG表示初始化的字节数
    M_compression_message.reserve( MAX_MESG );      //reserve是string内置的函数，用于预留空间
=======
    std::memset( M_message, 0, MAX_MESG );
    M_compression_message.reserve( MAX_MESG );
>>>>>>> master
    M_decompression_message.reserve( MAX_MESG );
}

/*-------------------------------------------------------------------*/
/*!

*/
<<<<<<< HEAD
BasicClient::~BasicClient()                     //在类空间被释放时用来刷新缓冲区
{
    //std::cerr << "delete BasicClient" << std::endl;
    if ( M_offline_out.is_open() )               //若离线服务器发出退出指令
=======
BasicClient::~BasicClient()
{
    //std::cerr << "delete BasicClient" << std::endl;
    if ( M_offline_out.is_open() )
>>>>>>> master
    {
        M_offline_out.flush();
        M_offline_out.close();
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
<<<<<<< HEAD
BasicClient::run( SoccerAgent * agent )            //传入agent的地址，如果不为空，就判断这个进程是否在线，是就上线，否则转为线下
{
    assert( agent );                               //判断括号里面的值是否为真，若为假则向错误流输出错误并终止程序
=======
BasicClient::run( SoccerAgent * agent )
{
    assert( agent );
>>>>>>> master

    if ( clientMode() == ONLINE )
    {
        runOnline( agent );
    }
    else // if ( clientMode() == OFFLINE )
    {
        runOffline( agent );
    }

    agent->handleExit();
}

/*-------------------------------------------------------------------*/
/*!

*/
void
BasicClient::runOnline( SoccerAgent * agent )
{
<<<<<<< HEAD
    if ( ! agent->handleStart()                   //只有服务器在线且agent成功连接上才继续进行
=======
    if ( ! agent->handleStart()
>>>>>>> master
         || ! isServerAlive() )
    {
        agent->handleExit();
        return;
    }

    // set interval timeout
<<<<<<< HEAD
    struct timeval interval;                      //timeval结构体存储了从1970.1.1以来的时间，这里用作程序的计时器

    fd_set read_fds;                             //下面使用select函数检查是否阻塞，用于异步情况，是一个数组，存放文件描述符
    fd_set read_fds_back; 

    FD_ZERO( &read_fds );                        //将read_fds这个文件描述符清空         
    FD_SET( M_socket->fd(), &read_fds );         //
=======
    struct timeval interval;

    fd_set read_fds;
    fd_set read_fds_back;

    FD_ZERO( &read_fds );
    FD_SET( M_socket->fd(), &read_fds );
>>>>>>> master
    read_fds_back = read_fds;

    int timeout_count = 0;
    long waited_msec = 0;

<<<<<<< HEAD
    while ( isServerAlive() )                                          //当服务器在线时
    {
        read_fds = read_fds_back;
        interval.tv_sec = M_interval_msec / 1000;                      //每次都要重置时间因为select函数会改变interval的值
        interval.tv_usec = ( M_interval_msec % 1000 ) * 1000;          //？？？这个时间设置的有点奇怪，M_interval_msec被初始化为10

        int ret = ::select( M_socket->fd() + 1, &read_fds,             //::用于全局变函数和区域内的重名函数区分,M_socket是指向UDPSocket的智能指针
                            static_cast< fd_set * >( 0 ),              //select函数是检查有没有文件可读，若有则返回一个大于0的值，若没有直到超时返回0，错误返回NULL
                            static_cast< fd_set * >( 0 ),
                            &interval );
        if ( ret < 0 )                                                 //发生错误的时候
        {
            perror( "select" );                                       //将select函数出错的情况输出出来
            break;
        }
        else if ( ret == 0 )                                          //等待超时没有信息返回
=======
    while ( isServerAlive() )
    {
        read_fds = read_fds_back;
        interval.tv_sec = M_interval_msec / 1000;
        interval.tv_usec = ( M_interval_msec % 1000 ) * 1000;

        int ret = ::select( M_socket->fd() + 1, &read_fds,
                            static_cast< fd_set * >( 0 ),
                            static_cast< fd_set * >( 0 ),
                            &interval );
        if ( ret < 0 )
        {
            perror( "select" );
            break;
        }
        else if ( ret == 0 )
>>>>>>> master
        {
            // no meesage. timeout.
            waited_msec += M_interval_msec;
            ++timeout_count;
<<<<<<< HEAD
            agent->handleTimeout( timeout_count,                     //agent执行处理timeout的函数
                                  waited_msec );
        }
        else                                                        //接收到了信息
=======
            agent->handleTimeout( timeout_count,
                                  waited_msec );
        }
        else
>>>>>>> master
        {
            // received message, reset wait time
            waited_msec = 0;
            timeout_count = 0;
<<<<<<< HEAD
            agent->handleMessage();                                 //进入信息处理函数
=======
            agent->handleMessage();
>>>>>>> master
        }
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
<<<<<<< HEAD
BasicClient::runOffline( SoccerAgent * agent )                          //agent掉线时的操作
{
    if ( ! agent->handleStartOffline() )                                 //如果执行离线开始函数失败，就将这个agent退出
=======
BasicClient::runOffline( SoccerAgent * agent )
{
    if ( ! agent->handleStartOffline() )
>>>>>>> master
    {
        agent->handleExit();
        return;
    }

<<<<<<< HEAD
    while ( isServerAlive() )                                           //如果server还在线
    {
        agent->handleMessageOffline();                                  //按离线情况处理信息
=======
    while ( isServerAlive() )
    {
        agent->handleMessageOffline();
>>>>>>> master
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
<<<<<<< HEAD
BasicClient::setIntervalMSec( const long & interval_msec )              //设置等待的时间间隔函数
{
    if ( interval_msec <= 0 )                                           //等待时间为负将输出错误
=======
BasicClient::setIntervalMSec( const long & interval_msec )
{
    if ( interval_msec <= 0 )
>>>>>>> master
    {
        std::cerr << "***ERROR*** interval msec must be positive value. ["
                  << interval_msec << "]"
                  << std::endl;
        return;
    }

<<<<<<< HEAD
    if ( interval_msec < 10 )                                           //小于10也输出错误
=======
    if ( interval_msec < 10 )
>>>>>>> master
    {
        std::cerr << "***ERROR*** interval msec should be more than or equal 10. ["
                  << interval_msec << "]"
                  << std::endl;
        return;
    }

    M_interval_msec = interval_msec;
}

/*-------------------------------------------------------------------*/
/*!

*/
void
<<<<<<< HEAD
BasicClient::setServerAlive( const bool alive )                         
{
    M_server_alive = alive;                                             //agent中用来记录server是否在线的变量
=======
BasicClient::setServerAlive( const bool alive )
{
    M_server_alive = alive;
>>>>>>> master
}

/*-------------------------------------------------------------------*/
/*!

*/
int
<<<<<<< HEAD
BasicClient::setCompressionLevel( const int level )                     //设置信息压缩等级的函数， M_compressor是GZCompressor的智能指针
{
#ifdef HAVE_LIBZ                                                        //只有在定义了HAVE_LIBZ的情况才可以正常调节压缩等级
    if ( level < 0 || 9 <= level )                                      //等级为为1-8
=======
BasicClient::setCompressionLevel( const int level )
{
#ifdef HAVE_LIBZ
    if ( level < 0 || 9 <= level )
>>>>>>> master
    {
        std::cerr << "***ERROR*** unsupported compression level "
                  << level << std::endl;
        return M_compression_level;
    }


    int old_level = M_compression_level;
    M_compression_level = level;

    if ( level == 0 )
    {
        M_compressor.reset();
        M_decompressor.reset();
        return old_level;
    }

<<<<<<< HEAD
    M_compressor                                                        //将M_compressor设置为指向新的等级的GZCompressor
=======
    M_compressor
>>>>>>> master
        = boost::shared_ptr< GZCompressor >( new GZCompressor( level ) );

    M_decompressor
        = boost::shared_ptr< GZDecompressor >( new GZDecompressor() );

    return old_level;
#else
    return 0;
#endif
}

/*-------------------------------------------------------------------*/
/*!

*/
bool
<<<<<<< HEAD
BasicClient::openOfflineLog( const std::string & filepath )             //开启离线日志记录数据
=======
BasicClient::openOfflineLog( const std::string & filepath )
>>>>>>> master
{
    if ( clientMode() == ONLINE )
    {
        M_offline_out.close();
        M_offline_out.open( filepath.c_str() );
        if ( ! M_offline_out.is_open() )
        {
            return false;
        }

        if ( ! M_decompression_message.empty() )
        {
            M_offline_out << M_decompression_message << std::endl;
        }

        return true;
    }
    else if ( clientMode() == OFFLINE )
    {
        M_offline_in.close();
        M_offline_in.open( filepath.c_str() );

        return M_offline_in.is_open();
    }

    return false;
}

/*-------------------------------------------------------------------*/
/*!

*/
void
BasicClient::printOfflineThink()
{
    if ( M_offline_out.is_open() )
    {
        M_offline_out << "(think)" << std::endl;
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
bool
BasicClient::connectTo( const char * hostname,
                        const int port,
                        const long & interval_msec )
{
<<<<<<< HEAD
    if ( clientMode() != ONLINE )                                 //如果记录到client未上线
=======
    if ( clientMode() != ONLINE )
>>>>>>> master
    {
        M_socket.reset();
        return true;
    }

<<<<<<< HEAD
    M_socket = boost::shared_ptr< UDPSocket >( new UDPSocket( hostname, port ) );//将M_socket设置为指向UDPSocket的智能指针

    if ( ! M_socket                                               //如果M_Socket为空或者指向的fd异常
=======
    M_socket = boost::shared_ptr< UDPSocket >( new UDPSocket( hostname, port ) );

    if ( ! M_socket
>>>>>>> master
         || M_socket->fd() == -1 )
    {
        std::cerr << "BasicClinet::connectTo() Failed to create connection."
                  << std::endl;
        setServerAlive( false );
        return false;
    }

    setServerAlive( true );

<<<<<<< HEAD
    setIntervalMSec( interval_msec );                                   //使用函数重置等待时间
 
=======
    setIntervalMSec( interval_msec );

>>>>>>> master
    return true;
}

/*-------------------------------------------------------------------*/
/*!

*/
int
<<<<<<< HEAD
BasicClient::sendMessage( const char * msg )                            //发送信息的函数
{
    if ( clientMode() != ONLINE )                                       //先确认是否在线
=======
BasicClient::sendMessage( const char * msg )
{
    if ( clientMode() != ONLINE )
>>>>>>> master
    {
        return 1;
    }

<<<<<<< HEAD
    if ( ! M_socket )                                                   //确认套接字是否为空
=======
    if ( ! M_socket )
>>>>>>> master
    {
        return 0;
    }

#ifdef HAVE_LIBZ
<<<<<<< HEAD
    if ( M_compression_level > 0                                        //根据压缩等级对信息进行压缩
         && M_compressor )
    {
        M_compressor->compress( msg,                                   
                                std::strlen( msg ) + 1,
                                M_compression_message );

        if ( ! M_compression_message.empty() )                          //如果压缩完不为空
        {
            return M_socket->send( M_compression_message.data(),        //返回发送信息成功与否
                                   M_compression_message.length() );    //传入data和length
=======
    if ( M_compression_level > 0
         && M_compressor )
    {
        M_compressor->compress( msg,
                                std::strlen( msg ) + 1,
                                M_compression_message );

        if ( ! M_compression_message.empty() )
        {
            return M_socket->send( M_compression_message.data(),
                                   M_compression_message.length() );
>>>>>>> master
        }

        return 0;
    }
#endif

    //std::cerr << "send [" << msg << "0]" << endl;
    // if the length of message is the result of only strlen,
    // server will reply "(warning message_not_null_terminated)"
<<<<<<< HEAD
    return M_socket->send( msg, std::strlen( msg ) + 1 );                //返回发送信息成功与否
=======
    return M_socket->send( msg, std::strlen( msg ) + 1 );
>>>>>>> master
}

/*-------------------------------------------------------------------*/
/*!

*/
int
<<<<<<< HEAD
BasicClient::recvMessage()                                              //接受信息的函数
=======
BasicClient::recvMessage()
>>>>>>> master
{
    //
    // offline mode
    //

    if ( clientMode() == OFFLINE )
    {
<<<<<<< HEAD
        while ( std::getline( M_offline_in, M_decompression_message ) )   //不断接受并解压缩信息,getline()用于从输入流中读取一行的信息，读取到M_decompression_message中
        {
            if ( M_decompression_message.empty() ) continue;

            return M_decompression_message.size();                       //最后返回信息的size
        }

        setServerAlive( false );                                        //如果在读取信息的时候出错。就将服务器状态设为false
=======
        while ( std::getline( M_offline_in, M_decompression_message ) )
        {
            if ( M_decompression_message.empty() ) continue;

            return M_decompression_message.size();
        }

        setServerAlive( false );
>>>>>>> master
        return 0;
    }

    //
    // online mode
    //

    if ( ! M_socket )
    {
        return 0;
    }

    int n = M_socket->receive( M_message, MAX_MESG );

    if ( n > 0 )
    {
<<<<<<< HEAD
#ifdef HAVE_LIBZ                                                        //如果包含了压缩的对应文件才会进行解压缩
=======
#ifdef HAVE_LIBZ
>>>>>>> master
        if ( M_compression_level > 0
             && M_decompressor )
        {
            M_decompressor->decompress( M_message,
                                        n,
                                        M_decompression_message );
        }
        else
#endif
<<<<<<< HEAD
        {                                                               //没有包含，直接将解压缩得到的信息赋值为接收到的信息
            M_message[n] = '\0';                                        //在最后一位置0
            M_decompression_message = M_message;
        }

        if ( M_offline_out.is_open() )                                  //离线流被打开了就将信息输出到离线流
=======
        {
            M_message[n] = '\0';
            M_decompression_message = M_message;
        }

        if ( M_offline_out.is_open() )
>>>>>>> master
        {
            M_offline_out << M_decompression_message << '\n';
        }
    }

    return n;
}

}
