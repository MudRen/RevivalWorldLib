#include <ansi.h>
#include <daemon.h>
#include <inherit.h>
#include <secure.h>
#include <lock.h>

inherit STANDARD_OBJECT;

void do_join(object me, string arg);
void do_start(object me, string arg);
void do_show(object me, string arg);
void do_find(object me, string arg);
void do_finding(object me, int x, int y);
private void reset_mine();
private varargs void flying(object me, int i);

nosave mapping actions = 	
([
	"join"			:	(: do_join :),
	"start"			:	(: do_start :),
	"show"			:	(: do_show :),
	"goto"			:	(: do_find :),
	"reset"			:	(: reset_mine :),
]);

nosave mapping corresponding =
([
	"north"			:	"south",
	"south"			:	"north",
	"east"			:	"west",
	"west"			:	"east",
	"northwest"		:	"southeast",
	"northeast"		:	"southwest",
	"southwest"		:	"northeast",
	"southeast"		:	"northwest",
	"up"			:	"down",
	"down"			:	"up",
]);

nosave string help = @HELP
踩地雷對戰版-使用說明

join			加入遊戲
start			開始遊戲
show			顯示佈雷區
goto <X軸> <Y軸>	掀開該方塊
reset			結束遊戲
HELP;

nosave array mine_map;
nosave int total_mine = 0, finded_mine = 0;
nosave int map_size = 0;
nosave int call_depth = 0;
nosave int player_turn = 0;
nosave object *players = allocate(0);

private int is_started()
{
	return pointerp(mine_map);
}
private int valid_coordinate(int x, int y)
{
	if( x < 0 || y < 0 || x >= sizeof(mine_map) || y >= sizeof(mine_map[0]) ) return 0;
	return 1;
}
private int valid_find(int x, int y)
{
	if( mine_map[x][y] > 0 )return 0;
	return 1;
}
private object query_player()
{
	return players[player_turn%sizeof(players)];
}
private int next_player()
{
	int i = ++player_turn;
	if( !sizeof(players) )
	{
		reset_mine();
		return -1;
	}
	msg(CYN"\n█ "HIC"第 " + (i+1) + " 步輪$ME。\n\n"NOR, query_player(), 0, 1);
	do_show(query_player(), 0);
	return i;
}
private void kick_player(object me, int x, int y)
{
	player_turn--;
	players -= ({ me });
	mine_map[x][y] = 10;
	me->faint();
	flying(me);
	tell(me, HIY"\n█ "YEL"遊戲結束，你輸了。\n\n"NOR);
}
private void reset_mine()
{
	set_temp("status", HIY"亂鬥受付中"NOR);
	mine_map = 0;
	player_turn = 0;
	players = allocate(0);
	msg(HIY"\n█ "YEL"遊戲結束。\n\n"NOR, this_object(), 0, 1);
}
private void leaver_filter()
{
	if( !is_started() ) return;
	players -= ({ 0 });
	if( !sizeof(players) ) reset_mine();
}
private void allocate_mine(int width, int height)
{
	int count;
	float ratio;
	
	total_mine = 0;
	// 建立矩陣
	mine_map = allocate(width);
	for(int x=0; x<width; x++)
		mine_map[x] = allocate(height);

	ratio = (random(3)+1)/10.0;
	// 隨機配置炸彈
	map_size = width*height;
	count = width*height*ratio;
	while(count > 0)
	{
		int x = random(width);
		int y = random(height);
		if( mine_map[x][y] == -1 ) continue;
		mine_map[x][y] = -1;
		count--;
		total_mine++;	
	}
	return;
}
private int find_mine(int x, int y)
{
	int mine_count;

	if( !valid_coordinate(x, y) ) return -2;
	if( mine_map[x][y] == -1 ) return 1;
	if( mine_map[x][y] > 0 ) return -1;
	
	//printf("call_depth: %d\n", ++call_depth);
	// Count mines
	mine_count = 0;

	for(int dx=-1; dx<=1; dx++)
		for(int dy=-1; dy<=1; dy++)
		{
			if( dx == 0 && dx == dy ) continue;
			if( !valid_coordinate(x + dx, y + dy) ) continue;
			if( mine_map[x + dx][y + dy] == -1 || mine_map[x + dx][y + dy] == 10 )
			{
				//printf("%d %d has bomb\n", x + dx+1, y + dy+1); 
				mine_count++;
			}
		}

	mine_map[x][y] = 1 + mine_count;
	finded_mine++;

	if( mine_count == 0 ) // Explore
		for(int dx=-1; dx<=1; dx++)
			for(int dy=-1; dy<=1; dy++)
			{
				if( dx == 0 && dx == dy ) continue;
				find_mine(x + dx, y + dy);
			}
	return 0;
}

private varargs void flying(object me, int i)
{
	mixed exit;
	mapping exits;
	object env;
	string arg, my_idname, from;

	env = environment(me);
	my_idname = me->query_idname();

	if( !mapp(exits = query("exits", env)) || !sizeof(exits) ) return;

	if( i > 10 )
		return broadcast(env, my_idname+"掉到了地上，並且翻滾了好幾圈，似乎傷的不清。\n", 0, me);

	arg = keys(exits)[random(sizeof(keys(exits)))];
	exit = exits[arg];
	from = CHINESE_D->to_chinese(corresponding[arg]);
	if( functionp(exit) ) exit = evaluate(exit, me);

	// 若是座標出口
	if( arrayp(exit) )
	{
		if( (MAP_D->query_map_system(exit))->query_coor_data(exit, "lock") & LOCKED && !wizardp(me) )
			return broadcast(exit, my_idname+"飛了過來，撞上了往 "+capitalize(arg)+" 方向的門。\n", 0, me);

		if( !wizardp(me) && (MAP_D->query_map_system(exit))->query_coor_data(exit, "lock") & WIZLOCKED )
			return broadcast(exit, my_idname+"飛了過來，撞上了往 "+capitalize(arg)+" 方向的光牆。\n", 0, me);
	}
	// 若是物件名稱出口
	else if( stringp(exit) )
	{
		if( query("lock/"+arg, env) & LOCKED && !wizardp(me))					   
			return broadcast(exit, my_idname+"飛了過來，撞上了往 "+capitalize(arg)+" 方向的門。\n", 0, me);

		if( !wizardp(me) && query("lock/"+arg, env) & WIZLOCKED )
			return broadcast(exit, my_idname+"飛了過來，撞上了往 "+capitalize(arg)+" 方向的光牆。\n", 0, me);
	}
	else return;

	broadcast(env, my_idname+"往"+(CHINESE_D->to_chinese(arg)||" "+capitalize(arg)+NOR" 方向")+"飛去。\n", 0, me);

	me->move(exit);

	if( from )
		broadcast(exit, my_idname+"從"+from+"飛了過來。\n", 0, me);
	else
		broadcast(exit, my_idname+"飛了過來。\n", 0, me);

	flying(me, i++);
}

void do_join(object me, string arg) 
{
	if( sizeof(players) >= 10 )
		return tell(me, "本機台的座位已滿，請稍後再來。\n");

	if( pointerp(mine_map) )
		return tell(me, "遊戲已開始。\n");

	if( member_array(me, players) != -1 )
		return tell(me, "您已經加入本遊戲了。\n");

	msg("$ME從口袋中掏出 2 枚所剩無幾的代幣投進了$YOU。\n", me, this_object(), 1);

	players += ({ me });
	set_temp("status", HIY"亂鬥受付中 (" + sizeof(players) + " 人)"NOR);
}
void do_start(object me, string arg)
{
	int i, max_x, max_y;
	
	if( member_array(me, players) == -1 )
		return tell(me, "您並未參加遊戲，請使用 join 指令加入遊戲。\n");
	if( is_started() )
		return tell(me, "遊戲已經開始，請使用 goto 指令來尋找地雷。\n");

	set_temp("status", HIY + sizeof(players) + " 人亂鬥中"NOR);
	msg(YEL"\n█ "HIY"踩地雷遊戲開始。\n\n"NOR, me, 0, 1);

	max_x = 8 + sizeof(players)*2;
	max_y = 8 + sizeof(players)*2;

	msg(CYN"█ "HIC"隨機產生地圖 "+max_x+"x"+max_y+"...\n"NOR, me, 0, 1);
	allocate_mine(max_x, max_y);
	
	msg(CYN"█ "HIC"由骰子擲出遊戲順序 ...\n"NOR, me, 0, 1);

	leaver_filter();
	i = sizeof(players);
	msg(CYN"█ "NOR, me, 0, 1);
	while(i)
	{
		int j = random(i);
		object ob = players[j];

		msg(HIC + (sizeof(players)-i+1) + ". " + ob->query_idname() + HIC", "NOR, me, 0, 1);
		players -= ({ ob });
		players += ({ ob });
		i--;
	}
	
	msg(CYN"\n█ "HIC"請輸入 help mine 查詢指令用法。\n"NOR, me, 0, 1);
	msg(HIY"\n█ "YEL"遊戲目標: 避開所有可能有地雷的地點。\n"NOR, me, 0, 1);
	msg(CYN"\n█ "HIC"第 " + (player_turn+1) + " 步輪$ME。\n\n"NOR, query_player(), 0, 1);
	do_show(query_player(), 0);
}
void do_show(object me, string arg)
{
	string msg = "佈雷區 (共 " + total_mine + " 枚炸彈, 剩餘 " + (map_size-finded_mine) + " 個地點):\n";
	int flag = 0;

	if( !is_started() )
		return tell(me, "遊戲尚未開始，請使用 start 指令開始遊戲。\n");

	if( wizardp(me) && arg == "mines" ) flag = 1;

	msg += HIG"   ";
	for(int i=0; i<sizeof(mine_map); i++)
		msg += sprintf("%s%2d"NOR, (i%2==0 ? NOR:HIG), i+1);
	msg += "\n"NOR;
	for(int y=0; y<sizeof(mine_map[0]); y++)
	{
		msg += sprintf(HIG"%2d "NOR, y+1);
		for(int x=0; x<sizeof(mine_map); x++)
		{
			string symbol;
			switch(mine_map[x][y]-1)
			{
			
			case -2:	if( flag ) { symbol = HIR"※"NOR; break; }
			case -1:	if( (x+y)%2 == 0 )
						symbol = BCYN + HIC"  "NOR;
					else	symbol = BBLU + HIC"  "NOR;
					break;
			case  0:	symbol = NOR"  "NOR;		break;
			case  1:	symbol = HIB"１"NOR;		break;
			case  2:	symbol = HIG"２"NOR;		break;
			case  3:	symbol = HIR"３"NOR;		break;
			case  4:	symbol = HIY"４"NOR;		break;
			case  5:	symbol = HIC"５"NOR;		break;
			case  6:	symbol = HIW"６"NOR;		break;
			case  7:	symbol = HIW"７"NOR;		break;
			case  8:	symbol = HIW"８"NOR;		break;
			case  9:	symbol = HIR"※"NOR;		break;
			default:	symbol = sprintf(HIW"%2d "NOR, mine_map[x][y]-1);
			}
			msg += sprintf("%2s", symbol);
		}
		msg += "\n";
	}
	tell(me, msg);	
}
void do_find(object me, string arg)
{
	int x, y;

	if( !is_started() )
		return tell(me, "遊戲尚未開始，請使用 start 指令開始遊戲。\n");

	if( me != query_player() )
		return tell(me, "現在還沒有輪到你。\n");

	if( !arg || sscanf(arg, "%d %d", x, y) != 2 )
		return tell(me, "指令標式: find <行> <列>\n");

	if( !valid_coordinate(x-1, y-1) )
		return tell(me, "座標已超出地圖範圍。\n");
	if( !valid_find(x-1, y-1) )
		return tell(me, "這個座標已經走過了。\n");

	msg("$ME小心翼翼的將腳步踩向 " + sprintf("%d, %d", x, y) + " 的座標上 ...\n", me, 0, 1);
	call_out((: do_finding :), 1, me, x, y);
	
}
void do_finding(object me, int x, int y)
{
	int res;

	leaver_filter();	// 檢查玩家是否存在
	call_depth = 0;		// 遞回深度計數

	res = find_mine(x-1, y-1);
	if( res < 0 ) return;
	if( res )
	{
		switch(random(2))
		{
		case 0: msg("$ME的腳下似乎沒有什麼異狀。\n", me, 0, 1); break;
		case 1: msg(HIR"\n$ME踩到地雷了！！\n\n"NOR, me, 0, 1); break;
		}
		msg("哄然巨響「碰」的一聲，只見$ME被炸成一團黑炭衝上雲端。\n", me, 0, 1);

		kick_player(me, x-1, y-1);

		CHANNEL_D->channel_broadcast("news",
			sprintf("%s一腳往地雷踩下，「碰」一聲的巨響，只見一團黑炭衝上雲端。\n", me->query_idname()));
		if( sizeof(players) == 1 )
		{
			msg(HIY"\n恭喜$ME踩地雷獲勝!!\n\n"NOR, me, 0, 1);
			reset_mine();
		}
		else next_player();
	}
	else
	{
		switch(random(2))
		{
		case 0: msg("$ME的腳下似乎沒有什麼異狀。\n", me, 0, 1); break;
		case 1: msg("$ME的腳下似乎要發生什麼事，但又什麼都沒發生。\n", me, 0, 1); break;
		}
		if( sizeof(players) == 1 && map_size-total_mine == finded_mine )
		{
			msg(HIY"\n恭喜$ME踩地雷獲勝!!\n\n"NOR, me, 0, 1);
			reset_mine();
		}
		else next_player();
	}
	return;
}

void heart_beat()
{
	leaver_filter();
}
void create()
{
	set_idname("mine game",HIC"踩地雷"NOR CYN"對戰版"NOR);
	set("flag/no_amount", 1);
	set("long", help);
	set("unit", "台");
	set("mass", -1);
	reset_mine();
	set_heart_beat(10);
}