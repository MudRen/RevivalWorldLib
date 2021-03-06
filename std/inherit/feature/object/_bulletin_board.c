/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : _bulletin_board.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-08-16
 * Note   : 留言板功能繼承檔
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <daemon.h>

#define BOARD_DATA_PATH		"/data/board/"

array summary;

string query_board_id(object ob)
{
	object env = environment(ob);

	string board_id;
	
	if( !query_temp("decorated", ob) )
		return 0;
	
	board_id = query("board_id", ob);

	if( !stringp(board_id) )
	{
		// 住宅中的個人留言板
		if( env->query_room_function() == "livingroom" && user_exists(query("owner", env)) )
			board_id = "player_"+query("owner", env);
		// 城市留言板
		else if( stringp(env->query_city()) )
			board_id = "city_"+env->query_city();
	}

	return board_id;	
}

// 取得存檔路徑
string query_path(object ob)
{
	string path = BOARD_DATA_PATH + query_board_id(ob) + "/";
	
	// 若沒有留言板目錄則建立
	if( file_size(path) != -2 ) mkdir(path);
		
	return path;	
}

// 重新整理摘要
varargs array check_summary(object ob)
{
	string author_name;
	string author_id;
	string title;
	string file;
	string *files;
	string path;

	if( !query_board_id(ob) )
		return 0;

	path = query_path(ob);
	files = get_dir(path);

	if( arrayp(summary) && sizeof(summary) && sizeof(summary)/3 == sizeof(files) && summary[<3] == to_int(files[<1]) )
		return summary;

	summary = allocate(0);

	// 讀取編排所有留言資料
	foreach( file in files )
	{
		sscanf( read_file(path+file,1,1), "%s(%s)|%s\n", author_name, author_id, title );

		summary += ({ to_int(file), author_name+"("+author_id+")", title });
	}
	
	return summary;
}

void done_post(object me, int time, string title, object ob, string arg)
{
	// 簽名檔
	string sig = query("signature", me);
	string content;

	content = me->query_idname()+"|"+title+"\n"+arg+(sig ? "\n\n"NOR WHT"──\n"NOR+sig+NOR : NOR);

	if( write_file( query_path(ob)+time, content ) )
	{
		tell(me, "留言完畢。\n");
		
		if( !wizardp(me) )
			set_temp("lastpost/time", time(), me);

		me->finish_input();
	}
	else
		return tell(me, "留言失敗，請通知巫師處理。\n");
}

void do_post(object me, string arg, object ob)
{
	int time = time();
	
	check_summary(ob);

	if( !query_board_id(ob) )
		return tell(me, "現在無法發表文章。\n");

	if(!arg)
		return tell(me, "請為您的文章指定一個標題。\n");

	if( noansi_strlen(arg) > 26 )
		return tell(me, "你的標題太長了，請縮短你的標題。\n");

	if( time() - query_temp("lastpost/time", me) < 20 )
		return tell(me, "抱歉目前發文時間必須間隔 20 秒。\n");

	while( member_array(time, summary) != -1 ) time++;

	summary += ({ time, me->query_idname(), arg });

	me->edit( (: done_post, me, time, arg, ob :) );
}

void do_discard(object me, string arg, object ob)
{
	int num, file;
	string board_id = query_board_id(ob);

	check_summary(ob);

	if( !board_id )
		return tell(me, "現在無法刪除文章。\n");

	if( !arg || !sscanf(arg, "%d", num) )
		return tell(me, "請輸入預刪除之文章編號。\n");

	num = (num-1)*3;

	if( num < 0 || num >= sizeof(summary) )
		return tell(me, "沒有這張留言。\n");

	file = summary[num];

	if( me->query_idname() != summary[num+1] )
	{
		string author_id;
		string my_id = me->query_id(1);
		sscanf( remove_ansi(summary[num+1]), "%*s(%s)", author_id );
		author_id = lower_case(author_id);

		if( 
			my_id == author_id || 
			(wizardp(me) && SECURE_D->level_num(my_id) >= SECURE_D->level_num(author_id)) ||
			(board_id[0..5] == "player" && my_id == board_id[7..]) ||
			(board_id[0..3] == "city" && CITY_D->is_mayor(board_id[5..], me))
		) ;
		else
			return tell(me, "這張留言不是"+ pnoun(2, me) +"寫的。\n");
	}

	if( rm(query_path(ob)+(file<1000000000?"0":"")+file) )
	{
		summary = summary[0..num-1] + summary[num+3..];

		tell(me, "刪除完畢。\n");
	}
	else tell(me, "刪除失敗，請通知巫師處理。\n");
}

void do_read(object me, string arg, object ob)
{
	string msg;
	int num, last_read, file;
	
	check_summary(ob);

	if( !query_board_id(ob) )
		return tell(me, "現在無法閱讀文章。\n");

	last_read = query("board_last_read/"+query_board_id(ob), me);

	if( !arg || arg == "new" || arg == "next" )
	{
		if( last_read )
		{
			int* unreads = filter_array(summary, (: intp($1) && $(last_read) < $1 :));
			
			if( sizeof(unreads) )
				num = member_array(unreads[0], summary);
			else
				num = sizeof(summary);
		}
		else
			num = 0;
	}
	else if( sscanf(arg, "%d", num) )
		num = (num-1) * 3;
	else
		return tell(me, "你要讀第幾張留言﹖\n");

	if( num < 0 || num > sizeof(summary) )
		return tell(me, "沒有這張留言。\n");

	else if( num == sizeof(summary) )
		return tell(me, "沒有新的留言。\n");

	file = summary[num];

	msg = sprintf("[1;46m篇 數[m %-27s[1;46m作 者[m %s\n[1;46m主 題[m %-27s[1;46m時 間[m %s[m\n───────────────[1;30m[[1;37mContent[m[1;30m][m────────────────\n%s\n%s",
	    "第"+CHINESE_D->chinese_number(num/3+1)+"篇",
	    summary[num+1],
	    summary[num+2],
	    TIME_D->replace_ctime(file),
	    read_file(query_path(ob)+(file<1000000000?"0":"")+file, 2),
	    "────────────────[1;30m[[1;37mEnd[m[1;30m][m─────────────────\n\n"
	);

	me->more( msg );

	if( file >= last_read ) 
		set("board_last_read/"+query_board_id(ob), file, me);
}

void do_followup(object me, string arg, object ob)
{
	int num, file, time = time();
	string msg="";
	string title;
	
	check_summary(ob);

	if( !query_board_id(ob) )
		return tell(me, "現在無法回覆文章。\n");

	if( !arg )
		return tell(me, "請輸入欲回覆的文章編號或 last 回覆最後一篇文章。\n");

	if( arg == "last" ) 
		num = sizeof(summary)-3;
	else if( sscanf(arg, "%d", num) )
		num = (num-1)*3;	
	else
		return tell(me, "請輸入欲回覆的文章編號。\n");
		
	if( num < 0 || num >= sizeof(summary) )
		return tell(me, "沒有這張留言。\n");

	file = summary[num];

	foreach( string word in explode( read_file(query_path(ob)+(file<1000000000?"0":"")+file, 2), "\n" ) )
	{
		if( word == "" || word[0..7]=="[1;30m>" ) continue;
		else if( word == NOR WHT"──" ) break;
		else if( word[0..5] == "[33m>" ) msg += "[1;30m> "+word[5..]+"\n";
		else if( word[0..5] == "[36m>" ) msg += "[33m> "+word[5..]+"\n";
		else msg += "[36m> "+word+"\n";
	}

	if( msg[0..5] == "[36m>" ) 
		msg = "[36m> ◎"+summary[num+1]+" 在 "+TIME_D->replace_ctime(file)+" 留下這篇留言﹕\n" + msg;

	msg += "\n◎"+me->query_idname()+" 在 "+TIME_D->replace_ctime(time)+" 留下這篇留言﹕\n";

	while( member_array(time, summary) != -1 ) time++;

	title = (summary[num+2][0..5] != "回覆：" ? "回覆：" : "") + summary[num+2];

	summary += ({ time, me->query_idname(), title });

	me->edit( (: done_post, me, time, title, ob :), msg );
}

void do_mail(object me, string arg, object ob)
{
	int file, num;
	string msg, email;
	
	check_summary(ob);

	if( !query_board_id(ob) )
		return tell(me, "現在無法郵寄文章。\n");

	if( !arg || !sscanf(arg, "%d", num) )
		return tell(me, "指令格式: mail <文章編號>\n");

	num = (num-1)*3;

	if( num < 0 || num >= sizeof(summary) )
		return tell(me, "沒有這張留言。\n");

	if( !stringp(email = query("email", me)) )
		return tell(me, pnoun(2, me)+"沒有設定電子郵件。\n");

	if( sscanf(email, "%*s@%*s.%*s") != 3 )
		return tell(me, pnoun(2, me)+"所設定的電子郵件信箱格式是不正確的。\n");

	file = summary[num];

	msg = "以下訊息轉載自 重生的世界(Revival World)\n\n";
	msg += sprintf("[1;46m篇 數[m %-27s[1;46m作 者[m %s\n[1;46m主 題[m %-27s[1;46m時 間[m %s[m\n─────────────[1;30m[[1;37mContent[m[1;30m][m──────────────\n%s\n%s",
	    "第"+CHINESE_D->chinese_number(num/3+1)+"篇",
	    summary[num+1],
	    summary[num+2],
	    TIME_D->replace_ctime(file),
	    read_file(query_path(ob)+(file<1000000000?"0":"")+file, 2),
	    "──────────────[1;30m[[1;37mEnd[m[1;30m][m───────────────\n\n"
	);
	msg = remove_ansi(msg);
	SMTP_D->mail(email, msg, "重生的世界(Revival World) - 文章轉載");
	tell(me, "文章已送至"+pnoun(2, me)+"的 E-mail 信箱("+email+")。\n");
}
