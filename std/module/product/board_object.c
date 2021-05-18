/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : object.c
 * Author : Clode@RevivalWorld
 * Date   : 2006-01-14
 * Note   : 
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <inherit.h>
#include <ansi.h>
#include <feature.h>
#include <daemon.h>
#include <message.h>

inherit STANDARD_MODULE_PRODUCT;
inherit BULLETIN_BOARD;

int is_board()
{
	return 1;
}

string short()
{
	int unread, size, last_read;
	array summary = check_summary(this_object());
	string type, boardid;

	boardid = query_board_id(this_object());

	size = sizeof(summary)/3;

	if( !size || !boardid ) 
		return ::short()+" "+NOR WHT+"["NOR" 沒有任何留言 "WHT"]"NOR;

	last_read = query("board_last_read/"+boardid, this_player()) || 0;

	unread = sizeof(filter_array(summary, (: intp($1) && $(last_read) < $1 :)));

	if( boardid[0..3] == "city" )
		type = HIM"城"NOR MAG"市"NOR;
	else if( boardid[0..5] == "player" )
		type = HIC"個"NOR CYN"人"NOR;
	else if( boardid[0..5] == "wizard" )
		type = HIY"巫"NOR YEL"師"NOR;
	else
		type = "";

	return ::short() +" "+NOR WHT+"[ "NOR+ size +" 張留言 " + (unread ? "﹐"+unread+" 張未讀"WHT"]"NOR : WHT"]"NOR)+" "+type;
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

varargs void create(string file, string type)
{
	::create(file, type);

	set("unit", "塊");
	set("mass", 20000);
	set("value", query_value());
	set("flag/no_amount", 1);

	actions = 
	([ 
	    "post"		:(: do_post :),
	    "read"		:(: do_read :),
	    "discard"		:(: do_discard :),
	    "followup"		:(: do_followup :),
	    "mail"		:(: do_mail :),
	]);

	setup_inlay();
}
