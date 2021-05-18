/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : Santa_Claus.c
 * Author : Kyo@RevivalWorld
 * Date   : 2007-12-20
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
	object ob;
	int quest = query_temp("christmas", me);
	int questgift = query_temp("christmasgift", me);
	int getgift = query("christmasgift", me);
	int test = range_random(1, 100);
	string msg1;

	if( !objectp(me) || !msg ) return;

	if( is_keyword(msg) )
	{
		do_command("point "+me->query_id(1)+" �A�O���O�ܷQ�i����");
		do_command("slap "+me->query_id(1));
		//me->faint();
		return;
	}

	if( strsrch(msg, "����") != -1 || strsrch(msg, "�e§��") != -1)
	{
		if( query("total_online_time",  me) < 24*60*60*2 )
			return tell( me,HIR"�p�B�͵�"+pnoun(2, me)+"���j�A�������a�I\n"NOR);

		if(getgift == 1)
			return tell( me,HIR"��L§�����L�k����ĤG����I�I\n"NOR);

		if(questgift == 5)
			return tell( me,HIR"�z�w�g�����e�������A�i�H��t�ϦѤH���(���y)�F�I�I\n"NOR);

		if(quest > 1)
		{
			do_command("say "+pnoun(2, me)+"�٦b�o��F���H���O���n�������e§���ܡH");
		} else {
			do_command("say ���کԳ������G���]���F�A"+pnoun(2, me)+"�@�N�������e§���ܡH");
			set_temp("christmas", 1, me); // �Ĥ@�B
		}
	}
	else if( strsrch(msg, "���y") != -1 )
	{
		if(getgift != 1)
		{
			if(questgift == 5)
			{
				set_temp("christmas", 0, me);
				set("christmasgift", 1, me); // ��L���аO
				if(test <= 15)
				{
					ob = new("/obj/festival/2007/christmas/deer_card");
					msg1 = ob->query_idname();
					log_file("command/christmas2007", me->query_idname()+"��o"+msg1);
					ob->move(me);
					CHANNEL_D->channel_broadcast("news", me->query_idname()+"�Ѷ}�F�t�ϸ`���ȡA�����F�t��§�� "+ob->query_idname()+"�C");
				}
				else if(test > 15 && test <= 30)
				{
					ob = new("/obj/festival/2007/christmas/deer_sled");
					msg1 = ob->query_idname();
					log_file("command/christmas2007", me->query_idname()+"��o"+msg1);
					ob->move(me);
					CHANNEL_D->channel_broadcast("news", me->query_idname()+"�Ѷ}�F�t�ϸ`���ȡA�����F�t��§�� "+ob->query_idname()+"�C");
				}
				else if(test > 30 && test <= 65)
				{
					ob = new("/obj/festival/2007/christmas/xmas_ring_hide.c");
					msg1 = ob->query_idname();
					log_file("command/christmas2007", me->query_idname()+"��o"+msg1);
					ob->move(me);
					CHANNEL_D->channel_broadcast("news", me->query_idname()+"�Ѷ}�F�t�ϸ`���ȡA�����F�t��§�� "+ob->query_idname()+"�C");
				}
				else {
					ob = new("/obj/festival/2007/christmas/xmas_ring.c");
					msg1 = ob->query_idname();
					log_file("command/christmas2007", me->query_idname()+"��o"+msg1);
					ob->move(me);
					CHANNEL_D->channel_broadcast("news", me->query_idname()+"�Ѷ}�F�t�ϸ`���ȡA�����F�t��§�� "+ob->query_idname()+"�C");
				}
				return;
			}
			else
			{
				do_command("say �٨S�������N�Q��§���A"+pnoun(2, me)+"�Q�����N���s�ӹL�a�I");
				set_temp("christmasgift", 0, me);
				set_temp("christmas", 0, me);
				//me->faint();
				return;
			}
			return;
		}
		return tell( me,HIR"��L§�����L�k����ĤG����I�I\n"NOR);
	}
}

void reply_emote(object me, string verb, string args)
{
	mixed loc, test;
	string *where_name = AREA_D->query_areas();
	string where = where_name[random(sizeof(where_name))];
	int num;
	int quest = query_temp("christmas", me);

	do
	{
  	where = where_name[random(sizeof(where_name))];
	} while ( where == "lostland" || where == "battlefield");

	num = random(AREA_D->query_area_nums(where));
	loc = arrange_city_location(range_random(0,99), range_random(0,99), where, num);

	test = loc_short(loc);

	// �S�@��=.="
	//if( is_keyword(args) )
	//{
	//	do_command("point "+me->query_id(1)+" �A�O���O�ܷQ�i����");
	//	do_command("slap "+me->query_id(1));
	//	me->faint();
	//	return;
	//}

	if( objectp(me) ) {
		switch(verb) {
			case "hi":
				do_command("draw " +me->query_id(1)+ " �e�~�f�H");
				break;
			case "nod":
				if(quest == 1)
				{
					object ob = new("/obj/festival/2007/christmas/gift_box.c");
					do_command("say �J�M"+pnoun(2, me)+"�@�N�����A�u�n"+pnoun(2, me)+"���ڰe������§�����p�B�͡A���N��"+pnoun(2, me)+"�@�����y�C");
					do_command("say �o��§���N�·�"+pnoun(2, me)+"���ڰe��"+test+"������b���ݪ��p�ġC");
					tell(me, this_object()->query_idname()+"����"+pnoun(2, me)+"�@��"+ob->query_idname()+"�C\n");
					ob->move(me);
					ob = new("/obj/festival/2007/christmas/child.c");
					set("santaid", me->query_id(1), ob);
					ob->move(loc);
					set_temp("christmas", 2, me); // �ĤG�B
				} else {
					do_command("arc " +me->query_id(1));
				}
				break;
			default:
				do_command("arc " +me->query_id(1));
				break;
		}
	}
}

void reply_get_object(object me, object ob, mixed amount)
{
	string basename = base_name(ob);
	int check = query("christmasgift", me);
	//int quest = query_temp("christmasgift", me);

	if( !objectp(ob) ) return;

	if(basename == "/obj/festival/2007/christmas/xmas_ring" || basename == "/obj/festival/2007/christmas/xmas_ring_hide" 
	|| basename == "/obj/festival/2007/christmas/deer_sled" )
	{
		if(check == 1)
		{
			do_command("say "+pnoun(2, me)+"���Q�n�o��§���ڡH�p�G"+pnoun(2, me)+"�@�N�A�����e����§���A���N���s��"+pnoun(2, me)+"�@�����y�I�I");
			set("christmasgift", 0, me); // �����k0
			set_temp("christmasgift", 0, me); // �B�J�k0
			tell( me,HIR"���ȭ��s�ҰʡA�Ч�t�ϦѤH���s�����ȡC\n"NOR);
			CHANNEL_D->channel_broadcast("news", me->query_idname()+"�M�w���s�A�����t�ϦѤH�@���A�Фj�a�����[�o���x�n�I�I");
			log_file("command/christmas2007", me->query_idname()+"���s���W�@�Ӫ��~���s������");
			destruct(ob);
		}
		else
		{
			do_command("say "+pnoun(2, me)+"�ڥ��٨S��L���y�a�I�I");
			destruct(ob);
		}
	} else {
		do_command("say �o�O"+pnoun(2, me)+"�������t��§���ܡH���¤F�I�I");
		destruct(ob);
	}

}

void destruct_self()
{
	object me, ob;
	mixed loc, test;
	string *where_name = AREA_D->query_areas();
	string where = where_name[random(sizeof(where_name))];
	int num;

	do
	{
  	where = where_name[random(sizeof(where_name))];
	} while ( where == "lostland" || where == "battlefield");

	num = random(AREA_D->query_area_nums(where));
	loc = arrange_city_location(range_random(0,99), range_random(0,99), where, num);

	test = loc_short(loc);

	ob = this_object();
	ob->move(loc);

	//CHANNEL_D->channel_broadcast("news", "�t�ϦѤH�Ө�F�C"+test, me);
	//CHANNEL_D->channel_broadcast("news", "�t�ϦѤH�Ө�F "+AREA_D->query_area_idname(where, num)+" �M��������L�e§�����H�C", me);
	CHANNEL_D->channel_broadcast("news", "�t�ϦѤH�Ө�F "+AREA_D->query_area_idname(where)+" "+test+" �M��������L�e§�����H�C", me);
	call_out( (: destruct_self :), 180 );
}

void create()
{
	set_idname("santa claus", HIR"�t��"HIW"�ѤH"NOR);

	set("long", "  �@�~�@�����t�ϸ`�~�|�X�{���t�ϦѤH�A�M��L�t�ϦѤH���P���O�o��ѤH�Y�W����
  ���A�ӥB��l�O�Ŧ⪺�C");
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

	//set("no_fight", 1);

	if( clonep() )
		call_out( (: destruct_self :), 180 );

	// �ҰʥD�ʦ欰
	set_behavior();

	startup_living();
}
