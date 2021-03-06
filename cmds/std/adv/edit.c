/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : edit.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-2-6
 * Note   : edit 指令
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <feature.h>
#include <message.h>


inherit COMMAND;

string help = @HELP
指令格式 : edit <檔名>, edit here

利用此一指令可直接在線上編輯檔案。
在 edit 下常用的指令或觀念:

用 增加(a), 取代(c) 啟動 'edit mode'.

要跳回 'command mode', 則在一行的起點輸入 '.' 再按 'enter' 鍵.

$ - 這個符號在 command mode 永遠都代表最後一行的行號.
:數字(n),(m)p ---- 列出檔案內容, n 為開始行號, m 為結束行號. 如果沒有
                   m,n 則列出 現在 所在行的內容. 如果 'p' 前只有一個數字
                   則列出數字所指行號的內容.
:數字(n),(m)d ---- 刪除由 n 到 m 行的內容. 如果 'd' 前只有一個數字, 則
                   刪除數字所指行號的內容.
:數字(n)a     ---- 在 n 行後加入東西. 於是進入 'edit mode'
:數字(n)i     ---- 在 n 行前加入東西. 於是進入 'edit mode'
:數字(n)c     ---- 修改第 n 行的內容. 於是進入 'edit mode', 之後
                   你所打入的東西都會取代第 n 行原有的內容.
              [註: 這個指令的 「有效」範圍是一行, 超過一行並不會讓你修改
                     n+1 行的東西.]
如果沒有指定行號, 則以目前所在行為準.
:x            ---- 存檔並離開 edit。
:q            ---- 離開 edit. [註: 此指令只適用於當檔案已經儲存過後, 或是檔案
                   沒有被更動過.]
:Q            ---- 如果檔案被動過, 但你不想寫入被更動的內容, 你可以用這個指
                   令離開 edit.
:!<cmd>       ---- 在 edit 下執行某些 mud 指令.
HELP;
private void exit_edit()
{
        this_player(1)->show_prompt();
}

private void command(object me, string arg)
{
	if( !is_command() ) return;
	
	if( !arg ) 
		return tell(me, "指令格式﹕edit <檔名>\n", CMDMSG);

	arg = resolve_path(me, arg);

	switch( file_size(arg) )
	{
		case -1:
			tell(me, "開始編輯新檔案 "+arg+"。\n", CMDMSG);
			break;
		case -2:
			return tell(me, "無法編輯一個目錄。\n", CMDMSG);
		default:
			tell(me, "開始編輯舊檔案 "+arg+"。\n", CMDMSG);
	}
        set("current_work/file", arg, me);
        ed(arg, "exit_edit");
}
