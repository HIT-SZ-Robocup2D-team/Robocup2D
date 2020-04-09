// -*-c++-*-

/*!
  \file basic_socket.cpp
  \brief socket class Source File.
*/

/*
 *Copyright:

 Copyright (C) Hidehisa AKIYAMA, Hiroki SHIMORA

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

#include "basic_socket.h"

#include <iostream>

#include <cstdio> // std::perror()
#include <cstring> // memset(), memcpy()
#include <cerrno> // errno

#ifdef HAVE_NETDB_H
<<<<<<< HEAD
#include <netdb.h> // linux特有库，网络编程相关；gethostbyname(), getaddrinfo(), freeaddrinfo()
=======
#include <netdb.h> // gethostbyname(), getaddrinfo(), freeaddrinfo()
>>>>>>> master
#endif
#ifdef HAVE_UNISTD_H
#include <unistd.h> // close()
#endif
<<<<<<< HEAD
#include <fcntl.h> // 文件操作fcntl() fiel control的缩写https://www.cnblogs.com/xuyh/p/3273082.html
=======
#include <fcntl.h> // fcntl()
>>>>>>> master
#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h> // socket(), getaddrinfo(), freeaddrinfo()
                       // connect(), send(), recv(), sendto(), recvfrom(),
                       // struct sockaddr_in, SOCK_STREAM, SOCK_DGRAM
#endif
#ifdef HAVE_SYS_SOCKET_H
#include <sys/socket.h> // socket(), getaddrinfo(), freeaddrinfo()
                        // connect(), send(), recv(), sendto(), recvfrom(),
                        // struct sockaddr_in, SOCK_STREAM, SOCK_DGRAM
#endif
#ifdef HAVE_ARPA_INET_H
#include <arpa/inet.h> // inet_addr()
#endif
#ifdef HAVE_NETINET_IN_H
#include <netinet/in.h> // struct sockaddr_in, struct in_addr, htons
#endif


namespace rcsc {

/*!
  \struct AddrImpl
  \brief Pimpl ideom. addres implementation class
*/
struct AddrImpl {
<<<<<<< HEAD
    typedef struct sockaddr_in AddrType; //!< socket address type       //将sockaddr_in重新命名

    AddrType addr_; //!< socket address                                 //这个是配置sock的变量，就作为socket使用
=======
    typedef struct sockaddr_in AddrType; //!< socket address type       //这个是配置sock的变量

    AddrType addr_; //!< socket address
>>>>>>> master
    int socket_type_; //!< socket type {SOCK_STREAM|SOCK_DGRAM}
};


/*-------------------------------------------------------------------*/
/*!

*/
<<<<<<< HEAD
BasicSocket::BasicSocket()                                              //构造函数
    : M_fd( -1 )
    , M_dest( new AddrImpl )
{
    std::memset( reinterpret_cast< char * >( &(M_dest->addr_) ),        //reinterpret_cast用在任意指针（或引用）类型之间的转换；以及指针与足够大的整数类型之间的转换；从整数类型（包括枚举类型）到指针类型，无视大小。
                 0,                                                     //重置为0
=======
BasicSocket::BasicSocket()
    : M_fd( -1 )
    , M_dest( new AddrImpl )
{
    std::memset( reinterpret_cast< char * >( &(M_dest->addr_) ),
                 0,
>>>>>>> master
                 sizeof( AddrImpl::AddrType ) );
    M_dest->socket_type_ = -1;
}

/*-------------------------------------------------------------------*/
/*!

*/
<<<<<<< HEAD
BasicSocket::~BasicSocket()                                               //析构函数就是调用this指针然后关闭
=======
BasicSocket::~BasicSocket()
>>>>>>> master
{
    this->close();
}

/*-------------------------------------------------------------------*/
/*!

*/
bool
BasicSocket::open( const SocketType type )
{
#ifdef HAVE_SOCKET
    // create socket

<<<<<<< HEAD
    switch( type ) {                                                    //根据传入的type将M_dest的socket_type改成相应的
=======
    switch( type ) {
>>>>>>> master
    case BasicSocket::STREAM_TYPE:
        M_dest->socket_type_ = SOCK_STREAM;
        break;
    case BasicSocket::DATAGRAM_TYPE:
        M_dest->socket_type_ = SOCK_DGRAM;
        break;
    default:
        std::cerr << "***ERROR*** BasicSocket::open() unknown socket type."
                  << std::endl;
        return -1;
    }

<<<<<<< HEAD
    M_fd = ::socket( AF_INET, M_dest->socket_type_, 0 );                //建立一个socket产生连接，返回之为socket的编号，-1为失败
=======
    M_fd = ::socket( AF_INET, M_dest->socket_type_, 0 );
>>>>>>> master
#endif

    if ( fd() == -1 )
    {
        std::cerr << "***ERROR*** BasicSocket::open() failed to open a socket."
                  << std::endl;
        std::perror( "socket" );
        std::cerr << errno << std::endl;
        return false;
    }

<<<<<<< HEAD
    ::fcntl( fd(), F_SETFD, FD_CLOEXEC ); // close on exec如果FD_CLOEXEC的值是0,那么在调用exec相关函数后文件句柄保持打开;否则的话,在成功调用exec相关函数后文件句柄将被关闭.

=======
    ::fcntl( fd(), F_SETFD, FD_CLOEXEC ); // close on exec
>>>>>>> master
    return true;
}

/*-------------------------------------------------------------------*/
/*!

*/
bool
BasicSocket::bind( const int port )
{
    if ( fd() == -1 )
    {
        return false;
    }

    AddrImpl::AddrType my_addr;
    std::memset( reinterpret_cast< char * >( &my_addr ),
                 0,
                 sizeof( AddrImpl::AddrType ) );
<<<<<<< HEAD
    my_addr.sin_family = AF_INET; // internet connection sin_family表示使用的协议族，AF_INET表示使用IPV4
    my_addr.sin_addr.s_addr = htonl( INADDR_ANY );  //将地址填入
    my_addr.sin_port = htons( port );                 //将端口填入

    if ( ::bind( fd(),                               //bind函数实际上是把这个进程和一个ip地址以及一个端口绑定起来
=======
    my_addr.sin_family = AF_INET; // internet connection
    my_addr.sin_addr.s_addr = htonl( INADDR_ANY );
    my_addr.sin_port = htons( port );

    if ( ::bind( fd(),
>>>>>>> master
                 reinterpret_cast< struct sockaddr * >( &my_addr ),
                 sizeof( AddrImpl::AddrType ) ) < 0 )
    {
        std::cerr << "***ERROR*** BasicSocket::bind() failed to bind."
                  << std::endl;
        this->close();
        return false;
    }

    return true;
}

/*-------------------------------------------------------------------*/
/*!

*/
bool
<<<<<<< HEAD
BasicSocket::setAddr( const char * hostname,                            //将hostname和地址联系起来
                      const int port )
{
#ifdef HAVE_GETADDRINFO                                                  //
    struct addrinfo hints;
    std::memset( &hints, 0, sizeof( hints ) );
    hints.ai_family = AF_INET;                                        //指定协议族
    hints.ai_socktype = M_dest->socket_type_;                         //指定socktype
    hints.ai_protocol = 0;                                             //指定protocal为IP协议

    struct addrinfo * res;
    int err = ::getaddrinfo( hostname, NULL, &hints, &res );          //处理hostname到addr的转换，返回一个指向addrinfo的指针
=======
BasicSocket::setAddr( const char * hostname,
                      const int port )
{
#ifdef HAVE_GETADDRINFO
    struct addrinfo hints;
    std::memset( &hints, 0, sizeof( hints ) );
    hints.ai_family = AF_INET;
    hints.ai_socktype = M_dest->socket_type_;
    hints.ai_protocol = 0;

    struct addrinfo * res;
    int err = ::getaddrinfo( hostname, NULL, &hints, &res );
>>>>>>> master
    if ( err != 0 )
    {
        std::cerr << "***ERROR*** BasicSocket::setAddr() failed to find host ["
                  << hostname << "]" << std::endl;
        std::cerr << "BasicSocket. error " << err
                  << gai_strerror( err ) << std::endl;
        this->close();
        return false;

    }

<<<<<<< HEAD
    M_dest->addr_.sin_addr.s_addr                                       //M_dest是BasicSocket类中指向addrImpl的指针
        = (reinterpret_cast< struct sockaddr_in * >(res->ai_addr))->sin_addr.s_addr;//将M_dest对应的地址赋值为由hostname转换而来的地址
    M_dest->addr_.sin_family = AF_INET;
    M_dest->addr_.sin_port = htons( port );                             //注意网络传输中的字节顺序本机传输的字节顺序相反，调用这个函数进行转换

    ::freeaddrinfo( res );                                              //free掉这个指针
=======
    M_dest->addr_.sin_addr.s_addr
        = (reinterpret_cast< struct sockaddr_in * >(res->ai_addr))->sin_addr.s_addr;
    M_dest->addr_.sin_family = AF_INET;
    M_dest->addr_.sin_port = htons( port );

    ::freeaddrinfo( res );
>>>>>>> master
    return true;

#endif

#ifdef HAVE_GETHOSTBYNAME
#ifdef HAVE_INET_ADDR
<<<<<<< HEAD
    M_dest->addr_.sin_addr.s_addr = ::inet_addr( hostname );            //？？？调用函数把hostname改为地址赋给s_addr
    if ( M_dest->addr_.sin_addr.s_addr == 0xffffffff )
#endif
    {
        struct hostent * host_entry = ::gethostbyname( hostname );      //通过hostname返回对应的一个结构体，里面含有IPV4地址，这个是上面的弱化版
=======
    M_dest->addr_.sin_addr.s_addr = ::inet_addr( hostname );
    if ( M_dest->addr_.sin_addr.s_addr == 0xffffffff )
#endif
    {
        struct hostent * host_entry = ::gethostbyname( hostname );
>>>>>>> master
        if ( ! host_entry )
        {
            std::cerr << hstrerror( h_errno ) << std::endl;
            std::cerr << "BasicSocket::setAddr() host not found ["
                      << hostname << "]" << std::endl;
            this->close();
            return false;
        }

        std::memcpy( &(M_dest->addr_.sin_addr.s_addr),
                     host_entry->h_addr_list[0],
                     host_entry->h_length );
    }

    M_dest->addr_.sin_family = AF_INET;
    M_dest->addr_.sin_port = htons( port );

    return true;
#endif

    std::cerr << "***ERROR*** BasicSocket::setAddr() no getaddrinfo or gethostbyname."
              << "failed to find host [" << hostname
              << "]" << std::endl;
    this->close();
    return false;
}

/*-------------------------------------------------------------------*/
/*!

*/
int
BasicSocket::connectToPresetAddr()
{
<<<<<<< HEAD
    int ret = ::connect( fd(),                                            //调用函数进行与addr连接，成功返，错误返回-1
=======
    int ret = ::connect( fd(),
>>>>>>> master
                         reinterpret_cast< const sockaddr * >( &(M_dest->addr_) ),
                         sizeof( AddrImpl::AddrType ) );
    if ( ret == -1 )
    {
        std::perror( "connect" );
    }

    return ret;
}

/*-------------------------------------------------------------------*/
/*!

*/
int
BasicSocket::setNonBlocking()
{
<<<<<<< HEAD
    int flags = ::fcntl( fd(), F_GETFL, 0 );                            //在修改文件描述符标志或文件状态标志时必须谨慎，先要取得现在的标志值，然后按照希望修改它，最后设置新标志值。不能只是执行F_SETFD或F_SETFL命令，这样会关闭以前设置的标志位
    if ( flags == -1 )                                                  //F_GETFL正常情况返回进程ID，错误返回-1
=======
    int flags = ::fcntl( fd(), F_GETFL, 0 );
    if ( flags == -1 )
>>>>>>> master
    {
        return flags;
    }

<<<<<<< HEAD
    return ::fcntl( fd(), F_SETFL, O_NONBLOCK | flags );                //设置给arg描述符一个状态，这里是非阻塞型的I/O，加上文件锁，当多个进程打开同一文件进行读写时可能会出现数据混乱的问题
=======
    return ::fcntl( fd(), F_SETFL, O_NONBLOCK | flags );
>>>>>>> master
}

/*-------------------------------------------------------------------*/
/*!

*/
int
BasicSocket::close()
{
    if ( fd() != -1 )
    {
<<<<<<< HEAD
        int ret = ::close( fd() );                                      //调用close函数来关闭文件描述符，成功返回0，错误返回-1
=======
        int ret = ::close( fd() );
>>>>>>> master
        M_fd = -1;
        std::memset( reinterpret_cast< char * >( &(M_dest->addr_) ),
                     0,
                     sizeof( AddrImpl::AddrType ) );
        return ret;
    }

    return 0;
}

/*-------------------------------------------------------------------*/
/*!

*/
int
<<<<<<< HEAD
BasicSocket::writeToStream( const char * msg,                           //发送信息流
                            const std::size_t len )
{
    int	n = ::send( fd(), msg, len, 0 );                                  //调用send函数发送信息
=======
BasicSocket::writeToStream( const char * msg,
                            const std::size_t len )
{
    int	n = ::send( fd(), msg, len, 0 );
>>>>>>> master

    if ( n == -1 )
    {
        std::perror( "send" );
    }

    return n;
}

/*-------------------------------------------------------------------*/
/*!

*/
int
<<<<<<< HEAD
BasicSocket::readFromStream( char * buf,                                  //接收信息流
                             const std::size_t len )
{
    int n = ::recv( fd(), buf, len, 0 );                                  //调用函数接受到缓冲区
    //std::cerr << "receive: " << n << " bytes" << std::endl;
    if ( n == -1 )
    {
        if ( errno == EWOULDBLOCK )                                       //阻塞的情况
=======
BasicSocket::readFromStream( char * buf,
                             const std::size_t len )
{
    int n = ::recv( fd(), buf, len, 0 );
    //std::cerr << "receive: " << n << " bytes" << std::endl;
    if ( n == -1 )
    {
        if ( errno == EWOULDBLOCK )
>>>>>>> master
        {
            return 0;
        }

        std::perror( "recv" );
        return -1;
    }

    return n;
}


/*-------------------------------------------------------------------*/
/*!

*/
int
<<<<<<< HEAD
BasicSocket::sendDatagramPacket( const char * data,                     //发送数据包就使用这个函数，是两种数据传输方式
                                 const std::size_t len )
{ 
    int n = ::sendto( fd(), data, len, 0,                               //send和sendto的区别https://www.cnblogs.com/developing/articles/10974904.html
=======
BasicSocket::sendDatagramPacket( const char * data,
                                 const std::size_t len )
{
    int n = ::sendto( fd(), data, len, 0,
>>>>>>> master
                      reinterpret_cast< const sockaddr * >( &(M_dest->addr_) ),
                      sizeof( AddrImpl::AddrType ) );

    if ( n != static_cast< int >( len ) )
    {
        std::perror( "sendto" );
        return -1;
    }

    return len;
}

/*-------------------------------------------------------------------*/
/*!

*/
int
<<<<<<< HEAD
BasicSocket::receiveDatagramPacket( char * buf,                         //数据包接收
=======
BasicSocket::receiveDatagramPacket( char * buf,
>>>>>>> master
                                    const std::size_t len,
                                    const bool overwrite_dist_addr )
{
    AddrImpl::AddrType from_addr;
    socklen_t from_size = sizeof( AddrImpl::AddrType );
    int n = ::recvfrom( fd(), buf, len, 0,
                        reinterpret_cast< struct sockaddr * >( &from_addr ),
                        &from_size );
    //std::cerr << "receive: " << n << " bytes" << std::endl;
    if ( n == -1 )
    {
        if ( errno == EWOULDBLOCK )
        {
            return 0;
        }

        std::perror( "recvfrom" );
        return -1;
    }

<<<<<<< HEAD
    if ( overwrite_dist_addr                                            //
=======
    if ( overwrite_dist_addr
>>>>>>> master
         && from_addr.sin_port != 0 )
    {
        //std::cerr << "dest port = " << from.sin_port << std::endl;
        M_dest->addr_.sin_port = from_addr.sin_port;
    }

    return n;
}

} // end namespace
