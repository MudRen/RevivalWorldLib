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
	"/obj/gem/gem0" : (: addn(BUFF_STAMINA_REGEN, standard_calculate($1), $2) :),
	"/obj/gem/gem3" : (: addn(BUFF_ENERGY_REGEN, standard_calculate($1), $2) :),
	"/obj/gem/gem6" : (: addn(BUFF_HEALTH_REGEN, standard_calculate($1), $2) :),
]);

array enhancement = 
({ 
	(: addn(BUFF_STAMINA_REGEN, $1, $2) :),
	(: addn(BUFF_ENERGY_REGEN, $1, $2) :),
	(: addn(BUFF_HEALTH_REGEN, $1, $2) :),
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
	"name":		"�_�۹��~",
	
	//�^��W��
	"id":		"gem jewel",

	//���ƻݨD
	"material":	([ "precious_metal":50, "gem":50 ]),

	//�ޯ�ݨD
	"skill":	([ "clothing-fac":70, "technology":70 ]),

	//�Ͳ��u�t
	"factory":	"clothing",
]);
