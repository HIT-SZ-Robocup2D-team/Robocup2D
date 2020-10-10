// -*-c++-*-

/*!
  \file basic_client.h
  \brief standard soccer client class Header File.
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

#ifndef RCSC_BASIC_CLIENT_H
#define RCSC_BASIC_CLIENT_H

#include <boost/shared_ptr.hpp>   //用来提供boost库中的智能指针shared_ptr,该指针只能用于堆中的对象，这个指针可以自动释放，不需要delete；模板shared_ptr<T> ptr(new T);  // T 可以是 int、char、类等各种类型

#include <fstream>                //文件流头文件，用于C++的文件操作
#include <string>

namespace rcsc {

class UDPSocket;
class GZCompressor;                //string包含的类
class GZDecompressor;             //string包含的类
class CmdLineParser;
class SoccerAgent;

/*!
  \class BasicClient
  \brief standard soccer clientt class

  This class supplies:
  - UDP connection interfece to the rcssserver.
  - bridge socket event to the basic agent.
 */
class BasicClient {
public:
    enum {                                                          //为什么要用枚举型呢？？？
        MAX_MESG = 8192, //!< max length of send/receive buffer.   //最大接受字节长度
    };

    /*!
      \enum Mode
      \brief types for sensory message server.
     */
    enum Mode {                                          //在线与掉线两种模式
        ONLINE,
        OFFLINE,
    };

private:

    //! client mode
    Mode M_client_mode;                          

    //! udp connection
    boost::shared_ptr< UDPSocket > M_socket;

    //! offline client input log file
    std::ifstream M_offline_in;               //文件操作

    //! offline client input log file
    std::ofstream M_offline_out;

    //! flag to check server status
    bool M_server_alive;

    //! timeout interval for select()
    long M_interval_msec;

    //! buffer to send/receive server message
    char M_message[MAX_MESG];

    //! send message compressor
    boost::shared_ptr< GZCompressor > M_compressor;

    //! receive message decompressor
    boost::shared_ptr< GZDecompressor > M_decompressor;

    //! gzip compression level
    int M_compression_level;

    //! compression message buffer
    std::string M_compression_message;

    //! compression message buffer
    std::string M_decompression_message;

    // nocopyable
    BasicClient( const BasicClient & );               //声明一个也构造函数，参数是BasicClient的引用
    BasicClient & operator=( const BasicClient & );  //operator =是一个重载运算符，相当于重新定义一个=，使用引用节省时间空间

public:

    /*!
      \brief init member variables
     */
    BasicClient();                                   //构造函数


    /*!
      \brief virtual destructor.
     */
    virtual                                          //析构函数，在结束时执行
    ~BasicClient();

    /*!
      \brief get the current client mode.
      \return current client mode.
     */
    Mode clientMode() const                         //定义一个ClientMode返回当前状态
      {
          return M_client_mode;
      }

    /*!
      \brief set client mode.
      \param mode new mode
     */
    void setClientMode( const Mode mode )          //用来改变mode
      {
          M_client_mode = mode;
      }

    /*!
      \brief open the offline client log file.
      \param filepath file path string to be opened.
      \return result status.
     */
    bool openOfflineLog( const std::string & filepath );

    /*!
      \brief write "(think)" message to offline log file.
     */
    void printOfflineThink();

    /*!
      \brief connect to the specified server with timeout value for select()
      \param hostname server host name
      \param port server port number
      \param interval_msec timeout interval for select() by milli second
      \return true if connection is created.
     */
    bool connectTo( const char * hostname,
                    const int port,
                    const long & interval_msec );

    /*!
      \brief program mainloop
      \param agent pointer to the soccer agent instance.

      Thie method keep infinite loop while client can estimate server is alive.
      To handle server message, select() is used.
      Timeout interval of select() is specified by M_interval_msec member variable.
      When server message is received, handleMessage() is called.
      When timeout occurs, handleTimeout() is called.
      When server is not alive, loop is end and handleExit() is called.
     */
    void run( SoccerAgent * agent );

private:

    /*!
      \brief program mainloop for online client mode.
      \param agent pointer to the soccer agent instance.
     */
    void runOnline( SoccerAgent * agent );

    /*!
      \brief program mainloop for offline client mode.
      \param agent pointer to the soccer agent instance.
     */
    void runOffline( SoccerAgent * agent );

public:

    /*!
      \brief set new interval time for select()
      \param interval_msec new interval by milli second
     */
    void setIntervalMSec( const long & interval_msec );

    /*!
      \brief set server status
      \param alive server status flag. if server is dead, this value becomes false.
     */
    void setServerAlive( const bool alive );

    /*!
      \brief set gzip compression level
      \param level compression level
      \return old compression level
     */
    int setCompressionLevel( const int level );

    /*!
      \brief send raw string to the server
      \param msg message to be sent
      \return result of ::sendto()
     */
    int sendMessage( const char * msg );

    /*!
      \brief receive server message in the socket queue
      \return length of received message
     */
    int recvMessage();

    /*!
      \brief check server alive status
      \return true if client can estimate server is alive
     */
    bool isServerAlive() const
      {
          return M_server_alive;
      }

    /*!
      \brief get recieved message buffer
      \return const pointer to the message buffer
     */
    const
    char * message() const
      {
          return M_decompression_message.c_str();
      }

};

}

#endif
