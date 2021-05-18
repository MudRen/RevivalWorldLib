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
	"/obj/farming/product/barley" : (: addn("effect", ($1 > 30 ? 30 : $1) * 20, $2) :),
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
			////"action"	: (: do_action :),
		]),
]);

// �]�w������
nosave mapping product_info = 
([	
	//����W��
	"name":		"��s",
	
	//�^��W��
	"id":		"beer",

	//���ƻݨD
	"material":	([ "distilled_water":1, "barrel":1, "/obj/farming/product/barley":2 ]),

	//�ޯ�ݨD
	"skill":	([ "drink-fac":30, "technology":30 ]),

	//�Ͳ��u�t
	"factory":	"drink",
]);
