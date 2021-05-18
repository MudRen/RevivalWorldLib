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
#include <condition.h>
#include <location.h>
#include <daemon.h>
#include <delay.h>
#include <combat.h>

inherit PRODUCT_ACTION_MOD;

#define FIRE_TIME	20

void launch(object me, object target, object ob)
{
	if( !objectp(me) || !objectp(target) || !objectp(ob) || !BATTLEFIELD_D->in_battle(me) || !BATTLEFIELD_D->in_battle(target) )
		return;
	
	if( !random(5) || (userp(target) && !BATTLEFIELD_D->in_battle(target)) || !target->is_living() || target->is_dead() || target->is_faint() || target->in_condition(INVINCIBILITY) || target->is_boss() )
	{
		msg("$ME"HIY"�o�g��"+ob->query_idname()+HIY"���R��$YOU"HIY"�C\n"NOR, me, target, 1);
	}
	else
	{
		object antimissile;
		int max_health = target->query_health_max();
		int damage = range_random(max_health/3, max_health/2);
		
		foreach(object inv in all_inventory(target))
		{
			if( inv->query_module() == "antimissile" )
			{
				antimissile = inv;
				break;
			}
		}
		
		if( objectp(antimissile) && random(5) )
		{	
			msg("$ME"HIY"�o�g��"+ob->query_idname()+HIY"�R��$YOU"HIY"�A���Q"+antimissile->query_idname()+"��פF�����A�ȳy�������ˮ`�C\n"NOR, me, target, 1);
			COMBAT_D->cause_damage(me, target, range_random(damage/4, damage/3), NO_FATALBLOW | NO_ATTACKDEFEND);
			destruct(antimissile, 1);
		}
		else
		{
			msg("$ME"HIY"�o�g��"+ob->query_idname()+HIY"�R��$YOU"HIY"�A�y���Y���ˮ`�C\n"NOR, me, target, 1);
			COMBAT_D->cause_damage(me, target, damage, NO_FATALBLOW | NO_ATTACKDEFEND);
		}
	}
	
	if( addn_temp("missile/count", -1, target) <= 0 )
		delete_temp("missile/count", target);

	destruct(ob, 1);
}

// ��m���~
void do_launch(object me, string arg, object ob)
{
	string *targetids;
	object *targets;
	object target;
	int flytime = range_random(3, 6);
	
	if( !BATTLEFIELD_D->in_battle(me) )
		return tell(me, pnoun(2, me)+"���b�Գ����A�L�k�o�g�ɼu�C\n");
			
	if( time() - query_temp("missile/fire_time", me) < FIRE_TIME )
		return tell(me, "�������� "+FIRE_TIME+" ������~��A���o�g"+ob->query_idname()+"�C\n");

	targetids = keys(BATTLEFIELD_D->query_battle_player()) - ({ me->query_id(1) });
		
	targets = map(targetids, (: find_player($1) :));
	targets = filter_array(targets, (: objectp($1) && !$(me)->is_in_my_group($1) && !$1->is_dead() && !$1->in_condition(INVINCIBILITY) :)) ;

	if( !sizeof(targets) )
		return tell(me, "�S����H�i�H�o�g�ɼu�C\n");
		
	target = targets[random(sizeof(targets))];
	
	msg(HIY"$ME"HIY"��"+loc_short(query_temp("location", me))+"�H���V"HIY"$YOU"HIY"�o�g"+ob->query_idname()+HIY"�A�N�b "+flytime+" �����R���ؼСC\n", me, target, 1);

	set_temp("missile/fire_time", time(), me);

	call_out((: launch :), flytime, me, target, ob);
		
	me->start_delay(MISSILE_DELAY_KEY, 1, pnoun(2, me)+"�Q�ɼu�o�g�ɪ��j�j�_�i�_�o�w�Y��V...");
}

// �]�w����ʧ@
nosave mapping action_info =
([
	"help"	: "�|���غc�����C",
	"action":
		([
			"launch"	: (: do_launch :),
		]),
]);

// �]�w������
nosave mapping product_info = 
([	
	//����W��
	"name":		"�ɼu",
	
	//�^��W��
	"id":		"missile",

	//���ƻݨD
	"material":	([ "sulfur":12, "mechanical_structure":2, "integrated_circuit":3, "rubber":1, "liquid_fuel":8 ]),

	//�ޯ�ݨD
	"skill":	([ "longrange-fac":70, "technology":70 ]),

	//�Ͳ��u�t
	"factory":	"longrange",
]);
