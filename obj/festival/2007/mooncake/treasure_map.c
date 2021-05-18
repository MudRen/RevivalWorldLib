/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : treasure_map.c
 * Author : Kyo@RevivalWorld
 * Date   : 2007-09-28
 * Note   : 藏寶圖
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
		CHANNEL_D->channel_broadcast("news", me->query_idname()+"的"+this_object()->query_idname()+"由於連續不斷翻閱化成灰燼後被風吹散。");
		destruct(this_object());
		set_temp("redig", 0, me);
		return;
	}
	set_temp("redig", test1, me);
	ob = new("/obj/festival/2007/mooncake/treasure_map");
	ob->move(me);
	tell( me,HIR"藏寶圖似乎有些改變。\n"NOR);
	tell( me,HIR"請勿連續使用 5 次，造成藏寶圖消失請自行負責。\n"NOR);
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
				log_file("command/mooncake2007", me->query_idname()+"獲得"+msg1);
				ob->move(me);
				CHANNEL_D->channel_broadcast("news", me->query_idname()+"根據"+this_object()->query_idname()+"上面的記載，終於挖到隱藏在 "+capitalize(loc[CITY])+" 的寶藏，得到 "+ob->query_idname()+"。");
				tell( me,HIR"藏寶圖破損消失了。\n"NOR);
				destruct(this_object());
			}
			else if(test > 2 && test < 51)
			{
				set_temp("redig", 0, me);
				me->earn_money("RW", money);
				CHANNEL_D->channel_broadcast("news", me->query_idname()+"根據"+this_object()->query_idname()+"意外的挖掘到 "+capitalize(loc[CITY])+" 隱藏的寶藏，得到 "+msg+"。");
				ob = new("/obj/festival/2007/mooncake/treasure_map");
				ob->move(me);
				tell( me,HIR"藏寶圖似乎有些改變。\n"NOR);
				destruct(this_object());
			}
			else
			{
				set_temp("redig", 0, me);
				CHANNEL_D->channel_broadcast("news", me->query_idname()+"根據"+this_object()->query_idname()+"意外的挖掘到 "+capitalize(loc[CITY])+" 隱藏的寶藏，結果發現是空的。");
				ob = new("/obj/festival/2007/mooncake/treasure_map");
				ob->move(me);
				tell( me,HIR"藏寶圖似乎有些改變。\n"NOR);
				destruct(this_object());
			}
		}
		return tell( me,"這裡並不是地圖上所記載的地點，請勿亂破壞水土保持。\n");
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

	set_idname("treasure map", HIW"藏"NOR"寶"HIW+BLK"圖"NOR);

	if( this_object()->set_shadow_database() ) return;

	set("unit", "張");
	set("long", "傳說中藏著大筆財富的藏寶圖。
　地圖標記的地方為 "HIR+where+NOR" 分區為 "HIW+num1+NOR" 座標為 ("HIY+x+NOR" , "HIY+y+NOR")");
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
