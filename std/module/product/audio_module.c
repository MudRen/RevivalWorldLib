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
	"/obj/materials/wood" : (: addn("effect", $1 * 10, $2) :),
]);

// ť���T
void do_play(object me, string arg, object ob)
{
	object cd;
	object env = environment(ob);
	
	if( !env->is_module_room() || !query_temp("decorated", ob) )
		return tell(me, ob->query_idname()+"�������C�b�ؿv�����~��ϥΡC\n");
	
	if( query("owner", env) != me->query_id(1) )
		return tell(me, env->query_room_name(1)+"���O"+pnoun(2, me)+"���ؿv���A�L�k�b�o�̨ϥέ��T�C\n");

	if( !arg || !objectp(cd = present(arg)) )
		return tell(me, pnoun(2, me)+"�Q�n������@�i CD�H\n");

	if( !cd->is_compact_disk() )
		return tell(me, cd->query_idname()+"���O CD�C\n");

	if( !random(500) )
	{
		msg("$ME�����a���U PLAY ��A�����p���M�u�I�v���@�n�A$YOU�_�X�}�}�·ϡA�a���F�I�I\n", me, ob, 1);
		destruct(ob, 1);
		return;
	}
		
	cd->play_cd(me, query("effect", ob));
}

// �]�w����ʧ@
nosave mapping action_info =
([
	"help"	: "��ť���T�C",
	"action":
		([
			"play"	: (: do_play :),
		]),
]);

// �]�w������
nosave mapping product_info = 
([	
	//����W��
	"name":		"���T",
	
	//�^��W��
	"id":		"audio",

	//���ƻݨD
	"material":	([ "integrated_circuit":2, "refined_timber":5, "plastic":3, "wire":1 ]),

	//�ޯ�ݨD
	"skill":	([ "furniture-fac":90, "technology":90 ]),

	//�Ͳ��u�t
	"factory":	"furniture",
]);
