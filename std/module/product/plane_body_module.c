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
	"name":		"��������",
	
	//�^��W��
	"id":		"plane body",

	//���ƻݨD
	"material":	([ "mold":5, "mechanical_structure":3, "steel":90, "man_made_fibers":10 ]),

	//�ޯ�ݨD
	"skill":	([ "aircraft-fac":20, "technology":20 ]),

	//�Ͳ��u�t
	"factory":	"aircraft",
]);
