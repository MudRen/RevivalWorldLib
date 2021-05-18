/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : stance_d.c
 * Author : Clode@Revival World
 * Date   : 2008-01-17
 * Note   : 姿態系統
 *
 -----------------------------------------
 */
//#pragma optimize

#include <ansi.h>

mapping stance_name = ([
"attack"	:	HIR"攻"NOR RED"擊"NOR,
"defend"	:	HIC"防"NOR CYN"禦"NOR,
"speed"		:	HIY"速"NOR YEL"度"NOR,
"medic"		:	HIG"醫"NOR GRN"療"NOR,
"attack-adv"	:	HIR"舞"NOR RED"火"NOR,
"defend-adv"	:	HIC"堰"NOR CYN"土"NOR,
"speed-adv"	:	HIY"疾"NOR YEL"風"NOR,
"medic-adv"	:	HIG"翔"NOR GRN"水"NOR,
]);

string query_stance_name(string key)
{
	return stance_name[key];
}

string query_name()
{
	return "姿態系統(STANCE_D)";
}

