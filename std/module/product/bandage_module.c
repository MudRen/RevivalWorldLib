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
#include <npc.h>
#include <daemon.h>

inherit PRODUCT_ACTION_MOD;

mapping inlay_effect =
([
	"/obj/farming/product/cotton" : (: addn("effect", ($1 > 100 ? 100 : $1) * 2, $2) :),
]);

array enhancement = 
({ 
	(: addn("effect", $1 * 2, $2) :),
});

// �]���^�a
void do_heal(object me, string arg, object ob)
{
	string stance_type = query("combat/stance/type", me);
	object *enemies;
	object target;
	float effect;

	if( !arg )
		return tell(me, pnoun(2, me)+"�Q�n���֥]��"+ob->query_idname()+"�H\n");
		
	if( me->is_module_npc() && query("job/type", me) != SOLDIER )
		return me->do_command("say �ڤ��O�x�H�L�k���H�]��\n");
		
	//���L������U���O
	if( me->is_delaying() )
	{
		if( me->is_module_npc() )
			return me->do_command("say "+me->query_delay_msg()+"\n");

		tell(me, me->query_delay_msg());
		return me->show_prompt();
	}
	
	if( query("killlock", me) )
		return tell(me, pnoun(2, me)+"�ثe�B�b�԰���w���A�A�L�k�i��]�ϡC\n");

	if( arg == "me" )
		target = me;
	else if( !objectp(target = present(arg)) )
		return tell(me, "�o�̨S�� "+arg+" �o�ӤH�C\n");
		
	if( !target->is_living() )
	{
		if( me->is_module_npc() )
			return me->do_command("say �ڵL�k��"+target->query_idname()+"�]��"+ob->query_idname()+"�C\n");
		else
			return tell(me, pnoun(2, me)+"�L�k��"+target->query_idname()+"�]��"+ob->query_idname()+"�C\n");
	}

	if( !me->is_in_my_group(target) )
	{
		if( me->is_module_npc() )
			return me->do_command("say �ڤ��b"+target->query_idname()+"������̡A�L�k��"+pnoun(3, target)+"�i��]�ϡC\n");
		else
			return tell(me, pnoun(2, me)+"���b"+target->query_idname()+"������̡A�L�k��"+pnoun(3, target)+"�i��]�ϡC\n");
	}

	if( !me->cost_energy(50) )
	{
		if( me->is_module_npc() )
			return me->do_command("say �ڨS���������믫�A�i��]�ϤF�C\n");
		else
			return tell(me, pnoun(2, me)+"�S���������믫�A�i��]�ϤF�C\n");
	}

	effect = to_float(query("effect", ob));

	if( stance_type == "medic" )
		effect *= 1. + me->query_skill_level("medic-stance")/500.;
	else if( stance_type == "medic-adv" )
		effect *= 1.2 + me->query_skill_level("medic-stance-adv")/500.;

	target->earn_health(to_int(effect));

	msg("$ME�N"+ob->query_idname()+"�]�Ϧb$YOU���ˤf�W�A��$YOU��_�F "HIY+to_int(effect)+NOR" �I���ͩR�ȡC\n", me, target, 1);
	
	destruct(ob, 1);
	
	enemies = COMBAT_D->query_fighting_targets(target);
	
	if( !sizeof(enemies) )
		return;
		
	foreach(ob in enemies)
	{
		if( !COMBAT_D->in_fight(me, ob) && ob->valid_enemy(me) && !ob->is_in_my_group(me) )
		{
			msg(HIR"$ME"HIR"�Q�n����$YOU�I�I\n"NOR, ob, me, 1);
			COMBAT_D->start_fight(me, ob);
		}
		
		ob->calculate_combat_hate(me, to_int(effect)/2);
	}
}

// �]�w����ʧ@
nosave mapping action_info =
([
	"help"	: "�|���غc�����C",
	"action":
		([
			"heal"		: (: do_heal :),
		]),
]);

// �]�w������
nosave mapping product_info = 
([	
	//����W��
	"name":		"�^�a",
	
	//�^��W��
	"id":		"bandage",

	//���ƻݨD
	"material":	([ "/obj/farming/product/cotton":10, "plastic":2 ]),

	//�ޯ�ݨD
	"skill":	([ "adventure-fac":50, "technology":50 ]),

	//�Ͳ��u�t
	"factory":	"adventure",
]);
