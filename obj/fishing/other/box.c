/* This	program	is a part of RW	mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File	  : box.c
 * Author : 
 * Date	  : 2003-5-31
 * Note	  : 寶箱
 * Update :
 *  o 2000-00-00  

 -----------------------------------------
 */

#include <ansi.h>
#include <message.h>
#include <inherit.h>
#include <daemon.h>
#include <map.h>

inherit	STANDARD_OBJECT;

mapping	actions;

void do_open(object me,	string arg, object box)
{
	int i;
	object ob, env;
	array loc = query_temp("location", box);

	env = environment(box);
	
	if( !box->id(arg) )
		return tell(me,	pnoun(2, me)+"要打開什麼東西？\n");

	if( arrayp(loc) && env && env->is_maproom() )
	{
		string owner = CITY_D->query_coor_data(loc, "owner");
		
		if( owner && owner[0..9] != "GOVERNMENT" && owner != me->query_id(1) && member_array(find_player(owner), present_objects(me)) != -1 )
			return msg("$YOU在一旁冷冷地看著$ME，$ME不敢隨意地開啟地上的寶箱。\n", me, find_player(owner), 1);
	}
		
	switch(	random(23) ) {
		case 0..8:
			i = range_random(20, 100);
			ob = new("/obj/fishing/fish/fish"+random(9));
			set_temp("amount", i, ob);
			msg("$ME打開寶箱一看，發現裡面裝著"+ob->short(1)+"。\n",	me, 0, 1);
			break;
		case 9..15:
			i = range_random(20, 100);
			ob = new("/obj/fishing/adv_fish/fish"+random(4));
			set_temp("amount", i, ob);
			msg("$ME打開寶箱一看，發現裡面裝著"+ob->short(1)+"。\n",	me, 0, 1);
			break;
		case 16..22:
			i = range_random(20, 100);
			ob = new("/obj/fishing/sp_fish/fish"+random(3));
			set_temp("amount", i, ob);
			msg("$ME打開寶箱一看，發現裡面裝著"+ob->short(1)+"。\n",	me, 0, 1);
			break;
	}

	if( objectp(ob)	)
	{
		if( !me->available_get(ob) )
		{
			loc = query_temp("location", me);
			
			if( MAP_D->query_map_system(loc) && (MAP_D->query_map_system(loc))->query_coor_data(loc, TYPE) == RIVER ) 
			{
				msg("$ME拿不動那麼多東西，"+ob->short(1)+"被水沖走消失了。\n", me, 0, 1);
				destruct(ob);
			}
			else
			{
				msg("$ME拿不動那麼多東西，只好先把"+ob->short(1)+"放在地上了。\n", me, 0, 1);
				ob->move_to_environment(me);
			}
		}
		else
			ob->move(me);
	}
		
	destruct(this_object(), 1);
}
void create()
{
	set_idname(HIY"treasure"NOR YEL" chest"NOR, HIY"寶"NOR YEL"箱"NOR);

	actions	= ([ "open" : (: do_open :) ]);

	if( this_object()->set_shadow_ob() ) return;
	
	set("unit", "個");
	set("value", 5000);
	set("mass", 500);
}
