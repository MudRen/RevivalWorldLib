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
	"/obj/materials/fuel" : (: addn("mass", -($1 > 10 ? 10 : $1) * 10, $2) :),
	"/obj/fishfarm/product/big_shrimp" : (: addn("effect", ($1 > 30 ? 30 : $1), $2) :),
]);

array enhancement = 
({ 
	(: addn("effect", $1, $2) :),
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
			////"action"	: (: do_action :),
		]),
]);

// �]�w������
nosave mapping product_info = 
([	
	//����W��
	"name":		"��s��",
	
	//�^��W��
	"id":		"beer shrimp",

	//���ƻݨD
	"material":	([ "/obj/fishfarm/product/big_shrimp":1, "beer":1, "natural_gas":2 ]),

	//�ޯ�ݨD
	"skill":	([ "food-fac":70, "technology":70 ]),

	//�Ͳ��u�t
	"factory":	"food",
]);
