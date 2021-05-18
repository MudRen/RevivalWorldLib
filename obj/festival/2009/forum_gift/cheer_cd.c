/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : cheer_cd.c
 * Author : Clode@RevivalWorld
 * Date   : 2009-08-11
 * Note   : 陳綺貞 CD!
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
	msg("$ME開始聆聽「"+this_object()->query_idname()+"」，純淨的聲音緩緩融化"+pnoun(2, me)+"的心靈。\n", me, 0, 1);
	me->start_condition(CHEER_COFFEE, effect);
}

void create()
{
    set_idname("cheer's cd", HIG"陳綺貞 "NOR GRN"精選專輯"NOR);
    set_temp("status", HIG"傳"NOR GRN"奇");

    if( this_object()->set_shadow_database() ) return;

    set("long", "陳綺貞精選專輯！！限量發行！！。\n");

    set("unit", "張");
    set("mass", 1);
    set("value", 100000);
    set("badsell", 1);
    set("flag/no_give", 1);
    set("flag/no_drop", 1);
}
