/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : alias.c
 * Author : Sinji@RevivalWorld
 * Date   : 2001-07-10
 * Note   : 指令別名取代
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <daemon.h>
#include <feature.h>
#include <message.h>

#define MAX_ALIASES	50

inherit COMMAND;

string help = @HELP
別名指令

使用者可自定指令的別名，能將一長串的指令縮短成簡短易記的單字
在設定的指令中，可以加入 $* 及 $# 做為指令別名的參數

 # 代表 1 到 N 個指令別名參數的個數
 * 代表所有參數
 
如 $1, $2, $3 分別代表指令別名的第一、第二、第三參數以空白區隔

範例及用法如下:

> alias da date
設定指令 date 的別名為 da，da 同 date

> alias ch chat $1 and $2
設定 chat 的別名為 ch，ch hello world 同 chat hello and world 

> alias ch chat $*
設定 chat 的別名為 ch，ch hello world 同 chat hello world 

> alias -d ch
刪除指令別名 ch 

指令格式:
alias			顯示所有自訂的指令別名
alias -d		顯示所有系統預設的指令別名
alias -d <指令別名>	刪除已自訂的指令別名
alias <別名> <指令>	設定指令 <指令> 別名為 <別名>
alias <使用者代號>	查看使用者的指令別名 (巫師專用)

相關指令: chfn, set, 
HELP;

private void do_command(object me, string arg)
{
	object target;
	string verb, cmd;
	int detail;

	if( arg )
	{
		if( arg == "-d" ) // 顯示詳細的指令別名
		{
			detail = 1;
			target = me;
		}
		else if( wizardp(me) && objectp(find_player(arg)) )
			target = find_player(arg);
	}
	else target = me;

	// 顯示目標的指令別名
	if( objectp(target) )
	{
		mapping alias;
		string msg, pnoun;
		int i;

		if( !detail )
			alias = target->query_alias();
		else
			alias = COMMAND_D->query_default_alias();

		pnoun = target == me ? pnoun(2, me) : target->query_idname();

		if( !(i = sizeof(alias)) ) 
			return tell(me, pnoun + "並未設定任何指令別名。\n");

		msg = pnoun + "目前共有 " + i + " 組指令別名如下: \n";

		foreach(verb in sort_array(keys(alias), 1))
			msg += sprintf(WHT" ["NOR HIW"%12|s"NOR WHT"]"NOR" %s"NOR"\n", verb, alias[verb]);

		return me->more(msg);
	}
	// 設定/刪除 指令別名
	if( sscanf(arg, "%s %s", verb, cmd) != 2 || verb == "alias" || cmd=="" )
		return tell(me, "指令格式錯誤，請參考 help alias 以取得更多的使用資訊。\n");

        if( stringp(verb) && !strlen(verb) )
                return tell(me, "請輸入正確的指令格式。\n");

	// 刪除
	if( verb == "-d" )
	{
		if( undefinedp(me->query_alias()[cmd]) )
			return tell(me, pnoun(2, me) + "並沒有設定 "+ cmd +" 這個指令別名。\n");

		me->del_alias(cmd);
		return tell(me, "指令別名 " + cmd + " 刪除完成。\n");
	}
	// 設定
        if( sizeof(me->query_alias()) > MAX_ALIASES && !wizardp(me) )
		return tell(me, "您的指令別名數量己達上限。");

	if( !undefinedp(COMMAND_D->query_default_alias()[verb]) )
		return tell(me, "指令 " + verb + " 已存在於預設的指令別名中，請參考 alias -d。\n");

	if( me->set_alias(verb, cmd) ) 
		tell(me, "設定指令 " + verb + " 別名為 " + cmd + " 完成。\n");
}
