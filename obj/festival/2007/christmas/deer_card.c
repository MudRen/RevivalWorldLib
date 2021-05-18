/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : deer_card.c
 * Author : Kyo@RevivalWorld
 * Date   : 2007-12-20
 * Note   : 麋鹿卡片
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

void do_praydeer(object me, string arg)
{
	object target;

	if( !arg )
		return tell(me, pnoun(2, me)+"想要對誰使用麋鹿卡效能？\n");

 	if(!objectp( target = present(arg, environment(me))) )
		return tell( me,"這裡沒有 "+arg+" 這個人。\n");

	if( !userp(target) )
		return tell(me, pnoun(2, me)+"只能對玩家使用麋鹿卡效能。\n");

	msg("$ME大喊一聲：使用魔法卡「"HIR"麋"NOR+RED"鹿"HIW"卡"NOR"」，$YOU獲得聖誕老人的祝福！！\n", me, target, 1);

	target->start_condition(CHRISTMAS_2007);
}

void create()
{
	set_idname("deer card", HIR"麋"NOR+RED"鹿"HIW"卡"NOR);

	if( this_object()->set_shadow_database() ) return;

	set("unit", "張");
	set("long", "遠古時代受到聖誕老人祝福的卡片，具有神奇的力量。");
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

	actions = ([ "praydeer" : (: do_praydeer :) ]);
}
