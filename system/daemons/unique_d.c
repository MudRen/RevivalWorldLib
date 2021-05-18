/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : unique_d.c
 * Author : Clode@RevivalWorld
 * Date   : 2006-03-11
 * Note   : �ߤ@���F
 * Update :
 *  o 2002-09-26 
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <daemon.h>

#define DATA		"/data/daemon/unique.o"

private mapping unique;

private nosave mapping items = 
([
	"/obj/systemproducts/pill_of_lost_strength" : 290000,
	"/obj/systemproducts/pill_of_lost_charisma" : 250000,
	"/obj/systemproducts/pill_of_lost_intelligence" : 400000,
	"/obj/systemproducts/pill_of_lost_physique" : 330000,
	"/obj/systemproducts/pill_of_lost_agility" : 370000,
	"/obj/systemproducts/pill_of_deadly_contract" : 270000,
	"/obj/systemproducts/pill_of_extreme_silence" : 350000,
	"/obj/systemproducts/pill_of_lost_stamina" : 360000,
	"/obj/systemproducts/pill_of_lost_health" : 310000,
	"/obj/systemproducts/pill_of_lost_energy" : 340000,
	"/obj/systemproducts/taichi" : 220000,
	"/obj/systemproducts/architectonic_high" : 220000,
]);

int save()
{
	save_object(DATA);
} 

void remove_item(string basename)
{
	map_delete(unique, basename);
	
	save();
}

int is_unique(string basename)
{
	return !undefinedp(unique[basename]);	
}

mapping query_unique()
{
	return unique;
}

void heart_beat()
{
	foreach(string basename, int time in unique)
	{	
		if( (unique[basename] -= 60) < 0 )
			map_delete(unique, basename);
	}
	
	foreach(string basename, int time in items)
	{
		if( undefinedp(unique[basename]) )
		{	
			mapping auc_data = allocate_mapping(0);
			object ob = load_object(basename);

			auc_data["seller"] = "";
			auc_data["basename"] = basename;
			auc_data["amount"] = 1;
			auc_data["minute"] = 120;
			auc_data["base_price"] = 100000000;
			auc_data["direct_price"] = 9999999999999;
			
			AUCTION_D->add_auction(auc_data);
			
			CHANNEL_D->channel_broadcast("auc", HIY"�������������G"NOR"�u"+QUANTITY_D->obj(ob, 1, 1)+"�v�A�����G"HIY+money(MONEY_D->query_default_money_unit(), auc_data["base_price"])+NOR"�A�����ʶR���G"HIY+money(MONEY_D->query_default_money_unit(), auc_data["direct_price"])+NOR"�A�Ѿl�ɶ��G"HIR+auc_data["minute"]+NOR" ����");
	
			unique[basename] = time + random(86400);
	
			break;
		}
	}
	
	save();
}

void create()
{
	if( !restore_object(DATA) )
	{
		unique = allocate_mapping(0);

		save();
	}

	set_heart_beat(60);
}

string query_name()
{
	return "�ߤ@�t��(UNIQUE_D)";
}
