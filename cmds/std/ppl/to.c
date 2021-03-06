/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : to.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-3-6
 * Note   : to 指令
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <feature.h>

inherit COMMAND;

string help = @HELP
多行輸入指令

這個指令可以讓您輸入多行訊息做為指定指令的參數
在新的一行中

輸入 '~q' 可以取消多行輸入 
輸入 '.'  可以結束多行輸入

指令格式:
to <指令>

相關指令:
HELP;

private void do_to(object me, string arg, string str)
{
	if( query("encode/gb", me) )
		str = B2G(str);

	me->force_me(arg ? arg+" "+str : str);
	me->finish_input();
}

private void do_command(object me, string arg)
{
	me->edit( (: do_to, me, arg :) );
}
