/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : speed-stance.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-08-31
 * Note   : 速度姿態
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
	set("name", 		"速度姿態");

	// 技能英文
	set("id",		"speed-stance");

	// 技能頭銜
	//set("title/100",	"");
	
	// 技能註解
	set("note",		"速度姿態訓練，最高可提升 20% 攻速");
	
	// 文明時代
	set("age",		1);
	
	// 技能種類
	set("type",		COMBAT_SKILL);
}

// 限制可學習對象, 可用來設定各種學習條件
varargs int allowable_learn(object ob)
{
	if( undefinedp(ob) )
		return PLAYER_SKILL | NPC_SKILL;
	else
	{
		if( ob->is_npc() )
			return 100;
		else
			return 100;
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
	return 0.05;
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
	return 0;
}

// 特殊標記
string query_special_type()
{
	return "";
}
