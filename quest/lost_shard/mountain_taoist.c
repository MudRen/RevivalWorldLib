/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : mountain_taoist.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-08-18
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
#include <quest.h>

inherit STANDARD_PERSON;

void do_command(string cmd);

void set_behavior()
{
	set("behavior/random", 
	    ([
			//(: do_command("sweat") :)   : 50,
	    ]));
}


void delete_behavior()
{
	delete("behavior");
}

void reply_say(object me, string msg)
{
	if( !objectp(me) ) return;

	//msg("$ME�q�q�a�b�@���j���Y�W�R���A���o�@�y�A���H�L���j�åL�X�G�b�ժ��G�l...�C\n", this_object(), 0, 1);
}

void reply_emote(object me, string verb, string args)
{
	if( !objectp(me) ) return;
}

void reply_get_object(object me, object ob, mixed amount)
{
	if( !objectp(ob) ) return;

	if( me->query_quest_step(QUEST_LOST_SHARD) == 2 && base_name(ob) == "/quest/lost_shard/letter" )
	{
		process_function(({
			1,
			(: msg("$ME��M���n�j���G�u���B���B���v...�C\n", this_object(), 0, 1) :),
			3,
			(: do_command("say �p�C�̳o�k�įu�O�J�o���o�S�J�x�A�~�M�N�o���D�ᵹ�ڳo�Ӧ��w���ݥ@�ƪ����Y��...�C") :),
			3,
			(: do_command("say ���ڷQ�o��ơA���@�W�]�u���ڳo�Ӥ����Ϊ����Y�٪��D�@�Ǩ䤤�����K...�C") :),
			3,
			(: do_command("say "+pnoun(2, $(me))+"�Ҧ������o�ǸH���A���O�@��q���򸨪��j�a���_��...�C") :),
			3,
			(: do_command("say ���o�ǸH���۷�ܮz�A"+pnoun(2, $(me))+"�S����k�����Q�γo�ǸH���h���}�q�������j�����q�D") :),
			3,
			(: do_command("say �ڥi�H��"+pnoun(2, $(me))+"�N�o�ǸH���i��j�ơA�е��ڤ@�������A�ګK�i�H�N�o�ǸH���O�b������") :),
			(: $(me)->set_quest_note(QUEST_LOST_SHARD, QUEST_LOST_SHARD_NAME, "���T�s�D�h�@�������A���L�j�ƨ��ǸH���C") :),
			(: $(me)->set_quest_step(QUEST_LOST_SHARD, 3) :),
		}));
		
		destruct(ob);
		return;
	}
	else if( me->query_quest_step(QUEST_LOST_SHARD) == 3 && ob->query_module() == "glass" )
	{
		process_function(({
			1,
			(: do_command("say �ܦn�A�ڳo�N����"+pnoun(2, $(me))+"�j�Ƥ@�U�o�ǸH���a...�C") :),
			1,
			(: msg("$ME�򴤵۬����P�Ҧ������j�Z���H���A�u���⤤�o�X�@�}�}��"HIC"�C"HIG"�m"HIR"�j"HIY"��"NOR"...\n", this_object(), 0, 1) :),
			8,
			(: msg("$ME�N�⻴����}...\n", this_object(), 0, 1) :),
			2,
			(: do_command("say �n�F�A�o�_�ͬO"+pnoun(2, $(me))+"���F�A�ܩ󨺶��򸨪��j�a���J�f�b���A�K�n�a"+pnoun(2, $(me))+"�ۤv�h�M��F...�C") :),
			(: msg("$ME�N�_�ͥ浹�F$YOU�C\n", this_object(), $(me), 1) :),
			(: new("/quest/lost_shard/lostland_key")->move($(me)) :),
			(: $(me)->set_quest_finish(QUEST_LOST_SHARD, QUEST_LOST_SHARD_NAME, "�q�T�s�D�h���W���o�F�@��q���򥢪��j���_�͡C") :),
			(: $(me)->set_quest_step(QUEST_LOST_SHARD, 4) :),
		}));
		
		if( !wizardp(me) )
			CHANNEL_D->channel_broadcast("city", me->query_idname()+"�Ѷ}�F�u"+QUEST_LOST_SHARD_NAME+"�v�C", me);

		destruct(ob);
		return;
	}

	process_function(({
		1,
		(: do_command("say ����"+pnoun(2, $(me))+"���n�N�A�ڤ��ûݭn�o�ӪF��...") :),
		1,
		(: do_command("drop all "+$(ob)->query_id(1)) :),
	}));
}


void create()
{
	set_idname("mountain taoist", HIY"�T�s"NOR YEL"�D�h"NOR);
	set_temp("status", ([HIY"�R"NOR YEL"��"NOR:-1]));

	set("long", "  �@��D�`�Ѫ��ѹD�h�A�ݰ_�ӭ׹D�w�[�A�@�T�Q���O�����ˤl�C");
	set("unit", "��");
	set("age", 112);
	set("gender", MALE_GENDER);
	set("heartbeat", 1); // �ä[�ʤ߸�

	set("attr/str", 500);
	set("attr/phy", 500);
	set("attr/int", 500);
	set("attr/agi", 500);
	set("attr/cha", 500);

	set("abi/stamina/max", 50000);
	set("abi/stamina/cur", 50000);
	set("abi/health/max", 50000);
	set("abi/health/cur", 50000);
	set("abi/energy/max", 50000);
	set("abi/energy/cur", 50000);

	set("no_fight", 1);

	// �ҰʥD�ʦ欰
	set_behavior();

	startup_living();
}
