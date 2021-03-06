/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : language.c
 * Author : Clode@RevivalWorld
 * Date   : 2006-09-08
 * Note   : language 指令
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <feature.h>
#include <message.h>
#include <ansi.h>

inherit COMMAND;

string help = @HELP
即時翻譯指令(Alpha Version)

利用 Google Translate 作即時 Mud 翻譯

目前可翻譯語言為

en	- 英文

HELP;

private void do_command(object me, string arg)
{
	string language = query("translate/language", me);

	if( !arg )
	{
		if( !language )
			return tell(me, pnoun(2, me)+"目前沒有作任何即時翻譯。\n");

		return tell(me, pnoun(2, me)+"目前所使用的語言為："+language+"。\n");
	}
	
	switch(arg)
	{
		case "-d":
		{
			if( !language )
				return tell(me, pnoun(2, me)+"原本就沒有設定即時翻譯功能。\n");

			delete("translate/language", me);
			tell(me, HIY+pnoun(2, me)+"取消了即時翻譯的功能。\n"NOR);
			me->save();
			break;
		}
		case "en":
		{
			
			tell(me, HIR"警告：目前翻譯功能尚未完備，可能導致許多文字的錯亂，關閉翻譯功能請按 language -d。\n"NOR);
			tell(me, HIC+pnoun(2, me)+"開啟即時翻譯功能，語言設定為英文(en)。\n"NOR);
			set("translate/language", arg, me);
			me->save();
			break;
		}
		default:
			return tell(me, "沒有 "+arg+" 語言。\n");
			break;
	}
}
