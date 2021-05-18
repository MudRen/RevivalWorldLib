/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : taichi.c
 * Author : Clode@RevivalWorld
 * Date   : 2009-08-10
 * Note   : 太極
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
	set("name", 		"易生太極");

	// 技能英文
	set("id",		"taichi");

	// 技能頭銜
	set("title/100",	HIW"太"NOR WHT"極"NOR);
	
	// 技能註解
	set("note",		"易有太極，是生兩儀。兩儀生四象，四象生八卦");
	
	// 文明時代
	set("age",		1);
	
	// 技能種類
	set("type",		SOCIAL_SKILL);
}

// 限制可學習對象, 可用來設定各種學習條件
varargs int allowable_learn(object ob)
{
	if( undefinedp(ob) )
		return PLAYER_SKILL;
	else
	{
		if( ob->is_npc() )
			return 0;
		else
			return 1;
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
	if( ob->is_module_npc() )
		return 100;
	else
		return 100;
}

// 可設定不同條件之經驗值乘數
float exp_multiple()
{
	return 1;
}

// 升級之前的事件
void after_level_up(object ob, int newlevel)
{

}

// 降級之後的事件
void before_level_down(object ob, int oldlevel)
{
	
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
	return HIW"史"NOR WHT"詩"NOR;
}

