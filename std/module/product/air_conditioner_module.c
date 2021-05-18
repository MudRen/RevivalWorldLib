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

mapping inlay_effect =
([
	"/obj/materials/water" : (: addn("effect", $1 * 30, $2) :),
]);

array enhancement = 
({ 
	(: addn("effect", $1 * 30, $2) :),
});

// �}�N��
void do_air(object me, string arg, object ob)
{
	object env = environment(ob);
	
	if( !env->is_module_room() || !query_temp("decorated", ob) )
		return tell(me, ob->query_idname()+"�������C�b�ؿv�����~��ϥΡC\n");
	
	if( query("owner", env) != me->query_id(1) )
		return tell(me, env->query_room_name(1)+"���O"+pnoun(2, me)+"���ؿv���A�L�k�b�o�̨ϥΧN��C\n");

	if( !random(300) )
	{
		msg("$ME���ݦa���}$YOU�A�����p���M�u�I�v���@�n�A$YOU�_�X�}�}�·ϡA�a���F�I�I\n", me, ob, 1);
		destruct(ob, 1);
		return;
	}
		
	msg("$ME���}"+ob->query_idname()+"�ñN�ū׽վ�� 10��C�A�G�D�z�ߪ��N�𪽧j�ӨӡC\n", me, 0, 1);
	me->start_condition(AIR_CONDITIONER, query("effect", ob));
}

// �]�w����ʧ@
nosave mapping action_info =
([
	"help"	: "�}�ҧN����C",
	"action":
		([
			"air"	: (: do_air :),
		]),
]);

// �]�w������
nosave mapping product_info = 
([	
	//����W��
	"name":		"�N���",
	
	//�^��W��
	"id":		"air conditioner",

	//���ƻݨD
	"material":	([ "integrated_circuit":2, "steel":5, "mechanical_structure":1, "wire":2 ]),

	//�ޯ�ݨD
	"skill":	([ "furniture-fac":95, "technology":95 ]),

	//�Ͳ��u�t
	"factory":	"furniture",
]);
