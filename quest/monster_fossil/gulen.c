/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : gulen.c
 * Author : Clode@RevivalWorld
 * Date   : 2009-12-24
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

inherit STANDARD_MONSTER;

mapping defeat_titles =
([
	10 		:	HIC"��"NOR CYN"��"HIY"�y"NOR YEL"�H"NOR,
	100		:	HIC"��"NOR CYN"��"HIB"�g"NOR BLU"�P"NOR,
	1000	:	HIC"��"NOR CYN"��"HIR"��"NOR RED"��"NOR,
	2000	:	HIC"��"NOR CYN"��"HIM"��"NOR MAG"��"NOR,
	5000	:	HIC"��"NOR CYN"��"WHT"��"HIW"��"NOR,
]);

void do_command(string cmd);

void set_behavior()
{
	set("behavior/random", 
	    ([
			(: do_command("sweat") :)   : 50,
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
		
	if( strsrch(msg, "�ƥ�") != -1 )
	{
		do_command("say ����I�H"+pnoun(2, me)+"��W�����Ǥƥ۶ܡH�֮����ڬݬݡI");
		return;
	}
	
	do_command("say "+pnoun(2, me)+"�b���Ǥ���H");
}

void reply_emote(object me, string verb, string args)
{
	if( !objectp(me) ) return;
		
	do_command("? "+me->query_id(1));
}

// ���`�ɪ��ƥ�
void reply_die()
{

}

// �Q���Ѯɪ��ƥ�
void reply_defeat(object* attackers)
{
	::reply_defeat(attackers);
}

void change_to_fighter()
{
	string file;
	string *random_drop = ({
		"/quest/monster_fossil/deep_sea_thorn",
		"/quest/monster_fossil/lava_meteor",
		"/quest/monster_fossil/jade_emerald",
		"/quest/monster_fossil/monster_gall",
		"/quest/monster_fossil/eagle_feather",
		"/quest/monster_fossil/eye_claw",
	});
	
	set("no_fight", 0);
	
	file = random_drop[random(sizeof(random_drop))];
	new(file)->move(this_object());
}

void reply_get_object(object me, object ob, mixed amount)
{
	string *fossils = query("quest/"+QUEST_FOSSIL+"/peice", me) || allocate(0);
	string basename;

	if( !objectp(ob) ) return;

	basename = base_name(ob);

	switch(basename)
	{
		case "/quest/monster_fossil/cat_fossil":
		case "/quest/monster_fossil/dog_fossil":
		case "/quest/monster_fossil/elephant_fossil":
		case "/quest/monster_fossil/lion_fossil":
		case "/quest/monster_fossil/orang_fossil":
		case "/quest/monster_fossil/panther_fossil":
		case "/quest/monster_fossil/pig_fossil":
		case "/quest/monster_fossil/python_fossil":
		case "/quest/monster_fossil/rabbit_fossil":
		case "/quest/monster_fossil/scolopendrid_fossil":
		case "/quest/monster_fossil/snake_fossil":
		case "/quest/monster_fossil/spider_fossil":
		case "/quest/monster_fossil/tiger_fossil":
		{
			destruct(ob);

			if( member_array(basename, fossils) != -1 )
			{
				do_command("say �ڤw�g���o���ƥۤF�A��"+pnoun(2, me)+"�J�M�n���ڪ��ܧڴN���Ȯ��...");
				break;
			}			
			
			fossils |= ({ basename });
			
			if( sizeof(fossils) == 13 )
			{
				process_function(({
					1,
					(: do_command("grin") :),
					(: do_command("say �Ӧn�F...") :),
					3,
					(: do_command("say �ٰO�o���ڻ��n��"+pnoun(2, $(me))+"���n�F��ܡH") :),
					2,
					(: do_command("say ���Ӯڥ��N�O�A"+pnoun(2, $(me))+"���I�֥s"+pnoun(2, $(me))+"����n�F�I���ӡI������...") :),
					(: change_to_fighter :),
					(: delete("quest/"+QUEST_FOSSIL+"/peice", $(me)) :),
					(: !wizardp($(me)) && CHANNEL_D->channel_broadcast("city", $(me)->query_idname()+" �Ѷ}�F�u"+QUEST_FOSSIL_NAME+"�v", $(me)) :),
					(: $(me)->set_quest_finish(QUEST_FOSSIL, QUEST_FOSSIL_NAME, "���ҥj�Ǯa���F�Ҧ����ƥۡA���L�o�����F�I�I") :),
				}));
			}
			else
			{
				if( me->query_quest_step(QUEST_FOSSIL) == 0 )
				{
					process_function(({
						(: do_command("say �Q����"+pnoun(2, $(me))+"�i�H���o�Ǥƥ�...") :),
						1,
						(: do_command("hehe") :),
						(: do_command("say "+pnoun(2, $(me))+"�p�G�i�H���ڳo���j�a�W�Ҧ��������ƥۡA�ڳo�̦��@�Ӧn�F��i�H�e���A�C") :),
						(: $(me)->set_quest_note(QUEST_FOSSIL, QUEST_FOSSIL_NAME, pnoun(2, $(me))+"�w�g������F "+sizeof($(fossils))+" ���ƥۡA���G�٦� "+(13-sizeof($(fossils)))+" �ؤƥ۩|�����C") :),
						(: $(me)->set_quest_step(QUEST_FOSSIL, 1) :),
					}));
				}
				else
				{
					process_function(({
						(: do_command("say �Ӧn�F�A�����٦� "+(13-sizeof($(fossils)))+" �ؤƥ�...") :),
						(: $(me)->set_quest_note(QUEST_FOSSIL, QUEST_FOSSIL_NAME, pnoun(2, $(me))+"�w�g������F "+sizeof($(fossils))+" ���ƥۡA���G�٦� "+(13-sizeof($(fossils)))+" �ؤƥ۩|�����C") :),
						(: $(me)->set_quest_step(QUEST_FOSSIL, 1) :),
					}));
				}

				set("quest/"+QUEST_FOSSIL+"/peice", fossils, me);
			}
			me->save();
			break;
		}
		default:
		{
			process_function(({
				1,
				(: do_command("say ���ڳo�ӷF���H�ڤ��ݭn�o�ӪF��...") :),
				1,
				(: do_command("drop all "+$(ob)->query_id(1)) :),
			}));
			break;
		}
	}
}

void create()
{
	set_idname("gulen", HIC"��"NOR CYN"��"NOR);
	set_temp("status", ([HIW"�ҥj"NOR WHT"�Ǯa"NOR : -1]));

	set("long", "  �@��q�~�a�Ӫ��ҥj�Ǯa�A�@���b�a���W�V�V�����A�L���W�n�����ܦh�}�_�j�Ǫ����~�C");
	set("unit", "��");
	set("age", 42);
	set("gender", MALE_GENDER);
	set("heartbeat", 1); // �ä[�ʤ߸�

	set("attr/str", 100);
	set("attr/phy", 50);
	set("attr/int", 50);
	set("attr/agi", 100);
	set("attr/cha", 50);

	set("abi/stamina/max", 20000);
	set("abi/stamina/cur", 20000);
	set("abi/health/max", 20000);
	set("abi/health/cur", 20000);
	set("abi/energy/max", 20000);
	set("abi/energy/cur", 20000);
	set("skills/unarmed/level", 130);
	set("skills/dodge/level", 130);
	set("skills/fatalblow/level", 50);

	set("no_fight", 1);

	// �ҰʥD�ʦ欰
	set_behavior();

	startup_living();
}
