/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : treasure_d.c
 * Author : Clode@RevivalWorld
 * Date   : 2006-02-07
 * Note   : 稀有珍寶精靈
 * Update :
 *  o 2006-00-00
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <daemon.h>
#include <map.h>

#define DATA_PATH	"/data/daemon/treasure.o"

int save()
{
	return save_object(DATA_PATH);	
}


void create()
{
	if( !restore_object(DATA_PATH) )
	{
		save();	
	}
}

int remove()
{
	return save();
}


void get_treasure(object me)
{
	int rate;

	rate = 1000000/COMBAT_D->total_value(me);

	if( rate < 1000 )
		rate = 1000;

	if( query("named", me) )
		rate = 15;
	
	/*
	if( !random(50000) )
	{
		switch(random(100))
		{
			case 0: new("/obj/adventure/map_5")->move(me); break;
			case 1..5: new("/obj/adventure/map_4")->move(me); break;
			case 6..15: new("/obj/adventure/map_3")->move(me); break;
			case 16..35: new("/obj/adventure/map_2")->move(me); break;
			default: new("/obj/adventure/map_1")->move(me); break;
		}
	}*/
}

void get_treasure_growth(object me, int growth_level, int land_type)
{
	int treasure_level;
	object ob;
	int my_intelligent = me->query_int();

	if( growth_level < 5 ) return;

	if( my_intelligent > 500 )
		my_intelligent = 500 + pow(my_intelligent - 500, 0.7);
	
	if( random(20000 - growth_level*200 - to_int(pow(to_float(my_intelligent), 0.95)) * random(40)) ) return;

	switch(land_type)
	{
	case FARM:
		{
			if( growth_level >= 16 && !random(3) )
			{
				treasure_level = 16;
				ob = new("/obj/farming/treasure/rice_god");
				me->earn_prestige(range_random(1,9));
			}
			else if( growth_level >= 15 && !random(3) )
			{
				treasure_level = 15;
				ob = new("/obj/farming/treasure/sky");
				me->earn_prestige(range_random(1,9));
			}
			else if( growth_level >= 14 && !random(3) )
			{
				treasure_level = 14;
				ob = new("/obj/farming/treasure/liangyi");
				me->earn_prestige(range_random(1,9));
			}
			else if( growth_level >= 13 && !random(3) )
			{
				treasure_level = 13;
				ob = new("/obj/farming/treasure/dashen");
				me->earn_prestige(range_random(1,9));
			}
			else if( growth_level >= 12 && !random(3) )
			{
				treasure_level = 12;
				ob = new("/obj/farming/treasure/wo_grass");
				me->earn_prestige(range_random(1,8));
			}
			else if( growth_level >= 11 && !random(3) )
			{
				treasure_level = 11;
				ob = new("/obj/farming/treasure/presl_pine");
				me->earn_prestige(range_random(1,7));
			}
			else if( growth_level >= 10 && !random(3) )
			{
				treasure_level = 10;
				ob = new("/obj/farming/treasure/buddha_wood");
				me->earn_prestige(range_random(1,6));
			}
			else if( growth_level >= 9 && !random(3) )
			{
				treasure_level = 9;
				ob = new("/obj/farming/treasure/stem_lotus");
				me->earn_prestige(range_random(1,5));	
			}
			else if( growth_level >= 8 && !random(3) )
			{
				treasure_level = 8;
				ob = new("/obj/farming/treasure/beauty_hibiscus");
				me->earn_prestige(range_random(1,4));
			}
			else if( growth_level >= 7 && !random(3) )
			{
				treasure_level = 7;
				ob = new("/obj/farming/treasure/night_lotus");
				me->earn_prestige(range_random(1,3));
			}
			else if( growth_level >= 6 && !random(3) )
			{
				treasure_level = 6;
				ob = new("/obj/farming/treasure/thousandleaf_lotus");
				me->earn_prestige(range_random(1,2));
			}
			else
			{
				treasure_level = 5;
				ob = new("/obj/farming/treasure/blood_orchid");
			}

			msg("$ME的農田生產出了"+ob->query_idname()+"。\n", me, 0, 1);
			CHANNEL_D->channel_broadcast("news", me->query_idname()+"的 "+growth_level+" 級"HIY"農田"NOR"產出了 "+HIW+treasure_level+NOR" 級"HIC"稀"NOR CYN"有"NOR"物品「"+ob->query_idname()+"」");
			log_file("command/grow", me->query_idname()+"得到"+ob->query_idname());

			ob->set_keep();
			ob->move(me);
			break;
		}
	case PASTURE:
		{
			if( growth_level >= 16 && !random(3) )
			{
				treasure_level = 16;
				ob = new("/obj/pasture/treasure/huge_lord");
				me->earn_prestige(range_random(1,9));
			}
			else if( growth_level >= 15 && !random(3) )
			{
				treasure_level = 15;
				ob = new("/obj/pasture/treasure/people");
				me->earn_prestige(range_random(1,9));
			}
			else if( growth_level >= 14 && !random(3) )
			{
				treasure_level = 14;
				ob = new("/obj/pasture/treasure/sishiang");
				me->earn_prestige(range_random(1,9));
			}
			else if( growth_level >= 13 && !random(3) )
			{
				treasure_level = 13;
				ob = new("/obj/pasture/treasure/wuju");
				me->earn_prestige(range_random(1,9));
			}
			else if( growth_level >= 12 && !random(3) )
			{
				treasure_level = 12;
				ob = new("/obj/pasture/treasure/tsouni_horse");
				me->earn_prestige(range_random(1,8));
			}
			else if( growth_level >= 11 && !random(3) )
			{
				treasure_level = 11;
				ob = new("/obj/pasture/treasure/nine_tail_fox");
				me->earn_prestige(range_random(1,7));
			}
			else if( growth_level >= 10 && !random(3) )
			{
				treasure_level = 10;
				ob = new("/obj/pasture/treasure/justice");
				me->earn_prestige(range_random(1,6));
			}
			else if( growth_level >= 9 && !random(3))
			{
				treasure_level = 9;
				ob = new("/obj/pasture/treasure/giant_eagle");
				me->earn_prestige(range_random(1,5));
			}
			else if( growth_level >= 8 && !random(3))
			{
				treasure_level = 8;
				ob = new("/obj/pasture/treasure/kylin");
				me->earn_prestige(range_random(1,4));
			}
			else if( growth_level >= 7 && !random(3))
			{
				treasure_level = 7;
				ob = new("/obj/pasture/treasure/fly_horse");
				me->earn_prestige(range_random(1,3));
			}
			else if( growth_level >= 6 && !random(3))
			{
				treasure_level = 6;
				ob = new("/obj/pasture/treasure/mammoth");
				me->earn_prestige(range_random(1,2));
			}
			else
			{
				treasure_level = 5;
				ob = new("/obj/pasture/treasure/pack_animal");
			}

			msg("$ME的牧場生產出了"+ob->query_idname()+"。\n", me, 0, 1);
			CHANNEL_D->channel_broadcast("news", me->query_idname()+"的 "+growth_level+" 級"HIY"牧場"NOR"產出了 "+HIW+treasure_level+NOR" 級"HIC"稀"NOR CYN"有"NOR"物品「"+ob->query_idname()+"」");
			log_file("command/grow", me->query_idname()+"得到"+ob->query_idname());

			ob->set_keep();
			ob->move(me);
			break;
		}
	case FISHFARM:
		{
			if( growth_level >= 16 && !random(3) )
			{
				treasure_level = 16;
				ob = new("/obj/fishfarm/treasure/namay");
				me->earn_prestige(range_random(1,9));
			}
			else if( growth_level >= 15 && !random(3) )
			{
				treasure_level = 15;
				ob = new("/obj/fishfarm/treasure/terra");
				me->earn_prestige(range_random(1,9));
			}
			else if( growth_level >= 14 && !random(3) )
			{
				treasure_level = 14;
				ob = new("/obj/fishfarm/treasure/bagua");
				me->earn_prestige(range_random(1,9));
			}
			else if( growth_level >= 13 && !random(3) )
			{
				treasure_level = 13;
				ob = new("/obj/fishfarm/treasure/jintu");
				me->earn_prestige(range_random(1,9));
			}
			else if( growth_level >= 12 && !random(3) )
			{
				treasure_level = 12;
				ob = new("/obj/fishfarm/treasure/maleh_gobi");
				me->earn_prestige(range_random(1,8));
			}
			else if( growth_level >= 11 && !random(3) )
			{
				treasure_level = 11;
				ob = new("/obj/fishfarm/treasure/zen_stone");
				me->earn_prestige(range_random(1,7));
			}
			else if( growth_level >= 10 && !random(3) )
			{
				treasure_level = 10;
				ob = new("/obj/fishfarm/treasure/king_stone");
				me->earn_prestige(range_random(1,6));
			}
			else if( growth_level >= 9 && !random(3) )
			{
				treasure_level = 9;
				ob = new("/obj/fishfarm/treasure/null_stone");
				me->earn_prestige(range_random(1,5));
			}
			else if( growth_level >= 8 && !random(3) )
			{
				treasure_level = 8;
				ob = new("/obj/fishfarm/treasure/bamboo_stone");
				me->earn_prestige(range_random(1,4));	
			}
			else if( growth_level >= 7 && !random(3) )
			{
				treasure_level = 7;
				ob = new("/obj/fishfarm/treasure/purple_stone");
				me->earn_prestige(range_random(1,3));
			}
			else if( growth_level >= 6 && !random(3) )
			{
				treasure_level = 6;
				ob = new("/obj/fishfarm/treasure/intelligence_stone");
				me->earn_prestige(range_random(1,2));
			}
			else
			{
				treasure_level = 5;
				ob = new("/obj/fishfarm/treasure/integrity_stone");
			}

			msg("$ME的養殖場生產出了"+ob->query_idname()+"。\n", me, 0, 1);
			CHANNEL_D->channel_broadcast("news", me->query_idname()+"的 "+growth_level+" 級"HIY"養殖場"NOR"產出了 "+HIW+treasure_level+NOR" 級"HIC"稀"NOR CYN"有"NOR"物品「"+ob->query_idname()+"」");
			log_file("command/grow", me->query_idname()+"得到"+ob->query_idname());

			ob->set_keep();
			ob->move(me);
			break;
		}
	}
}
string query_name()
{
	return "稀有寶藏系統(TREASURE_D)";
}
