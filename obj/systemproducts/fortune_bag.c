/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : fortune_bag.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-08-10
 * Note   : �ֳU
 * Update :
 *  o 2003-00-00  

 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>
#include <daemon.h>

inherit STANDARD_OBJECT;

mapping actions;

string *awards_paths_1 =
({
	"/obj/farming/seed/",
	"/obj/farming/product/",
	"/obj/pasture/breed/",
	"/obj/pasture/product/",
	"/obj/fishfarm/breed/",
	"/obj/fishfarm/product/",
});

string *awards_paths_2 =
({
	"/obj/farming/treasure/",
	"/obj/pasture/treasure/",
	"/obj/fishfarm/treasure/",
});

string *awards_paths_3 =
({
	"/obj/book/skill/",
});

string *awards_paths_4 =
({
	"/obj/gem/",
});


string *calculate_awards(string *awards_paths)
{
	string *all_awards = allocate(0);

	foreach(string path in awards_paths)
	foreach(string file in get_dir(path))
	{
		if( file[0] != '_' )
			all_awards |= ({ path + file[0..<3] });
	}
		
	return all_awards;
}

void do_openbag(object me, string arg, object ob)
{
	object award;
	string *awards;
	string idname;

	// �}����
	if( !random(80000) )
	{
		awards = calculate_awards(awards_paths_2);

		awards -= 
		({
			"/obj/pasture/treasure/tsouni_horse",
			"/obj/pasture/treasure/wuju",
			"/obj/pasture/treasure/sishiang",
			"/obj/pasture/treasure/sky",

			"/obj/farming/treasure/wo_grass",
			"/obj/farming/treasure/dashen",
			"/obj/farming/treasure/liangyi",
			"/obj/farming/treasure/people",
			
			"/obj/fishfarm/treasure/maleh_gobi",
			"/obj/fishfarm/treasure/jintu",
			"/obj/fishfarm/treasure/bagua",
			"/obj/fishfarm/treasure/terra",
		});

		award = new(awards[random(sizeof(awards))]);
		
		if( !wizardp(me) )
		CHANNEL_D->channel_broadcast("news", me->query_idname()+"��i�a���}"+ob->query_idname()+"�A���G�q�̭����X�u"+award->query_idname()+"�v�C");
		
		award->set_keep();
	}
	// ��
	else if( !random(20000) )
	{
		awards = calculate_awards(awards_paths_3);
		
		award = new(awards[random(sizeof(awards))]);
		
		if( !wizardp(me) )
		CHANNEL_D->channel_broadcast("news", me->query_idname()+"��i�a���}"+ob->query_idname()+"�A���G�q�̭����X�u"+award->query_idname()+"�v�C");
		
		award->set_keep();
	}
	else if( !random(1500) )
	{
		int insigne_count = random(2)+1;
		addn("battle_insigne", insigne_count, me);
		CHANNEL_D->channel_broadcast("news", me->query_idname()+"��i�a���}"+ob->query_idname()+"�A���G�q�̭����X�u"+insigne_count+" �I"HIC"��"NOR CYN"��"HIC"��"NOR CYN"��"NOR"�v�C");	
		me->save();
		
		awards = calculate_awards(awards_paths_1);
		award = new(awards[random(sizeof(awards))]);
	}
	// �_��
	else if( !random(300) )
	{
		awards = calculate_awards(awards_paths_4);
		
		award = new(awards[random(sizeof(awards))]);
	}
	// �u�t���~
	/*
	else if( !random(50) )
	{
		string file;

		awards = allocate(0);
		
		foreach(string path in get_dir("/product/"))
			awards += ({ "/product/"+path+"/" });

		awards = calculate_awards(awards);
		
		do
		{
			file = awards[random(sizeof(awards))];
		} while(!PRODUCT_D->manufactured_product(file));
		
		award = new(file);

	}
	*/
	// ��L���~
	else if( !objectp(award) )
	{
		awards = calculate_awards(awards_paths_1);
		
		award = new(awards[random(sizeof(awards))]);
	}
			
	idname = award->query_idname();
	
	log_file("command/fortunebag", me->query_idname()+"��o"+idname);
	
	msg("$ME��i�a���}"+ob->query_idname()+"�A���G�q�̭����X�u"+idname+"�v�C\n", me, 0, 1);

	if( me->available_get(award) )
	{
		award->move(me);
	}
	else
	{
		tell(me, pnoun(2, me)+"������"+idname+"�F�A�u�n����b�a�W�C\n");
		award->move_to_environment(me);
	}

	destruct(ob, 1);
}

void create()
{
	set_idname("fortune bag", "�ֳU");
	
	if( this_object()->set_shadow_database() ) return;
	
	set("unit", "��");
	set("long", "�@�ӯ������ֳU�A�]�\�u�O�@�Ӷǻ����M���Ψӧl�����F��...");
	set("mass", 100);
	set("value", 10000000);
	set("badsell", 1);
	actions = ([ "openbag" : (: do_openbag :) ]);
}
