/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : fortune_bag.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-08-10
 * Note   : 福袋
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

	// 稀有物
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
		CHANNEL_D->channel_broadcast("news", me->query_idname()+"緊張地打開"+ob->query_idname()+"，結果從裡面掉出「"+award->query_idname()+"」。");
		
		award->set_keep();
	}
	// 書
	else if( !random(20000) )
	{
		awards = calculate_awards(awards_paths_3);
		
		award = new(awards[random(sizeof(awards))]);
		
		if( !wizardp(me) )
		CHANNEL_D->channel_broadcast("news", me->query_idname()+"緊張地打開"+ob->query_idname()+"，結果從裡面掉出「"+award->query_idname()+"」。");
		
		award->set_keep();
	}
	else if( !random(1500) )
	{
		int insigne_count = random(2)+1;
		addn("battle_insigne", insigne_count, me);
		CHANNEL_D->channel_broadcast("news", me->query_idname()+"緊張地打開"+ob->query_idname()+"，結果從裡面掉出「"+insigne_count+" 點"HIC"戰"NOR CYN"爭"HIC"勳"NOR CYN"章"NOR"」。");	
		me->save();
		
		awards = calculate_awards(awards_paths_1);
		award = new(awards[random(sizeof(awards))]);
	}
	// 寶石
	else if( !random(300) )
	{
		awards = calculate_awards(awards_paths_4);
		
		award = new(awards[random(sizeof(awards))]);
	}
	// 工廠產品
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
	// 其他物品
	else if( !objectp(award) )
	{
		awards = calculate_awards(awards_paths_1);
		
		award = new(awards[random(sizeof(awards))]);
	}
			
	idname = award->query_idname();
	
	log_file("command/fortunebag", me->query_idname()+"獲得"+idname);
	
	msg("$ME緊張地打開"+ob->query_idname()+"，結果從裡面掉出「"+idname+"」。\n", me, 0, 1);

	if( me->available_get(award) )
	{
		award->move(me);
	}
	else
	{
		tell(me, pnoun(2, me)+"拿不動"+idname+"了，只好先放在地上。\n");
		award->move_to_environment(me);
	}

	destruct(ob, 1);
}

void create()
{
	set_idname("fortune bag", "福袋");
	
	if( this_object()->set_shadow_database() ) return;
	
	set("unit", "個");
	set("long", "一個神秘的福袋，也許只是一個傳說中專門用來吸金的東西...");
	set("mass", 100);
	set("value", 10000000);
	set("badsell", 1);
	actions = ([ "openbag" : (: do_openbag :) ]);
}
