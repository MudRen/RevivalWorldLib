/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : charisma-adv.c
 * Author : Clode@RevivalWorld
 * Date   : 2006-02-09
 * Note   : 魅力無涯
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <skill.h>
#include <inherit.h>
#include <message.h>
#include <daemon.h>

inherit STANDARD_SKILL;

void create()
{
	// 技能中文
	set("name", 		"魅力無涯");

	// 技能英文
	set("id",		"charisma-adv");

	// 技能頭銜
	//set("title/100",	"");
	
	// 技能註解
	set("note",		"魅力屬性神之鍛鍊");
	
	// 文明時代
	set("age",		1);
	
	// 技能種類
	set("type",		SOCIAL_SKILL | COMBAT_SKILL);
}

// 限制可學習對象, 可用來設定各種學習條件
varargs int allowable_learn(object ob)
{
	if( undefinedp(ob) )
		return 0;
	else
	{
		if( ob->is_npc() )
			return 0;
		else
			return 0;
	}
}

// 可自設每個等級需要的經驗值, 或直接使用 SKILL_D 預設數學式
int level_exp(int lv)
{
	return SKILL_D->query_level_exp(lv);
}

// 可設定各種最高等級條件, 
int max_level(object ob)
{
	return 100;
}

// 可設定不同條件之經驗值乘數
float exp_multiple()
{
	return 1.0;
}

// 升級之前的事件
void after_level_up(object ob, int newlevel)
{
	if( !(newlevel % 5) )
	{
		ob->add_cha(1);
		tell(ob, HIY+pnoun(2, ob)+"增加了 1 點的魅力屬性。\n"NOR);
	}
}

// 降級之後的事件
void before_level_down(object ob, int oldlevel)
{
	if( !(oldlevel % 5) )
	{
		ob->add_cha(-1);
		tell(ob, HIY+pnoun(2, ob)+"減少了 1 點的魅力屬性。\n"NOR);
	}
}

// 經驗值增加後的事件
void added_exp(object ob, float exp)
{

}

// 是否只能自我學習
int self_learning()
{
	return 1;
}

// 特殊標記
string query_special_type()
{
	return HIC"稀"NOR CYN"有"NOR;
}
