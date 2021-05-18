/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : moon_rabbit.c
 * Author : Kyo@RevivalWorld
 * Date   : 2007-09-22
 * Note   : ���
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
#include <npc.h>
#include <condition.h>
#include <feature.h>

inherit STANDARD_PERSON;

string *players = allocate(0);
mapping emote_players = allocate_mapping(0);
string *keywords = ({ "��", "��", "��" });

string *awards =
({
	"/obj/festival/2007/mooncake/moon_cake",
	"/obj/festival/2007/mooncake/armor/moonrabbit_belt",
	"/obj/festival/2007/mooncake/armor/moonrabbit_boots",
	"/obj/festival/2007/mooncake/armor/moonrabbit_bracers",
	"/obj/festival/2007/mooncake/armor/moonrabbit_cloak",
	"/obj/festival/2007/mooncake/armor/moonrabbit_cloth",
	"/obj/festival/2007/mooncake/armor/moonrabbit_gloves",
	"/obj/festival/2007/mooncake/armor/moonrabbit_helm",
});

void do_command(string cmd);

int is_keyword(string msg)
{
	for(int i=0; i<sizeof(keywords); i++)
		if( msg && strsrch(msg, keywords[i]) != -1 ) return 1;
	return 0;
}
void reply_action(object me)
{
	object ob = this_object();
	string id = me->query_id(1);
	string city, city1;

	city = query("city", me);
	city1 = query_temp("first", ob);

	if( undefinedp(emote_players[id]) )
		emote_players[id] = 1;
	else
		emote_players[id]++;
	switch(emote_players[id])
	{
	case 1:
		this_object()->do_command("point "+me->query_id(1)+" �z�O���O�n�����z rm");
		break;
	case 2:
		this_object()->do_command("say �A�O�t�O�Q�i�t��");
		break;
	case 3:
		this_object()->do_command("draw "+id+" �A�u�x�|�i���ӣz");
		break;
	case 4:
		this_object()->do_command("grin "+id);
		break;
	default:
		this_object()->do_command("gan "+id);
		//this_object()->do_command("kill "+id);
		//if( time() > 1190736000 ) { this_object()->do_command("say �������F�٨�"); break; }
		if( ! city1 ) {
			set_temp("first", city);
			msg(HIR"$ME"HIR"���$YOU"HIR"�j�ۡG�u�h���a�I�I�v�C\n"NOR, this_object(), me, 1);
			COMBAT_D->start_fight(this_object(), me, 1);
		}
		else if( city1 == city) {
			msg(HIR"$ME"HIR"���$YOU"HIR"�j�ۡG�u�h���a�I�I�v�C\n"NOR, this_object(), me, 1);
			COMBAT_D->start_fight(this_object(), me, 1);
		}
		else {
			tell( me,HIR"�w�g���H�����F�C\n"NOR);
		}
		break;
	}
	return;
}

void reply_say(object me, string msg)
{
	if( is_keyword(msg) )
	{
		reply_action(me);
		return;
	}
	/*
	if( msg == "" )
	{
		string id = me->query_id(1);
		if( member_array(id, players) < 0 )
		{
			object ob = new("/obj/festival/2007/mooncake/moon_cake.c");
			this_object()->do_command("good");
			msg("$YOU�q$ME�⤤���U�F"+ob->short()+"�C\n", this_object(), me, 1 );
			ob->move(me);
			players |= ({ id });
		}
		else
		{
			this_object()->do_command("slap "+id);
			this_object()->do_command("point "+id+" �z�����L�{�ٷQ�F���v");
		}
		return;
	}
	*/
}

void reply_emote(object me, string verb, string args)
{
	if( is_keyword(args) )
	{
		reply_action(me);
		return;
	}
	this_object()->do_command("hmm "+me->query_id(1)+" "+args);
}

int target_gone(object me)
{

}

void reply_get_object(object me, object ob, mixed amount)
{

}

// ���`�ɪ��ƥ�
void reply_die()
{
	object ob;
	mixed loc, test;
	string ob_idname;
	string *where_name = CITY_D->query_cities();
	string where = where_name[random(sizeof(where_name))];

	ob = new("/obj/festival/2007/mooncake/moon_cake");
	//ob = new(awards[random(sizeof(awards))]);
	ob->move(this_object());

	ob_idname = ob->query_idname();

	//CHANNEL_D->channel_broadcast("news", "�Y�H�����F�N������ߨñq��߫���ߨ��F"+ob_idname+"�C");

	//destruct(ob); // ���}��Ȯ� dest ��

	TREASURE_D->get_treasure(this_object());

	loc = arrange_city_location(range_random(0,99), range_random(0,99), where, random(CITY_D->query_city_num(where)));

	test = loc_short(loc);

	//ob = new("/obj/festival/2007/mooncake/moon_deer");
	//ob->move(loc);
	//shout(HIB"����"NOR" - �_������ߥX�{�b  "+test+" "NOR"����C\n");
}

// �Q���Ѯɪ��ƥ�
void reply_defeat(object* attackers)
{
	
}

// �ˬd�O�_���X�k�԰��ؼ�
int valid_enemy(object enemy)
{
	return 1;	
}

// �����ɪ��T��
varargs string combat_attack_message(object target, object weapon)
{
	switch(random(5))
	{
	case 0:
		return "$ME���c����$YOU�ļ��L�ӡA";
		break;
	case 1:
		return "$ME��$YOU���I��ļ��L�ӡA";
		break;
	case 2:
		return "$ME�Ϋ�L�r��$YOU���L���A";
		break;
	case 3:
		return "$ME�q�����ֳt����$YOU�A";
		break;
	case 4:
		return "$ME���Y�W������$YOU�A";
		break;
	}
}

// ���m�ɪ��T��
varargs string combat_defend_message(object attacker, object weapon)
{

	return "$ME���Y�W�����צ�F$YOU�������_�X�}�}����A";
}

// �S�����
int special_attack(object attacker, object defender)
{
	int time;

	if( !random(50) )
	{
		shout(HIB"����"NOR"-�j�a�]"+attacker->query_idname()+"����q�_�ʤF�@�U�C\n");
	}

	if( !random(30) )
	{
		foreach(object ob in COMBAT_D->query_fighting_targets(attacker))
		{
			if( !random(2) ) continue;

			if( ob == attacker || !ob->is_living() || ob->is_faint() || ob->is_dead() || ob->in_condition(INVINCIBILITY) || !same_environment(ob, attacker) ) continue;

			time = range_random(1, 5);

			msg("$ME�ֳt���ưʡA�@�ۡu"HIY"����"NOR YEL"�ļ�"NOR"�v�A��$YOU���Ἲ��("+time+" ��)�A�óy�� 50 �I�ˮ`�I�I�I\n"NOR, attacker, ob, 1);

			ob->start_delay("moondeer", time, pnoun(2, ob)+"�]���u"HIY"����"NOR YEL"�ļ�"NOR"�v���v�T�ӷw�t���C\n", pnoun(2, ob)+"����F�w�t�C\n");

			COMBAT_D->cause_damage(attacker, ob, 50);
		}
	}
	return 0;
}

int is_boss()
{
	return 1;
}

void destruct_self()
{
	/*
	object ob;
	mixed loc, test;
	string *where_name = CITY_D->query_cities();
	string where = where_name[random(sizeof(where_name))];

	loc = arrange_city_location(range_random(0,99), range_random(0,99), where, random(CITY_D->query_city_num(where)));

	test = loc_short(loc);

	ob = new("/obj/festival/2007/mooncake/moon_deer");
	ob->move(loc);
	shout(HIB"����"NOR" - �S���H�z���_����߲��ʨ�F  "+test+" "NOR"����C\n");
	*/
	shout(HIB"\n����"NOR" - �S���H�z����̶߭]�^�F��y�C\n\n");
	destruct(this_object());
}

void create()
{
	set_idname("moon rabbit", HIC"�_��"NOR+CYN"��"HIW"���"NOR);

	set("unit", "��");
	set("age", 999);
	set("gender", MALE_GENDER);

	set("attr/str", 10);
	set("attr/phy", 10);
	set("attr/int", 10);
	set("attr/agi", 10);
	set("attr/cha", 10);

	set("abi/stamina/max", 15000);
	set("abi/stamina/cur", 15000);
	set("abi/health/max", 15000);
	set("abi/health/cur", 15000);
	set("abi/energy/max", 15000);
	set("abi/energy/cur", 15000);
	set("job/name", HIW+"����������"+NOR); 
	set("job/type", SOLDIER);
	set("skills/unarmed/level", 10);
	set("skills/dodge/level", 10);

	set("long", "�@����`�~�|�X�{���S��ʪ��A����_�Ǫ����v���H���H�ӷX�A\n���ӥj�Ѫ��ǻ��A�u�n�復���X�T�y�A�|�D���i�Ȫ��g�@�C");
	set("no_fight", 1);

	startup_living();

	if( clonep() )
		call_out( (: destruct_self :), 420 );
}

int remove()
{
	destruct(all_inventory(this_object()));
}

