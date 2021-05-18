/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : mnlmudlist_d.c
 * Author : Clode@RevivalWorld
 * Date   : 2004-06-17
 * Note   : 
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */
 
#include <socket.h>
#include <socket_err.h>
#include <daemon.h>
#include <time.h>
#include <mudlib.h>

#define DATA_PATH	"/data/daemon/mnlmudlist.o"
#define MRTGDATA 	"/data/daemon/mrtg/mnlmudlist/"
#define MSG(x)		CHANNEL_D->channel_broadcast("nch", x)
#define DEBUG(x)	//CHANNEL_D->channel_broadcast("nch", x)

mapping mnlmudlist = allocate_mapping(0);

nosave string *connectsort = allocate(0);
nosave mapping resolvedaddress = allocate_mapping(0);

nosave int current_call_out_handler;
nosave int current_socket_port;

void over_waiting_connect(int socket_port);


int save()
{
	return save_object(DATA_PATH);	
}

void connect()
{
	int socket_port;
	string ipport;
	
	if( !sizeof(connectsort) ) return;
	
	ipport = connectsort[0];

	socket_port = socket_create(STREAM, "socket_create_read_callback", "socket_create_close_callback");
	
	if( socket_port < 0) return;

	current_socket_port = socket_port;
	
	DEBUG(sizeof(connectsort)+". "+ipport+" �s�u�ˬd");
	if( socket_connect(socket_port, resolvedaddress[ipport], "connect_read_callback", "connect_write_callback") < 0 ) 
		socket_close(socket_port);

	map_delete(mnlmudlist[ipport], "USERS");

	current_call_out_handler = call_out((: over_waiting_connect :), 10, socket_port);
}

void connect_check()
{
	string address, ipport;
	
	if( !sizeof(connectsort) )
	{
		save();
		
		MSG("MNLMUDLIST ��s��Ƨ����A�@ "+sizeof(mnlmudlist)+" �� Mud ���");
		return;
	}

	ipport = connectsort[0];

	mnlmudlist[ipport]["LAST_CONNECT_CHECK_TIME"] = time();

	sscanf(ipport, "%s %*d", address);

	if( sscanf(address, "%*d.%*d.%*d.%*d") == 4 )
	{
		resolvedaddress[ipport] = ipport;		
		connect();
	}
	else
	{
		DEBUG(sizeof(connectsort)+". "+address+" �i��W�ٸѪR");
		resolve(address, "resolve_address_callback");
	}
}

void resolve_address_callback(string address, string resolved, int key)
{
	int port;
	string ipport;
	
	remove_call_out(current_call_out_handler);

	if( !sizeof(connectsort) ) return;
	
	ipport = connectsort[0];

	if( !resolved )
	{
		mnlmudlist[ipport]["CONNECT_FAILED_TIMES"]++;
		map_delete(mnlmudlist[ipport], "USERS");
		
		DEBUG(sizeof(connectsort)+". "+ipport+" �ˬd�O��");
		connectsort = connectsort[1..];
		connect_check();
	}
	else
	{
		sscanf(ipport, "%*s %d", port);
		resolvedaddress[ipport] = resolved+" "+port;
	
		connect();
	}
}

void over_waiting_connect(int socket_port)
{
	string ipport;
	
	if( !sizeof(connectsort) ) return;
	
	ipport = connectsort[0];

	if( socket_port != current_socket_port )
	{
		socket_close(socket_port);
	}
	else
	{
		mnlmudlist[ipport]["CONNECT_FAILED_TIMES"]++;
		remove_call_out(current_call_out_handler);
		socket_close(current_socket_port);
	
		DEBUG(sizeof(connectsort)+". "+ipport+" �ˬd�O��");
		connectsort = connectsort[1..];
		connect_check();
	}
}

void connect_write_callback(int socket_port)
{   
	socket_write(socket_port, "big5\n");
}

void connect_read_callback(int socket_port, mixed info) 
{
	array usercount = allocate(3);
	string ipport;
	
	if( !sizeof(connectsort) ) return;
	
	ipport = connectsort[0];
	
	if( socket_port != current_socket_port )
	{
		socket_close(socket_port);
		return;
	}

	mnlmudlist[ipport]["LAST_CONTACT_TIME"] = time();

	remove_call_out(current_call_out_handler);
	
	map_delete(mnlmudlist[ipport], "CONNECT_FAILED_TIMES");

	if( stringp(info) && sizeof(mnlmudlist[ipport]["USERS_COUNT_PARSE"])>0 && !mnlmudlist[ipport]["USERS"] )
	{
		sscanf(remove_ansi(info), "%*s"+mnlmudlist[ipport]["USERS_COUNT_PARSE"]+"%*s", usercount[0], usercount[1], usercount[2] );
		
		usercount = map(usercount, (: CHINESE_D->chinese_to_number($1) :));

		mnlmudlist[ipport]["USERS"] = implode(usercount, (:$1+$2:));

		if( !mnlmudlist[ipport]["USERS"] )
		{
			// �Q���S�A���� read_callback �N���L users count �p��
			current_call_out_handler = call_out((: connect_read_callback :), 10, socket_port, 0);
			return;
		}
	}

	DEBUG(sizeof(connectsort)+". "+ipport+" �ˬd����");
	socket_close(socket_port);
 
	connectsort = connectsort[1..];
	connect_check();

}

// �N��@ Mud �[�J MRTG
void add_mud_to_mrtg(string name)
{
	mapping info = ([
		"Directory":"mnlmudlist/"+replace_string(name, " ", "_"),
		"Target":"`cat \""LIBRARY_PATH+MRTGDATA+name+"\"`",
		"YLegend":"user(s)",
		"LegendI":"�u�W�H��",
		"LegendO":"",
		"Legend1":"�b�C�������u�W�ϥΪ̤H��",
		"ShortLegend":"�H",
		"Legend2":"",
		"Title": mnlmudlist[name]["MUD_CHINESE_NAME"]+" - "+mnlmudlist[name]["MUD_ENGLISH_NAME"]+" �H�Ʋέp�C��",
		"PageTop":mnlmudlist[name]["MUD_CHINESE_NAME"]+" - "+mnlmudlist[name]["MUD_ENGLISH_NAME"]+" �H�Ʋέp�C��"
	]);
	
	// ������ Mud ���u�W���a��
	write_file(MRTGDATA+name, mnlmudlist[name]["USERS"]+"\n0", 1);

	MRTG_D->addmrtg(name, info);
}

// �N�O�W���a�`�ƥ[�J MRTG
void add_taiwan_muds_to_mrtg(int alluser)
{
	string name="0_total_count";
	mapping info=allocate_mapping(0);
	
	info = ([
		"Directory":"mnlmudlist/"+name,
		"Target":"`cat \""LIBRARY_PATH+MRTGDATA+name+"\"`",
		"YLegend":"user(s)",
		"LegendI":"�u�W�H��",
		"LegendO":"MUD ����",
		"Legend1":"�b�x�W MUDs �C�������u�W�`�ϥΪ̤H��",
		"Legend2":"��ɩҺʴ��� MUDs �ƶq",
		"ShortLegend":"&nbsp",
		"MaxBytes":3000,
		"AbsMax":5000,
		"Title": "�x�W MUDs �H�Ʋέp�C��",
		"PageTop":"�x�W MUDs �H�Ʋέp�C��"
	]);
	
	// �����Ҧ� Mud �u�W���a�`��
	write_file(MRTGDATA+"0_total_count", alluser+"\n"+sizeof(mnlmudlist), 1);

	MRTG_D->addmrtg(name, info);
}

// ��s MRTG ��T
void update_mrtg_data()
{
	int i;
	int hour;
	int alluser = 0;

	foreach( string name , mapping m in mnlmudlist )
	{
		if( !stringp(name) || !mapp(m) ) continue;
		
		// �����B�z
		call_out((:add_mud_to_mrtg, name:), ++i*2);

		alluser += m["USERS"];
	}

	hour = TIME_D->query_realtime_array()[HOUR];
	if( alluser > query("mnlmudlist/maxusers/hour/"+hour, SYSDATABASE_D->query_ob()) )
		set("mnlmudlist/maxusers/hour/"+hour, alluser, SYSDATABASE_D->query_ob());

	add_taiwan_muds_to_mrtg(alluser);
}

void reset_maxuser(int hour)
{
	delete("mnlmudlist/maxusers/hour/"+hour, SYSDATABASE_D->query_ob());
}

void distributed_connect()
{
	if( sizeof(connectsort) ) return;

	connectsort = keys(mnlmudlist);
	connect_check();
	
	update_mrtg_data();
	DEBUG("MNLMUDLIST �}�l���o��T");
}

void change_ipport(string old_ipport, string new_ipport)
{
	if( undefinedp(mnlmudlist[old_ipport]) )
		return;

	socket_close(current_socket_port);
	remove_call_out(current_call_out_handler);
	connectsort = allocate(0);

	mnlmudlist[new_ipport] = copy(mnlmudlist[old_ipport]);
	map_delete(mnlmudlist, old_ipport);
	
	distributed_connect();
	save();
}

void add_mud(string ipport)
{	
	if( !mapp(mnlmudlist[ipport]) )
		mnlmudlist[ipport] = allocate_mapping(0);
	
	socket_close(current_socket_port);
	remove_call_out(current_call_out_handler);
	connectsort = allocate(0);
	
	distributed_connect();
	save();
}

void add_mud_info(string ipport, string key, string value)
{	
	if( !mapp(mnlmudlist[ipport]) )
		add_mud(ipport);
	
	mnlmudlist[ipport][key] = value;
}

void remove_mud_info(string ipport, string key)
{
	if( !mapp(mnlmudlist[ipport]) ) return;
	
	map_delete(mnlmudlist[ipport], key);
}

void remove_mud(string ipport)
{
	MRTG_D->removemrtg(replace_string(ipport, " ", "_"));
	map_delete(mnlmudlist, ipport);

	socket_close(current_socket_port);
	remove_call_out(current_call_out_handler);
	connectsort = allocate(0);
	
	distributed_connect();

	save();
}

mapping query_mnlmudlist()
{
	return mnlmudlist;
}

int remove()
{
	socket_close(current_socket_port);
	remove_call_out(current_call_out_handler);
	
	return save();
}

void create()
{
	if( !restore_object(DATA_PATH) )
		save();

	distributed_connect();
}
string query_name()
{
	return "�d�ڦC��t��(MNLMUDLIST_D)";
}
