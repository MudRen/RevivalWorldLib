/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : Rwlibcomm_d.c
 * Author : Msr@Revival world
 * Date   : 2000-00-00
 * Note   : Rwlibcomm 精靈
 * Update :
 *  o 2000-00-00 
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <socket.h>
#include <socket_err.h>
#include <daemon.h>

#define USE_PORT 9994
#define monitor(x)	CHANNEL_D->channel_broadcast("sys", "RWLIBCOMM_D: " + x)

private nosave int my_socket, send_socket;
private nosave mapping muds;

mapping query_muds()
{
	return copy(muds);
}

private void create() 
{
	// 初始化 muds
	muds = allocate_mapping(0);

	if( (my_socket = socket_create( MUD, "read_callback", "close_callback")) < 0)
		return;

	if( socket_bind( my_socket, USE_PORT) < 0)
	{
		socket_close(my_socket);
		return;
	}

	if( socket_listen( my_socket, "listen_callback" ) < 0)
	{
		socket_close(my_socket);
		return;
	}
}

private void read_callback( int fd, mixed msg )
{
	string cmd = msg[0];
	
	switch(cmd) {
	case "reply_data" :
		muds[fd] = msg[1];
		monitor( sprintf("%s 已加入 Rwlib 連結", socket_address(fd)) );
		break;
	case "channel" : // 接收頻道訊息
	{
		/* 因為 rw 主機是 server, 訊息需要全部在這邊轉送去其他 mud
		   所以接收以後需要再轉送到其他 mud 						*/
		// ({ "channel", ([ "WHO" : who, "MSG" : msg, "EMOTE" : int, ])});
		CHANNEL_D->channel_broadcast( "rw", msg[1]["MSG"], msg[1]["EMOTE"]);
		break;
	}
	case "send_channel" : // 發送頻道訊息
	break;
	}
	 
	monitor( sprintf("由 %s 傳來訊息 %O", socket_address(fd), msg) );
	return;
}

private void close_callback( int fd )
{
	monitor( sprintf("來自 %s 的連線已結束", socket_address(fd)) );
	return;
}

private void write_callback( int fd )
{

}

// socket_listen - 傾聽 (listen) 一個 socket 的連接 (connection)。
void listen_callback(int fd)
{
	int new_fd, port;
	string address, ip;
	
	if((new_fd = socket_accept(fd, (: read_callback :), (: write_callback :) )) < 0)
		return;
	
	address = socket_address(new_fd);
	
	monitor( sprintf("連線來自 %s", address ) );

	if (! stringp(address) || sscanf(address, "%s %d", ip, port) != 2)
	{
		socket_close(new_fd);
		return;
	}

	// 記錄連線上的 Mud 加入 mudlist
	muds[new_fd] = allocate_mapping(0);

	// 要求傳回資訊, 並傳送加秘 key
	socket_write( new_fd, "reply_data" );
}

string query_name()
{
	return "RWLIB 通報系統(RWLIBCOMM_D)";
}
