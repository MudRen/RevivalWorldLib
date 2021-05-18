/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : moonrabbit_card.c
 * Author : Kyo@RevivalWorld
 * Date   : 2007-09-28
 * Note   : 月兔卡片
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

void do_prayrabbit(object me, string arg)
{
	object target;

	if( !arg )
		return tell(me, pnoun(2, me)+"想要對誰使用月懸碧空？\n");

 	if(!objectp( target = present(arg, environment(me))) )
		return tell( me,"這裡沒有 "+arg+" 這個人。\n");

	if( !userp(target) )
		return tell(me, pnoun(2, me)+"只能對玩家使用月兔卡效能。\n");

	msg("$ME大喊一聲：翻開覆蓋在檯面上的陷阱卡「"HIC"月"NOR+CYN"兔"HIW"卡"NOR"」，$YOU可獲得月兔的祝福！！\n", me, target, 1);

	target->start_condition(MOONCAKE_2007);
}

void create()
{
	set_idname("moonrabbit card", HIC"月"NOR+CYN"兔"HIW"卡"NOR);

	if( this_object()->set_shadow_database() ) return;

	set("unit", "張");
	set("long", "遠古時代受到月兔祝福的卡片，具有神奇的力量。");
	set("mass", 10);
	set("value", 1);
	set("flag/no_amount", 1);
	set("flag/no_drop", 1);
	set("flag/no_give", 1);

	set(BUFF_STR, 1);
	set(BUFF_PHY, 1);
	set(BUFF_AGI, 1);
	set(BUFF_INT, 1);
	set(BUFF_CHA, 1);

	set(BUFF_STAMINA_REGEN, 5);
	set(BUFF_HEALTH_REGEN, 5);
	set(BUFF_ENERGY_REGEN, 5);

	::setup_equipment(EQ_ETC, HIG"卡"NOR GRN"片"NOR);

	actions = ([ "prayrabbit" : (: do_prayrabbit :) ]);
}
