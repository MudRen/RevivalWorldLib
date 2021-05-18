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
	"/obj/materials/metal" : (: addn("effect", $1 * 30, $2) :),
]);

array enhancement = 
({ 
	(: addn("effect", $1 * 30, $2) :),
});

// �ݹq��
void do_watch(object me, string arg, object ob)
{
	object env = environment(ob);
	
	if( !env->is_module_room() || !query_temp("decorated", ob) )
		return tell(me, ob->query_idname()+"�������C�b�ؿv�����~���[�ݡC\n");
	
	if( query("owner", env) != me->query_id(1) )
		return tell(me, env->query_room_name(1)+"���O"+pnoun(2, me)+"���ؿv���A�L�k�b�o�̬ݹq���C\n");

	if( !random(500) )
	{
		msg("$ME���Ħa���}$YOU�A�����p���M�u�I�v���@�n�A$YOU�_�X�}�}�·ϡA�a���F�I�I\n", me, ob, 1);
		destruct(ob, 1);
		return;
	}
		
	msg("$ME���}"+ob->query_idname()+"�Y��U�ظ`�ءA��ɨƪ��A�ѥ[�`�F�C\n", me, 0, 1);
	me->start_condition(TELEVISION, query("effect", ob));
}

// �]�w����ʧ@
nosave mapping action_info =
([
	"help"	: "�[�ݹq���`�ءC",
	"action":
		([
			"watch"	: (: do_watch :),
		]),
]);

// �]�w������
nosave mapping product_info = 
([	
	//����W��
	"name":		"�q����",
	
	//�^��W��
	"id":		"television",

	//���ƻݨD
	"material":	([ "integrated_circuit":1, "glass":5, "mold":1, "wire":1 ]),

	//�ޯ�ݨD
	"skill":	([ "furniture-fac":70, "technology":70 ]),

	//�Ͳ��u�t
	"factory":	"furniture",
]);
