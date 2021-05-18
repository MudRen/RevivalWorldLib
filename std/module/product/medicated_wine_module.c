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
	"/obj/pasture/product/antler" : (: addn("effect", $1 * 30, $2) :),
]);

array enhancement = 
({ 
	(: addn("effect", $1 * 30, $2) :),
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
	"name":		"�İs",
	
	//�^��W��
	"id":		"medicated wine",

	//���ƻݨD
	"material":	([ "/obj/pasture/product/antler":1, "distilled_water":2, "glass":1 ]),

	//�ޯ�ݨD
	"skill":	([ "drink-fac":90, "technology":90 ]),

	//�Ͳ��u�t
	"factory":	"drink",
]);
