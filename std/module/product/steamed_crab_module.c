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

inherit PRODUCT_ACTION_MOD;

mapping inlay_effect =
([
	"/obj/materials/fuel" : (: addn("mass", -($1 > 10 ? 10 : $1 ) * 10, $2) :),
	"/obj/fishfarm/product/big_crab" : (: addn("effect", ($1 > 30 ? 30 : $1) * 20, $2) :),
]);

array enhancement = 
({ 
	(: addn("effect", $1 * 20, $2) :),
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
	"name":		"�M�]�j�h��",
	
	//�^��W��
	"id":		"steamed crab",

	//���ƻݨD
	"material":	([ "/obj/fishfarm/product/big_crab":1, "tap_water":2, "natural_gas":2 ]),

	//�ޯ�ݨD
	"skill":	([ "food-fac":90, "technology":90 ]),

	//�Ͳ��u�t
	"factory":	"food",
]);
