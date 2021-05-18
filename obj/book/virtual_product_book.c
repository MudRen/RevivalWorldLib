/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : virtual_product_book.c
 * Author : Clode@RevivalWorld
 * Date   : 2006-03-15
 * Note   : 產品配方
 * Update :
 *  o 2003-00-00  

 -----------------------------------------
 */

#include <inherit.h>
#include <ansi.h>
#include <skill.h>

inherit STANDARD_OBJECT;

void create(string product)
{
	string module = "/std/module/product/"+product+"_module.c";
	mapping product_info;
	
	if( !product || !module || !load_object(module) ) return;
		
	product_info = module->query_product_info();

	set_idname(HIW+product_info["id"]+NOR CYN" plan"+NOR, HIW+product_info["name"]+HIC"產品"NOR CYN"配方"NOR);
	set_temp("status", HIC"稀"NOR CYN"有");

	if( this_object()->set_shadow_database() ) return;
	
	set("long", "一本極為稀有的"+product_info["name"]+"("+capitalize(product_info["id"])+")產品配方，您可以拿這份配方到研發中心進行產品開發。\n  ");
	set("unit", "張");
	set("mass", 100);
	set("value", 10000);
	set("badsell", 1);
}
