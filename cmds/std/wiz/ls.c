/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : ls.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-2-5
 * Note   : ls 指令
 * Update :
 *  o 2001-10-08 Clode 新增參數 -a -l
 *
 -----------------------------------------
 */

#include <feature.h>
#include <daemon.h>
#include <secure.h>
#include <ansi.h>
#include <message.h>

#define	SHOW_ALL	1
#define SHOW_LONG	2
 
inherit COMMAND;
 
string help = @HELP
指令格式﹕ls [<路徑名>]
 
列出目錄下所有的子目錄及檔案﹐如果沒有指定目錄﹐則列出所在目錄
的內容﹐所列出的檔案中前面標示 * 號的是已經載入的物件。
 
範例:
'ls /' 會列出所有位於根目錄下的檔案及子目錄。
 
相關指令﹕cd、pwd。
HELP;


mapping color_tran =
([
HIC	:	CYN,
HIY	:	YEL,
HIR	:	RED,
WHT	:	WHT,
GRN	:	GRN,
HIG	:	GRN,
]);


private void command(object me, string arg)
{
	int i, j, width, col, len, opt;
	string arg2, dir, color, english, chinese, note, fn, show_files="", show_dir="", auth;
	array file, files;
 	mapping notes = ([]);
 
 	if( !is_command() ) return;
 
 	
 	if( arg )
 	{
 		sscanf(arg, "%s %s", arg, arg2);
 		
 		if( arg[0] == '-' )
 		{
 			if( strsrch(arg, "a") != -1 ) opt |= SHOW_ALL;
 			if( strsrch(arg, "l") != -1 ) opt |= SHOW_LONG;
 			
 			arg = 0;
 		}
 		else if( arg2 && arg2[0] == '-' )
 		{
 			if( strsrch(arg2, "a") != -1 ) opt |= SHOW_ALL;
 			if( strsrch(arg2, "l") != -1 ) opt |= SHOW_LONG;
 			
 			arg2 = 0;
 		}
 	}
 	
	dir = resolve_path(me, arg || arg2);
	
        if( dir[<1] != '/' && file_size(dir) == -2) dir += "/";
	
	if( !sizeof(files = get_dir(dir, -1)) )
	{
		if (file_size(dir)==-2) 	
			tell(me, "此目錄內無任何檔案。\n");
		else 
			tell(me, "沒有這個目錄。\n");
			
		return;
	}
	
	dir = dir[0..strsrch(dir,"/",-1)];

	if( file_size(dir+".dir")>0 )
	foreach( note in explode(read_file(dir+".dir"), "\n") )
	{
		if( !sscanf(note, "%s %s", english, chinese) ) continue;
		notes[english] = chinese;
	}
		
	foreach( file in files )
	{ 	
		fn = file[0];
		if( !(opt & SHOW_ALL) && fn[0] == '.' ) files -= ({ file });
		else if( (len=strlen(fn+notes[fn])) > width-1 ) width = len + 1;
	}
	
	col = (query_temp("windowsize/width", me) || 80) / (width+5);
	
 	foreach( file in files )
 	{
 		fn = file[0];
 		note = notes[fn] || "";
 		
 		if( file[1] == -2 )
 		{
 			auth = "d";
 			color = HIC;
 			
 			if( !SECURE_D->valid_check(dir+fn, me, 0, READ) )
 				color = HIR, auth += "-";
 			else
 				auth += "r";
 			
 			if( !SECURE_D->valid_check(dir+fn, me, 0, WRITE) )
 				color = HIR, auth += "-";
 			else
 				auth += "w";
 			
 			if( !SECURE_D->valid_check(dir+fn, me, 0, COMPILE) )
 				color = HIR, auth += "-";
 			else
 				auth += "x";	
 				
 			
 			if( opt & SHOW_LONG )
 			{
 				i++;
 				show_dir += sprintf("%-5s %10s %s %-20s%-20s\n",auth ,"<DIR>", TIME_D->replace_ctime(file[2]),color + fn +"/"+ NOR, (color_tran[color]||"")+(notes[fn]||"")+NOR);
 			}
 			else
 			{
 				show_dir += sprintf("   * %-*s%s", 
					width,
                        		color+fn+"/"+NOR+(color_tran[color]||"")+note+NOR,
					++i%col ? "" : "\n" );
			}
 		}
 		else
		{	
			if( find_object(dir+fn) || (fn[<2..] == ".c" && find_object(dir+fn[0..<3]))) color = HIG;
			else if( fn[<2..] == ".o" && find_object(dir+fn[0..<3]) ) color = HIY;
			else if( fn[<2..] == ".c" ) color = GRN;
			else if( fn[<2..] == ".o" ) color = YEL;
			else if( fn[0] == '.' )	color = WHT;
			else color = NOR;
			
			auth = "-";
			
 			if( !SECURE_D->valid_check(dir+fn, me, 0, READ) )
 				color = HIR, auth += "-";
 			else
 				auth += "r";
 			
 			if( !SECURE_D->valid_check(dir+fn, me, 0, WRITE) )
 				color = HIR, auth += "-";
 			else
 				auth += "w";
 			
 			if( !SECURE_D->valid_check(dir+fn, me, 0, COMPILE) )
 				color = HIR, auth += "-";
 			else if( fn[<2..] == ".c" )
 				auth += "x";
 			else
 				auth += "-";
 				
			if( opt & SHOW_LONG )
			{
				j++;
				show_files += sprintf("%-5s %10s %s %-20s%-20s\n",auth ,file[1]+"",TIME_D->replace_ctime(file[2]),color + fn + NOR, (color_tran[color]||"")+(notes[fn]||"")+NOR);
			}
			else
			{
				show_files += sprintf("%4s %-*s%s", 
					WHT+(file[1]/1024+1),
					width,
                       			NOR+color+fn+NOR" "+(color_tran[color]||"")+note+NOR,
					++j%col ? "" : "\n" );
			}
		}
	}

	// 我在幹嘛...-.-
	if( dir == "/" ) 
		dir += " "NOR CYN+"根目錄";
	else if( file_size(dir[0..strsrch(dir[0..<2], "/", -1)]+".dir")>0 )
	foreach( note in explode(read_file(dir[0..strsrch(dir[0..<2], "/", -1)]+".dir"), "\n") )
	{
		if( !sscanf(note, "%s %s", english, chinese) ) continue;
		if( english == dir[(strsrch(dir[0..<2], "/",-1)+1)..<2] )
		{
			dir += " "NOR CYN+chinese;
			break;
		}
	}
	
	me->more(HIC+"目錄:"+dir+NOR + "\n共 "+i+" 個資料夾，"+j+" 個檔案\n" + show_dir + (!(opt & SHOW_LONG)&& i%col?"\n":"") + show_files + "\n");
}
