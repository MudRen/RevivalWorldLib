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
#include <daemon.h>
#include <map.h>
#include <buff.h>

inherit PRODUCT_ACTION_MOD;

mapping inlay_effect =
([
	"/obj/gem/gem4" : (: addn(BUFF_LOADING_MAX, to_int(pow(to_float($1), 0.9))*100, $2) :),
	"/obj/gem/gem5" : (: addn(BUFF_SLOT_MAX, to_int(pow(to_float($1), 0.5)), $2) :),
]);

array enhancement = 
({ 
	(: addn(BUFF_LOADING_MAX, $1 * 10, $2) :),
	(: addn(BUFF_SLOT_MAX, $1, $2) :),
});

// �}��
void do_drive(object me, string arg, object ob)
{

}

// �]�w����ʧ@
nosave mapping action_info =
([
	"help"	: "�|���غc�����C",
	"action":
		([
			//"drive"		: (: do_drive :),
		]),
]);

// �]�w������
nosave mapping product_info = 
([	
	//����W��
	"name":		"�d��",
	
	//�^��W��
	"id":		"truck",

	//���ƻݨD
	"material":	([ "tire":14, "car_body":1, "engine":1, "steel":100, "integrated_circuit":1, "mechanical_structure":5,"petroleum":500 ]),

	//�ޯ�ݨD
	"skill":	([ "transportation-fac":80, "technology":80 ]),

	//�Ͳ��u�t
	"factory":	"transportation",
]);
