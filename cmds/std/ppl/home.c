/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : home.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-3-27
 * Note   : home 指令
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <daemon.h>
#include <feature.h>
#include <message.h>

inherit COMMAND;

string help = @HELP
　　回家的指令，必須先建造住宅建築並在客廳進行 sethome 的設定動作。
HELP;

#define HOME_STAMINA		500

private void do_command(object me, string arg)
{
	string file;
	object home;

	if( wizardp(me) )
	{
		if( arg )
			file = wiz_home(arg)+"/workroom";
		else
			file = query("home", me) || (wiz_home(me->query_id(1))+"/workroom");

		//intact_file_name(ref file);
		
		if( !file_exists(file) )
			return tell(me, "這個人沒有工作室。\n");
	
		home = load_object(file);
		
		if( environment(me) == home ) 
			return tell(me, pnoun(2, me)+"已經在"+(arg?" "+capitalize(arg)+" ":"自己")+"的工作室了。\n");
			
		msg("$ME化作一陣"HIG"綠"NOR GRN"光"NOR"往"+(arg?" "+capitalize(arg)+" ":"自己")+"的工作室飛了過去。\n",me, 0, 1, ENVMSG);
	
		me->move(home);
	
		msg("$ME化作一陣"HIG"綠"NOR GRN"光"NOR"往"+(arg?" "+capitalize(arg)+" ":"自己")+"的工作室飛了過來。\n",me, 0, 1, ENVMSG, ({me}));
	}
	else
	{
		if( !me->valid_move() ) 
			return;

		file = query("home", me);
		
		if( !stringp(file) || !file_exists(file) || !objectp(home = load_object(file)) || query("owner", home) != me->query_id(1) || query("function", home) != "livingroom")
		{
			delete("home", me);
			return tell(me, pnoun(2, me)+"尚未設定"+pnoun(2, me)+"家的位址(必須建造住宅)。\n");
		}

		if( environment(me) == home ) 
			return tell(me, pnoun(2, me)+"已經在家裡了。\n");

		if( BATTLEFIELD_D->in_battle(me) )
			return tell(me, pnoun(2, me)+"正在戰場中，若確定要離開戰場請使用 battle cancel 指令。\n");

		if( !me->cost_stamina(HOME_STAMINA) )
			return tell(me, pnoun(2, me)+"沒有足夠的體力回到家裡。\n");

		msg("$ME歸心似箭，耗費 "+HOME_STAMINA+" 點的體力馬不停蹄地前往「"+query("short", home)+"」。\n",me, 0, 1, ENVMSG);
	
		me->move(home);
	
		msg("$ME歸心似箭，耗費 "+HOME_STAMINA+" 點的體力馬不停蹄地回到「"+query("short", home)+"」。\n",me, 0, 1, ENVMSG, ({me}));
	}
}