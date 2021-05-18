/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : taichi.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-12-20
 * Note   : 太極圖
 * Update :
 *  o 2003-00-00  

 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>
#include <feature.h>
#include <skill.h>

inherit STANDARD_OBJECT;

#define BOOK_EXP	200
#define COST_EXP	100000

mapping actions;

void do_readbook(object me, string arg)
{
	int exp;
	string skill = "taichi";

	if( me->is_module_npc() )
	{
		me->do_command("say 我無法學習此技能。");
		return;
	}

	exp = BOOK_EXP * me->query_int();

	msg("$ME仔細閱讀$YOU，共得到 "+exp+" 點的「"+(SKILL(skill))->query_idname()+"」經驗值。\n", me, this_object(), 1);
	me->add_skill_exp(skill, exp);
	me->save();
	msg("$YOU在$ME閱讀完後便化為碎片。\n", me, this_object(), 1);

	destruct(this_object(), 1);
}

void create()
{
	set_idname(HIW"tai "NOR WHT"chi"NOR, HIW"太"NOR WHT"極"NOR"圖");

	if( this_object()->set_shadow_database() ) return;

	set("unit", "個");
	set("long", "易有太極，是生兩儀。兩儀生四象，四象生八卦。");
	set("mass", 10);
	set("value", 1);

	actions = ([ "readbook" : (: do_readbook :) ]);
}
