/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : skill_d.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-04-13
 * Note   : 技能精靈
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <skill.h>

string *skills;
string *sorted_skills;

#define EXP_SHIFT		7700

string *manual_skill_sort =
({
	HIG"屬性能力"NOR GRN"培養"NOR,
	"strength",
	"strength-adv",
	"physique",
	"physique-adv",
	"intelligence",
	"intelligence-adv",
	"agility",
	"agility-adv",
	"charisma",
	"charisma-adv",
	"stamina",
	"stamina-adv",
	"health",
	"health-adv",
	"energy",
	"energy-adv",
	"food",
	"drink",
	"feature",

	HIG"綜合技巧"NOR GRN"訓練"NOR,
	"leadership",
	"leadership-adv",
	"architectonic",
	"architectonic-adv",
	"architectonic-high",
	"exchange",
	"price",
	"stock",
	"estaterebate",
	"sport",
	"consciousness",
	"teach",
	"eloquence",
	"idle",
	"technology",
	"taichi",
	
	HIG"基本原料"NOR GRN"生產"NOR,
	"metal",
	"stone",
	"water",
	"wood",
	"fuel",

	HIG"特殊原料"NOR GRN"生產"NOR,
	"fishing",
	"farming",
	"pasture",
	"fishfarm",
	
	
	HIG"一階工廠"NOR GRN"生產"NOR,
	"metalclassify",
	"stoneclassify",
	"waterclassify",
	"woodclassify",
	"fuelclassify",

	HIG"二階工廠"NOR GRN"生產"NOR,
	"food-fac",
	"drink-fac",
	"clothing-fac",
	"furniture-fac",
	"hardware-fac",
	"chemical-fac",
	"machinery-fac",
	"electrics-fac",
	"transportation-fac",
	"adventure-fac",
	"shortrange-fac",
	"armor-fac",
	"perfume-fac",
	"instrument-fac",
	"entertainment-fac",
	"longrange-fac",
	"heavyarmor-fac",
	"magic-fac",
	"aircraft-fac",
	
	
	HIY"員工工作"NOR YEL"技能"NOR,
	"storemanage",
	"security",
	"factorymanage",
	"researchmanage",

	HIY"員工運動"NOR YEL"技能"NOR,
	"righthand",
	"lefthand",
	"twohands",
	"fourseam",
	"twoseam",
	"curveball",
	"slider",
	"forkball",
	"sinker",
	"hitpower",
	"hitaccuracy",
	"hitrange",
	"fldaccuracy",
	"fldrange",
	
	HIR"基本戰鬥"NOR RED"技能"NOR,
	"dodge",
	"unarmed",
	"blade",
	"stick",
	"sword",
	"axe",
	
	HIR"樂器戰鬥"NOR RED"技能"NOR,
	"flute",
	"guitar",

	HIR"法力戰鬥"NOR RED"技能"NOR,
	"staff",

	HIR"戰鬥姿態"NOR RED"技能"NOR,
	"attack-stance",
	"defend-stance",
	"speed-stance",
	"medic-stance",
	"attack-stance-adv",
	"defend-stance-adv",
	"speed-stance-adv",
	"medic-stance-adv",

	HIR"特殊戰鬥"NOR RED"技能"NOR,
	"fatalblow",

});

int query_level_exp(int lv)
{
	if( lv < 1 ) return 0;
	
	// 0 - 100 級一種算法
	if( lv <= 100 )
	{
		lv = 100 - lv;
		return to_int((10000000 + EXP_SHIFT)/pow(1+(((lv+39)/10)/250.), lv/0.7) - EXP_SHIFT);
	}
	else if( lv <= 200 )
	{
		lv = 200 - lv;
		return to_int((30000000 + EXP_SHIFT)/pow(1+(((lv+39)/10)/250.), lv/0.7) - EXP_SHIFT)+query_level_exp(100);
	}
	else if( lv <= 300 )
	{
		lv = 300 - lv;
		return to_int((50000000 + EXP_SHIFT)/pow(1+(((lv+39)/10)/250.), lv/0.7) - EXP_SHIFT)+query_level_exp(200);
	}
	else if( lv <= 400 )
	{
		lv = 400 - lv;
		return to_int((70000000 + EXP_SHIFT)/pow(1+(((lv+39)/10)/250.), lv/0.7) - EXP_SHIFT)+query_level_exp(300);
	}
	else if( lv <= 500 )
	{
		lv = 500 - lv;
		return to_int((90000000 + EXP_SHIFT)/pow(1+(((lv+39)/10)/250.), lv/0.7) - EXP_SHIFT)+query_level_exp(400);
	}
	else if( lv <= 600 )
	{
		lv = 600 - lv;
		return to_int((110000000 + EXP_SHIFT)/pow(1+(((lv+39)/10)/250.), lv/0.7) - EXP_SHIFT)+query_level_exp(500);
	}
	else if( lv <= 700 )
	{
		lv = 700 - lv;
		return to_int((130000000 + EXP_SHIFT)/pow(1+(((lv+39)/10)/250.), lv/0.7) - EXP_SHIFT)+query_level_exp(600);
	}
	else if( lv <= 800 )
	{
		lv = 800 - lv;
		return to_int((150000000 + EXP_SHIFT)/pow(1+(((lv+39)/10)/250.), lv/0.7) - EXP_SHIFT)+query_level_exp(700);
	}
	else if( lv <= 900 )
	{
		lv = 900 - lv;
		return to_int((170000000 + EXP_SHIFT)/pow(1+(((lv+39)/10)/250.), lv/0.7) - EXP_SHIFT)+query_level_exp(800);
	}
	else if( lv <= 1000 )
	{
		lv = 1000 - lv;
		return to_int((190000000 + EXP_SHIFT)/pow(1+(((lv+39)/10)/250.), lv/0.7) - EXP_SHIFT)+query_level_exp(900);
	}
	else
		return 0;
}


string *query_skills()
{
	return skills;
}

string *query_sorted_skills()
{
	return sorted_skills;
}

int skill_exists(string skill)
{
	return member_array(skill, skills) == -1 ? 0 : 1;
}

string *baseball_limited_skills = ({ "fourseam", "twoseam", "curveball", "slider", "forkball", "sinker", "hitpower", "hitaccuracy", "hitrange", "fldaccuracy", "fldrange" });
string *query_baseball_limited_skills()
{
	return baseball_limited_skills;
}

int query_baseball_limited_level()
{
	return 2500;
}

void create()
{	
	skills = map(get_dir(SKILL_ROOT), (: $1 = $1[0..<3] :));
	sorted_skills = manual_skill_sort | skills;//sort_array(skills, (: member_array($1, manual_skill_sort) - member_array($2, manual_skill_sort) :));
}

string query_name()
{
	return "技能系統(SKILL_D)";
}
