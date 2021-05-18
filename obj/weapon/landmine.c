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

private void destruct_self(object target)
{
	if( objectp(target) )
		delete_temp("landmine/hit", target);
	
	destruct(this_object(), 1);
}

private void auto_destruct()
{
	msg("$ME��m�L�[�L�HĲ�ΡA�w�g���׷l�a�C\n", this_object(), 0, 1);
	destruct(this_object(), 1);
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
		
		call_out((: auto_destruct :), 120);
	}
}

void init(object target)
{
	int max_health;
	object attacker = find_player(query_temp("attacker"));
	int damage;

	if( !objectp(attacker) || !BATTLEFIELD_D->in_battle(attacker) )
	{
		msg("$ME���F"+this_object()->query_idname()+"�A���n�O�ӷl�a���a�p�C\n", target, 0, 1);
		call_out((: destruct_self :), 0);
		return;
	}
	
	if( !random(2) || !target->is_user_ob() || !BATTLEFIELD_D->in_battle(target) || target->is_dead() || target->is_faint() || target->in_condition(INVINCIBILITY) || query_temp("landmine/hit", target) )
		return;

	set_temp("landmine/hit", 1, target);
	
	max_health = target->query_health_max();
	damage = range_random(max_health/10, max_health/5);
		
	if( target != attacker )
		msg(HIY"�u�F�v���@�n"NOR"�A$ME���F$YOU��m��"+this_object()->query_idname()+"�C\n"NOR, target, attacker, 1);
	else
		msg(HIY"�u�F�v���@�n"NOR"�A$ME���F$ME�ۤv��m��"+this_object()->query_idname()+"�C\n"NOR, attacker, 0, 1);
			
	call_out((: destruct_self :), 0, target);
		
	COMBAT_D->cause_damage(attacker, target, damage, NO_FATALBLOW | NO_ATTACKDEFEND);
}
