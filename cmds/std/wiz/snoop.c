/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : snoop.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-4-14
 * Note   : snoop 指令
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <feature.h>
#include <daemon.h>
#include <message.h>

inherit COMMAND;

string help = @HELP
	標準 snoop 指令。
snoop 		target			開始對 target 監聽
snoop -d 	(target||all)   	取消對 target 監聽
snoop -log 	target			開始記錄
snoop -log	target	filename	將訊息記錄到 filename
snoop -logstop 	target			停止記錄

HELP;

//#define SNOOP_SHADOW "/obj/etc/snoop_shadow.c"

mapping container=([]);

void notify_snooper_catch(object snoopee,string msg)
{
	string msgs = "";
	
	if( undefinedp(container[snoopee]) )
	{
		remove_shadow(snoopee);
		return;
	}
	
	if( query_temp("snoop_log",snoopee) )
	{
		write_file(query_temp("snoop_log",snoopee),sprintf("--%s--\n%s",ctime(time())[4..18],msg));
	}
	
        foreach( string message in explode(msg,"\n") )
	if( message )
                msgs+=sprintf(WHT"[%|12s]-"NOR"%s"NOR"\n",remove_ansi(snoopee->query_id()),message);
	
	
	foreach( object snooper in container[snoopee])
	{
		tell(snooper,msgs,"snoop");
	}
	
}

void notify_snooper_cmd(object snoopee, string msg)
{
	//string msgs = "";
	
	if( undefinedp(container[snoopee]) )
		return;
	
	if( query_temp("snoop_log",snoopee) )
	{
		write_file(query_temp("snoop_log",snoopee),sprintf("%s\n",HIY+msg+NOR));
	}
	
	foreach( object snooper in container[snoopee])
	{
		//tell(snooper,HIY+remove_ansi(msg)+NOR);
		tell(snooper,sprintf(WHT"[%|12s]-"HIY"%s"NOR"\n",remove_ansi(snoopee->query_id()),remove_ansi(msg)),"snoop");
	}
	
}

void remove_user(object ob)
{
	if( call_stack(1)[3]!=load_object("/cmds/std/ppl/quit.c"))
	{
		tell(find_player("cookys"),sprintf("%O\n",call_stack(1)));
		return ;
	}
		
	//remove_shadow(ob);
	
	map_delete(container,ob);
}

private void command(object me, string arg)
{
	object target;
	string msg="";
	
	if( !is_command() ) return;

//	old_target = query_snooping(me);

	if( !arg )
	{
		if( !sizeof(container) ) return tell(me,"沒有人在監聽中！\n");
		foreach(object snoopee,mixed snoopers in container)
		{
			msg+=sprintf("%15s ：%s\n%15s",snoopee->query_idname(),query_temp("snoop_log",snoopee)?(HIR+query_temp("snoop_log",snoopee)+NOR):" ","");
			//printf("%O\n",container);
			if( sizeof(snoopers) )
			foreach(object snooper in snoopers)	
				msg+=sprintf("%12O",snooper->query_idname());
			msg+="\n";
		}
		if( !msg ) 
			tell(me,"沒有人在監聽中！\n");
		else 	tell(me,msg);
		return ;
	}
	
	
	
	if( sscanf(arg,"-d %s",arg)==1 )
	{
		if( arg=="all")
		{
			foreach(object snoopee, mixed snoopers in container)
				if( arrayp(snoopers) && member_array(me, snoopers)!=-1 )
				{
					container[snoopee] = container[snoopee]-({me});
					tell(me,"取消對"+snoopee->query_idname()+"的監聽。\n");
					if( sizeof(container[snoopee])<1 && !query_temp("snoop_log",snoopee) )
					{
						map_delete(container,snoopee);
						//remove_shadow(snoopee);
					}
				}
			return;
		}
		
		if( !(target=find_player(arg) || present(arg, environment(me)) ) )
			return tell(me,"這邊沒有 "+arg+" 這樣東西！\n");
		
		if( undefinedp(container[target]) )
			return tell(me,"你並沒有 snoop "+target->query_idname()+"。\n");
		
		if( member_array(me,container[target]) == -1 )
			return tell(me,"你並沒有 snoop "+target->query_idname()+"。\n");
			
		container[target]-=({me});
		
		if( sizeof(container[target])<1 && !query_temp("snoop_log",target) )
		{
			map_delete(container,target);
			//remove_shadow(target);
		}		
		tell(me,"取消對"+target->query_idname()+"的監聽。\n");
		
		return;
		
	}
	
	if( sscanf(arg,"-log %s",arg)==1 )
	{
		string file;
		
		
		if( sscanf(arg,"%s %s",arg,file)==2 );
		
		
		if( !(target=find_player(arg) || present(arg, environment(me)) ) )
			return tell(me,"這邊沒有 "+arg+" 這樣東西！\n");
		
		if( !file )  file="/log/command/snoop_log/"+replace_string(target->query_id(1)," ","_");
		
		set_temp("snoop_log",file,target);
	//	new(SNOOP_SHADOW,target);
		if( undefinedp(container[target]) )
			container[target]=({});
		tell(me,"開始記錄 "+target->query_idname()+"的一舉一動到[ "+file+" ]。\n");
		return;
	}
	
	if( sscanf(arg,"-logstop %s",arg)==1 )
	{
		if( !(target=find_player(arg) || present(arg, environment(me)) ) )
			return tell(me,"這邊沒有 "+arg+" 這樣東西！\n");
		
		if( undefinedp(container[target]) || !query_temp("snoop_log",target))
			return tell(me,"他本來就不在 log 中！\n");
			
		delete_temp("snoop_log",target);
		
		if( sizeof(container[target]) < 1 )
		{
			map_delete(container,target);
			//remove_shadow(target);
		}
		
		tell(me,"取消對"+target->query_idname()+"的 log 動作。\n");
		
		return;
	}
	
	if( !(target=find_player(arg) || present(arg, environment(me)) ) )
		return tell(me,"這邊沒有 "+arg+" 這樣東西！\n");
		
	if( target == me )
			return tell(me, pnoun(2, me)+"無法竊聽自己。\n");

	//new(SNOOP_SHADOW,target);
	
	if( undefinedp(container[target]) )
		container[target]=allocate(0);
	container[target]-=({0});
	container[target] += ({ me });
	
	
	tell(me, pnoun(2, me)+"開始對"+target->query_idname()+"加以竊聽。\n");
	
	//foreach(object ob in children("/obj/etc/snoop_shadow")) if( clonep(ob) && !query_shadowing(ob) ) destruct(ob);
}               

void remove()
{
	foreach(object snoopee,mixed snoopers in container)
	{
		if( sizeof(snoopers) )
			foreach(object snooper in snoopers)	
				tell(snooper,"停止對："+snoopee->query_idname()+"的監聽。\n");
		//remove_shadow(snoopee);
	}
}