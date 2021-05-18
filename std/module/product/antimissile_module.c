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

#include <ansi.h>
#include <feature.h>
#include <condition.h>
#include <location.h>
#include <daemon.h>
#include <delay.h>

inherit PRODUCT_ACTION_MOD;

// �]�w����ʧ@
nosave mapping action_info =
([
	"help"	: "�|���غc�����C",
	"action":
		([
			//"launch"	: (: do_launch :),
		]),
]);

// �]�w������
nosave mapping product_info = 
([	
	//����W��
	"name":		"�ɼu���m��",
	
	//�^��W��
	"id":		"antimissile",

	//���ƻݨD
	"material":	([ "mechanical_structure":2, "integrated_circuit":2, "petroleum":5 ]),

	//�ޯ�ݨD
	"skill":	([ "longrange-fac":90, "technology":90 ]),

	//�Ͳ��u�t
	"factory":	"longrange",
]);
