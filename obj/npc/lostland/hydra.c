/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : hydra.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-11-25
 * Note   : �E�Y�D���w��
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

/*
	�t�ιw�]�� NPC �����禡
	void reply_emote(object me, string verb, string args);
	void reply_say(object me, string msg);
	void reply_catch_tell(object me, string verb, string args);
	void reply_get_object(object me, object ob, mixed amount);
	void reply_get_money(object me, string unit, mixed money);
*/

#include <ansi.h>
#include <inherit.h>
#include <gender.h>
#include <daemon.h>
#include <condition.h>

inherit STANDARD_MONSTER;

int reborn_time = 0;
int call_out_handle;

mapping defeat_titles =
([
	10 		:	HIW"���w��"HIY"�y"NOR YEL"�H"NOR,
	100		:	HIW"���w��"HIB"�g"NOR BLU"�P"NOR,
	1000	:	HIW"���w��"HIR"��"NOR RED"��"NOR,
	2000	:	HIW"���w��"HIM"��"NOR MAG"��"NOR,
	5000	:	HIW"���w��"WHT"��"HIW"��"NOR,
]);

void do_command(string cmd);

/*
void reply_say(object me, string msg)
{

}
*/

void reply_emote(object me, string verb, string args)
{
	this_object()->do_command("hmm "+me->query_id(1));
}

int target_gone(object me)
{

}

/*
void reply_get_object(object me, object ob, mixed amount)
{

}
*/

// ���`�ɪ��ƥ�
void reply_die()
{
	if( reborn_time == 8 )
	{
		int is_named = query("named");
		int num = environment(this_object())->query_num();
		object ob;
		string* random_drop;
		
		CHANNEL_D->channel_broadcast("combat", this_object()->query_idname()+"�R�R�a���b"+AREA_D->query_area_idname("lostland", num)+"���j�a�W�A���j�c�F�Ƭ�"HIR"�C"HIY"�m"HIG"��"HIC"�~"NOR"�A�v������...");
		
		foreach(ob in users())
		{
			if( ob->is_user_ob() && interactive(ob) && !wizardp(ob) )
				ob->start_condition(LOSTLAND);
		}
		
		if( !random(is_named ? 8 : 40) )
		{
			random_drop = ({ "/obj/weapon/lord_sword" });
		}
		else if( !random(is_named ? 4 : 20) )
		{
			random_drop =
			({
				"/obj/weapon/hydra_stick",
				"/obj/armor/zen_hydra_breast_plate",
				"/obj/armor/zen_hydra_leggings",
				"/obj/armor/zen_hydra_helm",
				"/obj/armor/zen_hydra_gloves",
				"/obj/armor/zen_hydra_cloak",			
				"/obj/armor/zen_hydra_bracers",
				"/obj/armor/zen_hydra_boots",
				"/obj/armor/zen_hydra_belt",
			});
		}
		else
		{
			random_drop =
			({
				"/obj/weapon/hydra_stick",
				"/obj/armor/hydra_breast_plate",
				"/obj/armor/hydra_leggings",
				"/obj/armor/hydra_helm",
				"/obj/armor/hydra_gloves",
				"/obj/armor/hydra_cloak",			
				"/obj/armor/hydra_bracers",
				"/obj/armor/hydra_boots",
				"/obj/armor/hydra_belt",
			});
		}
	
		ob = new(random_drop[random(sizeof(random_drop))]);
		ob->move(this_object());
	
		TREASURE_D->get_treasure(this_object());
	}
	else
	{
		call_out( (: destruct(this_object()) :), 1);
	}
}

// �Q���Ѯɪ��ƥ�
void reply_defeat(object* attackers)
{
	if( reborn_time == 8 )
	{
		::reply_defeat(attackers);
		::start_cooldown(attackers, LOSTSEAL);
		
		remove_call_out(call_out_handle);
	}
	else
	{
		object ob = new(base_name(this_object()), reborn_time + 1, attackers);
			
		ob->move_to_environment(this_object());
			
		msg("$ME��۩Ҧ��H��q�ۡC\n", ob, 0, 1);
	}
}

// �ˬd�O�_���X�k�԰��ؼ�
int valid_enemy(object enemy)
{
	return ::check_cooldown(enemy, LOSTSEAL);
}

// �����ɪ��T��
varargs string combat_attack_message(object target, object weapon)
{
	switch(random(5))
	{
		case 0:
			return "$ME�Ҧ����Y�P�ɩ�$YOU�r�O�����A";
			break;
		case 1:
			return "$ME��q���U�ļ�$YOU�A";
			break;
		case 2:
			return "$ME��_���}��$YOU�Ҧb��m�r�O��U�A";
			break;
		case 3:
			return "$ME�H�U��O�q�N���鱽��$YOU�A";
			break;
		case 4:
			return "$ME�r�O�a��$YOU�ļ������A";
			break;
	}
}

// ���m�ɪ��T��
varargs string combat_defend_message(object attacker, object weapon)
{
	
	
}

// �S�����
void special_attack(object attacker, object defender)
{
	int times;
	int damage;

	if( !random(100) )
	{
		if( !random(2) )
			shout(HIB"����"NOR"-�j�a�]"+attacker->query_idname()+"����q�_�ʤF�@�U�C\n");

		msg("$ME�i�}�j�L�s��V$YOU�Q�X�j�q�u"HIW"�n��"NOR WHT"�r�d"NOR"�v�A�y��$YOU�Y�����ˮ`�I�I\n"NOR, attacker, defender, 1);
		
		times = random(8)+1;

		while(times--)
		{
			damage += range_random(40, 80);
			msg("$ME�i�}�j�L�s��V$YOU�Q�X�j�q�u"HIW"�n��"NOR WHT"�r�d"NOR"�v�A�y��$YOU�Y�����ˮ`�I�I\n"NOR, attacker, defender, 1);
		}
		
		COMBAT_D->cause_damage(attacker, defender, damage);
		
		return;
	}
	
	if( !random(100) )
	{
		int x = 49;
		int y = 49;
		array loc;

		while(x == 49)
			x = range_random(44, 54);
		while(y == 49)
			y = range_random(44, 54);

		foreach(object ob in COMBAT_D->query_fighting_targets(attacker))
		{
			if( random(10) )
				continue;

			if( ob == attacker || ob == defender || !ob->is_living() || ob->is_faint() || ob->is_dead() || ob->in_condition(INVINCIBILITY) || !same_environment(ob, attacker) ) continue;
		
			loc = arrange_area_location(x, y, "lostland", 2);
					
			msg("$ME�b�餺��n��q�A�r�M���|�P�z�X�u"HIY"�٤�"NOR YEL"�_�i"NOR"�v�A�N$YOU�_����"+loc_short(loc)+"�I�I\n"NOR, attacker, ob, 1);
			
			ob->move(loc);
			
			msg("$ME�Q$YOU�_����"+loc_short(loc)+"�C\n", ob, attacker, 1);
		}
	}
}

int is_boss()
{
	return 3;
}

void create(int i, object* attackers)
{
	reborn_time = i;

	set_idname(HIREV WHT"hy"HIW"dra"NOR, HIREV WHT+CHINESE_D->chinese_number(9-i)+"�Y�D"HIW"���w��"NOR);

	set("unit", "��");
	set("age", 3000);
	set("gender", ANIMAL_GENDER);
	
	set("attr/str", 300);
	set("attr/phy", 250);
	set("attr/int", 300);
	set("attr/agi", 250);
	set("attr/cha", 300);

	set("abi/stamina/max", (i+1)*5000);
	set("abi/stamina/cur", (i+1)*5000);
	set("abi/health/max", (i+1)*5000);
	set("abi/health/cur", (i+1)*5000);
	set("abi/energy/max", (i+1)*5000);
	set("abi/energy/cur", (i+1)*5000);
	set("job/name", HIW+"������"+NOR);

	set("skills/unarmed/level", 160);
	set("skills/dodge/level", 160);
	set("skills/fatalblow/level", 6);

	set("long", "�@��þ���ܤ��̱j�����Ǫ������ʭ����Ǵ���(Typhon)�M�k���D���Ǥ̳ؼw��(Echinda)�ҥ͡A�屼�C�@���Y���|�A�͡A�ڻ����������Y�O�åͤ������C");

	set("autokill", 1);
	
	if( reborn_time == 8 )
	{
		set("prestige", 1);// ���`�ᵹ�����a���n��
		
		if( !random(15) )
			MAPNPC_D->become_named_npc(this_object());
		
		call_out_handle = "/system/daemons/mapnpc/area_lostland_2"->delay_start_cooldown(attackers);
	}

	startup_living();
}

int remove()
{
	destruct(all_inventory(this_object()));
}
