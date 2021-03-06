/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : update.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-02-06
 * Note   : update 指令
 * Update :
 *  o 2002-11-05 Clode rewrite command update
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <feature.h>
#include <daemon.h>
#include <secure.h>
#include <object.h>
#include <message.h>

inherit COMMAND;

#define DEEP_INHERIT_UPDATE	1
#define BINARY_DATA		"/data/binaries/"

string help = @HELP
	標準 update 指令
HELP;

varargs private void update(object me, string *update_list, int largenum)
{
	mixed err, env;
	int reset_command, shadowing, timeexpression;
	object ob, void_ob, child, *children, *inv, childenv, inv_elem;
	array childloc;
	int childamount;
	int childkeep;
	int childequip;
	int childdecorated;
	string file;
	int status;
	object group_leader;

	void_ob = load_object(VOID_OB);
	
	if( largenum )
		CHANNEL_D->channel_broadcast("news", "系統開始進行更新，請稍候...");

	timeexpression = time_expression {
	foreach( file in update_list )
	{
		inv = allocate(0);
		children = allocate(0);
		
		reset_eval_cost();

		/*
		if( !SECURE_D->permission(file, my_id, WRITE) || !SECURE_D->permission(file, my_id, READ))
		{	
			tell(me, pnoun(2, me)+"沒有重新編譯 "+file+" 的權利。\n");
			continue;
		}
		*/
		
		if( !file_exists(file) )
		{
			tell(me, "沒有 "+file+" 這個檔案。\n");
			continue;
		}

		if( objectp(ob = find_object(file)) )
		{
			if( ob == void_ob )
			{
				tell(me, pnoun(2, me)+"沒有重新編譯 VOID_OB 的權利。\n");
				continue;
			}
			
			if( ob == this_object() )
			{
				tell(me, "請手動 reset_cmd。\n");
				tell(me, "編譯與載入 "+file+" 完成。\n");
				
				if( !query("no_update_msg", me) )
					CHANNEL_D->channel_broadcast("sys", "編譯與載入 "+file+" 完成。");

				// 重新啟動系統心跳計算
				call_out( (: set_heartbeat_interval(1) :), 5);

				destruct(this_object());
				continue;
			}
			
			if( ob->is_module_room() )
				inv = filter_array(all_inventory(ob), (: $1->is_user_ob() :));
			else
				inv = all_inventory(ob);
			
			if( ob->is_npc() )
				group_leader = ob->query_group_leader();

			inv->move(void_ob);
			
			children = children(file) - ({ ob });
			
			foreach( child in children )
			{
				if( !environment(child) || userp(child) )
				{
					children -= ({ child });
					destruct(child);
					continue;
				}
				if( child->query_shadow_ob() == ob )
					set_temp("shadowing_ob", 1, child);
			}
			
			env = environment(ob);
			
			if( objectp(env) && env->is_maproom() )
				env = query_temp("location", ob);

			destruct(ob);
			
			rm(BINARY_DATA+file[0..<3]+".b");
		}
		
		if( (err = catch( load_object(file) )) )
		{
			tell(me, sprintf( "發生錯誤﹕%s\n", err ));
			
			if( !query("no_update_msg", me) )
				CHANNEL_D->channel_broadcast("sys", "編譯 "+file+" 發生錯誤。");
			
			continue;
		}

		if( !largenum )
		{
			tell(me, "編譯與載入 "+file+" 完成。\n");
			
			if( !query("no_update_msg", me) )
				CHANNEL_D->channel_broadcast("sys", "編譯與載入 "+file+" 完成。");
		}
		
		if( file[0..5] == "/cmds/" ) reset_command = 1;
		
		ob = find_object(file);
		
		inv->move(ob);
		
		foreach(inv_elem in inv)
			if( inv_elem && inv_elem->is_equipping() )
				ob->equip(inv_elem, ref status);
			
		tell(inv, NOR GRN"["HIG"更新"NOR GRN"]"NOR+"這裡的房間環境已更新至最新的版本。\n");

		foreach( child in children )
		{
			shadowing = query_temp("shadowing_ob", child);
			childenv = environment(child);
			childloc = query_temp("location", child);
			childamount = child->query_amount();
			childkeep = child->is_keeping();
			childequip = child->is_equipping();
			childdecorated = query_temp("decorated", child);

			if( childequip )
				childenv->unequip(child, ref status);

			//if( userp(childenv) )
				//tell(environment(child), NOR GRN"["HIG"更新"NOR GRN"]"NOR+pnoun(2, childenv)+"身上的"+child->query_idname()+"已更新至最新的版本。\n");
			
			// 若是地圖物件, 則由地圖物件管理系統去重生
			if( query_temp("mapnpc", child) )
			{
				destruct(child);
				continue;
			}
			else
			{
				destruct(child);
			
				child = clone_object(file);
				
				if( childamount )
					child->set_amount(childamount);
	
				if( childkeep )
					child->set_keep(childkeep);
	
				if( shadowing )
					set("shadow_ob", ob, child);
	
				if( childequip )
					childenv->equip(child, ref status);
	
				if( childdecorated )
					set_temp("decorated", 1, child);

				if( childloc )
					child->move(childloc);
				else
					child->move(childenv);
			}
		}
		
		if( objectp(group_leader) )
			group_leader->add_group_member(ob);

		ob->move(env);
		
		log_file("command/update", file, 3);
	}};
	
	if( largenum )
	{
		tell(me, "編譯與載入 "+sizeof(update_list)+" 檔完成。\n");
		CHANNEL_D->channel_broadcast("sys", "編譯與載入 "+sizeof(update_list)+" 檔完成，花費 "+sprintf("%2.5f", timeexpression/1000000.)+" 秒。");
		CHANNEL_D->channel_broadcast("news", "更新完畢，總共更新 "+sizeof(update_list)+" 項程式，花費 "+sprintf("%2.5f", timeexpression/1000000.)+" 秒。");
	}

	set("current_work/file", file, me);
	
	if( reset_command )
		COMMAND_D->do_reset();
	
	me->remove_status(query_temp("last_time_update_status", me));
	me->add_status(set_temp("last_time_update_status", HIW"更"WHT"新"NOR, me), 30);
}

private void command(object me, string arg)
{
	int option, time;
	string *parse, *update_list, arg2;
	object ob;
	
	if( !is_command() ) return;
	
	if( !arg ) arg = "";
	
	parse = single_quote_parse(arg);
	
	switch( sizeof(parse) )
	{
		case 2:
			arg = parse[0];
			arg2 = parse[1];
			if( parse[1][0] != '-' ) break;
			
			foreach( int a in parse[1][1..] )
			{
				switch(a)
				{
					case 'd':	option |= DEEP_INHERIT_UPDATE;	break;
					default: break;
				}
			}
			break;
		case 0:
			arg = query("current_work/file", me);
			break;
		default:
			arg = parse[0];
			break;
	}

	if( !arg ) arg = query("current_work/file", me);
	if( !arg ) return tell(me, "請輸入欲重新編譯或載入之檔案或物件名稱。\n");

	// 暴力更新所有 module_room
	if( arg == "all" )
	{
		return tell(me, "這個指令會導致 crash，暫停使用。\n");

		update_list = allocate(0);

		if( arg2 == "module_room-master" ) 
		{
			// 關閉系統心跳計算
			set_heartbeat_interval(0);
			foreach( ob in objects((: $1->is_module_room() && query("function", $1)=="front" :)) )
			{
				reset_eval_cost();
				update_list |= ({ base_name(ob) });
			}
			// 重新啟動系統心跳計算
			call_out( (: set_heartbeat_interval(1) :), 5);
		}
		else if( sscanf(arg2, "module_room-%s", arg) == 1 )
		{
			// 關閉系統心跳計算
			set_heartbeat_interval(0);
			foreach( ob in objects((: $1->is_module_room() && sscanf(base_name($1), "/city/"+$(arg)+"/%*s") :)) )
			{
				reset_eval_cost();
				update_list |= ({ base_name(ob) });
			}
			// 重新啟動系統心跳計算
			call_out( (: set_heartbeat_interval(1) :), 5);
		}
		else 
		{
			// 關閉系統心跳計算
			set_heartbeat_interval(0);
			switch( arg2 )
			{
				// 更新所有標準繼承樹
				case "standard":
					foreach( ob in objects((: sscanf(base_name($1), "/std/inherit/standard/%*s") :)) )
					{
						reset_eval_cost();
						update_list |= deep_inherit_list(ob);
						update_list |= ({ base_name(ob) });
					}
					break;
				case "module_room":
					foreach( ob in objects((: $1->is_module_room() :)) )
					{
						reset_eval_cost();
						update_list |= ({ base_name(ob) });
					}
					break;
				case "module_object":
					foreach( ob in objects((: $1->is_module_object() :)) )
					{
						reset_eval_cost();
						update_list |= ({ base_name(ob) });
					}
					break;
				case "module_npc":
					foreach( ob in objects((: $1->is_module_npc() :)) )
					{
						reset_eval_cost();
						update_list |= ({ base_name(ob) });
					}
					break;
				case "standard_module":
					foreach( ob in objects((: sscanf(base_name($1), "/std/module/%*s") :)) )
					{
						reset_eval_cost();
						update_list |= deep_inherit_list(ob);
						update_list |= ({ base_name(ob) });
					}
					break;			
			}
			// 重新啟動系統心跳計算
			call_out( (: set_heartbeat_interval(1) :), 5);
		}
		if( sizeof(update_list) )
		{
			CHANNEL_D->channel_broadcast("sys", "一分鐘後開始進行全面更新 "+arg2+" 物件，共 "+NUMBER_D->number_symbol(sizeof(update_list))+" 個物件。");
			CHANNEL_D->channel_broadcast("news", "一分鐘後系統準備進行大規模更新，預備更新 "+NUMBER_D->number_symbol(sizeof(update_list))+" 個物件，更新過程中將會停頓數秒鐘至數十分鐘。");
			call_out((: CHANNEL_D->channel_broadcast("news", "開始進行系統更新...") :), 59);
			call_out((:update, me, update_list, 1:), 60);
			return;
		}
		else
			return tell(me, "目前並未載入任何符合項目的物件。\n");
	}

	// 更新角色身體
	if( objectp(ob = find_player(arg)) || arg == "-me" || arg == "-alluser")
	{
		object *ob_arr;
		
		if( ob ) ob_arr = ({ ob });
		else if( arg == "-me" ) ob_arr = ({ me });
		else if( arg == "-alluser" ) ob_arr = filter_array(users(), (: $1->is_user_ob() && environment($1) :));

		foreach( object update_ob in ob_arr )
		{
			msg("一陣"NOR WHT"黑影"NOR"突然掃過$ME的身體.....。\n", update_ob, 0, 1);
			call_out( (: call_other, LOGIN_D, "update_player", update_ob :), 1 );
		}
		return;
	}

	set_heartbeat_interval(0);
	
	arg = resolve_path(me, arg);
	
	intact_file_name(ref arg);

	switch(file_size(arg))
	{
		case -1:
			arg = arg[strsrch(arg, "/", -1)+1..];
			if( objectp(ob = find_object(arg) || present(arg, me) || present(arg, environment(me))) )
			{
				string basename = base_name(ob);
				
				intact_file_name(ref basename);
				
				if( file_exists(basename) )
				{
					if( basename[<2..<1] == ".o" )
						basename = basename[0..<3];

					update_list = ({ basename });
				}
			}
			break;
		case -2:
			add_slash(ref arg);
			update_list = allocate(0);
			
			foreach( string file in get_dir(arg) )
				if( file_size(arg+file) > 0 )
				{
					if( file[<2..<1] == ".o" )
						update_list += ({ arg+file[0..<3] });
					else
						update_list += ({ arg+file });
				}
			break;
		default:
		
			if( arg[<2..<1] == ".o" )
				arg = arg[0..<3];
				
			if( option & DEEP_INHERIT_UPDATE )
				ob = find_object(arg);

			update_list = ({ arg });
	}

	if( objectp(ob) && option & DEEP_INHERIT_UPDATE )
	{
		update_list += deep_inherit_list(ob);

		reverse_array(ref update_list);
	}

	if( !arrayp(update_list) )
	{
		call_out( (: set_heartbeat_interval(1) :), 0);
		return tell(me, "沒有這個檔案或是物件。\n");
	}

	time = time_expression 
	{
		evaluate((:update, me, update_list:));
	};

	if( time > 1000000 )
		CHANNEL_D->channel_broadcast("news", sprintf("程式更新導致遊戲停頓(>1 秒)，共花費 %.2f 秒。", time/1000000. ));
		
	// 重新啟動系統心跳計算
	call_out( (: set_heartbeat_interval(1) :), 0);
}
