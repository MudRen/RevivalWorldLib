/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : semote.c
 * Author : Sinji@RevivalWorld
 * Date   : 2002-09-12
 * Note   : 指令
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <daemon.h>
#include <emotion.h>
#include <feature.h>
#include <message.h>

inherit COMMAND;

#define BAR	"────────────────────────────────────────\n"

string help = @HELP
表情查詢指令

這個指令會列出目前所有可以使用的表情指令

指令格式:
semote
semote -t		查看表情指令的使用排行表
semote -l		查看表情指令的簡易說明
semote <表情代號>	查看 <表情代號> 的詳細表情訊息


相關指令: command
HELP;

private string replace_keys(string description)
{
	description = replace_string(description, "$ME", HIC"你"NOR);
	description = replace_string(description, "$YOU", HIG"某人"NOR);
	description = replace_string(description, "$T", HIM"事物"NOR);

	return description;
}

private void do_command(object me, string arg)
{
	string msg;

	if( arg == "-t" )
	{
		int i;
		string *emotions = EMOTE_D->query_emotions();
		mapping emotion_sort = allocate_mapping(sizeof(emotions));

		foreach( string e in emotions )
		emotion_sort[e] = EMOTE_D->query_emotion(e)[USE];

		emotions = sort_array(emotions, (: count( $(emotion_sort)[$1],">",$(emotion_sort)[$2] )? -1:1:));

		msg = "表情指令使用排名:\n";
		foreach( string e in emotions )
			msg += sprintf("%4s %-20s %-10s 次\n", HIW+(++i)+"."NOR, e, emotion_sort[e]);

		return me->more(msg+"\n");
	}
	else if( arg == "-l" )
	{
		int i;
		string *emotions = EMOTE_D->query_emotions();
		mapping emotion_sort = allocate_mapping(sizeof(emotions));

		foreach( string e in emotions )
		emotion_sort[e] = EMOTE_D->query_emotion(e)[NOTE];

		emotions = sort_array(emotions, (: strcmp($1, $2) :));

		msg = "表情指令簡介說明\n";
		foreach( string e in emotions )
		{
			msg += sprintf("%4s %-20s %-10s\n", HIW+(++i)+"."NOR, e, emotion_sort[e]||WHT"無簡介"NOR);
		}
		return me->more(msg+"\n");
	}
	else if( arg )
	{
		array emotion = EMOTE_D->query_emotion(arg);

		if( undefinedp(emotion) )
			return tell(me, "沒有 " + arg + " 這個表情指令。\n", CMDMSG);

		msg = "表情 "+arg+" 之詳細敘述如下：\n"BAR;

		msg += NOR WHT"1.無對象無附加文字    ："NOR+replace_keys(emotion[NONE_ALL])+"\n";
		msg += NOR WHT"2.無對象有附加文字    ："NOR+replace_keys(emotion[NONE_ALL_ARG])+"\n";
		msg += NOR WHT"3.對象為自己無附加文字："NOR+replace_keys(emotion[ME_ALL])+"\n";
		msg += NOR WHT"4.對象為自己有附加文字："NOR+replace_keys(emotion[ME_ALL_ARG])+"\n";
		msg += NOR WHT"5.對象為某人無附加文字："NOR+replace_keys(emotion[YOU_ALL])+"\n";
		msg += NOR WHT"6.對象為某人有附加文字："NOR+replace_keys(emotion[YOU_ALL_ARG])+"\n";
		msg += NOR WHT"7.表情註解            ："NOR+(emotion[NOTE] || "無註解")+"\n";
		msg += NOR WHT"8.使用次數            ："NOR+emotion[USE]+" 次\n";
	}
	else
	{
		int size;
		string *emotions = sort_array(EMOTE_D->query_emotions(), 1);

		size = sizeof(emotions);
		msg = "目前共有 "+size+" 種表情指令可以使用，列表如下：\n"BAR;

		msg += sprintf("%-=80s", sprintf("%@-:10s", emotions)) + "\n";
		msg += "\n請輸入 semote <表情代號> 觀看詳細敘述。\n";
	}
	me->more(msg + BAR);
}
