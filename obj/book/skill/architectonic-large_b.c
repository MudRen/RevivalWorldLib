/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : architectonic-large_b.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-12-23
 * Note   : 巨型建築技能書
 * Update :
 *  o 2003-00-00  

 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>
#include <skill.h>

#define BOOK_SKILL 	"architectonic-adv"
#define BOOK_EXP	10000
#define BOOK_LEVEL	"B"
#define BASE_SKILL	([ "architectonic" : 100 ])

inherit STANDARD_OBJECT;

mapping actions;

void do_readbook(object me, string arg)
{
	foreach(string skill, int level in BASE_SKILL)
	{
		if( me->query_skill_level(skill) < level )
			return tell(me, pnoun(2, me)+"的「"+(SKILL(skill))->query_idname()+"」等級還不足以用來瞭解「"+(SKILL(BOOK_SKILL))->query_idname()+"」。\n");
	}

 	msg("$ME開始認真讀起$YOU，只見$ME豁然開朗似的不斷的點頭，似乎從裡頭得到了很多的東西。\n", me, this_object(), 1);

	tell(me, pnoun(2, me)+"獲得了 "+BOOK_EXP+" 點「"+(SKILL(BOOK_SKILL))->query_idname()+"」經驗值。\n");
	me->add_skill_exp(BOOK_SKILL, BOOK_EXP);
	
	msg("$YOU在$ME閱讀完後便化為碎片。\n", me, this_object(), 1);

	destruct(this_object(), 1);
}

void create()
{
	set_idname(HIG+(SKILL(BOOK_SKILL))->query_id()+NOR GRN" book "BOOK_LEVEL+NOR, HIG+(SKILL(BOOK_SKILL))->query_name()+NOR GRN"技術書 "BOOK_LEVEL" 卷"NOR);
	set_temp("status", HIC"稀"NOR CYN"有");

	if( this_object()->set_shadow_database() ) return;
	
	set("unit", "本");
	set("mass", 300);
	set("value", 10000);
	set("badsell", 1);

	actions = ([ "readbook" : (: do_readbook :) ]);

}