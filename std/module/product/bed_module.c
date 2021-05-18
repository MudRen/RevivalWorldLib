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
#include <delay.h>
#include <daemon.h>

inherit PRODUCT_ACTION_MOD;

#define REGEN	30

mapping inlay_effect =
([
	"/obj/pasture/product/wool" : (: addn("effect", ($1 > 100 ? 100 : $1) / 5, $2) :),
]);

array enhancement = 
({ 
	(: addn("effect", $1, $2) :),
});

// ��ı
void do_sleep(object me, string arg, object ob)
{

}

// �]�w����ʧ@
nosave mapping action_info =
([
	"help"	: "��ı�C",
	"action":
		([
			//"sleep"	: (: do_sleep :),
		]),
]);

// �]�w������
nosave mapping product_info = 
([	
	//����W��
	"name":		"��",
	
	//�^��W��
	"id":		"bed",

	//���ƻݨD
	"material":	([ "high_timber":30, "/obj/farming/product/cotton":10, "/obj/pasture/product/wool":5 ]),

	//�ޯ�ݨD
	"skill":	([ "furniture-fac":50, "technology":50 ]),

	//�Ͳ��u�t
	"factory":	"furniture",
]);
