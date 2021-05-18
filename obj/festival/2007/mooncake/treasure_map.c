/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : treasure_map.c
 * Author : Kyo@RevivalWorld
 * Date   : 2007-09-28
 * Note   : ���_��
 * Update :
 *  o 2003-00-00  


 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>
#include <location.h>
#include <daemon.h>

inherit STANDARD_OBJECT;

mapping actions;

void do_redig(object me, string arg)
{
	object ob;
	int test = query_temp("redig", me);
	int test1 = test + 1;

	if(test == 5)
	{
		CHANNEL_D->channel_broadcast("news", me->query_idname()+"��"+this_object()->query_idname()+"�ѩ�s���_½�\�Ʀ����u��Q���j���C");
		destruct(this_object());
		set_temp("redig", 0, me);
		return;
	}
	set_temp("redig", test1, me);
	ob = new("/obj/festival/2007/mooncake/treasure_map");
	ob->move(me);
	tell( me,HIR"���_�Ϧ��G���ǧ��ܡC\n"NOR);
	tell( me,HIR"�Фųs��ϥ� 5 ���A�y�����_�Ϯ����Цۦ�t�d�C\n"NOR);
	destruct(this_object());
}

void do_dig_here(object me, string arg)
{
	object ob;
	object env = environment(me);
	array loc = query_temp("location", me);
	int money = range_random(1, 1000000);
	int test = range_random(1, 100);
	string msg1;

	string msg = HIY"$RW "+NUMBER_D->number_symbol(money)+NOR;

	if( env->is_maproom() )
	{
		//tell(find_player("kyo"),sprintf("%O\n",loc[NUM] +1));
		//tell(find_player("kyo"),sprintf("%O\n",query("map/num") ));
		if(loc[CITY] == query("map/city") && loc[X]+1 == query("map/x") && loc[Y]+1 == query("map/y") && loc[NUM] == query("map/num"))
		{
			if(test <= 2)
			{
				set_temp("redig", 0, me);
				ob = new("/obj/festival/2007/mooncake/moonrabbit_card");
				msg1 = ob->query_idname();
				log_file("command/mooncake2007", me->query_idname()+"��o"+msg1);
				ob->move(me);
				CHANNEL_D->channel_broadcast("news", me->query_idname()+"�ھ�"+this_object()->query_idname()+"�W�����O���A�ש�������æb "+capitalize(loc[CITY])+" ���_�áA�o�� "+ob->query_idname()+"�C");
				tell( me,HIR"���_�ϯ}�l�����F�C\n"NOR);
				destruct(this_object());
			}
			else if(test > 2 && test < 51)
			{
				set_temp("redig", 0, me);
				me->earn_money("RW", money);
				CHANNEL_D->channel_broadcast("news", me->query_idname()+"�ھ�"+this_object()->query_idname()+"�N�~�������� "+capitalize(loc[CITY])+" ���ê��_�áA�o�� "+msg+"�C");
				ob = new("/obj/festival/2007/mooncake/treasure_map");
				ob->move(me);
				tell( me,HIR"���_�Ϧ��G���ǧ��ܡC\n"NOR);
				destruct(this_object());
			}
			else
			{
				set_temp("redig", 0, me);
				CHANNEL_D->channel_broadcast("news", me->query_idname()+"�ھ�"+this_object()->query_idname()+"�N�~�������� "+capitalize(loc[CITY])+" ���ê��_�áA���G�o�{�O�Ū��C");
				ob = new("/obj/festival/2007/mooncake/treasure_map");
				ob->move(me);
				tell( me,HIR"���_�Ϧ��G���ǧ��ܡC\n"NOR);
				destruct(this_object());
			}
		}
		return tell( me,"�o�̨ä��O�a�ϤW�ҰO�����a�I�A�ФŶï}�a���g�O���C\n");
	}
}

void create()
{
	string *where_name = CITY_D->query_cities();
	string where = where_name[random(sizeof(where_name))];

	int x = range_random(0,99);
	int y = range_random(0,99);
	int num = random(CITY_D->query_city_num(where) );
	int num1 = num + 1;

	set_idname("treasure map", HIW"��"NOR"�_"HIW+BLK"��"NOR);

	if( this_object()->set_shadow_database() ) return;

	set("unit", "�i");
	set("long", "�ǻ����õۤj���]�I�����_�ϡC
�@�a�ϼаO���a�謰 "HIR+where+NOR" ���Ϭ� "HIW+num1+NOR" �y�Ь� ("HIY+x+NOR" , "HIY+y+NOR")");
	set("mass", 1);
	set("value", 1);
	set("flag/no_amount", 1);
	
	set("map/x", x);
	set("map/y", y);
	set("map/city", where);
	set("map/num", num);

	actions = ([ "dig_here" : (: do_dig_here :),
							 "redig" : (: do_redig :) ]);
}
