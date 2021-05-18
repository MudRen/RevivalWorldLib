/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : standard_monster.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-07-19
 * Note   : 標準怪物 NPC 繼承檔
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <feature.h>
#include <inherit.h>

inherit STANDARD_NPC;

void reply_defeat(object* attackers)
{
	int count;
	
	mapping defeat_titles = fetch_variable("defeat_titles");

	if( mapp(defeat_titles) )
	foreach(object attacker in attackers)
	{
		if( !objectp(attacker) || !attacker->is_user_ob() )
			return;

		count = addn("combat/defeat/"+replace_string(base_name(this_object()), "/", "#"), 1, attacker);
		
		if( stringp(defeat_titles[count]) )
		{
			attacker->add_title(defeat_titles[count], "擊殺"+replace_string(this_object()->query_idname(), "的屍體(Corpse of ", "(")+"達 "+count+" 隻以上");
			attacker->save();
		}
	}
}

void start_cooldown(object *attackers, string condition)
{
	foreach(object attacker in attackers)
	{
		if( !objectp(attacker) ) continue;
			
		if( attacker->is_user_ob() )
		{
			attacker->start_condition(condition);
		}
		else if( attacker->is_module_npc() || attacker->is_summon_mob() )
		{
			attacker = load_user(query("boss", attacker));
			
			if( objectp(attacker) )
			{
				if( !attacker->in_condition(condition) )
					attacker->start_condition(condition);
				
				if( !userp(attacker) )
				{
					attacker->save();
					destruct(attacker);
				}
			}
		}
	}
}

int check_cooldown(object enemy, string condition)
{
	if( enemy->is_module_npc() )
		enemy = find_player(query("boss", enemy) || "");
		
	if( !objectp(enemy) )
		return 0;
			
	return !enemy->in_condition(condition);
}

int is_monster()
{
	return 1;
}

