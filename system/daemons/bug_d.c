/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : bug_d.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-07-21
 * Note   : 臭蟲回報精靈
 * Update :
 *  o 2003-00-00
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <daemon.h>
#include <message.h>
#include <mudlib.h>

#define DATA_PATH		"/data/bug/"
#define DEFAULT_LOAD		10
#define SERIAL_NUMBER_KEY	"bug"

#define REPLY_AUTHOR		0
#define REPLY_MESSAGE		1

#define LIST_OPT_ALL	(1<<0)

array buginfo;

int bug_exists(string number)
{
	return file_exists(DATA_PATH+number);
}

void reset_buginfo()
{
	buginfo = allocate(0);
	
	foreach(string file in sort_array(get_dir(DATA_PATH), (: count($1, "<", $2) ? 1 : -1 :))[0..DEFAULT_LOAD-1])
		buginfo += ({ restore_variable(read_file(DATA_PATH+file)) });
}

// 列出 Bug 列表
string list_bug(int options)
{
	string *listmsg;
	array list_buginfo;
	
	if( options & LIST_OPT_ALL )
	{
		list_buginfo = allocate(0);
		
		foreach(string file in get_dir(DATA_PATH))
			list_buginfo += ({ restore_variable(read_file(DATA_PATH+file)) });
	}
	else
		list_buginfo = buginfo;

	list_buginfo = sort_array(list_buginfo, (: count($1["number"], "<", $2["number"]) ? 1 : -1 :));
	listmsg = ({ "\n"+MUD_FULL_NAME+HIY" 臭蟲回報系統\n"NOR });
	listmsg += ({ WHT"─────────────────────────────────────\n"NOR });
	listmsg += ({ "編號 回報者                   主題                          處理狀態  回應\n" });
	listmsg += ({ WHT"─────────────────────────────────────\n"NOR });
	
	foreach( mapping data in list_buginfo )
	{		
		listmsg += ({ sprintf(HIY"%-4s"NOR" %-24s %-:30s%|8s %5s\n", data["number"], data["author_idname"], data["title"], data["status"], sizeof(data["reply"]) ? sizeof(data["reply"])+"" : "") });	
	}
	listmsg += ({ WHT"─────────────────────────────────────\n"NOR });
	
	if( options & LIST_OPT_ALL )
		listmsg += ({ "列出所有回報資料\n" });
	else
		listmsg += ({ "列出最近 "+DEFAULT_LOAD+" 項回報資料\n" });	
	
	return implode(listmsg, "");
}

// 新增 Bug 資料
string add_bug(object me, string title, string content)
{
	mapping bugdata = allocate_mapping(0);
	
	if( !objectp(me) || !stringp(title) || !stringp(content) )
		error("BUG_D add_bug() 錯誤輸入參數");
	
	bugdata["time"] = time();
	bugdata["number"] = SERIAL_NUMBER_D->query_serial_number(SERIAL_NUMBER_KEY);
	
	bugdata["author_id"] = me->query_id(1);
	bugdata["author_idname"] = me->query_idname();
	
	bugdata["title"] = title;
	bugdata["content"] = content;
	
	bugdata["status"] = HIR"未處理"NOR;

	bugdata["reply"] = allocate(0);
			
	if( write_file(DATA_PATH+bugdata["number"], save_variable(bugdata)) )
		reset_buginfo();
	else
		error("BUG_D 無法儲存新增 bug 資料");
		
	CHANNEL_D->channel_broadcast("sys", me->query_idname()+"新增 Bug 回報，「"+title+NOR"」，編號："+bugdata["number"]+"。");
	
	return bugdata["number"];
}

// 回應 Bug 處理情形
varargs void reply_bug(object me, string number, string status, string message)
{
	mapping data;

	if( !bug_exists(number) )
		error("BUG_D reply_bug() 無此檔案");
	else
		data = restore_variable(read_file(DATA_PATH+number));
	
	data["status"] = status;
	CHANNEL_D->channel_broadcast("sys", me->query_idname()+"修改編號第 "+number+" 號 Bug 處理狀況為「"+status+"」");
	
	if( find_player(data["author_id"]) )
		tell(find_player(data["author_id"]), me->query_idname()+"回應編號第 "+number+" 號 Bug 處理狀況為「"+status+"」。\n");

	if( !undefinedp(message) )
	{
		if( !arrayp(data["reply"]) )
			data["reply"] = allocate(0);

		data["reply"] += ({ ({ me->query_idname(), message }) });
		CHANNEL_D->channel_broadcast("sys", me->query_idname()+"輸入編號第 "+number+" 號 Bug 處理回應");
	}
	
	write_file(DATA_PATH+number, save_variable(data), 1);
	reset_buginfo();
}

// 移除 Bug 資料
varargs void remove_bug(object me, string number, int reply)
{
	if( !bug_exists(number) )
		error("BUG_D remove_bug() 無此檔案");

	// 刪除整個 Bug 資訊
	if( undefinedp(reply) )
	{
		if( !rm(DATA_PATH+number) )
			error("BUG_D remove_bug() 無法刪除檔案");

		reset_buginfo();
	}
	// 刪除其中一個回應資料
	else
	{
		mapping data = restore_variable(read_file(DATA_PATH+number));

		reply--;
		
		if( reply < sizeof(data["reply"]) && reply >= 0 )
		{
			if( reply == 0 )
				data["reply"] = data["reply"][1..];
			else
				data["reply"] = data["reply"][0..reply-1] + data["reply"][reply+1..];
		}
		
		write_file(DATA_PATH+number, save_variable(data), 1);
		reset_buginfo();
	}
}

// 查詢 Bug 資料
string query_bug(string number, object me)
{
	string bugmsg;
	mapping data;
	
	if( !bug_exists(number) )
		error("BUG_D query_bug() 無此檔案");
	else
		data = restore_variable(read_file(DATA_PATH+number));
		
	bugmsg =  sprintf(HIM"編號"NOR" %-20s "HIM"主題"NOR" %s\n"NOR, data["number"], data["title"]);
	bugmsg += sprintf(HIM"時間"NOR" %-20s "HIM"回報"NOR" %s\n", TIME_D->replace_ctime(data["time"]), data["author_idname"]);
	bugmsg += sprintf(HIM"狀況"NOR" %-20s "HIM"回應"NOR" %d\n", data["status"], sizeof(data["reply"]));
	bugmsg += WHT"─────────────────────────────────────\n"NOR;
	
	if( wizardp(me) || me->query_id(1) == data["author_id"] )
		bugmsg += data["content"]+"\n";
	else
		bugmsg += WHT"("+pnoun(2, me)+"的權限無法閱讀詳細回報內容)\n"NOR;
	
	if( sizeof(data["reply"]) )
	{
		int replies;
		
		foreach( array reply in data["reply"] )
		{
			replies++;
			bugmsg += NOR YEL"\n***"HIY" 第 "+replies+" 篇回應："+reply[REPLY_AUTHOR]+NOR YEL" ***\n"NOR;
			bugmsg += reply[REPLY_MESSAGE]+NOR"\n";
		}
	}
	
	bugmsg += WHT"─────────────────────────────────────\n"NOR;
	
	return bugmsg;
}

void create()
{
	reset_buginfo();
}
string query_name()
{
	return "臭蟲回報系統(BUG_D)";
}
