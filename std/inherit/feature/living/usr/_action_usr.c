/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : _action_usr.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-8-20
 * Note   : RW action 處理方式:
 *	    1. 環境物件與環境物件的內容物上所包含的 action, 皆在使用者下達指令時即時搜尋
 *	    2. 使用者物件的內容物上所包含的 action, 則是在內容物進入或離開使用者物件時, 
 *　　　　　　 進行 add_action 與 remove_action 的動作.
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <map.h>
#include <daemon.h>
#include <function.h>
#include <message.h>
#include <ansi.h>

private nosave mapping actions = allocate_mapping(0);

mapping query_actions()
{
	return copy(actions);
}

/*
  使用於當物件進入 this_object() 時, 加入此物件之所有 actions
  或是直接加入該物件的某個 action
*/
varargs nomask void add_action(object ob, mapping commands)
{
	mapping action;
	
	if( !objectp(ob) ) return;
	
	action = commands || ob->query_actions();
	
	if( !mapp(action) ) return;
		
	foreach( string verb, function fp in action )
	{
		if( !mapp(actions[verb]) )
			actions[verb] = ([ ob : fp ]);
		else 
			actions[verb] += ([ ob : fp ]);
	}
}

/* 
  使用於當物件離開 this_object() 時, 刪除此物件之所有 actions 
  或是直接移除該物件的某個 action
*/
varargs nomask void remove_action(object ob, mapping commands)
{
	mapping action;
	
	if( !objectp(ob) ) return;
	
	action = commands || ob->query_actions();
	
	if( !mapp(action) ) return;
	
	foreach( string verb, function fp in action )
	{
		if( !mapp(actions[verb]) ) continue;
		
		map_delete(actions[verb], ob);
		
		if( !sizeof(actions[verb]) ) map_delete(actions, verb);
	}
}
	
private nomask void do_multi_actions(string verb, string arg, mapping fit_actions, object *action_obs, string input)
{
	int i = to_int(input);
	
	if( i < 1 ) 
		i = 1;
	
	if( i > sizeof(fit_actions) )
		i = sizeof(fit_actions);
	
	i--;

	if( functionp(fit_actions[action_obs[i]]) & FP_OWNER_DESTED )
		return tell(this_object(), verb+" 指令函式指標擁有者已被摧毀，請通知巫師處理。\n"); 

	evaluate(fit_actions[action_obs[i]], this_object(), arg, action_obs[i]);
	
	this_object()->finish_input();
}

private nomask void do_action(mapping fit_actions, string verb, string arg)
{
	if( sizeof(fit_actions) == 1 )
	{
		function verb_fp = values(fit_actions)[0];
			
		if( functionp(verb_fp) & FP_OWNER_DESTED )
			return tell(this_object(), verb+" 之指令函式指標擁有者已被摧毀，請通知巫師處理。\n"); 

		evaluate(verb_fp, this_object(), arg, keys(fit_actions)[0]);
	}
	else
	{
		int i;
		object *action_obs = allocate(0);
		string msg = "此指令共 "+sizeof(fit_actions)+" 種選擇，請選擇"+pnoun(2, this_object())+"要執行 "+verb+" 指令的目標：\n";
			
		foreach( object ob , function func in fit_actions )
		{
			msg += HIY+(++i)+". "NOR+(ob->short(1)||query("short", ob)||file_name(ob))+" "+(objectp(environment(ob)) && userp(environment(ob))?HIC"身"NOR CYN"上"NOR : HIY"地"NOR YEL"上"NOR)+"\n";
			action_obs += ({ ob });
		}

		msg += "請輸入號碼：";
			
		tell(this_object(), msg);

		input_to( (: do_multi_actions, verb, arg, fit_actions, action_obs :) );
	}
}
