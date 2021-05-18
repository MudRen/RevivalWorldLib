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

// �Ұʦa�p
void do_activate(object me, string arg, object ob)
{
	if( !BATTLEFIELD_D->in_battle(me) && !wizardp(me) )
		return tell(me, me->query_idname()+"���b�Գ����A�L�k��m�a�p�����C\n");
		
	if( environment(ob) == me )
	{
		msg("$ME�p���l�l�a�Ұ�$YOU�é�m�b�a���C\n", me, ob, 1);
		new("/obj/weapon/landmine", me, ob)->move_to_environment(me);
	}
	else if( environment(ob)->is_maproom() )
	{
		msg("$ME�p���l�l�a�Ұʦa���W��$YOU�C\n", me, ob, 1);
		new("/obj/weapon/landmine", me, ob)->move_to_environment(ob);
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
			"activate"	: (: do_activate :),
		]),
]);

// �]�w������
nosave mapping product_info = 
([	
	//����W��
	"name":		"�a�p",
	
	//�^��W��
	"id":		"landmine",

	//���ƻݨD
	"material":	([ "sulfur":3, "mold":1, "wire":1 ]),

	//�ޯ�ݨD
	"skill":	([ "longrange-fac":30, "technology":30 ]),

	//�Ͳ��u�t
	"factory":	"longrange",
]);
