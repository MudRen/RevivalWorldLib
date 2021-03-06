/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : eval.c
 * Author : Sinji@RevivalWorld
 * Date   : 2001-9-14
 * Note   : eval 指令
 * Update :
 *
 -----------------------------------------
 */

#include <feature.h>
#include <message.h>

#define EVAL_FILE       "eval.c"
#define COST_TRIM       -8
#define EXPT_TRIM       -2
inherit COMMAND;

string help = @HELP
        標準 eval 指令。
HELP;

private int parse_script(string arg);
private void command(object me, string arg)
{
	string lines, path, file, err;
	object eval_ob;

	if( !is_command() ) return;
	
	if( !arg )
		return tell(me, "指令格式：eval <LPC Script>\n");

	arg = replace_string(arg, "\n", "");
	if( arg[<1] != ';' ) arg += ";";
	parse_script(arg);

	/* Following is content of eval file */
	lines = "// This file written by eval command. \n";
	lines += "#define COST_TRIM     " + COST_TRIM + "\n";
	lines += "#define EXPT_TRIM     " + EXPT_TRIM + "\n";
	lines += @FILE
#include <ansi.h>
#include <daemon.h>
#include <object.h>
#include <feature.h>
#include <function.h>
#include <eval.h>
#include <message.h>
#include <runtime_config.h>
#include <quest.h>
#include <condition.h>
#include <map.h>
#include <areadata.h>
#include <citydata.h>
#include <buff.h>
#include <equipment.h>

void create()
{
        mapping sinfo, einfo;
        int cost, time_exp; 
        float utime, stime, etime;
        string msg;
        object me = this_player();

        sinfo = rusage();
        cost = eval_cost();
        time_exp = time_expression {
FILE + arg + @FILE

        };
        einfo = rusage();
        cost -= eval_cost();

        utime = to_float(einfo["utime"] - sinfo["utime"])/1000000;
        stime = to_float(einfo["stime"] - sinfo["stime"])/1000000;
        etime = to_float(time_exp + EXPT_TRIM)/1000000;

        msg  = sprintf("\n效率評估: %d\n", cost + COST_TRIM);
        msg += sprintf("系統時間: %.6f s\n", stime);
        msg += sprintf("使用時間: %.6f s\n", utime);
        msg += sprintf("運算時間: %.6f s\n", etime);
        
        tell(me, msg);
}
FILE;
	/* Above is content of eval file */
	path = wiz_home(me->query_id(1));
	file = sprintf("%s/%s", path, EVAL_FILE);
	
	if( file_size(path) != -2 )
		return tell(me, "沒有 "+path+" 這個目錄。\n評估前請先建立此目錄。\n");
	
	if( eval_ob = find_object(file) ) 
		destruct(eval_ob);
	
	write_file(file, lines, 1);
	
	if( !(err = catch(load_object(file))) )
	{
		tell(me, "編譯成功。\n");
                rm(file);

		if( eval_ob = find_object(file) ) 
			destruct(eval_ob);
	} 
	else
		tell(me, "編譯失敗。\n");
}

private int parse_script(string arg)
{
	int idx, chr, mark, marked;
	int s_symbol, m_symbol, b_symbol, d_quote, s_quote;
	foreach(string cmd in explode(arg, ";"))
	{
		for(idx=0; idx<strlen(cmd); idx++)
		{
			chr = cmd[idx];
			mark = (chr == '\\');
			if( mark ) marked++;
			if( !(s_quote%2) && (!marked || !(marked%2)) && chr == '\"' ) d_quote++;
			if( !(d_quote%2) && (!marked || !(marked%2)) && chr == '\'' ) s_quote++;
			if( !mark ) marked = 0;
			if( d_quote%2 ) continue;
			switch(chr)
			{
			case '(':       s_symbol++;     break;
			case ')':       s_symbol--;     break;
			case '[':       m_symbol++;     break;
			case ']':       m_symbol--;     break;
			case '{':       b_symbol++;     break;
			case '}':       b_symbol--;     break; 
			}
		}
	}
	if( d_quote%2 )
		printf("Prasing error: 雙引號不正常結束。\n");
	if( s_quote%2 )
		printf("Prasing error: 單引號不正常結束。\n");
	if( s_symbol )
		printf("Prasing error: () 括號必須成對 [%s了 %d 個右括號]。\n",
		    (s_symbol > 0 ? "少":"多"), to_int(abs(s_symbol)));
	if( m_symbol )
		printf("Prasing error: [] 括號必須成對 [%s了 %d 個右括號]。\n",
		    (m_symbol > 0 ? "少":"多"), to_int(abs(m_symbol)));
	if( b_symbol )
		printf("Prasing error: {} 括號必須成對 [%s了 %d 個右括號]。\n",
		    (b_symbol > 0 ? "少":"多"), to_int(abs(b_symbol)));
}
