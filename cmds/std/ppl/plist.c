/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : plist.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-04-06
 * Note   : 玩家指令程式
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <skill.h>
#include <feature.h>
#include <daemon.h>

#define PRODUCT_MODULE_PATH "/std/module/product/"

inherit COMMAND;

string help = @HELP
工廠可生產的產品列表

plist			- 列出所有可生產產品列表
plist '產品ID'		- 詳細列出某項產品的生產資料
plist product		- 列出您已研發設計出來的所有產品
plist product '編號'	- 列出某個研發設計產品的詳細資料

相關指令: blist
HELP;

string *sorted_factory = ({
	"metal",
	"stone",
	"water",
	"wood",
	"fuel",
	
	"food",
	"drink",
	"clothing",
	"furniture",
	"hardware",
	"chemical",
	"machinery",
	"electrics",
	"transportation",
	"medicine",
	"adventure",
	"shortrange",
	"armor",
	"perfume",
	
	"instrument",
	"entertainment",
	"longrange",
	"heavyarmor",
	"magic",
	"aircraft",
});

private void do_command(object me, string arg)
{
	int number;
	mapping info, subinfo;
	object materialob;
	object target;
	object ob;
	string msg;
	string *products = allocate(0);
	int value;

	foreach( string file in get_dir(PRODUCT_MODULE_PATH) )
	{
		if( sscanf(file, "%s_module.c", file) == 1 )
			products |= ({ file });
	}
	
	products = sort_array(products, (: 
		member_array((PRODUCT_MODULE_PATH+$1+"_module.c")->query_product_info()["factory"], sorted_factory) - 
		member_array((PRODUCT_MODULE_PATH+$2+"_module.c")->query_product_info()["factory"], sorted_factory) ||
		query("value", load_object(PRODUCT_MODULE_PATH+$1+"_object.c")) - query("value", load_object(PRODUCT_MODULE_PATH+$2+"_object.c"))
		 :));

	if( !arg || objectp(target = find_player(arg)) )
	{
		string module;
		mapping inlay_effect;

		msg = "產品列表，目前共可生產 "+sizeof(products)+" 種產品\n";
		msg += WHT"─────────────────────────────────────\n"NOR;
		msg +=    "名稱                          生產工廠     鑲嵌   估計耗時     最低價值\n"NOR;
		msg += WHT"─────────────────────────────────────\n"NOR;
		
		foreach(string product in products)
		{
			module = PRODUCT_MODULE_PATH+product+"_module.c";
			info = module->query_product_info();
			ob = load_object(PRODUCT_MODULE_PATH+product+"_object.c");
			value = query("value", ob);
			inlay_effect = fetch_variable("inlay_effect", load_object(module));
			
			info["timecost"] = to_int(pow(value, 0.32));
	
			msg += sprintf("%-30s%-12s  %-5s "HIG"%-8s     "HIY"%s"NOR"\n",
				info["name"]+"("+capitalize(info["id"])+")",
				BUILDING_D->query_building(info["factory"])[0],
				sizeof(inlay_effect) > 0 ? HIY + sizeof(inlay_effect) + NOR : "",
				NUMBER_D->number_symbol(info["timecost"]),
				NUMBER_D->number_symbol(value),
				);
		}
		
		msg += WHT"─────────────────────────────────────\n"NOR;
		msg += "輸入 plist '產品ID' 可查詢細節資料\n\n";
		
		return me->more(msg);
	}
	else if( arg == "product" )
	{
		int i;
		mapping product_info;

		products = query("products", me);
		
		if( !arrayp(products) || !sizeof(products) )
			return tell(me, pnoun(2, me)+"尚未設計出任何可以生產的產品。\n");

		msg = pnoun(2, me)+"目前已經研發完成的產品依照最新發表順序如下表：\n";
		msg += WHT"───────────────────────────────────────\n"NOR;
		msg += HIW"編號 產品名稱                          產品種類                      價值          \n"NOR;
		msg += WHT"───────────────────────────────────────\n"NOR;
		
		i = 0;
		foreach(string file in products)
		{
			catch(ob = load_object(file));
		
			if( !objectp(ob) ) continue;
				
			product_info = ob->query_product_info();
			
			msg += sprintf("%-4s %-33s %-30s%s\n", (++i)+".", ob->query_idname(), product_info["name"]+"("+product_info["id"]+")", NUMBER_D->number_symbol(query("value", ob)));
		}
		msg += WHT"───────────────────────────────────────\n"NOR;

		return me->more(msg);
	}
	else if( sscanf(arg, "product %d", number) )
	{
		products = query("products", me);
		
		if( !arrayp(products) || !sizeof(products) )
			return tell(me, pnoun(2, me)+"尚未設計出任何可以生產的產品。\n");
		
		if( number <= 0 || number > sizeof(products) )
			return tell(me, "請輸入正確的產品編號。\n");

		catch(ob = new(products[number-1]));
		
		tell(me, ob->long());
		
		destruct(ob);

		return;
	}
	/* 優惠時間已截止
	else if( sscanf(arg, "return %d", number) )
	{
		mapping inlay;
		object inlay_ob;

		products = query("products", me);
		
		if( !arrayp(products) || !sizeof(products) )
			return tell(me, pnoun(2, me)+"尚未設計出任何可以生產的產品。\n");
		
		if( number <= 0 || number > sizeof(products) )
			return tell(me, "請輸入正確的產品編號。\n");

		catch(ob = load_object(products[number-1]));
		
		if( query("inlay_returned", ob) )
			return tell(me, pnoun(2, me)+"已經回收過這個產品的舊有鑲嵌物品。\n");
		
		inlay = query("design/inlay", ob);
		
		if( !mapp(inlay) || !sizeof(inlay) )
			return tell(me, "這項產品從沒有鑲嵌過任何物品。\n");
			
		foreach(string basename, int num in inlay)
		{
			inlay_ob = new(basename);
		
			if( !objectp(inlay_ob) ) continue;

			inlay_ob->set_keep();

			msg("$ME取回 "+num+" 個"+inlay_ob->query_idname()+"。\n", me, 0, 1);

			if( inlay_ob->no_amount() )
				inlay_ob->move(me, num);
			else
				inlay_ob->set_amount(num);
			
			inlay_ob->move(me);
		}

		set("inlay_returned", 1, ob);
	
		ob->save();
		me->save();

		msg("$ME將所有鑲嵌物品取回。\n", me, 0, 1);

		return;
	}
	*/
	else if( member_array(replace_string(lower_case(arg), " ", "_"), products) != -1 )
	{
		string module;
		mapping inlay_effect;

		arg = replace_string(lower_case(arg), " ", "_");
		
		module = PRODUCT_MODULE_PATH+arg+"_module.c";
		info = module->query_product_info();
		ob = load_object(PRODUCT_MODULE_PATH+arg+"_object.c");
		value = query("value", ob);

		info["timecost"] = to_int(pow(value, 0.32));
		info["cost"] = value*1000;
		info["technology"] = to_int(pow(value, 0.7));

		msg = info["name"]+"("+capitalize(info["id"])+")的詳細產品資料如下：\n";
		msg += WHT"─────────────────────────────────────\n"NOR;
		msg += sprintf(HIW"產品"NOR WHT"名稱"NOR" %-30s "HIW"製造"NOR WHT"耗時"NOR" %s\n", info["name"]+"("+capitalize(info["id"])+")", HIG+NUMBER_D->number_symbol(info["timecost"])+NOR GRN" 秒/單位(未鑲嵌)"NOR);
		msg += sprintf(HIW"生產"NOR WHT"工廠"NOR" %-30s "HIW"科技"NOR WHT"耗費"NOR" %s\n", BUILDING_D->query_building(info["factory"])[0], HIC+NUMBER_D->number_symbol(info["technology"])+NOR CYN" 科技值"NOR);
		msg += sprintf(HIW"最低"NOR WHT"價值"NOR" %-30s "HIW"研發"NOR WHT"費用"NOR" %s\n", HIY+NUMBER_D->number_symbol(value)+NOR YEL" 元"NOR, HIY+NUMBER_D->number_symbol(info["cost"])+NOR YEL" 元"NOR);
		msg += sprintf(HIW"產品"NOR WHT"重量"NOR" %-30s \n", HIR+NUMBER_D->number_symbol(query("mass", ob))+NOR RED" 公克"NOR);

		msg += HIW"製造"NOR WHT"原料\n"NOR;
		
		foreach( string material, int amount in info["material"] )
		{
			if( !objectp(materialob = load_object(material)) )
			{
				subinfo = (PRODUCT_MODULE_PATH+material+"_module.c")->query_product_info();
				msg += sprintf("         %-39s "HIM"%d"NOR MAG" 單位\n"NOR, subinfo["name"]+"("+capitalize(subinfo["id"])+")", amount);
			}
			else
				msg += sprintf("         %-39s "HIM"%d"NOR MAG" 單位\n"NOR, materialob->query_idname(), amount);
		}

		msg += HIW"技能"NOR WHT"需求\n"NOR;
		foreach( string skill, int level in info["skill"] )
			msg += sprintf("         %-39s "HIW"%-3d"NOR WHT" 級\n"NOR, (SKILL(skill))->query_idname(), level);
		
		inlay_effect = fetch_variable("inlay_effect", load_object(module));
		
		if( mapp(inlay_effect) )
		{
			msg += HIW"特殊"NOR WHT"鑲嵌\n"NOR;
			foreach( string file, function fp in inlay_effect )
				msg += sprintf("         %s\n"NOR, file->query_idname());
		}

		msg += WHT"─────────────────────────────────────\n"NOR;
		
		return tell(me, msg);
	}
	return tell(me, "沒有 "+arg+" 這種產品。\n");
}