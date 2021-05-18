/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : history_d.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-07-14
 * Note   : 紀錄遊戲內的各大重要歷史事件
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <daemon.h>

#define DATA_PATH	"/data/daemon/history.o"

array history;

int save()
{
	return save_object(DATA_PATH);	
}

int remove()
{
	return save();
}

void add_history(string key, string value)
{
	history = ({ time(), key, value }) + history;
	
	MYSQL_SCHEDULE_D->refresh_history();
	
	save();
}

void remove_history(int index)
{
	history = history[0..((index-1)*3)-1] + history[(index-1)*3+3..];
	
	MYSQL_SCHEDULE_D->refresh_history();

	save();
}

array query_history()
{
	return copy(history);
}

void create()
{
	if( !restore_object(DATA_PATH) )
	{
		history = allocate(0);
		save();
	}
}

string query_name()
{
	return "歷史事件系統(HISTORY_D)";
}
