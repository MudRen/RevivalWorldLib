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
	"name":		"�n��q��",
	
	//�^��W��
	"id":		"integrated circuit",

	//���ƻݨD
	"material":	([ "precious_metal":10, "distilled_water":10, "silicon":10 ]),

	//�ޯ�ݨD
	"skill":	([ "electrics-fac":70, "technology":70 ]),

	//�Ͳ��u�t
	"factory":	"electrics",
]);
