/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : recall.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-2-2
 * Note   : recall 指令
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <daemon.h>
#include <feature.h>
#include <object.h>
#include <message.h>

inherit COMMAND;

string help = @HELP
recall		- 這個指令可以讓您在迷路的時候快速地回到巫師大廳

HELP;

#define WIZ_HALL_UP	"/wiz/wizhall/room_wizhall_7"

private void do_command(object me, string arg)
{
	object recallroom;
	object env = environment(me);
	object *inv;

	if( BATTLEFIELD_D->in_battle(me) )
		return tell(me, pnoun(2, me)+"正在戰場中，若確定要離開戰場請使用 battle cancel 指令。\n");
		
	if( wizardp(me) )
		recallroom = load_object(WIZ_HALL_UP);
	else if( query("total_online_time", me) > 60*60*24*30 )
		return tell(me, pnoun(2, me)+"的累計上線時間已經超過 30 天，無法再使用快速 recall 的服務，請步行或善用城市中的各種交通設施。\n");
	else
		recallroom = load_object(WIZ_HALL);

	inv = all_inventory(recallroom);

	if( env && env == recallroom ) 
		return tell(me, pnoun(2, me)+"已經在"+query("short", env)+"了。\n");

	msg("$ME舉起右手在面前極速畫了一個圓，竟劃破了面前的空間，只見圓內的空間真空將$ME給吸了進去。\n", me, 0, 1);

	me->move(recallroom);

	inv = filter_array(inv, (: userp($1) :));

	if( sizeof(inv) )
		msg("$ME從"+query("short", recallroom)+"的角落空間裂縫中鑽出，嚇了$YOU一跳。\n", me, inv[random(sizeof(inv))], 1);
	else
		msg("$ME從"+query("short", recallroom)+"的角落空間裂縫中鑽出。\n", me, 0, 1);
}