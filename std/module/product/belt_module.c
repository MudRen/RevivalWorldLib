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
	
	if( value < 0 )
		value = 0;
	else if( value > 5 )
		value = 5;
		
	return value;
}

mapping inlay_effect =
([
	"/obj/gem/gem0" : (: addn(BUFF_STR, standard_calculate($1, 5), $2) :),
	"/obj/gem/gem1" : (: addn(BUFF_STAMINA_MAX, standard_calculate($1) * 30, $2) :),
	"/obj/gem/gem2" : (: addn(BUFF_PHY, standard_calculate($1, 5), $2) :),
	"/obj/gem/gem3" : (: addn(BUFF_AGI, standard_calculate($1, 5), $2) :),
	"/obj/gem/gem4" : (: addn(BUFF_CHA, standard_calculate($1, 5), $2) :),
	"/obj/gem/gem5" : (: addn(DEFEND, standard_calculate($1) * 2, $2) :),
	"/obj/gem/gem6" : (: addn(BUFF_HEALTH_MAX, standard_calculate($1) * 5, $2) :),
	"/quest/monster_fossil/eagle_feather" : (: addn(BUFF_COMBO_CHANCE, calculate_chance($1), $2) :),
]);

array enhancement = 
({ 
	(: addn(BUFF_STR, $1, $2) :),
	(: addn(BUFF_STAMINA_MAX, $1, $2) :),
	(: addn(BUFF_PHY, $1, $2) :),
	(: addn(BUFF_AGI, $1, $2) :),
	(: addn(BUFF_CHA, $1, $2) :),
	(: addn(DEFEND, $1, $2) :),
	(: addn(BUFF_HEALTH_MAX, $1, $2) :),
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
	"name":		"�y�a",
	
	//�^��W��
	"id":		"belt",

	//���ƻݨD
	"material":	([ "steel":20, "/obj/pasture/product/leather":6 ]),

	//�ޯ�ݨD
	"skill":	([ "armor-fac":30, "technology":30 ]),

	//�Ͳ��u�t
	"factory":	"armor",
]);
