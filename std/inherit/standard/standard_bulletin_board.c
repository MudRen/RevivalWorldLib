/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : bulletin_board.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-3-21
 * Note   : 標準留言板繼承檔
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <feature.h>
#include <inherit.h>
#include <daemon.h>

inherit STANDARD_OBJECT;
inherit BULLETIN_BOARD;

mapping actions = 
([ 
    "post"		:(: do_post :),
    "read"		:(: do_read :),
    "discard"		:(: do_discard :),
    "followup"		:(: do_followup :),
    "mail"		:(: do_mail :),
]);

int is_board()
{
	return 1;
}

string short()
{
	int unread, size, last_read;
	array summary = check_summary(this_object());

	last_read = query("board_last_read/"+query_board_id(this_object()), this_player()) || 0;

	size = sizeof(summary)/3;

	if( !size || !query_board_id(this_object()) ) 
		return ::short()+" "+NOR WHT+"["NOR" 沒有任何留言 "WHT"]"NOR;

	unread = sizeof(filter_array(summary, (: intp($1) && $(last_read) < $1 :)));

	return ::short() +" "+NOR WHT+"[ "NOR+ size +" 張留言 " + (unread ? "﹐"+unread+" 張未讀"WHT"]"NOR : WHT"]"NOR);
}

string long()
{
	string msg;
	array summary = check_summary(this_object());
	int last_read = query("board_last_read/"+query_board_id(this_object()), this_player());


	msg = sprintf("\n%-34s%s", this_object()->query_idname(), NOR WHT"─────────────────────────\n\n"NOR);

	for(int i=0;i<sizeof(summary);i+=3)
		msg += sprintf("%s[%4d] %s %-26s %-30s <%15s>\n",(summary[i]>last_read?HIY:""), i/3+1, NOR, summary[i+1], summary[i+2], TIME_D->replace_ctime(summary[i]));

	msg += NOR WHT"\n──────────────────────────────────────────\n\n"NOR;

	return msg;
}
