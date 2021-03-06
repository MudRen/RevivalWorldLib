/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : _save.c
 * Author : Clode@RevivalWorld
 * Date   : 2000-12-30
 * Note   : 儲存與讀取路徑程式
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <daemon.h>

// 只允許以下路徑下的物件可以進行儲存
#define VALID_SAVE_PATH	\
({			\
"/area",		\
"/city",		\
"/data",		\
"/product",		\
"/npc",			\
})

nosave int delay_save_call_out_handle = 0;

varargs nomask int save(string file)
{
	object ob = this_object();

	remove_call_out(delay_save_call_out_handle);
	delay_save_call_out_handle = 0;

	if( !objectp(ob) )
		return 0;

	if( !stringp(file) )
		file = ob->query_save_file() || base_name(ob);

	// 只允許特定路徑的儲存動作
	if( member_array(file[0..strsrch(file[1..], "/")], VALID_SAVE_PATH) == -1 )
		return 0;

	// 只有原始物件有權利儲存資料
	if( file_name(ob) != base_name(ob) )
		return 0;

	// 若有 nosave() 則不儲存
	if( ob->no_save(file) )
		return 0;

	// 建立完整路徑
	intact_path(file);

	if( userp(ob) || ob->is_module_npc() || ob->is_module_room() )
		ob->save_inventory();
	
	//CHANNEL_D->channel_broadcast("nch", file +" saved");

	if( !save_object(file) )
		log_file("save_failed", file);
	else
		return 1;
}

void delay_save(int sec)
{
	if( !delay_save_call_out_handle )
	{
		// 增加 IO 分散度(但可能造成資料不同步)
		if( sec >= 60 )
			sec = range_random(sec-10, sec+10);

		delay_save_call_out_handle = call_out( (: save :), sec );
	}
}

varargs nomask varargs int restore(string file, int i)
{
	if( undefinedp(file) )
		file = this_object()->query_save_file() || base_name(this_object());
		
	if( !stringp(file) )
		return 0;

	if( restore_object(file, i) )
	{
		this_object()->reset_objects();
		return 1;
	}
	else
		return 0;
}
