/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : module.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-04-06
 * Note   : 
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <feature.h>
#include <buff.h>

inherit PRODUCT_ACTION_MOD;

int calculate_chance(int value)
{
	value = to_int(pow(to_float(value), 0.5));
		
	return value;
}

int no_available(object me)
{
	return query("battlereward/swordbook", me) ? 0 : 1;
}

mapping inlay_effect =
([
	"/obj/gem/gem0" : (: addn(BUFF_STR, standard_calculate($1), $2) :),
	"/obj/gem/gem1" : (: addn(ATTACK, standard_calculate($1), $2) :),
	"/obj/gem/gem3" : (: addn(BUFF_AGI, standard_calculate($1), $2) :),
	"/quest/monster_fossil/lava_meteor" : (: set(BUFF_FATAL_POWER, calculate_chance($1*5) > 12 ? 60 : calculate_chance($1*5)*5, $2) :),
	"/quest/monster_fossil/monster_gall" : (: set(BUFF_FATAL_CHANCE, calculate_chance($1) > 6 ? 6 : calculate_chance($1), $2) :),
	"/obj/inlay/nessiteras_fist_secret_art_1st" : (: set("nessiteras_fist_secret_art_1st", $1, $2) :),
	"/obj/inlay/nessiteras_fist_secret_art_2nd" : (: set("nessiteras_fist_secret_art_2nd", $1, $2) :),
	"/obj/inlay/lord_sword_secret_art_1st" : (: set("lord_sword_secret_art_1st", $1, $2) :),
	"/obj/inlay/lord_sword_secret_art_2nd" : (: set("lord_sword_secret_art_2nd", $1, $2) :),
	"/obj/inlay/sky_1st_secret_art" : (: set("sky_1st_secret_art", $1, $2) :),
]);

array enhancement = 
({ 
	(: addn(ATTACK, $1, $2) :),
	(: addn(BUFF_AGI, $1, $2) :),
	(: addn(BUFF_STR, $1, $2) :),
});

// ��m���~
void do_action(object me, string arg)
{


}

// �]�w����ʧ@
nosave mapping action_info =
([
	"help"	: "�|���غc�����C",
	"action":
		([
			//"action"	: (: do_action :),
		]),
]);

// �]�w������
nosave mapping product_info = 
([	
	//����W��
	"name":		"�C",
	
	//�^��W��
	"id":		"sword",

	//���ƻݨD
	"material":	([ "steel":150, "gem_jewel":2, "man_made_fibers":15, "mold":6 ]),

	//�ޯ�ݨD
	"skill":	([ "shortrange-fac":100, "technology":100 ]),

	//�Ͳ��u�t
	"factory":	"shortrange",
]);
