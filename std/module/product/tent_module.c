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
#include <delay.h>
#include <daemon.h>

inherit PRODUCT_ACTION_MOD;

#define REGEN	50

mapping inlay_effect =
([
	"/obj/materials/metal" : (: addn("effect", $1 * 20, $2) :),
]);

array enhancement = 
({ 
	(: addn("effect", $1 * 20, $2) :),
});

// ��ı��
void do_sleep(object me, string arg, object ob)
{
	int cost = me->query_stamina_max() - me->query_stamina_cur();
	int time;
	int regen;

	if( COMBAT_D->in_fight(me) )
		return tell(me, pnoun(2, me)+"���b�԰����C\n");

	if( me->is_delaying() )
		return tell(me, me->query_delay_msg());

	if( !random(query("effect", ob)) )
	{
		msg("$ME���Ħa�p�i$YOU�̡A�u�ԡI�v���@�n�伵�b�O���K�[�~�M�_���F�A�ݨӬO�S��k�A�ΤF�C\n", me, ob, 1);
		
		destruct(ob, 1);
		
		return;
	}

	msg("$ME�ΪA�a���b$YOU�̡A���F�Ө����K�@�U(��_ "+cost+" ��O)�C\n", me, ob, 1);
	
	regen = me->stamina_regen()+REGEN;
	
	time = 2*cost/regen + 1;
	
	if( time <= 0 )
		tell(me, "�𮧧����C\n");
	else
	{
		set_temp("rest_regen/stamina", REGEN, me);
		me->start_delay(REST_DELAY_KEY, time, pnoun(2, me)+"���b"+ob->query_idname()+"���𮧡C\n", "�𮧧����C\n", bind((: delete_temp("rest_regen", $(me)), $(me)->set_stamina_full() :), me));
	}
}

// �]�w����ʧ@
nosave mapping action_info =
([
	"help"	: "��ı�𮧡C",
	"action":
		([
			"sleep"	: (: do_sleep :),
		]),
]);

// �]�w������
nosave mapping product_info = 
([	
	//����W��
	"name":		"�b�O",
	
	//�^��W��
	"id":		"tent",

	//���ƻݨD
	"material":	([ "man_made_fibers":1, "steel":1]),

	//�ޯ�ݨD
	"skill":	([ "adventure-fac":10, "technology":10 ]),

	//�Ͳ��u�t
	"factory":	"adventure",
]);
