/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : char_d.c
 * Author : Clode@RevivalWorld
 * Date   : 2000-12-29
 * Note   : 角色精靈
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <daemon.h>
#include <gender.h>

mapping DEFAULT_ATTRIBUTION = 
([
	"age"			: 15,
	"attr/str"		: 10,
	"attr/phy"		: 10,
	"attr/int"		: 10,
	"attr/agi"		: 10,
	"attr/cha"		: 10,
	"abi/stamina/max"	: 500,
	"abi/stamina/cur"	: 500,
	"abi/health/max"	: 500,
	"abi/health/cur"	: 500,
	"abi/energy/max"	: 500,
	"abi/energy/cur"	: 500,
	"exp/social/tot"	: 0,
	"exp/social/cur"	: 0,
	"exp/combat/tot"	: 0,
	"exp/combat/cur"	: 0,
	"stat/drink/max"	: 100,
	"stat/drink/cur"	: 0,
	"stat/food/max"		: 100,
	"stat/food/cur"		: 0
]);

void create_char(object user, int gender)
{
	if( previous_object() != find_object(PPL_LOGIN_D) ) return;

	// 性別
	set("gender", gender, user);
	
	foreach(string attr, int value in DEFAULT_ATTRIBUTION)
	{
		if( !query(attr, user) )
			set(attr, value, user);
	}
	
	set("createtime", time(), user);

	// 設定標準頻道
	set("channels", CHANNEL_D->query_default_channel(user->query_id(1)), user);

	// 標準暫存訊息
	set("msg/chat", 1, user);
	set("msg/rumor", 1, user);
	set("msg/tell", 1, user);
	set("msg/say", 1, user);
	set("msg/emotion", 1, user);
}

void create_npc(object npc)
{
	if( !npc->is_npc() ) return;
	
	foreach(string attr, int value in DEFAULT_ATTRIBUTION)
	{
		if( !query(attr, npc) )
			set(attr, value, npc);
	}
}

int destruct_char(string id)
{
	string path = user_path(id);
	string backup_path;
	
	if( file_size(path) != -2 ) return 0;
	
	// 刪除勞工資訊
	LABOR_D->del_labors(id);

	// 移除密碼資訊
	PASSWORD_D->del_password(id);
	
	// 移除房地產資訊
	ESTATE_D->remove_estate(id);
	
	// 移除金錢資訊
	MONEY_D->clear_money(id);

	// 移除市民資訊
	CITY_D->remove_citizen(id);
	
	// 刪除排行榜紀錄
	TOP_D->delete_top_id(id);
	
	// 註銷企業成員
	ENTERPRISE_D->unregister_member(id);

	// 刪除期貨資料
	FUTURES_D->remove_positions(id);
	FUTURES_D->remove_orders(id);

	// 刪除棒球球隊資料
	BASEBALL_D->cancel_game(id);
	BASEBALL_D->leave_season(id);
	BASEBALL_D->delete_setup(id);

	backup_path = user_deleted_path(id);
	intact_path(backup_path);
	
	rmtree(user_backup_path(id));
	rmtree(user_deleted_path(id));
	
	return !rename(path, backup_path);
}

void create()
{

}

string query_name()
{
	return "角色系統(CHAR_D)";
}
