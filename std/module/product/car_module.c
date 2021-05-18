/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : module.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-04-06
 * Note   : 
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <feature.h>
#include <daemon.h>
#include <map.h>
#include <buff.h>

inherit PRODUCT_ACTION_MOD;

mapping inlay_effect =
([
	"/obj/gem/gem4" : (: addn(BUFF_LOADING_MAX, to_int(pow(to_float($1), 0.9)) * 10, $2) :),
	"/obj/gem/gem5" : (: addn(BUFF_SLOT_MAX, to_int(pow(to_float($1), 0.5)), $2) :),
]);

array enhancement = 
({ 
	(: addn(BUFF_LOADING_MAX, $1 * 5, $2) :),
	(: addn(BUFF_SLOT_MAX, $1, $2) :),
});

// 開車
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

	if( !me->is_equipping_object(ob) )
		return tell(me, pnoun(2, me)+"必須先裝備"+ob->query_idname()+"。\n");

	if( !arg || sscanf(arg, "%d,%d", x, y) != 2 )
		return tell(me, pnoun(2, me)+"想要把車開去哪裡？(例：50,50)\n");

	x--;
	y--;

	if( env->is_maproom() )
	{
		city = env->query_city();
		num = env->query_num();
		
		loc = query_temp("location", me);
		
		loctype = MAP_D->query_coor_data(loc, TYPE);

		if( loctype == SEA || loctype == RIVER )
			return tell(me, pnoun(2, me)+"目前所在位置是海洋或河流，無法開車。\n");
	}
	else
	{
		if( env->query_floor() > 1 )
			return tell(me, pnoun(2, me)+"必須先回到一樓才能開車。\n");

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

	msg("$ME開著"+ob->query_idname()+"往"+loc_short(loc)+"開了過去。\n", me, 0, 1);
	me->move(loc);
	msg("$ME開著"+ob->query_idname()+"來到了"+loc_short(loc)+"。\n", me, 0, 1, 0, ({ me }));
}

// 設定物件動作
nosave mapping action_info =
([
	"help"	: "尚未建構說明。",
	"action":
		([
			"drive"		: (: do_drive :),
		]),
]);

// 設定物件資料
nosave mapping product_info = 
([	
	//中文名稱
	"name":		"汽車",
	
	//英文名稱
	"id":		"car",

	//材料需求
	"material":	([ "tire":4, "car_body":1, "engine":1, "/obj/pasture/product/leather":5, "integrated_circuit":1, "petroleum":100 ]),

	//技能需求
	"skill":	([ "transportation-fac":90, "technology":90 ]),

	//生產工廠
	"factory":	"transportation",
]);
