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
#include <condition.h>

inherit PRODUCT_ACTION_MOD;

// ��m���~
void do_action(object me, string arg)
{


}

// �]�w����ʧ@
nosave mapping action_info =
([
	"help"	: "��ť���T�C",
	"action":
		([
			//"play"	: (: do_play :),
		]),
]);

// �]�w������
nosave mapping product_info = 
([	
	//����W��
	"name":		"���֥���",
	
	//�^��W��
	"id":		"compact disk",

	//���ƻݨD
	"material":	([ "precious_metal":2, "plastic":2, "sulfur":1 ]),

	//�ޯ�ݨD
	"skill":	([ "instrument-fac":70, "technology":70 ]),

	//�Ͳ��u�t
	"factory":	"instrument",
]);
