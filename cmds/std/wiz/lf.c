/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : lf.c
 * Author : Clode@RevivalWorld
 * Date   : 2004-04-22
 * Note   : lf 指令
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <feature.h>
#include <message.h>

inherit COMMAND;

string help = @HELP
        標準 lf 指令。
HELP;

#define NAME		0
#define ARGS_NUM	1
#define RETURN_TYPE	2
#define ARGS_TYPE	3..

private void command(object me, string arg)
{
	array funcs;
	string msg;
	object ob;

	if( !is_command() ) return;

	if( !arg )
		return tell(me, "請輸入欲查詢函式列表的檔案名稱或物件名稱。\n", CMDMSG);
		
	if( !objectp(ob = find_player(arg) || present(arg, me) || present(arg, environment(me))) )
		arg = resolve_path(me, arg);
	
	if( !objectp(ob) && !objectp(ob = load_object(arg)) )
		return tell(me, "沒有 "+arg+" 這個物件或檔案。\n", CMDMSG);
	
	if( !sizeof(funcs = functions(ob, 1)) )
		return tell(me, (ob->query_idname() || ob->short() || base_name(ob))+"物件上沒有任何的函式。\n", CMDMSG);

	msg = (ob->query_idname() || ob->short() || base_name(ob))+"物件的函式列表如下：\n";
	
	funcs = sort_array(funcs, (: strcmp($1[NAME], $2[NAME]) :));
	foreach( array func in funcs )
	{
		msg += function_exists(func[NAME], ob) ? "" : NOR WHT"private "NOR;

		msg += func[RETURN_TYPE]+HIY+func[NAME]+NOR YEL"("+implode(map(func[ARGS_TYPE],(:$1[0..<2]:)),", ")+");\n"NOR;
	}
	
	me->more(msg+"\n");
}
