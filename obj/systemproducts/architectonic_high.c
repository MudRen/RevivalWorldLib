/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : architectonic_high.c
 * Author : Clode@RevivalWorld
 * Date   : 2010-01-23
 * Note   : 偉大建築
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
	string skill = "architectonic-high";

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
	set_idname(WHT"great "HIW"architectonic"NOR, WHT"偉大"HIW"建築"NOR YEL"古籍"NOR);

	if( this_object()->set_shadow_database() ) return;

	set("unit", "本");
	set("long", "遠古諸神遺留在人間的古籍，裡頭記載著各種神乎奇技的建築技術。");
	set("mass", 10);
	set("value", 1);

	actions = ([ "readbook" : (: do_readbook :) ]);
}
