/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : deer_sled.c
 * Author : Kyo@RevivalWorld
 * Date   : 2007-12-20
 * Note   : 麋鹿雪橇
 * Update :
 *  o 2003-00-00  

 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>
#include <feature.h>
#include <equipment.h>
#include <buff.h>
#include <daemon.h>
#include <map.h>

inherit STANDARD_OBJECT;
inherit EQUIPMENT;

mapping actions;

void do_drive(object me, string arg, object ob)
{
	int x, y;
	object env = environment(me);
	string city = env->query_city();
	int num = env->query_num();
	int loctype;
	array loc;

	if( !me->valid_move() ) 
		return;

	if( !arg || sscanf(arg, "%d,%d", x, y) != 2 )
		return tell(me, pnoun(2, me)+"想要駕著雪橇去哪裡？(例：50,50)\n");

	x--;
	y--;

	if( env->is_maproom() )
	{
		city = env->query_city();
		num = env->query_num();
	}
	else
	{
		city = env->query_city();
		num = env->query_city_num();
	}

	if( !CITY_D->city_exist(city, num) )
		return tell(me, pnoun(2, me)+"不在任何城市裡。\n");

	if( !CITY_D->valid_coordinate(x, y, city, num) )
		return tell(me, pnoun(2, me)+"必須輸入正確的座標位置。\n");             

	loc = arrange_city_location(x, y, city, num);

	loctype = CITY_D->query_coor_data(loc, TYPE);

	if( loctype != ROAD && loctype != BRIDGE )
		return tell(me, "座標"+loc_short(loc)+"並不是道路或橋樑，"+pnoun(2, me)+"沒辦法將"+ob->query_idname()+"開去那裡。\n");

	msg("$ME搭著"+ob->query_idname()+"往"+loc_short(loc)+"開了過去。\n", me, 0, 1);
	me->move(loc);
	msg("$ME搭著"+ob->query_idname()+"來到了"+loc_short(loc)+"。\n", me, 0, 1, 0, ({ me }));
}

void create()
{
	set_idname("deer sled", HIR"麋"NOR+RED"鹿"HIW"雪橇"NOR);

	if( this_object()->set_shadow_database() ) return;

	set("unit", "台");
	set("long", "聖誕節送的特別禮物。");
	set("mass", 10000);
	set("value", 1);
	set("flag/no_amount", 1);
	set("flag/no_drop", 1);
	set("flag/no_give", 1);

	set(BUFF_STR, 1);
	set(BUFF_PHY, 1);
	set(BUFF_AGI, 1);
	set(BUFF_INT, 1);
	set(BUFF_CHA, 1);

	set(BUFF_LOADING_MAX, 150);
	set(BUFF_SLOT_MAX, 15);

	set(BUFF_STAMINA_REGEN, 5);
	set(BUFF_HEALTH_REGEN, 5);
	set(BUFF_ENERGY_REGEN, 5);


	set("buff/status", HIR"麋"NOR RED"鹿"HIW"雪橇"NOR);

	::setup_equipment(EQ_VEHICLE, HIC"汽"NOR CYN"車"NOR);

	actions = ([ "drive" : (: do_drive :) ]);
}
