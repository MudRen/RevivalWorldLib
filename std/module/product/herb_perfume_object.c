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

#include <ansi.h>
#include <inherit.h>
#include <feature.h>
#include <condition.h>
#include <use.h>

inherit STANDARD_MODULE_PRODUCT;

varargs void create(string file, string type)
{
	::create(file, type);

	set("unit", "�~");
	set("mass", 100);
	set("value", query_value());
	set("effect", 3600);

	setup_inlay();
}

string query_description()
{
	return "�B�į�M���[�� "+(query("effect")/60)+" ����";
}

int do_use(object me)
{
	int effect = query("effect");

	msg("$ME�b���W�Q�x$YOU�C\n", me, this_object(), 1);
	me->start_condition(PERFUME_HERB, effect);

	return DESTRUCT_ONE;
}
