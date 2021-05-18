/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : Rwlibcomm_d.c
 * Author : Msr@Revival world
 * Date   : 2000-00-00
 * Note   : Rwlibcomm ���F
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
	// ��l�� muds
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
		monitor( sprintf("%s �w�[�J Rwlib �s��", socket_address(fd)) );
		break;
	case "channel" : // �����W�D�T��
	{
		/* �]�� rw �D���O server, �T���ݭn�����b�o����e�h��L mud
		   �ҥH�����H��ݭn�A��e���L mud 						*/
		// ({ "channel", ([ "WHO" : who, "MSG" : msg, "EMOTE" : int, ])});
		CHANNEL_D->channel_broadcast( "rw", msg[1]["MSG"], msg[1]["EMOTE"]);
		break;
	}
	case "send_channel" : // �o�e�W�D�T��
	break;
	}
	 
	monitor( sprintf("�� %s �ǨӰT�� %O", socket_address(fd), msg) );
	return;
}

private void close_callback( int fd )
{
	monitor( sprintf("�Ӧ� %s ���s�u�w����", socket_address(fd)) );
	return;
}

private void write_callback( int fd )
{

}

// socket_listen - ��ť (listen) �@�� socket ���s�� (connection)�C
void listen_callback(int fd)
{
	int new_fd, port;
	string address, ip;
	
	if((new_fd = socket_accept(fd, (: read_callback :), (: write_callback :) )) < 0)
		return;
	
	address = socket_address(new_fd);
	
	monitor( sprintf("�s�u�Ӧ� %s", address ) );

	if (! stringp(address) || sscanf(address, "%s %d", ip, port) != 2)
	{
		socket_close(new_fd);
		return;
	}

	// �O���s�u�W�� Mud �[�J mudlist
	muds[new_fd] = allocate_mapping(0);

	// �n�D�Ǧ^��T, �öǰe�[�� key
	socket_write( new_fd, "reply_data" );
}

string query_name()
{
	return "RWLIB �q���t��(RWLIBCOMM_D)";
}
