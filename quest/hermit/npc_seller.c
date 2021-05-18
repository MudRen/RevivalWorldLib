/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : npc_seller.c
 * Author : Clode@RevivalWorld
 * Date   : 2008-01-31
 * Note   : 
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

inherit STANDARD_PERSON;

void do_command(string cmd);



void set_behavior()
{
	set("behavior/random", 
	    ([
		(: msg("$ME�@�y�M�`�a�ݵۤ�W���ѡA�@�ʤ]����...�C\n", this_object(), 0, 1) :)   : 500,
	    ]));
}

void reply_say(object me, string msg)
{
	do_command("snort");
	do_command("say �n����n�I"+pnoun(2, me)+"�]�n�R�ޯ�ѬO���O�H�u�������خѦ�����n�ݪ�...");
}

void reply_emote(object me, string verb, string args)
{
	if( same_environment(this_object(), me) )
		do_command(verb + " " + me->query_id(1) + " " + (args||""));
}

void reply_get_object(object me, object ob, mixed amount)
{
	process_function(({
		1,
		do_command("say �o�O����H���ڳo�ӷF���H"),
		1,
		do_command("drop all "+ob->query_id(1)),
	    }));
}

void reply_get_money(object me, string unit, mixed money)
{

}

int is_seller_npc()
{
	return 1;
}

void create()
{
	set_idname("hermit","���h");
	
	set("unit", "��");
	
	set("age", 35);
	set("gender", MALE_GENDER);

	set("heartbeat", 1); // �ä[�ʤ߸�

	set("attr/str", 1000);
	set("attr/phy", 200);
	set("attr/int", 300);
	set("attr/agi", 500);
	set("attr/cha", 10);

	set("abi/stamina/max", 3000);
	set("abi/stamina/cur", 3000);
	set("abi/health/max", 8000);
	set("abi/health/cur", 8000);
	set("abi/energy/max", 1000);
	set("abi/energy/cur", 1000);

	set("no_fight", 1);

	// �ҰʥD�ʦ欰
	set_behavior();
	
	startup_living();
}



