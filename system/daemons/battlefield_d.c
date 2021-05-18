/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : battlefield_d.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-10-21
 * Note   : 戰場系統
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <daemon.h>
#include <areadata.h>
#include <location.h>
#include <map.h>
#include <condition.h>
#include <citycondition.h>

#define BATTLE_AREA		"battlefield"
#define BATTLE_NUM		0
#define VALID_DIRECTION 	({ "north", "south", "east", "west", "northwest", "southwest", "northeast", "southeast" })
#define BATTLE_NPC		([ "/obj/npc/battlefield/berserker" : range_random(100, 120), "/obj/npc/battlefield/mars" : 1, ])
#define BATTLE_TIME		1080
#define BATTLE_JOIN_TIME	1200
#define BATTLE_X_START		24
#define BATTLE_Y_START		24
#define BATTLE_X_END		74
#define BATTLE_Y_END		74

string battle_name = HIY"榮耀"NOR YEL"之戰"NOR;
object *battle_npc = allocate(0);
mapping battle_player = allocate_mapping(0);

int battle_open_flag = 0;
int battle_start_flag = 0;
int battle_time_countdown = 0;
int battle_join_countdown = 0;

int in_battle(object me)
{
	object env = environment(me);
	
	if( !objectp(env) )
		return 0;
		
	return battle_start_flag && env->query_area() == BATTLE_AREA && env->query_num() == BATTLE_NUM;
}

int inside_battlefield(object me)
{
	object env = environment(me);

	if( !objectp(env) )
		return 0;

	return env->query_area() == BATTLE_AREA && env->query_num() == BATTLE_NUM;	
}

int is_battlefield(array loc)
{
	return loc[AREA] == BATTLE_AREA && loc[NUM] == BATTLE_NUM;
}

int query_battle_time()
{
	return battle_time_countdown;
}

int valid_loc(array loc)
{
	int type = MAP_D->query_coor_data(loc, TYPE);
	
	if( type == RIVER || type == WALL )
		return 0;

	return 1;	
}

int query_player_number(object player)
{
	return member_array(player->query_id(1), keys(battle_player)) + 1;
}

array query_random_loc(int xstart, int xend, int ystart, int yend, string area, int num)
{
	array loc;
	
	do
	{
		loc = arrange_area_location(range_random(xstart, xend), range_random(ystart, yend), BATTLE_AREA, BATTLE_NUM);

	} while(!valid_loc(loc));
	
	return loc;
}

void move_to_random_loc(object me)
{
	array loc = query_random_loc(BATTLE_X_START, BATTLE_X_END, BATTLE_Y_START, BATTLE_Y_END, BATTLE_AREA, BATTLE_NUM);
	
	me->move(loc);
}

void show_stance_statistic()
{
	if( sizeof(battle_player) > 0 )
	{
		int attack=0, defend=0, medic=0, speed=0, none=0;
		object player;

		foreach(string id in keys(battle_player))
		{
			if( !objectp(player = find_player(id)) )
				continue;
	
			switch(query("combat/stance/type", player))
			{
				case "attack":
				case "attack-adv":
					attack++;
					break;
				case "defend":
				case "defend-adv":
					defend++;
					break;
				case "speed":
				case "speed-adv":
					speed++;
					break;
				case "medic":
				case "medic-adv":
					medic++;
					break;
				default:
					none++;
					break;
			}
		}
		
		CHANNEL_D->channel_broadcast("combat", HIR"戰"NOR RED"爭"NOR"：目前報名者採用的戰鬥姿態統計"
			+STANCE_D->query_stance_name("defend")+"["+defend+"]、"
			+STANCE_D->query_stance_name("attack")+"["+attack+"]、"
			+STANCE_D->query_stance_name("speed")+"["+speed+"]、"
			+STANCE_D->query_stance_name("medic")+"["+medic+"]、"
			+WHT"無姿態"NOR"["+none+"]。"
		);
	}
}

void remove_battlefield_condition(object player)
{
	player->stop_condition(BATTLEFIELD_EXPERIENCE);
	player->stop_condition(BATTLEFIELD_GLORY);
	player->stop_condition(BATTLEFIELD_FEAR);
	delete_temp("battlefield_captcha_code", player);
	
	if( player->query_stamina_cur() > player->query_stamina_max() || player->query_health_cur() > player->query_health_max() || player->query_energy_cur() > player->query_energy_max() )
		player->set_all_ability_full();
}

int is_battle_open()
{
	return battle_open_flag;	
}

// 參與戰爭
void join_battle(object me)
{
	string id = me->query_id(1);

	if( !battle_open_flag )
		return tell(me, battle_name+"目前無法報名。\n");

	if( !undefinedp(battle_player[id]) )
		return tell(me, pnoun(2, me)+"已經報名參與這次的戰爭。\n");

	battle_player[id] = allocate_mapping(0);
	battle_player[id]["jointime"] = time();

	CHANNEL_D->channel_broadcast("combat", HIR"戰"NOR RED"爭"NOR"：目前共有 "+sizeof(battle_player)+" 位玩家報名參與「"+battle_name+"」");
	show_stance_statistic();

	if( battle_start_flag )
	{
		move_to_random_loc(me);
		tell(me, pnoun(2, me)+"被傳送到戰場中...\n");
	}
}

// 取消參與戰爭
void cancel_battle(object me)
{
	string id = me->query_id(1);

	remove_battlefield_condition(me);

	if( undefinedp(battle_player[id]) )
		return tell(me, pnoun(2, me)+"原本就沒有報名參與這次的戰爭。\n");

	map_delete(battle_player, id);

	CHANNEL_D->channel_broadcast("combat", HIR"戰"NOR RED"爭"NOR"：目前共有 "+sizeof(battle_player)+" 位玩家報名參與「"+battle_name+"」");
	
	if( in_battle(me) )
	{			
		string homefile = query("home", me);
		object home;
			
		if( !stringp(homefile) || !file_exists(homefile) || !objectp(home = load_object(homefile)) || query("owner", home) != id || query("function", home) != "livingroom")
			me->move("/wiz/wizhall/room_wizhall_1");
		else
			me->move(home);
	}
}

void add_npc(string file)
{
	array loc = query_random_loc(BATTLE_X_START, BATTLE_X_END, BATTLE_Y_START, BATTLE_Y_END, BATTLE_AREA, BATTLE_NUM);
	object npc = new(file);

	reset_eval_cost();

	set_temp("mapnpc", 1, npc);
	battle_npc += ({ npc });
	npc->move(loc);	
	broadcast(loc, HIY+npc->query_idname()+HIY"突然從角落竄了出來！\n"NOR);	
}

// 啟動戰爭
void start_battle()
{
	array loc;
	object player;
  object* unavailable_players = allocate(0);
  
	if( sizeof(battle_player) < 5 )
	{
		CHANNEL_D->channel_broadcast("combat", HIR"戰"NOR RED"爭"NOR"：報名參與「"+battle_name+"」戰役的玩家不足 5 人，取消本次戰役");
		return;
	}

	foreach(string file, int count in BATTLE_NPC)
	{
		while(count--)
			add_npc(file);
	}
		
	CHANNEL_D->channel_broadcast("combat", HIR"戰"NOR RED"爭"NOR"：「"+battle_name+"」開始出征！共計 "+sizeof(battle_player)+" 位玩家參與「"+battle_name+"」，計時 "+(BATTLE_TIME/60)+" 分鐘結束");

	foreach(string id, mapping data in battle_player)
	{
		reset_eval_cost();

		if( !objectp(player = find_player(id)) )
			continue;

		if( player->is_flying() )
		{
			tell(player, pnoun(2, player)+"正在飛行中，無法加入戰場。\n");
			unavailable_players |= ({ player });
			continue;
		}
			
		loc = query_random_loc(BATTLE_X_START, BATTLE_X_END, BATTLE_Y_START, BATTLE_Y_END, BATTLE_AREA, BATTLE_NUM);
		
		player->move(loc);
		
		tell(player, "\a「"+battle_name+"」開始，"+pnoun(2, player)+"被傳送到戰場中...\n");
		
		player->remove_follower();
		delete_temp("assisting", player);
	}	
	
	foreach(player in unavailable_players)
		cancel_battle(player);

	battle_start_flag = 1;
	battle_time_countdown = BATTLE_TIME;
}

// 開始接受報名
void start_join()
{
	CHANNEL_D->channel_broadcast("combat", HIR"戰"NOR RED"爭"NOR"：重生的世界發出戰爭集結，即將在 "+(BATTLE_JOIN_TIME/60)+" 分鐘後發動「"+battle_name+"」，請使用 battle 指令加入戰役。");
	
	battle_open_flag = 1;
	
	battle_join_countdown = BATTLE_JOIN_TIME;
	
	set_heart_beat(1);
	
	tell(users(), "\a");
}

int get_player_score(string id)
{
	if( undefinedp(battle_player[id]) ) return 0;
		
	return battle_player[id]["killplayer"]*20 + battle_player[id]["killnpc"] + battle_player[id]["damage"]/50000 + battle_player[id]["bonus"];
}

string* get_sorted_players()
{
	return sort_array(keys(battle_player), (: get_player_score($2) - get_player_score($1) :));
}

void calculate_score()
{
	int score;
	string city;
	mapping cityscore = allocate_mapping(0);
	int rank;
	object home, player;
	string file;
	int insigne_count;
	string *sorted_players = get_sorted_players();
	int lv1,lv2,lv3,lv4;
	int players_count = sizeof(sorted_players);
	
	map_delete(battle_player, 0);

	CHANNEL_D->channel_broadcast("combat", WHT"────────────────────────────────"NOR);

	lv1 = players_count/20;
	if( lv1 < 1 ) lv1 = 1;
		
	lv2 = players_count/4;
	if( lv2 < lv1+2 ) lv2 = lv1+2;
		
	lv3 = to_int(players_count/1.5);
	if( lv3 < lv2+3 ) lv3 = lv2+3;
	
	lv4 = to_int(players_count/1.2);
	if( lv4 < lv3+5 ) lv4 = lv3+5;

	rank = 0;
	foreach(string id in sorted_players)
	{
		if( !objectp(player = find_player(id)) )
			continue;

		city = query("city", player);
		
		score = get_player_score(id);
		
		battle_player[id]["score"] = score;
		
		CHANNEL_D->channel_broadcast("combat", HIR"戰"NOR RED"爭"NOR"："+(++rank)+"."+(CITY_D->query_city_name(city)||"無市藉")+"的"+player->query_idname()+"共擊殺 "+battle_player[id]["killplayer"]+" 位玩家、"+battle_player[id]["killnpc"]+" 個電腦角色，總計造成敵人 "+NUMBER_D->number_symbol(battle_player[id]["damage"])+" 點傷害，戰績結算為 "+NUMBER_D->number_symbol(score)+(battle_player[id]["bonus"]>=0 ? "("NOR CYN"+"HIC+battle_player[id]["bonus"]+NOR")":"("NOR RED"-"HIR+(-battle_player[id]["bonus"])+NOR")")+" 分");
		
		if( undefinedp(cityscore[city]) )
			cityscore[city] = allocate_mapping(0);

		if( cityscore[city]["score"] < score )
			cityscore[city]["score"] = score;

		cityscore[city]["player"] += 1;
		
		addn("combat/battle/score", score, player);
		
		if( score < 25 || battle_player[id]["jointime"] > time() - 600 ) continue;

		insigne_count = 0;

		if( rank <= lv1 )
		{
			player->start_condition(BATTLEFIELD_PLAYER_1, 172800);
			insigne_count = 8;
		}
		else if( rank <= lv2 )
		{
			player->start_condition(BATTLEFIELD_PLAYER_1, 172800);
			insigne_count = 7;
		}
		else if( rank <= lv3 )
		{
			insigne_count = 7;
		}
		else if( rank <= lv4 )
		{
			insigne_count = 6;
		}
		else
		{
			insigne_count = 2;
		}

		if( insigne_count > 0 )
		{
			addn("battle_insigne", insigne_count, player);
			tell(player, pnoun(2, player)+"獲得 "+insigne_count+" 點"HIC"戰"NOR CYN"爭"HIC"勳"NOR CYN"章"NOR"。\n");
		}
		
		player->save();
	}
	
	CHANNEL_D->channel_broadcast("combat", WHT"────────────────────────────────"NOR);
	
	rank = 0;
	foreach(city in sort_array(keys(cityscore), (: $(cityscore)[$2]["score"] - $(cityscore)[$1]["score"] :)))
	{
		CHANNEL_D->channel_broadcast("combat", HIR"戰"NOR RED"爭"NOR"："+(CITY_D->query_city_idname(city)||"無市藉")+"的戰績計算結果為 "+NUMBER_D->number_symbol(cityscore[city]["score"])+"，獲得城市排名第 "+(++rank)+" 名。\n");	
		
		switch(rank)
		{
			case 1:
			case 2:
				switch(random(3))
				{
					case 0:
						CHANNEL_D->channel_broadcast("city", CITY_D->query_city_idname(city)+"所有城市獲得「"BATTLEFIELD_FLOURISH_BUFF"」祝福，繁榮度增加 "HIG"20"NOR GRN"%"NOR"。", city);
						CITY_D->set_city_info(city, BATTLEFIELD_FLOURISH_BUFF, time()+86400);
						break;
					case 1:
						CHANNEL_D->channel_broadcast("city", CITY_D->query_city_idname(city)+"所有城市獲得「"BATTLEFIELD_GROW_BUFF"」祝福，農、牧、養殖產量增加 "HIG"15"NOR GRN"%"NOR"。", city);
						CITY_D->set_city_info(city, BATTLEFIELD_GROW_BUFF, time()+86400);
						break;
					case 2:
						CHANNEL_D->channel_broadcast("city", CITY_D->query_city_idname(city)+"所有城市獲得「"BATTLEFIELD_SHOPPING_BUFF"」祝福，消費總額增加 "HIG"30"NOR GRN"%"NOR"。", city);
						CITY_D->set_city_info(city, BATTLEFIELD_SHOPPING_BUFF, time()+86400);
						break;
				}
				break;
		}
	}
	
	CHANNEL_D->channel_broadcast("combat", WHT"────────────────────────────────"NOR);
	
	foreach(string id in keys(battle_player))
	{
		if( !objectp(player = find_player(id)) )
			continue;

		if( environment(player)->query_area() != BATTLE_AREA || environment(player)->query_num() != BATTLE_NUM )
			continue;
			
		file = query("home", player);
		
		if( !stringp(file) || !file_exists(file) || !objectp(home = load_object(file)) || query("owner", home) != player->query_id(1) || query("function", home) != "livingroom")
			player->move("/wiz/wizhall/room_wizhall_1");
		else
			player->move(home);

		remove_battlefield_condition(player);
		player->stop_condition(DIE);

		tell(player, "「"+battle_name+"」結束，"+pnoun(2, player)+"被傳離戰場...\n");
		
		if( battle_player[id]["jointime"] > time() - 600 ) continue;
			
		if( battle_player[id]["killplayer"] >= 10 )
		{
			player->add_title(HIR"加"NOR RED"害"HIR"者"NOR, "在一場戰爭中擊殺 10 位以上玩家");
			
			if( battle_player[id]["killplayer"] >= 25 )
			{
				player->add_title(HIR"人間"NOR RED"兇器"NOR, "在一場戰爭中擊殺 25 位以上玩家");
		
				if( battle_player[id]["killplayer"] >= 40 )
				{
					player->add_title(HIR"殺"NOR RED"人"HIR"魔"NOR, "在一場戰爭中擊殺 40 位以上玩家");
					
					if( battle_player[id]["killplayer"] >= 60 )
						player->add_title(HIR"嗜"NOR RED"血"HIR"人"NOR RED"魔"NOR, "在一場戰爭中擊殺 60 位以上玩家");
				}
			}
		}
					
		if( battle_player[id]["killnpc"] >= 200 )
		{
			player->add_title(HIR"奪命"NOR RED"追殺"NOR, "在一場戰爭中擊殺 200 個以上電腦角色");
			
			if( battle_player[id]["killnpc"] >= 300 )
			{
				player->add_title(HIR"戰國"NOR RED"無雙"NOR, "在一場戰爭中擊殺 300 個以上電腦角色");
			
				if( battle_player[id]["killnpc"] >= 400 )
					player->add_title(HIR"勇者"NOR RED"無懼"NOR, "在一場戰爭中擊殺 400 個以上電腦角色");
			}	
		}
		
		if( battle_player[id]["dead"] == 0 )
		{
			player->add_title(HIC"適者"NOR CYN"生存"NOR, "在一場戰爭中從未遭到擊殺");
			
			if( sorted_players[0] == id )
				player->add_title(HIY"強者"NOR YEL"無敵"NOR, "在一場戰爭中積分第一並且從未遭到擊殺");
				
			if( battle_player[id]["killplayer"] == 0 )
				player->add_title(HIG"和平"NOR GRN"使者"NOR, "在一場戰爭中未遭到擊殺同時也未曾擊殺其他玩家");
		}
			
		if( battle_player[id]["dead"] >= 10 )
		{
			player->add_title(HIG"被"NOR GRN"害"HIG"人"NOR, "在一場戰爭中死亡 10 次以上");

			if( battle_player[id]["dead"] >= 20 )
				player->add_title(HIM"肉"NOR MAG"醬"NOR, "在一場戰爭中死亡 20 次以上");
		}
			
		if( battle_player[id]["damage"] > 2000000 )
		{
			player->add_title(HIY"致命"NOR YEL"傷害"NOR, "在一場戰爭中總傷害值達 2,000,000 以上");
			
			if( battle_player[id]["damage"] > 3000000 )
			{
				player->add_title(HIY"暴力"NOR YEL"份子"NOR, "在一場戰爭中總傷害值達 3,000,000 以上");
				
				if( battle_player[id]["damage"] > 4000000 )
				{
					player->add_title(HIY"全對"NOR YEL"失控"NOR, "在一場戰爭中總傷害值達 4,000,000 以上");
					
					if( battle_player[id]["damage"] > 5000000 )
						player->add_title(HIY"恐怖"NOR YEL"主義"NOR, "在一場戰爭中總傷害值達 5,000,000 以上");
				}
			}
		}
		
		if( battle_player[id]["score"] > 1000 )
		{
			player->add_title(HIY"爭戰"NOR YEL"天下"NOR, "在一場戰爭中總分達 1,000 分以上");
			
			if( battle_player[id]["score"] > 2000 )
				player->add_title(HIY"戰"NOR YEL"爭"HIY"之"NOR YEL"王"NOR, "在一場戰爭中總分達 2,000 分以上");
		}

		player->save();
	}

	battle_player = allocate_mapping(0);
}

void stop_battle()
{
	object player;
	set_heart_beat(0);

	foreach(string id in keys(battle_player))
	{
		if( !objectp(player = find_player(id)) )
			continue;

		COMBAT_D->stop_fight(player);
		
		remove_battlefield_condition(player);
	}

	foreach(object npc in battle_npc)
		destruct(npc);

	battle_open_flag = 0;
	battle_start_flag = 0;
	battle_time_countdown = 0;
	
	call_out((: calculate_score :), 30);
		
	CHANNEL_D->channel_broadcast("combat", WHT"────────────────────────────────"NOR);
	CHANNEL_D->channel_broadcast("combat", HIR"戰"NOR RED"爭"NOR"：「"+battle_name+"」終於漸漸平息...");
	CHANNEL_D->channel_broadcast("combat", HIR"戰"NOR RED"爭"NOR"：正在統計戰績...");
	CHANNEL_D->channel_broadcast("combat", WHT"────────────────────────────────"NOR);
}

void add_damage_score(object me, int score)
{
	string id = me->query_id(1);

	if( undefinedp(battle_player[id]) )
		return;

	battle_player[id]["damage"] += score;
}

void add_killnpc_score(object me, int score)
{
	string id = me->query_id(1);
		
	if( undefinedp(battle_player[id]) )
		return;

	battle_player[id]["killnpc"] += score;
	
	me->start_condition(BATTLEFIELD_EXPERIENCE);
}

void add_dead_score(object me, int score)
{
	string id = me->query_id(1);
	string *sorted_players = get_sorted_players();
	
	if( undefinedp(battle_player[id]) )
		return;

	if( sizeof(sorted_players) > 0 && sorted_players[0] == id )
		battle_player[id]["bonus"] -= 40;
	
	battle_player[id]["dead"] += score;	
}

void add_bonus_buff(object me, int force)
{
	if( !in_battle(me) ) return;

	if( force || battle_time_countdown < BATTLE_TIME - 420 )
	{
		me->start_condition(BATTLEFIELD_EXPERIENCE);
		me->start_condition(BATTLEFIELD_EXPERIENCE);
		me->start_condition(BATTLEFIELD_EXPERIENCE);
	}
}

void add_killplayer_score(object me, int score)
{
	string id = me->query_id(1);

	if( undefinedp(battle_player[id]) )
		return;

	battle_player[id]["killplayer"] += score;
}
void add_bonus_score(object me, int score)
{
	string id = me->query_id(1);

	if( undefinedp(battle_player[id]) )
		return;

	battle_player[id]["bonus"] += score;	
}

mapping query_battle_player()
{
	return battle_player;
}

int remove()
{
	if( sizeof(battle_player) )
	{
		object player;
		
		CHANNEL_D->channel_broadcast("combat", HIR"戰"NOR RED"爭"NOR"：系統更新，重置「"+battle_name+"」");

		foreach(string id in keys(battle_player))
		{
			if( !objectp(player = find_player(id)) )
				continue;
			
			COMBAT_D->stop_fight(player);
			remove_battlefield_condition(player);
			player->stop_condition(DIE);
		}
	}
	foreach(object npc in battle_npc)
		destruct(npc);

	return 1;
}


void refresh_battlefield_map(string area, int num)
{
	int x, y;
	array loc;
	object room;
	string path = AREA_NUM_ROOM(area, num);

	foreach(string file in get_dir(path))
	{
		room = load_object(path+file[0..<3]);
		
		destruct(room);
		
		rm(path+file);
	}
	
	for(x=0;x<100;++x)
	for(y=0;y<100;++y)
	{
		loc = arrange_area_location(x, y, area, num);

		if( AREA_D->query_coor_data(loc, ROOM) )
		{
			AREA_D->delete_coor_data(loc);
			AREA_D->delete_coor_icon(loc);
		}
	}
}
					
void heart_beat()
{
	array loc;
	string *directions;

	if( battle_join_countdown > 0 )
	{
		switch(--battle_join_countdown)
		{
			case 0:
				start_battle();
				break;
			case 60:
			case 120:
			case 180:
			case 300:
			case 600:
			case 900:
			case 1200:
				CHANNEL_D->channel_broadcast("combat", HIR"戰"NOR RED"爭"NOR"：重生的世界發出戰爭集結，即將在 "+(battle_join_countdown/60)+" 分鐘後發動「"+battle_name+"」，請使用 battle 指令加入戰役。");
				break;
		}
		
		return;
	}
	
	if( !battle_start_flag )
		return;

	if( --battle_time_countdown <= 0 )
	{
		stop_battle();
		return;
	}

	if( battle_time_countdown == 60 || battle_time_countdown == 180 )
		CHANNEL_D->channel_broadcast("combat", HIR"戰"NOR RED"爭"NOR"：「"+battle_name+"」即將在 "+(battle_time_countdown/60)+" 分鐘後結束...");
		
	if( !(battle_time_countdown%90) )
	{
		object statue;
		
		if( !random(2) )
			statue = new("/obj/battlefield/battlefield_glory_statue");
		else
			statue = new("/obj/battlefield/battlefield_fear_statue");
		
		loc = query_random_loc(BATTLE_X_START+8, BATTLE_X_END-8, BATTLE_Y_START+8, BATTLE_Y_END-8, BATTLE_AREA, BATTLE_NUM);
		
		statue->move(loc);
		
		loc[X] += -7 + random(15);
		loc[Y] += -7 + random(15);

		CHANNEL_D->channel_broadcast("combat", HIR"戰"NOR RED"爭"NOR"：具有強大神秘力量的石碑出現在"+loc_short(loc)+"附近...");
	}
	
	if( !(battle_time_countdown%60) )
	{
		object player;
		string* sorted_players = get_sorted_players();
		int size = sizeof(sorted_players);
	
		for(int i=size*2/3;i<size;++i)
		{
			if( objectp(player = find_player(sorted_players[i])) )
			{
				tell(player, pnoun(2, player)+"目前的戰績落後，獲得額外能力加持。\n");
	
				player->start_condition(BATTLEFIELD_EXPERIENCE);
				player->start_condition(BATTLEFIELD_EXPERIENCE);
				
				if( i > size*4/5 )
				{
					player->start_condition(BATTLEFIELD_EXPERIENCE);
					player->start_condition(BATTLEFIELD_EXPERIENCE);
				}
			}
		}
	}

	foreach(object npc in battle_npc)
	{	
		if( !objectp(npc) )
			continue;
			
		if( !random(10) )
		{
			if( COMBAT_D->in_fight(npc) )
				continue;
		
			directions = VALID_DIRECTION;

			loc = query_temp("location", npc);
			
			if( !arrayp(loc) ) continue;
			
			foreach(string direction in VALID_DIRECTION)
				if( !MAP_D->valid_move(npc, direction, loc) )
					directions -= ({ direction });
			
			if( sizeof(directions) )
				npc->do_command("go "+directions[random(sizeof(directions))]);
		}
	}
	
	battle_npc -= ({ 0 });
}

void create()
{
	
	
}

string query_name()
{
	return "戰場系統(BATTLEFIELD_D)";
}
