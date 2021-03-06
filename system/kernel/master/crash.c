/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : crash.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-07-05
 * Note   : included by Master Object
 * Update :
 *  o 2002-00-00

 -----------------------------------------
 */
#include <ansi.h>
#include <mudlib.h>
#include <daemon.h>

#define LOG		"system/crashes"

/* 當發生了 driver segmentation fault 或 bus error 等錯誤時, 將呼叫此函式 */
private void crash(string error, object command_giver, object current_object)
{	
	log_file(LOG, MUD_ENGLISH_NAME + " crashed on: " + ctime(time()) +", error: " + error + "\n");
	log_file(LOG, "command_giver: " + (command_giver ? file_name(command_giver) : "none") + "\n");
	log_file(LOG, "current_object: " + (current_object ? file_name(current_object) : "none") + "\n");
	log_file(LOG, "call_stack(0): "+sprintf("%O\n", call_stack(0)));
	log_file(LOG, "call_stack(1): "+sprintf("%O\n", call_stack(1)));
	log_file(LOG, "call_stack(2): "+sprintf("%O\n", call_stack(2)));
	log_file(LOG, "call_stack(3): "+sprintf("%O\n", call_stack(3)));
	log_file(LOG, "this_player(): "+sprintf("%O\n", this_player()));
	log_file(LOG, "this_player(1): "+sprintf("%O\n", this_player(1)));
	log_file(LOG, "this_player_command: " + (this_player() ? sprintf("%O\n", query_temp("command", this_player())) : "none"));
	log_file(LOG, "previous_object(): "+sprintf("%O\n", previous_object()));
	
	foreach( object ob in users() )
	{
		reset_eval_cost();

		if( objectp(ob) && objectp(environment(ob)) )
			ob->save();
	}

	MONEY_D->save();
	reset_eval_cost();

	MAP_D->save();
	reset_eval_cost();

	CITY_D->save_all();
	reset_eval_cost();

	log_file(LOG, "系統資料備份完畢");

	shout(MUD_FULL_NAME+HIR"系統發生了前所未有的錯誤，要倒閉了！重要資料備份完畢！\n"NOR);
	
	foreach( object ob in users() )
		flush_messages(ob);
}

/* mud 緩慢關閉的過程 */
private int slow_shutdown( int min )
{
	shout("system", MUD_FULL_NAME"系統記憶體不足，再過 "+min+" 分鐘就要倒閉了!!\n");
	return 1;
}
