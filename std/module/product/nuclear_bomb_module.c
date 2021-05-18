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
#include <daemon.h>

inherit PRODUCT_ACTION_MOD;

#define ACTIVATE_TIME 	1200

// �Ұʮּu
void do_nuke(object me, string arg, object ob)
{
	if( !BATTLEFIELD_D->in_battle(me) && !wizardp(me) )
		return tell(me, me->query_idname()+"���b�Գ����A�L�k�Ұʮּu�����C\n");
		
	if( time() - query_temp("nuclear_bomb/time", me) < ACTIVATE_TIME )
		return tell(me, "�������� "+ACTIVATE_TIME+" ������~��A���ϥ�"+ob->query_idname()+"�C\n");
		
	if( environment(ob) == me )
	{
		msg("$ME�p���l�l�a�Ұ�$YOU�é�m�b�a���C\n", me, ob, 1);
		new("/obj/weapon/nuclear_bomb", me, ob)->move_to_environment(me);
	}
	else if( environment(ob)->is_maproom() )
	{
		msg("$ME�p���l�l�a�Ұʦa���W��$YOU�C\n", me, ob, 1);
		new("/obj/weapon/nuclear_bomb", me, ob)->move_to_environment(ob);
	}
	else
	{
		msg("$ME�L�k�b�o�̱Ұ�$YOU�C\n", me, ob, 1);
		return;	
	}
	
	destruct(ob, 1);
}

// �]�w����ʧ@
nosave mapping action_info =
([
	"help"	: "�|���غc�����C",
	"action":
		([
			"nuke"	: (: do_nuke :),
		]),
]);

// �]�w������
nosave mapping product_info = 
([	
	//����W��
	"name":		"�ּu",
	
	//�^��W��
	"id":		"nuclear bomb",

	//���ƻݨD
	"material":	([ "precious_metal":50, "liquid_fuel":100, "man_made_fibers":5, "mold":5, "integrated_circuit":10, "gem":20 ]),

	//�ޯ�ݨD
	"skill":	([ "longrange-fac":100, "technology":100 ]),

	//�Ͳ��u�t
	"factory":	"longrange",
]);
