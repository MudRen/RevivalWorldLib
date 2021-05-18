/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : object.c
 * Author : Clode@RevivalWorld
 * Date   : 2006-01-14
 * Note   : 
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <inherit.h>
#include <condition.h>

inherit STANDARD_MODULE_PRODUCT;

int is_compact_disk()
{
	return 1;
}

void play_cd(object me, int effect)
{
	msg("$ME�}�l��ť�u"+this_object()->query_idname()+"�v�A�߱����дe�@���ӪšC\n", me, 0, 1);
	me->start_condition(AUDIO, effect);
	destruct(this_object(), 1);
}

varargs void create(string file, string type)
{
	::create(file, type);

	set("unit", "�i");
	set("mass", 10);
	set("value", query_value());

	setup_inlay();
}
