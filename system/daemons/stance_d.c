/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : stance_d.c
 * Author : Clode@Revival World
 * Date   : 2008-01-17
 * Note   : ���A�t��
 *
 -----------------------------------------
 */
//#pragma optimize

#include <ansi.h>

mapping stance_name = ([
"attack"	:	HIR"��"NOR RED"��"NOR,
"defend"	:	HIC"��"NOR CYN"�m"NOR,
"speed"		:	HIY"�t"NOR YEL"��"NOR,
"medic"		:	HIG"��"NOR GRN"��"NOR,
"attack-adv"	:	HIR"�R"NOR RED"��"NOR,
"defend-adv"	:	HIC"��"NOR CYN"�g"NOR,
"speed-adv"	:	HIY"�e"NOR YEL"��"NOR,
"medic-adv"	:	HIG"��"NOR GRN"��"NOR,
]);

string query_stance_name(string key)
{
	return stance_name[key];
}

string query_name()
{
	return "���A�t��(STANCE_D)";
}

