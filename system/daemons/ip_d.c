/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : ip_d.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-04-12
 * Note   : IP 精靈
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ip.h>
#include <daemon.h>

#define DATA		"/data/daemon/ip"+__SAVE_EXTENSION__
#define LOG		"daemon/ip"

private mapping ipdata;

int save()
{
	return save_object(DATA);
}

void set_ip(object me, string ip, int status, int time)
{
	ipdata[ip] = allocate_mapping(0);

	ipdata[ip]["status"] = status;
	ipdata[ip]["recordtime"] = time();
	
	if( time )
		ipdata[ip]["releasetime"] = time()+time;
	
	log_file(LOG, me->query_idname()+"設定 IP["+ip+"] 的狀態為 "+status);
	
	save();
}

void delete_ip(object me, string ip)
{
	map_delete(ipdata, ip);

	log_file(LOG, me->query_idname()+"刪除 IP["+ip+"] 的狀態設定");
	
	save();
}

void set_ip_note(object me, string ip, string note)
{
	ipdata[ip]["note"] = note;
	
	save();
}

mapping query_ip(string ip)
{
	return copy(ipdata[ip]);
}

int query_ip_status(string ip)
{
	foreach(string limitedip, mapping data in ipdata)
	if( regexp(ip, limitedip) )
		return data["status"];
}

mapping query_ipdata()
{
	return copy(ipdata);
}

void heart_beat()
{
	int time = time();
	foreach(string ip, mapping data in ipdata)
		if( data["releasetime"] && data["releasetime"] < time )
			map_delete(ipdata, ip);
}

private nomask void create()
{
  	if( clonep() )
  		destruct(this_object());
  	
 	if( !restore_object(DATA) )
  	{
  		ipdata = allocate_mapping(0);
  		save();
  	}
  	
  	set_heart_beat(60);
}

int remove()
{
	return save();
}

string query_name()
{
	return "IP 管理系統(IP_D)";
}
