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

mapping inlay_effect =
([
	"/obj/gem/gem1" : (: addn(BUFF_INT, standard_calculate($1) * 2, $2) :),
	"/obj/gem/gem5" : (: addn(SPEED, standard_calculate($1) * 2, $2) :),
]);

array enhancement = 
({ 
	(: addn(BUFF_INT, $1, $2) :),
	(: addn(SPEED, $1, $2) :),
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
	"name":		"��",
	
	//�^��W��
	"id":		"flute",

	//���ƻݨD
	"material":	([ "mold":3, "refined_timber":10, "precious_metal":2 ]),

	//�ޯ�ݨD
	"skill":	([ "instrument-fac":30, "technology":30 ]),

	//�Ͳ��u�t
	"factory":	"instrument",
]);
