/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : mu_qing_yi.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-12-13
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
			(: do_command("sing") :)   : 50,
	    ]));
}

void delete_behavior()
{
	delete("behavior");
}

void reply_say(object me, string msg)
{
	if( !objectp(me) ) return;

	if( !msg || !msg[0] )
		return;
		
	if( strsrch(msg, "�_��") != -1 )
	{
		do_command("say �ڤ����D�L�O�֡A���ڥi�H�q�o�ӦW�r���P����@�ѤH�����j�R");
		return;
	}
	
	if( strsrch(msg, "�Z��") != -1 )
	{
		do_command("say �ڤ����D�L�O�֡A���ڪ��D�L�w�g�D�J�����A�H�ʹN�O�p���A�ñ��z�Ҿ֦����@���a...");
		return;
	}
	
	if( strsrch(msg, "�¦�") != -1 )
	{
		do_command("say ��...�¦Ъ��A�o���¾�ܰQ�H���w�A��~�e�ڴ��J���L�o�A�Ⱥ޷�ɧڤw�w�����骺���G�A���ڤ����Ԥߧi�D�o�o�ӥ��w�o�ͪ��d�@");
		return;
	}
	
	if( strsrch(msg, "�H��") != -1 )
	{
		do_command("say �¦Щҫ����X�����賥�s�H�Ѫ��S��ä��b�󥦪��ĩʡA�ӬO�b��ĭ�|�F�Ƥd�~�Ҳ��ͪ��F��");
		return;
	}
	
	if( strsrch(msg, "�F��") != -1 )
	{
		do_command("smile "+me->query_id(1));
		do_command("say "+pnoun(2, me)+"�|�J�W���¦СA�K�O���t�H�A�W�Ѧ۷|��"+pnoun(2, me)+"����");
		
		if( me->query_quest_step(QUEST_YIN_LING_YU_2) == 1 )
		{
			me->set_quest_note(QUEST_YIN_LING_YU_2, QUEST_YIN_LING_YU_2_NAME, "���F�P�k�y�p�C�̡z�A���D�F��ӳo�賥�s�H�Ѩ㦳�ۯ������F�ʡC");
			me->set_quest_step(QUEST_YIN_LING_YU_2, 2);
		}
		return;
	}
	
	if( strsrch(msg, "����") != -1 )
	{
		do_command("say �ѹa�|��ô�a�H��.....");
		return;
	}
		
	if( strsrch(msg, "�ƥ�") != -1 )
	{
		do_command("say "+pnoun(2, me)+"�O���q�۵M�ɥͪ��W���o���Ǥƥ۶ܡHť���̪�J�կ����a�ϨӤF�@�Ӧҥj�Ǯa�M���b�����o�Ǥƥ�...");	
		return;
	}
	
	if( me->query_quest_step(QUEST_CITY_WALKER) == 1 && strsrch(msg, "�����C�H") != -1 )
	{
		do_command("say ��H"+pnoun(2, me)+"�Q���D�@������L���Ʊ��ܡH");
		me->set_quest_note(QUEST_CITY_WALKER, QUEST_CITY_WALKER_NAME, "�γ\��Ū���q�ֵ��p�C��ťť�C");
		me->set_quest_step(QUEST_CITY_WALKER, 2);
		return;
	}
	
	if( me->query_quest_step(QUEST_CITY_WALKER) == 2 && strsrch(msg, "�@�b�֦����ۥ�������") != -1 )
	{
		msg("$ME�`�N��$YOU���F�����ά�MŸ�ʤF�@�U�A���H�Y�S�^�_�쥻���ˤl...�C\n", me, this_object(), 1);
		do_command("say ...��Ӧp��");
		do_command("say �o���֥s�@�u��@�Ӥ����_�����ڡv�A"+pnoun(2, me)+"�O�ۧa�C");
		
		me->set_quest_note(QUEST_CITY_WALKER, QUEST_CITY_WALKER_NAME, "��ӳo���֥s�@�u��@�Ӥ����_�����ڡv�C");
		me->set_quest_step(QUEST_CITY_WALKER, 3);
		return;
	}
	
	
	if( strsrch(msg, "���j�Z���H��") != -1 )
	{
		do_command("say ��H"+pnoun(2, me)+"�ݹL���Ƕǻ������H���ܡH");
		return;
	}
	
	if( strsrch(msg, "�ݹL") != -1 || strsrch(msg, "�ǻ������H��") != -1 )
	{
		int current_quest_step = me->query_quest_step(QUEST_LOST_SHARD);

		process_function(
		({
			(: do_command("ponder") :),
			3,
			(: do_command("say �Ʀ~�e�A�b�@��������B�X�g���j�夤...") :),
			3,
			(: do_command("say �O���ۼƤd�~�e�����@��Z�h����@�⻷�j���M�A���ƭӤj���ñٱ��L�Ʈ`�H���Ǫ�...") :),
			3,
			(: do_command("say ������Z�h�o�b�@���X����������F�ܸ�A�q������A�]�S�H���L�L...") :),
			3,
			(: do_command("say ����@�ɤW����ǥX���H�B�򻷥j���M���H���A���S�H���D�o�ǸH���������...") :),
			3,
			(: do_command("say �p�G�ڥi�H����o�ǸH���A�γ\�ڥi�H�q���o��@�Ƿ�쨺��Z�h���ҥH���ܪ��u��...") :),
			3,
			(: do_command("say ��M�A�o�γ\�u���L�O�@�Ӧѱ������ǻ��Ӥw...") :),
		}));
		
		if( current_quest_step == 0 || current_quest_step == 4 )
		{
			me->set_quest_note(QUEST_LOST_SHARD, QUEST_LOST_SHARD_NAME, "��ӥ@�ɤW�����۳\�h�������H���A�����������ܩγ\���ǥγB�C");
			me->set_quest_step(QUEST_LOST_SHARD, 1);
		}
		return;
	}
	
	if( strsrch(msg, "�H��") != -1 )
	{
		do_command("say ��H����H���H");
		return;
	}
}

void reply_emote(object me, string verb, string args)
{
	if( !objectp(me) ) return;
	
	do_command("bow "+me->query_id(1));
}

void got_all_lost_shard(object me, object ob)
{
	
	
}

void reply_get_object(object me, object ob, mixed amount)
{
	string *lost_shards = query("quest_temp/"+QUEST_LOST_SHARD+"/shard", me) || allocate(0);
	string basename;

	if( !objectp(ob) ) return;

	basename = base_name(ob);

	if( me->query_quest_step(QUEST_LOST_SHARD) == 1 )
	{
		switch(basename)
		{
			case "/quest/lost_shard/lost_shard_1":
			case "/quest/lost_shard/lost_shard_2":
			case "/quest/lost_shard/lost_shard_3":
			case "/quest/lost_shard/lost_shard_4":
			case "/quest/lost_shard/lost_shard_5":
			case "/quest/lost_shard/lost_shard_6":
			
				lost_shards |= ({ basename });
				
				if( sizeof(lost_shards) == 6 )
				{
					process_function(({
						1,
						(: do_command("say �D�`�n...") :),
						3,
						(: do_command("say �b"+pnoun(2, $(me))+"�浹�ڳo�̫�@���H���ɪ��P�ɡA�ڤ]�ѳz�o�ǸH���������ê��̤j���K�C") :),
						3,
						(: do_command("say "+pnoun(2, $(me))+"�еy���@�|��A�ڷǳƤ@�ʫH��"+pnoun(2, $(me))+"...") :),
						1,
						(: msg("$ME���_�ȵ��}�l���g�@�ʮѫH...�C\n", this_object(), 0, 1) :),
						8,
						(: msg("$ME��U�⤤�����A�N�Ҧ����H���P�H�@����J�H�ʤ�..�C\n", this_object(), 0, 1) :),
						2,
						(: do_command("say ��o�ʫH�浹�@�컷�b�t�@�Ӥj���W���ѹD�h�A�L�|�i�D"+pnoun(2, $(me))+"���U�Ӹӫ��...") :),
						1,
						(: msg("$ME�N�ѫH�浹�F$YOU�C\n", this_object(), $(me), 1) :),
						(: new("/quest/lost_shard/letter")->move($(me)) :),
						(: delete("quest_temp/"+QUEST_LOST_SHARD+"/shard", $(me)) :),
						(: $(me)->set_quest_note(QUEST_LOST_SHARD, QUEST_LOST_SHARD_NAME, "�N�p�C�̼��g���H�浹�@�컷�b�t�@�Ӥj���W���ѹD�h�C") :),
						(: $(me)->set_quest_step(QUEST_LOST_SHARD, 2) :),
					}));
				}
				else
				{
					me->set_quest_note(QUEST_LOST_SHARD, QUEST_LOST_SHARD_NAME, pnoun(2, me)+"�w�g������F "+sizeof(lost_shards)+" ���H���A�����������ܩγ\���ǥγB�C");
	
					process_function(({
						1,
						(: do_command("nod �Q����"+pnoun(2, $(me))+"�i�H���o�ǸH��...") :),
						2,
						(: do_command("say �o���H���D�`�ܮz�A�H�ɳ��i��l�a�A�ڷ|��"+pnoun(2, $(me))+"�����O�s�n�A��"+pnoun(2, $(me))+"�������Ҧ��H���ɡA�ڷQ�ګK�i�H�ѸԥX�̭������K�C") :),
					}));
				
					set("quest_temp/"+QUEST_LOST_SHARD+"/shard", lost_shards, me);
					
				}
				me->save();
				destruct(ob);
				break;
			default:
			{
				process_function(({
					1,
					(: do_command("say �ѹa�|��ô�a�H...") :),
					1,
					(: do_command("drop all "+$(ob)->query_id(1)) :),
				}));
				break;
			}
		}
		
		return;
	}
	
	process_function(({
		1,
		(: do_command("say �ѹa�|��ô�a�H...") :),
		1,
		(: do_command("drop all "+$(ob)->query_id(1)) :),
	}));
}


void create()
{
	set_idname("mu qing yi", HIG"�p"NOR GRN"�C��"NOR);
	set_temp("status", ([HIG"�@�~"NOR GRN"�P�k"NOR:-1]));

	set("long", "�n�M�q���~���k�l�A�o����護���ϤH���������ۦo�A���b�o�����Ǥ��ѱo�ۺF��©�C");
	set("unit", "��");
	set("age", 24);
	set("gender", FEMALE_GENDER);
	set("heartbeat", 1); // �ä[�ʤ߸�

	set("attr/str", 1500);
	set("attr/phy", 1200);
	set("attr/int", 100);
	set("attr/agi", 700);
	set("attr/cha", 200);

	set("abi/stamina/max", 3000);
	set("abi/stamina/cur", 3000);
	set("abi/health/max", 11000);
	set("abi/health/cur", 11000);
	set("abi/energy/max", 1000);
	set("abi/energy/cur", 1000);

	set("no_fight", 1);

	// �ҰʥD�ʦ欰
	set_behavior();

	startup_living();
}
