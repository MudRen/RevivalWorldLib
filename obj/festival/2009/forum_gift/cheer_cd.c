/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : cheer_cd.c
 * Author : Clode@RevivalWorld
 * Date   : 2009-08-11
 * Note   : �����s CD!
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>
#include <feature.h>
#include <condition.h>
#include <buff.h>

inherit STANDARD_OBJECT;

int is_compact_disk()
{
	return 1;
}

void play_cd(object me, int effect)
{
	msg("$ME�}�l��ť�u"+this_object()->query_idname()+"�v�A�²b���n���w�w�Ĥ�"+pnoun(2, me)+"�����F�C\n", me, 0, 1);
	me->start_condition(CHEER_COFFEE, effect);
}

void create()
{
    set_idname("cheer's cd", HIG"�����s "NOR GRN"���M��"NOR);
    set_temp("status", HIG"��"NOR GRN"�_");

    if( this_object()->set_shadow_database() ) return;

    set("long", "�����s���M��I�I���q�o��I�I�C\n");

    set("unit", "�i");
    set("mass", 1);
    set("value", 100000);
    set("badsell", 1);
    set("flag/no_give", 1);
    set("flag/no_drop", 1);
}
