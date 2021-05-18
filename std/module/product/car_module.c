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

// �}��
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
		return tell(me, pnoun(2, me)+"�������˳�"+ob->query_idname()+"�C\n");

	if( !arg || sscanf(arg, "%d,%d", x, y) != 2 )
		return tell(me, pnoun(2, me)+"�Q�n�⨮�}�h���̡H(�ҡG50,50)\n");

	x--;
	y--;

	if( env->is_maproom() )
	{
		city = env->query_city();
		num = env->query_num();
		
		loc = query_temp("location", me);
		
		loctype = MAP_D->query_coor_data(loc, TYPE);

		if( loctype == SEA || loctype == RIVER )
			return tell(me, pnoun(2, me)+"�ثe�Ҧb��m�O���v�Ϊe�y�A�L�k�}���C\n");
	}
	else
	{
		if( env->query_floor() > 1 )
			return tell(me, pnoun(2, me)+"�������^��@�Ӥ~��}���C\n");

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

	msg("$ME�}��"+ob->query_idname()+"��"+loc_short(loc)+"�}�F�L�h�C\n", me, 0, 1);
	me->move(loc);
	msg("$ME�}��"+ob->query_idname()+"�Ө�F"+loc_short(loc)+"�C\n", me, 0, 1, 0, ({ me }));
}

// �]�w����ʧ@
nosave mapping action_info =
([
	"help"	: "�|���غc�����C",
	"action":
		([
			"drive"		: (: do_drive :),
		]),
]);

// �]�w������
nosave mapping product_info = 
([	
	//����W��
	"name":		"�T��",
	
	//�^��W��
	"id":		"car",

	//���ƻݨD
	"material":	([ "tire":4, "car_body":1, "engine":1, "/obj/pasture/product/leather":5, "integrated_circuit":1, "petroleum":100 ]),

	//�ޯ�ݨD
	"skill":	([ "transportation-fac":90, "technology":90 ]),

	//�Ͳ��u�t
	"factory":	"transportation",
]);
