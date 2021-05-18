/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : lu_hua.c
 * Author : Kyo@RevivalWorld
 * Date   : 2007-09-27
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

string *keywords = ({ "��", "��", "��", "��", "test", });

void do_command(string cmd);

int is_keyword(string msg)
{
	for(int i=0; i<sizeof(keywords); i++)
		if( msg && strsrch(msg, keywords[i]) != -1 ) return 1;
	return 0;
}

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
	if( !objectp(me) || !msg ) return;

	if( is_keyword(msg) )
	{
		do_command("point "+me->query_id(1)+" �A�O���O�ܷQ�i����");
		do_command("slap "+me->query_id(1));
		me->faint();
		return;
	}

	if( strsrch(msg, "����") != -1 || strsrch(msg, "���") != -1)
		do_command("say "+pnoun(2, me)+"�Q�n���D����§�������K�ܡH");
}

void reply_emote(object me, string verb, string args)
{
	/* �S�@��=.="
	if( is_keyword(args) )
	{
		do_command("point "+me->query_id(1)+" �A�O���O�ܷQ�i����");
		do_command("slap "+me->query_id(1));
		me->faint();
		return;
	}
	*/
	if( objectp(me) ) {
		switch(verb) {
			case "hi":
				do_command("draw " +me->query_id(1)+ " �e�~�f�H");
				break;
			case "nod":
				do_command("say �J�M"+pnoun(2, me)+"�Q���D�A�u�n"+pnoun(2, me)+"�����F���ͪ��@�ɨ�ثe�������§�������A���N�i�D"+pnoun(2, me)+"�C");
				break;
			default:
				do_command("arc " +me->query_id(1));
				break;
		}
	}
}

void destruct_self()
{
	object me, ob;
	mixed loc, test;
	string *where_name = CITY_D->query_cities();
	string where = where_name[random(sizeof(where_name))];

	loc = arrange_city_location(range_random(0,99), range_random(0,99), where, random(CITY_D->query_city_num(where)));

	test = loc_short(loc);

	ob = this_object();
	ob->move(loc);

	CHANNEL_D->channel_broadcast("sys", "�����_�ê�Npc���ʨ�F"+test, me);
	CHANNEL_D->channel_broadcast("news", "�ǻD���Ӫ��D�����_�ï��K���H�X�{�b "+where+" �C", me);
}

void reply_get_object(object me, object ob, mixed amount)
{
	string *moon_gifts = query("quest_temp/"+QUEST_MOON_RABBIT+"/gift", me) || allocate(0);
	string basename = base_name(ob);

	if( !objectp(ob) ) return;

	switch(basename)
	{
		case "/obj/festival/2006/mooncake/cake_glass":
		case "/obj/festival/2006/mooncake/rabbit_ear":
		case "/obj/festival/2006/mooncake/wukang_axe":
		case "/obj/festival/2006/mooncake/shaddock_hat":
		case "/obj/festival/2006/mooncake/hairpin":
		case "/obj/festival/2007/mooncake/rabbit_stick":
		case "/obj/festival/2007/mooncake/hoi_bow":

			moon_gifts |= ({ basename });

			if( sizeof(moon_gifts) == 7 )
			{
				CHANNEL_D->channel_broadcast("news", "�ڻ�"+me->query_idname()+"��o�F�@�i�j�N���d�U�����_�ϡC", me);
				process_function(({
					1,
					(: do_command("say �˯��t...") :),
					1,
					(: do_command("say ��"+pnoun(2, $(me))+"����j�_��Rw�˯��a...-.-") :),
					(: delete("quest_temp/"+QUEST_MOON_RABBIT+"/gift", $(me)) :),
					(: new("/obj/festival/2007/mooncake/treasure_map")->move($(me)) :),
				}));
				//CHANNEL_D->channel_broadcast("news", "�ڻ�"+me->query_idname()+"��o�F�@�i�j�N���d�U�����_�ϡC", me);
			}
			else
			{
				process_function(({
					1,
					(: do_command("say �٤����I�h����"+pnoun(2, $(me))+"�O���O�Q�w����P�` -.-") :),
					1,
					(: do_command("say ���n���U�@�ӫ����Y�����A"+pnoun(2, $(me))+"�ۤv�L�ӧ䯫") :),
					//(: destruct_self :),
				}));
				set("quest_temp/"+QUEST_MOON_RABBIT+"/gift", moon_gifts, me);	
			}
			me->save();
			destruct(ob);
			break;
		default:
		{
			process_function(({
			1,
			(: do_command("say "+pnoun(2, $(me))+"�����o�ӪF��F�ܡH�����ݭn�o�ӪF��...") :),
			1,
			(: do_command("drop all "+$(ob)->query_id(1)) :),
			}));
			break;
		}
	}
}

void create()
{
	set_idname("lu hua", HIY"��"NOR+YEL"��"NOR);
	set_temp("status", HIY"�I"NOR+YEL"��"NOR);

	set("long", "  �@�쯫�����G�p�ѥ͡A���G���D�ѤU�����K�A�ۺ٦ۤv�����C");
	set("unit", "��");
	set("age", 999);
	set("gender", MALE_GENDER);
	set("heartbeat", 1); // �ä[�ʤ߸�

	set("attr/str", 999);
	set("attr/phy", 999);
	set("attr/int", 999);
	set("attr/agi", 999);
	set("attr/cha", 999);

	set("abi/stamina/max", 99999);
	set("abi/stamina/cur", 99999);
	set("abi/health/max", 99999);
	set("abi/health/cur", 99999);
	set("abi/energy/max", 99999);
	set("abi/energy/cur", 99999);

	set("no_fight", 1);

	//if( clonep() )
		//call_out( (: destruct_self :), 300 );

	// �ҰʥD�ʦ欰
	set_behavior();

	startup_living();
}
