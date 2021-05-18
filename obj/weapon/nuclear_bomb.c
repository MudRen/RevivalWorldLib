/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : landmine.c
 * Author : Clode@RevivalWorld
 * Date   : 2010-08-27
 * Note   : �a�p
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>
#include <feature.h>
#include <condition.h>
#include <daemon.h>
#include <combat.h>

inherit STANDARD_OBJECT;

private void destruct_self()
{
	destruct(this_object(), 1);
}

void explode()
{
	object attacker = find_player(query_temp("attacker"));
	object *targets = keys(MAP_D->query_range_inventory(query_temp("location"), 13)||allocate_mapping(0));
	int damage;

	if( !objectp(attacker) || !BATTLEFIELD_D->in_battle(attacker) )
	{
		call_out((: destruct_self :), 0);
		return;
	}

	if( !sizeof(targets) )
		tell(attacker, pnoun(2, attacker)+"���z���ּu�S�������H�y���ˮ`�C\n");
	else
	foreach(object target in targets)
	{	
		if( !target->is_living() || target->is_dead() || target->is_boss() || target->in_condition(INVINCIBILITY) )
			continue;
		
		if( attacker != target )
			msg(HIY"$ME"HIY"���z���ּu"NOR YEL"����"HIY"�P"NOR YEL"�_�i"HIY"��tŧ���ӨӡA$YOU"HIY"�@�}�ջ蠟��P�����������H���Һ��A���e�u�ѤU"HIR"�P"NOR RED"�V"HIY"...\n"NOR, attacker, target, 1);
		else
			msg(HIY"$ME"HIY"���z���ּu"NOR YEL"����"HIY"�P"NOR YEL"�_�i"HIY"��tŧ���ӨӡA$ME"HIY"�@�}�ջ蠟��P�����������H���Һ��A���e�u�ѤU"HIR"�P"NOR RED"�V"HIY"...\n"NOR, attacker, 0, 1);
		
		damage = to_int(target->query_health_max()/1.5);
		
		target->start_condition(RADIATION, 60, 5, attacker);
		
		COMBAT_D->cause_damage(attacker, target, damage, NO_FATALBLOW | NO_ATTACKDEFEND);
	}

	CHANNEL_D->channel_broadcast("combat", HIY+attacker->query_idname()+HIY"���z���ּu"NOR YEL"����"HIY"�P"NOR YEL"�_�i"HIY"��tŧ���ӨӡA�@�� "NOR YEL+sizeof(targets)+HIY" �ӥͪ�����F�����C"NOR);

	call_out((: destruct_self :), 0);
}

void create(object attacker, object product)
{
	if( !objectp(attacker) || !objectp(product) )
	{
		call_out((: destruct_self :), 0);
	}
	else
	{
		set_idname(product->query_id(), product->query_name());
		set_temp("status", HIM"��m"NOR MAG"�̡G"NOR+attacker->query_name());
	
		set("unit", "��");
		set("mass", -1);
		set("value", product->query_value());
		set("flag/no_amount", 1);
	
		set_temp("attacker", attacker->query_id(1));
		
		set_temp("nuclear_bomb/time", time(), attacker);
		
		CHANNEL_D->channel_broadcast("combat", "�Y�H�N�_�ʹ��i"+product->query_idname()+"�A����"HIY"�Ұʮּu�˼� "NOR YEL"30"HIY" ��"NOR"�A�ְk�R�ڡI�I");
	
		call_out( (: explode :), 30 );
	}
}
