/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : _save.c
 * Author : Clode@RevivalWorld
 * Date   : 2000-12-30
 * Note   : �x�s�PŪ�����|�{��
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <daemon.h>

// �u���\�H�U���|�U������i�H�i���x�s
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

	// �u���\�S�w���|���x�s�ʧ@
	if( member_array(file[0..strsrch(file[1..], "/")], VALID_SAVE_PATH) == -1 )
		return 0;

	// �u����l�����v�Q�x�s���
	if( file_name(ob) != base_name(ob) )
		return 0;

	// �Y�� nosave() �h���x�s
	if( ob->no_save(file) )
		return 0;

	// �إߧ�����|
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
		// �W�[ IO ������(���i��y����Ƥ��P�B)
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
