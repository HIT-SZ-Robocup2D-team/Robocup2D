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

#include "sample_player.h"

#include <rcsc/common/basic_client.h>

#include <iostream>
#include <cstdlib> // 用来提供exit函数
#include <cerrno> // errno 是一个用来反馈错误的变量，由系统维护，可以用来检查程序，正常为0，详情https://www.cnblogs.com/Jimmy1988/p/7485133.html
#include <cstring> // strerror 用来返回errno对应的错误字符串的指针，和上一个配合使用打印错误原因
#include <csignal> // sigaction用来查询和设置信号处理方式（具体怎样不懂）

namespace {

SamplePlayer agent;

/*-------------------------------------------------------------------*/
void
sig_exit_handle( int )                                  //结束程序的函数，是非正常死亡
{
    std::cerr << "Killed. Exiting..." << std::endl;
    agent.finalize();
    std::exit( EXIT_FAILURE );
}

}

/*-------------------------------------------------------------------*/
int
main( int argc, char **argv )
{
    struct sigaction sig_action;
    sig_action.sa_handler = &sig_exit_handle;                              //将sigaction函数的处理定义为自己的sig_exit_handle来处理断开连接等情况
    sig_action.sa_flags = 0;                                              //SIGACTION函数处理成功返回0，失败-1，这是函数处理失败时的情况
    if ( sigaction( SIGINT, &sig_action , NULL ) != 0                     //SIGINT为交互注意信号
         || sigaction( SIGTERM, &sig_action , NULL ) != 0                 //SIGTERM为程序的终止信号
         || sigaction( SIGHUP, &sig_action , NULL ) != 0 )                //SIGHUP为连接结束信号
        /*if ( signal(SIGINT, &sigExitHandle) == SIG_ERR 
          || signal(SIGTERM, &sigExitHandle) == SIG_ERR
          || signal(SIGHUP, &sigExitHandle) == SIG_ERR )*/
    {
        std::cerr << __FILE__ << ": " << __LINE__
                  << ": could not set signal handler: "
                  << std::strerror( errno ) << std::endl;
        std::exit( EXIT_FAILURE );
    }                                                                   //这一节意思是遇到断开等情况没有成功的调用自己设定的函数是打印错误

    rcsc::BasicClient client;

    if ( ! agent.init( &client, argc, argv ) )
    {
        return EXIT_FAILURE;
    }

    /*
      You should add your copyright message here.
     */
    // std::cout << "*****************************************************************\n"
    //           << " This program is modified by <Team Name>\n"
    //           << " Copyright 20xx. <Your name>.\n"
    //           << " <Affiliation>\n"
    //           << " All rights reserved.\n"
    //           << "*****************************************************************\n"
    //           << std::flush;

    /*
      Do NOT remove the following copyright notice!
     */
    std::cout << "*****************************************************************\n"
              << " This program is based on agent2d created by Hidehisa Akiyama.\n"
              << " Copyright 2006 - 2011. Hidehisa Akiyama and Hiroki Shimora.\n"
              << " All rights reserved.\n"
              << "*****************************************************************\n"
              << std::flush;

    client.run( &agent );

    return EXIT_SUCCESS;
}
