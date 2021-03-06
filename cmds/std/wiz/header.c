/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : header.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-09-15
 * Note   : 建立與修改檔案標頭檔
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
可自動建立檔案標頭, 並可修改標頭內容
ex :
header filename.c //若 filename 尚無 RW 格式標頭, 則自動建立預設標頭, 作者為下此指令之人
header filename author clode // 設定 filename 作者為 clode
header filename update update_info // 自動往下遞增 update 資訊
header filename note note_info // 填寫 NOTE(註解) 欄

同時支援多行輸入
to header filename author 輸入多名作者
to header filename update 輸入多行更新資訊
to header filename note 輸入多行註解資訊
HELP;


string default_header	= @HEADER
/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : %^FILENAME%^
 * Author : %^AUTHOR%^
 * Date   : %^DATE%^
 * Note   : 
 * Update :
 *  o %^THISYEAR%^-00-00
 *
 -----------------------------------------
 */

HEADER;

private int create_default_header(object me, string filename, string ref file)
{
	string header, create_date;
	
	if( !is_command() ) return 0;
	
	create_date = TIME_D->replace_ctime(time());
	
	header = terminal_colour(default_header, 
		([
			"FILENAME"	: filename[(strsrch(filename, "/",-1)+1)..], 
			"AUTHOR"	: capitalize(me->query_id(1))+"@RevivalWorld",
			"DATE"		: create_date[6..9]+"-"+create_date[0..1]+"-"+create_date[3..4],
			"THISYEAR"	: create_date[6..9],
		]));
	
	file = header + file;
	
	return write_file(filename, file, 1);
}

private void command(object me, string arg)
{
	int line;
	string header, *parse, *efile;
	string file, filename, option, arg2;
       	
       	if( !is_command() ) return;
       	
       	if( !arg )
       		return tell(me, "請輸入想要建立標頭的檔案名稱。\n", CMDMSG);

	parse = single_quote_parse(arg);
	
	arg = parse[0];
	
	if( sizeof(parse) >= 3 )
	{
		option = parse[1];
		arg2 = implode(parse[2..], " ");
	}

	arg = resolve_path(me, arg);
	
	intact_file_name(ref arg);
	
	filename = arg;
	
	if( !file_exist(filename) )
		return tell(me, "沒有 "+filename+" 這個檔案。\n", CMDMSG);
	
	if( filename[<2..<1] == __SAVE_EXTENSION__ )
		return tell(me, "這個檔案型式不適合建立標頭。\n", CMDMSG);
		
	file = read_file(arg);
	
	// 檢查是否已經建立標頭
	if( file[0..37] != "/* This program is a part of RW mudlib" )
	{
		if( create_default_header(me, filename, ref file) )
			tell(me, header +"\n"+filename+" 預設標頭建立完畢。\n", CMDMSG);
		else
			return tell(me, pnoun(2, me)+"的權限不足以處理這個檔案。\n", CMDMSG);
	}
	else if( !option || !arg2 )
		return tell(me, filename+" 的標頭已經建立。\n", CMDMSG);
	
	if( option && arg2 )
	{
		efile = explode(file, "\n");
		
		switch( lower_case(option) )
		{
			case "author":
			{
				string author, *multi_author;
				
				multi_author = explode(arg2, "\n");
				
				for(int i=0;i<sizeof(multi_author);i++)
				{
					if( !i )
						author = remove_fringe_blanks(capitalize(multi_author[i]))+NOR"@RevivalWorld\n";
					else
						author += " *          "+remove_fringe_blanks(capitalize(multi_author[i]))+NOR"@RevivalWorld\n";
				}

				foreach( string ref f in efile )
				{
					if( f[0..10] == " * Author :" )
					{
						f = f[0..10]+" "+remove_fringe_blanks(kill_repeat_ansi(author));
						break;
					}
				}
							
				break;
			}
			case "update":
			{
				string update="", *multi_update;
				string date = TIME_D->replace_ctime(time());
				
				multi_update = explode(arg2, "\n");
				
				for(int i=0;i<sizeof(multi_update);i++)
				{
					if( !i )
						update += " *  o "+date[6..9]+"-"+date[0..1]+"-"+date[3..4]+" "+capitalize(me->query_id(1))+" "+remove_fringe_blanks(multi_update[i])+NOR+"\n";		
					else
						update += " *"+repeat_string(" ",16+strlen(me->query_id(1)))+remove_fringe_blanks(multi_update[i])+NOR+"\n";
				}
				
				for(int i=0;i<sizeof(efile);i++)
				{
					if( efile[i][0..10] == " * Update :" )
					{
						if( efile[i+1][10..15] == "-00-00" )
						{
							efile[i+1] = " "+remove_fringe_blanks(update);
							break;
						}
						
						while( efile[++i] != " *" );
						
						efile = efile[0..i-1] + ({ " "+remove_fringe_blanks(kill_repeat_ansi(update)) }) + efile[i..];
						
						break;
					}
				}
				
				break;
			}
			case "note":
			{
				string note, *multi_note;
				
				multi_note = explode(arg2, "\n");
				
				for(int i=0;i<sizeof(multi_note);i++)
				{
					if( !i )
						note = remove_fringe_blanks(multi_note[i])+NOR+"\n";
					else
						note += " *          "+remove_fringe_blanks(multi_note[i])+NOR+"\n";
				}
				
				foreach( string ref f in efile )
				{
					if( f[0..10] == " * Note   :" )
					{
						f = f[0..10]+" "+remove_fringe_blanks(kill_repeat_ansi(note));
						break;
					}
				}
				
				break;
			}
			default: return tell(me, "沒有 "+option+" 這個選項。\n", CMDMSG);
		}
		
		line = member_array(" */", efile) + 1;
		
		if( write_file(filename, implode(efile, "\n"), 1) )
		{
			CHANNEL_D->channel_broadcast("sys", me->query_idname()+"修改 "+filename+" 標頭。\n");
			tell(me, read_file(filename, 1, line) +"\n\n"+filename+" 預設標頭建立完畢。\n", CMDMSG);
		}
		else
			return tell(me, pnoun(2, me)+"的權限不足以處理這個檔案。\n", CMDMSG);
		
	}
}