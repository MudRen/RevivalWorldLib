/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : room_wizhall_1.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-08-23
 * Note   : 標準房間
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>
#include <message.h>
#include <daemon.h>

inherit STANDARD_ROOM;

string help = @HELP
在這個平凡無奇的房間內並沒有任何特殊的資訊。
HELP;

// 預設出口
#define DEFAULT_CITY_EXIT(city, num)	({ 49, 49, 0, city, num, 0, "49/49/0/"+city+"/"+num+"/0" })

mapping actions;

array south_exit(object ob)
{
	string city = query("city", ob);
	int num = query("section_num", ob);

	if( !sizeof(CITY_D->query_cities()) )
		return 0;

	if( !CITY_D->city_exist(city) )
	{
		string *cities = CITY_D->query_cities() - ({ "wizard" });
		
		if( sizeof(cities) )
		{	
			do
			{
				city = cities[random(sizeof(cities))];
			}
			while( CITY_D->query_city_info(city, "no_occupy") );
		}
		else
			return 0;
	}
	
	if( CITY_D->city_exist(city, num) )
		return CITY_D->query_section_info(city, num, "wizhall") || DEFAULT_CITY_EXIT(city, num);
	else
		return CITY_D->query_section_info(city, 0, "wizhall") || DEFAULT_CITY_EXIT(city, 0);
}

void do_gocity(object me, string arg)
{
	int num;
	string my_city = query("city", me);
	string *lines;

	if( !arg )
		return tell(me, pnoun(2, me)+"想要去哪座城市？請 help gocity 參考指令用法並使用 mayor 指令查詢所有城市資料。\n");
	
	arg = remove_ansi(lower_case(arg));
	
	lines = explode(arg, " ");
	if(undefinedp(num = to_int(lines[<1]))) {
		num = 1;
		arg = implode(lines, " ");
	}
	else {
		arg = implode(lines[0..<2], " ");
	}
	

	if( !num ) num = 1;

	if( !CITY_D->city_exist(arg, num-1) )
		return tell(me, "這個世界並沒有這座城市。\n");

	if( !wizardp(me) )
	{
		if( CITY_D->query_city_info(arg, "no_occupy") )
			return tell(me, "這座城市無法從這裡進入。\n");
			
		if( my_city && my_city != arg )
			return tell(me, pnoun(2, me)+"是"+CITY_D->query_city_idname(my_city)+"的市民，無法再自由進入其他城市。\n");
		
		if( arg == "wizard" )
			return tell(me, CITY_D->query_city_idname(arg, num)+"是巫師城市，只有巫師能夠進入。\n");
	}
	
	msg("$ME穿過巫師神殿中心匯聚能量後所開啟的通道，前往"+CITY_D->query_city_idname(arg, num-1)+"。\n", me, 0, 1);
	me->move(CITY_D->query_section_info(arg, num-1, "wizhall") || DEFAULT_CITY_EXIT(arg, (num-1)));
	msg("$ME穿過巫師神殿通道來到了"+CITY_D->query_city_idname(arg, num-1)+"。\n", me, 0, 1);
}

void create()
{
	set("short", HIW"巫師"NOR WHT"神殿"NOR);
	set("long",@TEXT
    巨大的巫師神殿兩側各豎立著四支巨型石柱，支撐著數十尺高屋頂，神殿的左
右更是寬達數百尺，在這棟古老的石造建築中，可以感覺到一股[1;37m神聖的能量[m正緩緩
的進入身體，同時卻也可以感受到建築深處緩緩襲來的[0;1;30m陰冷[m之感，似乎在這座神殿
裡，有著許多不為人知的秘密，由此往[1;33m South [m方向可以走向自己所屬的城市，尚
未入籍任何城市者往南走則會隨意走到任一個城市，或是利用[1;33m gocity [m指令前往您
所指定的城市。使用 [1;33mmayor [m指令可以查詢各個城市的詳細資料，作為入籍時的參
考。
TEXT);

	set("exits", ([
		"east" : "/wiz/wizhall/room_wizhall_2",
		"west" : "/wiz/wizhall/room_wizhall_6",
		"north" : "/wiz/wizhall/room_wizhall_4",
		"south" : (: south_exit($1) :),
		
	]));
	

	set("objects", ([
		"/obj/board/player" : 1,
	]));
	
	set_heart_beat(100);

	actions = ([ "gocity" : (: do_gocity :) ]);
	
	set("help/gocity", 
@HELP
無國籍者可以使用此指令自由前往任一座城市，指令用法如下
    
    gocity '城市ID'	- 前往某城市
    
    例如
    gocity center	- 前往 center 城市
    
城市 ID 可由 mayor 指令查詢
HELP,
	);

	::reset_objects();
}

void heart_beat()
{
	int heal;
	string roomshort = query("short");
	
	foreach( object ob in all_inventory(this_object()) )
	{
		if( !ob->is_living() ) return;

		heal = 0;
		
		if( !ob->is_stamina_full() )
		{
			heal = 1;
			ob->earn_stamina(20+random(21));
		}

		if( !ob->is_health_full() )
		{
			heal = 1;
			ob->earn_health(20+random(21));
		}

		if( !ob->is_energy_full() )
		{
			heal = 1;
			ob->earn_energy(20+random(21));
		}
		
		if( heal )
			msg(roomshort+"裡神聖的能量正緩緩地注入著$ME的身體。\n"NOR, ob, 0, 1);
	}
}

void init(object ob)
{
	if( !ob->is_living() ) return;

	if( query("total_online_time", ob) < 24*60*60 )
	{
		string msg = HIY"\n新手提示──────\n"
			NOR YEL"歡迎"+pnoun(2, ob)+"進入重生的世界線上 Mud 遊戲。\n"
			"新手請先詳加閱讀 http://www.revivalworld.org/rw/documents/newbie 之官網教學文件\n"
			"看完教學文件後若有任何其他問題歡迎利用「chat xxx」聊天指令向線上其他玩家詢問\n"
			"即時地圖請參考 http://www.revivalworld.org/rw/online\n"
			HIY"──────────\n\n"NOR;
			
		tell(ob, msg);
	}
}

