/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : top_d.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-06-28
 * Note   : 
 * Update :
 *  o 2002-00-00
 *
 -----------------------------------------
 */
#include <ansi.h>
#include <daemon.h>

#define DATA_PATH	"/data/daemon/top.o"


#define TOP_MAX		1100
#define TOP_BUF		100

#define TOP_ID		0
#define TOP_NUMBER	1
#define TOP_TIME	2

mapping top = allocate_mapping(0);

/* TOP 資料格式
top = ([
	排行榜類型1 : ({ // 依數字排列
			({ 代號1, 數字, 時間, 參數 ... }), // TOP 1
			({ 代號2, 數字, 時間, 參數 ... }), // TOP 2
			({ 代號3, 數字, 時間, 參數 ... }), // TOP 3
			// ...
		}),
	排行榜類型2 : ({ // 依數字排列
			({ 代號1, 數字, 時間, 參數 ... }), // TOP 1
			({ 代號2, 數字, 時間, 參數 ... }), // TOP 2
			({ 代號3, 數字, 時間, 參數 ... }), // TOP 3
			// ...
		}),
]);
*/

int save()
{
	return save_object(DATA_PATH);	
}

void create()
{
	if( !restore_object(DATA_PATH) )
		save();	
}

int remove()
{
	return save();
}

varargs mixed query_top(string key, int rank)
{
	if( key )
	{
		if( rank )
		{
			if( rank < 0 || rank > sizeof(top[key]) ) return;
			return top[key][rank-1];
		}
		if( undefinedp(top[key]) ) return allocate(0);
		return top[key][0..TOP_MAX-TOP_BUF-1];
	}
	return top;

}

// 檢查 TOP 是否存在
int top_exists(string key, string id)
{
	if( undefinedp(top[key]) ) return 0;

	foreach(array data in top[key])
	if( data[TOP_ID] == id )
		return 1;

	return 0;
}

// 刪除 TOP 資料
varargs void delete_top(string key, string id)
{
	int pos, top_size;

	if( undefinedp(id) )
	{
		map_delete(top, key);
		return;
	}
	if( undefinedp(top[key]) ) return;

	top_size = sizeof(top[key]);
	for(pos=0; pos<top_size; pos++)
	{
		if( top[key][pos][TOP_ID] == id )
			top[key][pos] = 0;
	}
	top[key] -= ({ 0 });

	return;
}

void delete_top_id(string id)
{
	foreach(string key, array data in top)
	delete_top(key, id);
}

// 進行排序動作
varargs void sort_top(mixed key)
{
	reset_eval_cost();

	if( undefinedp(key) )
	{
		foreach(key, array data in top)
		{
			top[key] = sort_array(top[key], (: $1[TOP_NUMBER] < $2[TOP_NUMBER] || ($1[TOP_NUMBER] == $2[TOP_NUMBER] && $1[TOP_TIME] > $2[TOP_TIME]) ? 1 : -1 :));
			top[key] = top[key][0..TOP_MAX-1];
		}
		return;
	}
	else if( undefinedp(top[key]) ) 
		return;

	top[key] = sort_array(top[key], (: $1[TOP_NUMBER] < $2[TOP_NUMBER] || ($1[TOP_NUMBER] == $2[TOP_NUMBER] && $1[TOP_TIME] > $2[TOP_TIME]) ? 1 : -1 :));
	top[key] = top[key][0..TOP_MAX-1];
}

// 更新 TOP 資料
varargs void update_top(string key, string id, mixed val, mixed arg ...)
{
	if( !key || !id )
		return;

	val = to_int(big_number_check(val));
		
	if( undefinedp(top[key]) ) top[key] = allocate(0);

	delete_top(key, id);

	top[key] += ({ ({ id, val, time() }) + (arg ? arg: allocate(0)) });
	sort_top(key);

	// 有些資料需要立即儲存, 避免意外狀況導致資料遺失
	switch(key)
	{
		case "building":
			save();
			break;
		default:
			break;
	}

	return;
}

/* Feature */
void update_top_rich(object ob)
{
	string id, idname, city;
	mapping stockdata;
	int all_cash, all_estate, all_stock, assets;

	if( !ob || !ob->is_user_ob() ) return;

	id = ob->query_id(1);

	if( SECURE_D->is_wizard(id) ) return;

	idname = ob->query_idname();
	city = query("city", ob);
	stockdata = query("stock", ob);
	
	if( mapp(stockdata) && sizeof(stockdata) )
	{
		mapping stocks = STOCK_D->query_stocks();
		foreach(string stock, mapping data in stockdata) {
			if( undefinedp(stocks[to_int(stock)]) ) continue;
				all_stock += data["amount"] * 10000 * stocks[to_int(stock)]["收盤"];
		}
	}

	all_cash = MONEY_D->query_all_cash(id);
	all_estate = ESTATE_D->query_all_estate_value(id, 1);

	assets = all_cash + all_estate + all_stock;

	update_top("cash", id, all_cash, idname, city);
	update_top("estate", id, all_estate, idname, city);
	update_top("rich", id, assets, idname, city);
	update_top("stockvalue", id, all_stock, idname, city);

	MONEY_D->set_assets(id, assets);

	set("totalassets", assets, ob);
}

/* Feature */
void update_top_social_exp(object ob)
{
	string id;

	if( !ob || !ob->is_user_ob() ) return;

	id = ob->query_id(1);

	if( SECURE_D->is_wizard(id) ) return;

	update_top("social_exp", id, query("exp/social/tot", ob), ob->query_idname(), query("city", ob));
}


/* Feature */
void update_top_combat_exp(object ob)
{
	string id;

	if( !ob || !ob->is_user_ob() ) return;

	id = ob->query_id(1);

	if( SECURE_D->is_wizard(id) ) return;

	update_top("combat_exp", id, query("exp/combat/tot", ob), ob->query_idname(), query("city", ob));
}

void update_top_skill(object ob)
{
	string id, idname, city;
	int fullskills;
	mapping skills;

	if( !ob || !ob->is_user_ob() ) return;

	if( !mapp(skills = query("skills", ob)) ) return;

	id = ob->query_id(1);

	if( SECURE_D->is_wizard(id) ) return;

	idname = ob->query_idname();
	city = query("city", ob);	

	foreach(string skillname, mapping data in skills)
	{
		/* 於升降級時處理
		if( !arrayp(top["skill/"+skillname]) )
			top["skill/"+skillname] = allocate(0);

		update_top("skill/"+skillname, id, data["level"], idname, city);
		*/

		if( data["level"] == 100 )
			fullskills++;
	}

	update_top("skill", id, fullskills, idname, city);
}

void update_top_combat(object ob)
{
	string id;
	string city;
	string idname;

	if( !ob || !ob->is_user_ob() ) return;

	id = ob->query_id(1);

	if( SECURE_D->is_wizard(id) ) return;

	city = query("city", ob);
	idname = ob->query_idname();

	update_top("combat_kill_win", id, copy(query("combat/kill/win", ob)), idname, city);
	update_top("combat_kill_winplayer", id, copy(query("combat/kill/winplayer", ob)), idname, city);
	update_top("combat_kill_lose", id, copy(query("combat/kill/lose", ob)), idname, city);
	
	if( BATTLEFIELD_D->in_battle(ob) ) return;

	update_top("combat_battle_score", id, copy(query("combat/battle/score", ob)), idname, city);
	update_top("combat_attack", id, COMBAT_D->attack_value(ob), idname, city);
	update_top("combat_defend", id, COMBAT_D->defend_value(ob), idname, city);
	update_top("combat_speed", id, COMBAT_D->speed_value(ob), idname, city);
}

void update_top_attr(object ob)
{
	string id, idname, city;

	if( !ob || !ob->is_user_ob() ) return;

	if( BATTLEFIELD_D->in_battle(ob) ) return;

	id = ob->query_id(1);

	if( SECURE_D->is_wizard(id) ) return;

	idname = ob->query_idname();
	city = query("city", ob);	

	update_top("attr/strength", id, ob->query_str(), idname, city);
	update_top("attr/physique", id, ob->query_phy(), idname, city);
	update_top("attr/intelligence", id, ob->query_int(), idname, city);
	update_top("attr/agility", id, ob->query_agi(), idname, city);
	update_top("attr/charisma", id, ob->query_cha(), idname, city);
	update_top("attr/stamina", id, ob->query_stamina_max(), idname, city);
	update_top("attr/health", id, ob->query_health_max(), idname, city);
	update_top("attr/energy", id, ob->query_energy_max(), idname, city);
}

void update_top_age(object ob)
{
	string id;

	if( !ob || !ob->is_user_ob() ) return;

	id = ob->query_id(1);

	if( SECURE_D->is_wizard(id) ) return;

	update_top("age", id, query("total_online_time", ob), ob->query_idname(), query("city", ob));
}

void update_top_futures(object ob)
{
	string id;
	int total_futures_gain;
	
	if( !ob || !ob->is_user_ob() ) return;
		
	total_futures_gain = query("total_futures_gain", ob);
	
	id = ob->query_id(1);
	
	if( SECURE_D->is_wizard(id) ) return;

	if( total_futures_gain < -100000000000 && !ob->have_title(HIY"斷頭"NOR YEL"初學"NOR) )
		ob->add_title(HIY"斷頭"NOR YEL"初學"NOR);
	if( total_futures_gain < -200000000000 && !ob->have_title(HIY"斷頭"NOR YEL"學家"NOR) )
		ob->add_title(HIY"斷頭"NOR YEL"學家"NOR);
	if( total_futures_gain < -400000000000 && !ob->have_title(HIY"斷頭"NOR YEL"大師"NOR) )
		ob->add_title(HIY"斷頭"NOR YEL"大師"NOR);
	if( total_futures_gain < -600000000000 && !ob->have_title(HIY"斷頭"NOR YEL"天神"NOR) )
		ob->add_title(HIY"斷頭"NOR YEL"天神"NOR);

	if( total_futures_gain > 100000000000 && !ob->have_title(HIY"期貨"NOR YEL"初學"NOR) )
		ob->add_title(HIY"期貨"NOR YEL"初學"NOR);
	if( total_futures_gain > 200000000000 && !ob->have_title(HIY"期貨"NOR YEL"學家"NOR) )
		ob->add_title(HIY"期貨"NOR YEL"學家"NOR);
	if( total_futures_gain > 400000000000 && !ob->have_title(HIY"期貨"NOR YEL"大師"NOR) )
		ob->add_title(HIY"期貨"NOR YEL"大師"NOR);
	if( total_futures_gain > 600000000000 && !ob->have_title(HIY"期貨"NOR YEL"元老"NOR) )
		ob->add_title(HIY"期貨"NOR YEL"元老"NOR);
	if( total_futures_gain > 800000000000 && !ob->have_title(HIY"期貨"NOR YEL"尊者"NOR) )
		ob->add_title(HIY"期貨"NOR YEL"尊者"NOR);
	if( total_futures_gain > 1000000000000 && !ob->have_title(HIY"期貨"NOR YEL"之王"NOR) )
		ob->add_title(HIY"期貨"NOR YEL"之王"NOR);
	if( total_futures_gain > 1500000000000 && !ob->have_title(HIY"期貨"NOR YEL"聖祖"NOR) )
		ob->add_title(HIY"期貨"NOR YEL"聖祖"NOR);
	if( total_futures_gain > 2000000000000 && !ob->have_title(HIY"期貨"NOR YEL"天神"NOR) )
		ob->add_title(HIY"期貨"NOR YEL"天神"NOR);

	if( total_futures_gain != 0 )
		update_top("futures", id, total_futures_gain, ob->query_idname(), query("city", ob));
	else
		delete_top("futures", id);
}

void calculate_top(object me)
{
	update_top_attr(me);
	update_top_skill(me);
	update_top_rich(me);
	update_top_social_exp(me);
	update_top_combat_exp(me);
	update_top_combat(me);
	update_top_age(me);
}

void switch_top(string key_from, string key_to)
{
	top[key_to] = copy(top[key_from]);	
	map_delete(top, key_from);
}

string query_name()
{
	return "排行榜系統(TOP_D)";
}

string *query_top_key()
{
	return keys(top);
}


