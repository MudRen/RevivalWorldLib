/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : deer_sled.c
 * Author : Kyo@RevivalWorld
 * Date   : 2007-12-20
 * Note   : �G������
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
		return tell(me, pnoun(2, me)+"�Q�n�r�۳����h���̡H(�ҡG50,50)\n");

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
		return tell(me, pnoun(2, me)+"���b���󫰥��̡C\n");

	if( !CITY_D->valid_coordinate(x, y, city, num) )
		return tell(me, pnoun(2, me)+"������J���T���y�Ц�m�C\n");             

	loc = arrange_city_location(x, y, city, num);

	loctype = CITY_D->query_coor_data(loc, TYPE);

	if( loctype != ROAD && loctype != BRIDGE )
		return tell(me, "�y��"+loc_short(loc)+"�ä��O�D���ξ��١A"+pnoun(2, me)+"�S��k�N"+ob->query_idname()+"�}�h���̡C\n");

	msg("$ME�f��"+ob->query_idname()+"��"+loc_short(loc)+"�}�F�L�h�C\n", me, 0, 1);
	me->move(loc);
	msg("$ME�f��"+ob->query_idname()+"�Ө�F"+loc_short(loc)+"�C\n", me, 0, 1, 0, ({ me }));
}

void create()
{
	set_idname("deer sled", HIR"�G"NOR+RED"��"HIW"����"NOR);

	if( this_object()->set_shadow_database() ) return;

	set("unit", "�x");
	set("long", "�t�ϸ`�e���S�O§���C");
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


	set("buff/status", HIR"�G"NOR RED"��"HIW"����"NOR);

	::setup_equipment(EQ_VEHICLE, HIC"�T"NOR CYN"��"NOR);

	actions = ([ "drive" : (: do_drive :) ]);
}
