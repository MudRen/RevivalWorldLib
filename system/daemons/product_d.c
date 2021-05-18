/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : product_d.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-05-03
 * Note   : 產品精靈
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <map.h>
#include <daemon.h>
#include <ansi.h>
#include <terrain.h>

#define DATA_PATH			"/data/daemon/product.o"
#define PRODUCT_MODULE_PATH		"/std/module/product/"

mapping products;
mapping supply_and_demand;

private nosave string *world_product =
({
	"/obj/materials/metal",
	"/obj/materials/stone",
	"/obj/materials/water",
	"/obj/materials/wood",
	"/obj/materials/fuel",

	"/obj/fishing/rod/coarse_rod",

	"/obj/fishing/bait/bread_bug",
	"/obj/fishing/bait/prawn",
	"/obj/fishing/bait/tsou_chip",
	"/obj/fishing/bait/worm",

	"/obj/fishing/fish/fish0",
	"/obj/fishing/fish/fish1",
	"/obj/fishing/fish/fish2",
	"/obj/fishing/fish/fish3",
	"/obj/fishing/fish/fish4",
	"/obj/fishing/fish/fish5",
	"/obj/fishing/fish/fish6",
	"/obj/fishing/fish/fish7",
	"/obj/fishing/fish/fish8",
	
	"/obj/fishing/adv_fish/fish0",
	"/obj/fishing/adv_fish/fish1",
	"/obj/fishing/adv_fish/fish2",
	"/obj/fishing/adv_fish/fish3",

	"/obj/fishing/sp_fish/fish0",
	"/obj/fishing/sp_fish/fish1",
	"/obj/fishing/sp_fish/fish2",

	"/obj/fishing/other/box",
	
	"/obj/collection/tool/coarse_pickax",
	"/obj/collection/tool/coarse_drill",
	"/obj/collection/tool/coarse_bucket",
	"/obj/collection/tool/coarse_saw",
	"/obj/collection/tool/coarse_shovel",

	"/obj/etc/newspaper",
	
	"/obj/gem/gem0",
	"/obj/gem/gem1",
	"/obj/gem/gem2",
	"/obj/gem/gem3",
	"/obj/gem/gem4",
	"/obj/gem/gem5",
	"/obj/gem/gem6",
	
	"/obj/farming/seed/herbage_seed",
	"/obj/farming/seed/corn_seed",
	"/obj/farming/seed/cotton_seed",
	"/obj/farming/seed/rice_seedling",
	"/obj/farming/seed/barley_seedling",
	"/obj/farming/seed/wheat_seedling",
	"/obj/farming/seed/herb_seed",
	"/obj/farming/seed/rose_seed",
	"/obj/farming/seed/sunflower_seed",
	"/obj/farming/seed/lily_seed",

	"/obj/farming/product/herbage",
	"/obj/farming/product/corn",
	"/obj/farming/product/rice",
	"/obj/farming/product/barley",
	"/obj/farming/product/cotton",
	"/obj/farming/product/wheat",
	"/obj/farming/product/herb",
	"/obj/farming/product/sunflower",
	"/obj/farming/product/rose",
	"/obj/farming/product/lily",

	"/obj/pasture/breed/chicken_stud",
	"/obj/pasture/breed/pig_stud",
	"/obj/pasture/breed/sheep_stud",
	"/obj/pasture/breed/cattle_stud",
	"/obj/pasture/breed/deer_stud",

	"/obj/pasture/product/chicken",
	"/obj/pasture/product/egg",
	"/obj/pasture/product/pork",
	"/obj/pasture/product/mutton",
	"/obj/pasture/product/wool",
	"/obj/pasture/product/horseflesh",
	"/obj/pasture/product/beef",
	"/obj/pasture/product/raw_milk",
	"/obj/pasture/product/venison",
	"/obj/pasture/product/antler",
	"/obj/pasture/product/leather",
	
	"/obj/fishfarm/breed/clam",
	"/obj/fishfarm/breed/young_crab",
	"/obj/fishfarm/breed/young_shrimp",

	"/obj/fishfarm/product/big_shrimp",
	"/obj/fishfarm/product/big_crab",
	"/obj/fishfarm/product/pearl",
	
	"/obj/systemproducts/flatbed_tricycle",
	"/obj/systemproducts/dark_plum",
	"/obj/systemproducts/pure_water",
	"/obj/systemproducts/steamed_roll",
	"/obj/systemproducts/fortune_bag",
	"/obj/systemproducts/pill_of_strength",
	"/obj/systemproducts/pill_of_physique",
	"/obj/systemproducts/pill_of_intelligence",
	"/obj/systemproducts/pill_of_agility",
	"/obj/systemproducts/pill_of_charisma",
	"/obj/systemproducts/pill_of_invincibility",
	"/obj/systemproducts/pill_of_purification",
});

int save()
{
	return save_object(DATA_PATH);	
}

int remove()
{
	return save();
}

string *query_world_product()
{
	return world_product;
}

// 取得可以生產的產品 module 物件陣列
string *query_available_module_products()
{
	string file;
	string *types;
	object module;
	
	types = allocate(0);
	
	foreach(file in get_dir(PRODUCT_MODULE_PATH))
	{
		if( file[<9..<1] != "_module.c" ) continue;

		catch( module = load_object(PRODUCT_MODULE_PATH+file) );
		
		if( !objectp(module) ) continue;
		
		sscanf(file, "%s_module.c", file);
		types += ({ file });
	}
	
	return types;
}

void count_product_manufacture(string file, int amount)
{
	object ob;
	string owner;

	products[file] += amount;
	
	if( catch(ob = load_object(file)) || !objectp(ob) ) return;
	
	owner = query("design/owner", ob);

	if( SECURE_D->is_wizard(owner) ) return;

	TOP_D->update_top("products", file, products[file]*query("value", ob), ob->query_idname(), owner, products[file]);
}

void refresh_product_top()
{
	foreach(string file, int amount in products)
		count_product_manufacture(file, 0);
}

int manufactured_product(string file)
{
	return !undefinedp(products[file]);
}

mapping query_products()
{
	return products;
}

void create()
{
	if( !restore_object(DATA_PATH) )
	{
		products = allocate_mapping(0);
		save();
	}
}

void count_supply_and_demand(string product, int amount)
{
	if( !mapp(supply_and_demand) )
		supply_and_demand = allocate_mapping(0);

	supply_and_demand[product] += amount;
}

varargs mixed query_supply_and_demand(string product)
{
	if( undefinedp(product) )
		return supply_and_demand;
	else
		return supply_and_demand[product];
}

void clear_supply_and_demand()
{
	supply_and_demand = allocate_mapping(0);
	save();
}

string query_name()
{
	return "產品系統(PRODUCT_D)";
}
