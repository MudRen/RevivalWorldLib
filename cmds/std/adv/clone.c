/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : clone.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-4-11
 * Note   : clone 指令
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <daemon.h>
#include <ansi.h>
#include <feature.h>
#include <message.h>

inherit COMMAND;

string help = @HELP
指令格式：clone <物件|檔案>

複製一個物件或檔案。
HELP;

private void command(object me, string arg)
{
	int amount;
	string *parse;
	object obj;
	
	if( !is_command() ) return;
	
	if( !arg )
		return tell(me, "指令格式：clone <物件|檔案>\n");
		
	parse = single_quote_parse(arg);
	
	if( sizeof(parse) > 1 )
	{
		if( to_float(parse[0]) == 0. )
			arg = parse[0];
		else
		{
			amount = to_int(parse[0]);
			arg = parse[1];
			
			if( amount < 1 )
				amount = 1;
		}
	}
	else
		arg = parse[0];
	
	if( objectp(obj = present(arg)) )
	{	
		arg = base_name(obj);
		obj = new(arg);
	}
	else
	{
		arg = resolve_path(me, arg);
		intact_file_name(ref arg);
	}

	if( arg[<2..<1] == ".o" ) arg = arg[0..<3];

	if( !objectp(obj = clone_object(arg)) )
		return tell(me, "無法複製物件 "+arg+"。\n");

	if( environment(me) ) 
	{
		if( amount && !obj->is_living() )
		{
			if( !obj->no_amount() )
				obj->set_amount(amount);
		}
		else amount = 1;
		
		msg("$ME雙手平伸，比了幾下手勢後，突然在一陣"HIC"藍"NOR CYN"光"NOR"下出現了 "+NUMBER_D->number_symbol(amount)+" "+(query("unit", obj)||"個")+("$YOU"||file_name(obj))+"。\n", me,obj, 1, ENVMSG);

		if( obj->is_living() )
			obj->move_to_environment(me);
		else if( !me->available_get(obj, amount) )
		{
			tell(me, pnoun(2, me)+"拿不動那麼多東西，只好先放在地上。\n");
			obj->move_to_environment(me);
		}
		else
			obj->move(me, amount);

		return;
	}
	else
	{
		destruct(obj);
		tell(me, pnoun(2, me)+"並不身處於任何環境，請通知巫師處理。\n");
	}
}