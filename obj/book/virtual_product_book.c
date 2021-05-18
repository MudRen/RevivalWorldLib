/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : virtual_product_book.c
 * Author : Clode@RevivalWorld
 * Date   : 2006-03-15
 * Note   : ���~�t��
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

	set_idname(HIW+product_info["id"]+NOR CYN" plan"+NOR, HIW+product_info["name"]+HIC"���~"NOR CYN"�t��"NOR);
	set_temp("status", HIC"�}"NOR CYN"��");

	if( this_object()->set_shadow_database() ) return;
	
	set("long", "�@�������}����"+product_info["name"]+"("+capitalize(product_info["id"])+")���~�t��A�z�i�H���o���t����o���߶i�沣�~�}�o�C\n  ");
	set("unit", "�i");
	set("mass", 100);
	set("value", 10000);
	set("badsell", 1);
}
