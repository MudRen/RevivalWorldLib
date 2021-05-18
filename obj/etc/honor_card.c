/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : honor_card.c
 * Author : Clode@RevivalWorld
 * Date   : 2008-02-09
 * Note   : 榮耀卡片
 * Update :
 *  o 2003-00-00  

 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>
#include <condition.h>
#include <feature.h>
#include <equipment.h>
#include <buff.h>
#include <daemon.h>

inherit STANDARD_OBJECT;
inherit EQUIPMENT;

mapping actions;

void do_pray(object me, string arg)
{
	object target;

	if( !arg )
		return tell(me, pnoun(2, me)+"想要對誰使用榮耀卡片？\n");

 	if(!objectp( target = present(arg, environment(me))) )
		return tell( me,"這裡沒有 "+arg+" 這個人。\n");

	if( !userp(target) && !target->is_module_npc() )
		return tell(me, pnoun(2, me)+"只能對玩家或員工使用榮耀卡片。\n");

	msg("$ME大喊一聲：使用魔法卡「"HIY"榮"NOR+YEL"耀"HIY"卡"NOR YEL"片"NOR"」，$YOU獲得真誠的祝福！！\n", me, target, 1);

	target->start_condition(HONOR);
}

void create()
{
	set_idname("honor card", HIY"榮"NOR+YEL"耀"HIY"卡"NOR YEL"片"NOR);

	if( this_object()->set_shadow_database() ) return;

	set("unit", "張");
	set("long", "象徵遊戲中最高的榮耀，具有神奇的力量，收集愈多張效果愈強。");
	set("mass", 10);
	set("value", 1);
	set("flag/no_drop", 1);
	set("flag/no_give", 1);

	::setup_equipment(EQ_ETC, HIG"卡"NOR GRN"片"NOR);

	actions = ([ "pray" : (: do_pray :) ]);
}
