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
	"/obj/gem/gem1" : (: addn(ATTACK, standard_calculate($1) * 2, $2) :),
	"/obj/gem/gem4" : (: addn(SPEED, standard_calculate($1) * 2, $2) :),
	"/obj/gem/gem5" : (: addn(DEFEND, standard_calculate($1) * 2, $2) :),
]);

array enhancement = 
({ 
	(: addn(ATTACK, $1, $2) :),
	(: addn(SPEED, $1, $2) :),
	(: addn(DEFEND, $1, $2) :),
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
	"name":		"�ï]���~",
	
	//�^��W��
	"id":		"pearl jewel",

	//���ƻݨD
	"material":	([ "precious_metal":50, "/obj/fishfarm/product/pearl":2 ]),

	//�ޯ�ݨD
	"skill":	([ "clothing-fac":90, "technology":90 ]),

	//�Ͳ��u�t
	"factory":	"clothing",
]);
