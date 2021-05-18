/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : research.c
 * Author : Clode@RevivalWorld
 * Date   : 2006-01-07
 * Note   : 研發中心
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <map.h>
#include <feature.h>
#include <message.h>
#include <daemon.h>
#include <npc.h>
#include <skill.h>

inherit ROOM_ACTION_MOD;
#define PRODUCT_MODULE_PATH		"/std/module/product/"

#define RANDOM_THING			({ "日式拉麵", "米台目", "豬血糕", "牛肉麵", "岡山羊肉爐", "薑母鴨", "蛋炒飯", "人參雞", "臭豆腐", "彰化肉圓", "大腸包小腸", "新竹貢丸", "台南碗粿", "南投意麵", "埔里米粉", "米粉粳", "炒螺肉", "蚵仔麵線", "蚵仔煎", "藥燉排骨", "客家小炒", "擔仔麵", "臭臭鍋", "雪花冰", "吃剩的泡麵", "章魚丸子", "花生麻糬", "台中太陽餅", "鼎邊銼", "十八王公肉粽", "千年靈芝", "天山雪蓮", "大還丹", "巴西磨菇", "金桔檸檬汁", "珍珠奶茶", "客家擂茶", "阿飄", "傳說中的好人卡", "情人去死去死團永久會員", "充氣娃娃", "教授", "教育部長", "總統"})

#define COST_SOCIAL_EXP	1000000

// 取得所有可以生產的產品 module 物件陣列
private object *query_available_products(object me)
{
	string product;
	object *modules, module;
	
	modules = allocate(0);
	
	foreach(product in get_dir(PRODUCT_MODULE_PATH))
	{
		if( product[<9..<1] != "_module.c" ) continue;

		catch( module = load_object(PRODUCT_MODULE_PATH + product) );
		
		if( !objectp(module) || module->no_available(me) ) continue;
				
		modules += ({ module });
	}
	
	return modules;
}

void do_list_action(object me, string arg)
{
	int i;
	string file, msg;
	object product;
	string *productslist;
	object env = environment(me);
	string owner = query("owner", env);
	
	if( owner != me->query_id(1) && wizardp(me) )
	{
		object owner_ob = find_player(owner) || load_user(owner);
		
		productslist = query("products", find_player(owner));
			
		if( !sizeof(productslist) )
			return tell(me, owner+" 尚未設計出任何可以生產的產品。\n");
		
		if( !userp(owner_ob) )
			destruct(owner_ob);
	}
	else
	{
		productslist = query("products", me);
	
		if( !sizeof(productslist) )
			return tell(me, pnoun(2, me)+"尚未設計出任何可以生產的產品。\n");
	}
	
	msg = (owner == me->query_id(1) ? pnoun(2, me) : owner+" ")+"目前已經研發完成的產品如下：\n"WHT"────────────────────────────────\n"NOR;
	
	i = 0;
	foreach(file in productslist)
	{
		catch(product = load_object(file));
	
		if( !objectp(product) ) continue;
			
		msg += sprintf("%-4s %s %s\n", (++i)+".", product->query_idname(), wizardp(me) ? file : "");
	}
	
	msg += WHT"────────────────────────────────\n"NOR;
	
	me->more(msg);
}

void do_design_action(object me, string arg)
{
	string value;
	mapping product_info;
	object *products, product;
	object env = environment(me);
	object master = env->query_master();

	if( !arg )
	{
		string msg;
		mapping design = query("design", master);
		
		if( !mapp(design) )
			return tell(me, "目前沒有任何產品研發資料，是否要研發產品？(type, id, name, inlay, finish)\n");
			
		msg = "目前最新研發中的產品為：\n";
		msg += WHT"────────────────────────────────────\n"NOR;
		msg += HIW"產品種類"NOR WHT"(type)"NOR" ："NOR+(design["type"]||"未設計")+"\n";
		msg += HIR"產品英文"NOR RED"(id)"NOR"   ："NOR+(design["id"]||"未設計")+"\n";
		msg += HIC"產品中文"NOR CYN"(name)"NOR" ："NOR+(design["name"]||"未設計")+"\n";
		msg += HIY"鑲嵌物品"NOR YEL"(inlay)"NOR"：\n"NOR;
			
		if( mapp(design["inlay"]) )
		foreach(string file, int num in copy(design["inlay"]))
		{
			if( !file_exists(file) ) continue;

			product = load_object(file);
			
			if( !objectp(product) ) continue;
			
			msg += "                 "+QUANTITY_D->obj(product, num, 1)+"\n";
		}
		else
			msg += "                 未鑲嵌任何物品\n";
		
		msg += WHT"────────────────────────────────────\n"NOR;

		
		tell(me, msg);
		return;
	}

	if( query("owner", env) != me->query_id(1) && !wizardp(me) )
		return tell(me, pnoun(2, me)+"不是這間研發中心的擁有者。\n");

	sscanf(arg, "%s %s", arg, value);
	
	switch(arg)
	{
		case "type":
		{
			if( query("design/type", master) )
				return tell(me, pnoun(2, me)+"必須先完成或取消目前正在研發的產品。\n");
				
			if( !value || !value[0] )
				return tell(me, pnoun(2, me)+"想要研發哪一種產品(plist)？\n");

			value = lower_case(value);

			products = query_available_products(me);
		
			if( !sizeof(products) )
				return tell(me, "目前無法研發任何產品。\n");
			
			foreach( product in products )
			{
				product_info = product->query_product_info();
	
				if( product_info["id"] == value )
				{
					foreach(string skill, int level in product_info["skill"])
						if( me->query_skill_level(skill) < level )
							return tell(me, pnoun(2, me)+"的"+(SKILL(skill))->query_name()+"等級不足 "+level+" 無法研發此項產品。\n");
	
					set("design/type", value, master);

					msg("$ME將開發的產品種類設定為「"+product_info["name"]+"("+product_info["id"]+")」。\n", me, 0, 1);
					master->save();
					return;
				}
			}
			
			return tell(me, "無法研發 "+value+" 這種產品，"+pnoun(2, me)+"想要研發哪一種產品(plist)？\n");
			break;
		}
		case "name":
		{
			string name;
			if( !query("design/type", master) )
				return tell(me, "請先決定要研發的產品種類(type)。\n");

			if( !value || !value[0] )
				return tell(me, pnoun(2, me)+"想要替產品取什麼樣的名稱？\n");

			products = query_available_products(me);
			foreach( product in products )
			{
				product_info = product->query_product_info();
				
				if( product_info["id"] == query("design/type", master) )
				{
					name = product_info["name"];
					break;
				}
			}

			value = kill_repeat_ansi(trim(value)+NOR);
	
			if( noansi_strlen(value) > 16 || noansi_strlen(value) < 2 )
				return tell(me, "產品名稱長度必須在 2 到 16 個字元之間。\n");

			msg("$ME將產品之名稱設計為「"+value+"」。\n", me, 0, 1);
			
			set("design/name", value, master);
			
			master->save();
			break;
		}
		case "id":
		{
			if( !query("design/type", master) )
				return tell(me, "請先決定要研發的產品種類(type)。\n");

			if( !value || !value[0] )
				return tell(me, pnoun(2, me)+"想要替產品取什麼樣的英文名稱？\n");

			value = kill_repeat_ansi(lower_case(trim(value))+NOR);

			if( !is_english(value) )
				return tell(me, "請用「英文」取產品的英文名稱。\n");
			
			if( noansi_strlen(value) > 16 || noansi_strlen(value) < 3 )
				return tell(me, "英文名稱必須在 3 到 16 個英文字數之間。\n");

			msg("$ME將產品之英文名稱設計為「"+value+"」。\n", me, 0, 1);
			
			set("design/id", value, master);
			
			master->save();
			break;
		}
		case "delete":
		{
			string *productslist = query("products", me)||allocate(0);
			int num;
			
			if( !value || !value[0] || !to_int(value) )
				return tell(me, "請輸入正確的編號。\n");
			
			num = to_int(value) - 1;
				
			if( num < 0 || num > sizeof(productslist)-1 )
				return tell(me, "沒有這個編號的產品。\n");
			
			product = load_object(productslist[num]);
			
			productslist = productslist[0..num-1] + productslist[num+1..];
			
			set("products", copy(productslist), me);
			
			msg("$ME將編號為 "+value+" 的產品「$YOU」從研發產品列表中刪除。\n", me, product, 1);
			
			me->save();
			break;
		}
		case "inlay":
		{
			object ob;
			mapping inlay;
			int amount;

			if( !value || !value[0] )
				return tell(me, pnoun(2, me)+"想要將什麼物品鑲嵌在產品中？\n");
			
			if( value == "cancel" )
			{
				inlay = query("design/inlay", master);
				
				if( !mapp(inlay) || !sizeof(inlay) )
					return tell(me, "目前沒有鑲嵌任何物品。\n");
					
				foreach(string basename, int num in inlay)
				{
					ob = new(basename);
				
					if( query("flag/no_amount", ob) )
						ob->move(me, num);
					else
						ob->set_amount(num);
						
					ob->move(me);
				}	
				
				delete("design/inlay", master);
				master->save();
				msg("$ME將所有鑲嵌物品取回。\n", me, 0, 1);
				return;
			}
			
			sscanf(value, "%d %s", amount, value);

			if( !objectp(ob = present(value, me)) )
				return tell(me, pnoun(2, me)+"的身上沒有 "+value+" 這種物品。\n");
			
			if(me->is_equipping_object(ob) )
				return tell(me, pnoun(2, me)+"必須先卸除"+ob->query_idname()+"的裝備狀態。\n");

			if(ob->is_keeping())
				return tell(me, pnoun(2, me)+"必須先取消"+ob->query_idname()+"的保留狀態。\n");

			if( amount <= 0 )
				amount = 1;
				
			if( amount > ob->query_amount() )
				return tell(me, pnoun(2, me)+"身上沒有這麼多的"+ob->query_idname()+"。\n");

			inlay = query("design/inlay", master) || allocate_mapping(0);
			
			inlay[base_name(ob)] += amount;

			set("design/inlay", inlay, master);

			msg("$ME將 "+amount+" 個$YOU鑲嵌到產品設計模型上進行測試，產品的表面隱隱發出"HIW"奇特"NOR"的"WHT"光芒"NOR"。\n", me, ob, 1);	
			
			destruct(ob, amount);
			master->delay_save(60);
			me->delay_save(60);
			break;
		}
		case "cancel":
		{
			object ob;
			mapping inlay = query("design/inlay", master);
			
			if( mapp(inlay) )		
			foreach(string basename, int num in inlay)
			{
				ob = new(basename);
			
				if( query("flag/no_amount", ob) )
					ob->move(me, num);
				else
				{
					ob->set_amount(num);
					ob->move(me);
				}
			}	

			msg("$ME將所有鑲嵌物品取回。\n", me, 0, 1);
			msg("$ME取消了目前正在研發的產品。\n", me, 0, 1);
			delete("design", master);

			master->save();
			me->save();
			break;
		}	
		case "finish":
		{
			object book;
			int technology;
			string *productslist = query("products", me)||allocate(0);
			string type = query("design/type", master);
			string id = query("design/id", master);
			string name = query("design/name", master);
			mapping inlay = query("design/inlay", master);
			string money_unit = master->query_money_unit();
			int inlay_num;
			int cost_money;
				
			if( !type )
				return tell(me, "未設定生產產品種類。\n");

			if( !id )
				return tell(me, "未設定產品英文名稱。\n");

			if( !name )
				return tell(me, "未設定產品中文名稱。\n");

			type = replace_string(type, " ", "_");

			product = load_object(PRODUCT_MODULE_PATH+type+"_object.c");
			product_info = load_object(PRODUCT_MODULE_PATH+type+"_module.c")->query_product_info();

			technology = to_int(pow(to_float(query("value", product)), 0.7));	

			if( query("technology", master) < technology )
				return tell(me, "研發中心目前累積的科技值不足以開發此種產品。\n");

			if( count(me->query_social_exp_cur(), "<", COST_SOCIAL_EXP) )
				return tell(me, pnoun(2, me)+"的社會經驗值不足 "+COST_SOCIAL_EXP+"，無法進行研發。\n");

			foreach(object inv in all_inventory(me))
			if( base_name(inv) == "/obj/book/product/"+type )
			{
				book = inv;
				break;
			}
			
			if( !objectp(book) )
				return tell(me, pnoun(2, me)+"身上必須要有「"+product_info["name"]+"("+product_info["id"]+")」的配方書才可以進行產品開發。\n");

			cost_money = query("value", product) * 1000;

			if( !me->spend_money(money_unit, cost_money) )
				return tell(me, pnoun(2, me)+"的現金不足 "HIY+money(money_unit, cost_money)+NOR" 不足以支應研發經費。\n");

			if( !me->cost_social_exp(COST_SOCIAL_EXP) )
				return tell(me, pnoun(2, me)+"的社會經驗值不足 "+COST_SOCIAL_EXP+"，無法進行研發。\n");

			product = load_object("/product/"+type+"/"+SERIAL_NUMBER_D->query_serial_number("product/"+type));
			
			product->set_idname(id, name);		

			msg("$ME耗費了 "HIC+technology+NOR" 點科技值、"HIG+COST_SOCIAL_EXP+NOR" 社會經驗值與 "HIY+money(money_unit, cost_money)+NOR" 進行「"+product->query_idname()+"」產品的開發。\n", me, 0, 1);
			
			addn("technology", -technology, master);

			set("design/owner", me->query_id(1), product);
			set("design/city", CITY_D->query_city_idname(master->query_city()), product);
			set("design/room", env->query_room_name(), product);
			set("design/time", time(), product);
			set("design/type", type, product);

			// 作鑲嵌物的設定
			if( mapp(inlay) )
			{
				object inlay_ob;

				inlay_num = implode(values(inlay), (: $1+$2 :));
				set("design/inlay", inlay, product);		
				
				foreach(string basename, int num in inlay)
				{
					inlay_ob = new(basename);
				
					msg("$ME取回 "+num+" 個"+inlay_ob->query_idname()+"。\n", me, 0, 1);
				
					if( query("flag/no_amount", inlay_ob) )
						inlay_ob->move(me, num);
					else
					{
						inlay_ob->set_amount(num);
						inlay_ob->move(me);
					}
				}
			}

			set("value", product->query_value(), product);
			product->save();			
			productslist = ({ base_name(product) }) | productslist;
			
			delete("design", master);

			destruct(book, 1);

			set("products", copy(productslist), me);
			me->save();
			master->save();

			if( !wizardp(me) )
				CHANNEL_D->channel_broadcast("news", me->query_idname()+"的"+query("short", env)+"完成最新「"+product_info["name"]+"("+product_info["id"]+")」產品設計並命名為「"+product->query_idname()+"」，鑲嵌物品數為 "+inlay_num+"，價值為 "HIY+NUMBER_D->number_symbol(query("value", product))+NOR"。");

			break;
		}
		default:
			return tell(me, "無法開發 "+arg+" 這種產品。\n");
			break;
	}
}

void heart_beat(object room)
{
	int technologylevel;
	int researchmanage;
	int technology;
	int userinside;
	int npcinside;
	string owner = query("owner", room);

	object npc, ob;
	object *npcs;
	string *slaves;

	if( !find_player(owner) ) return;

	npcs = allocate(0);
	slaves = room->query_slave();

	if( !query_temp("research_heart_beat", room) )
		set_temp("research_heart_beat", random(10240), room);
		
	if( !(addn_temp("research_heart_beat", 1, room)%240) )
		room->save();

	foreach(ob in all_inventory(room))
	{
		if( ob->is_module_npc() && query("job/type", ob) == RD && query("boss",ob) == owner )
		{
			npcs += ({ ob });
			++npcinside;
		}
		
		if( userp(ob) )
			userinside = 1;
	}
	
	if( npcinside )
	{
		foreach(npc in npcs)
		{
			technologylevel += npc->query_skill_level("technology");
			researchmanage += npc->query_skill_level("researchmanage");
		}
		
		technologylevel /= npcinside;
		researchmanage /= 10;

		npc = npcs[random(sizeof(npcs))];
	}

	set("technologylevel", technologylevel, room);
	set("researchmanage", researchmanage, room);

	if( researchmanage < sizeof(slaves) + 1 )
	{
		if( userinside )
			broadcast(room, "研發管理的能力無法支撐過量的研發中心連鎖。\n");
		return;	
	}

	if( npcinside )
	{
		if( random(technologylevel) < 20 )
		{
			technology = to_int((-random(2)-1) * pow(sizeof(slaves)+1, 0.3));
			
			if( userinside )
			{
				if( !random(2) )
					msg("$ME打了個大呵欠，「呵～碰！」口水滴到電路板上把設備給燒壞了！(科技值 "HIR+technology+NOR")\n", npc, 0, 1);
				else
					msg("$ME突發奇想把「"HIY+RANDOM_THING[random(sizeof(RANDOM_THING))]+NOR"」通通往化學原料裡倒，只見濃濃"WHT"黑煙"NOR"佈滿整個實驗室！(科技值 "HIR+technology+NOR")\n", npc, 0, 1);
			}
		}
		else
		{
			technology = random(to_int(pow(technologylevel / 6 * (sizeof(slaves)+1), 0.5)));
			
			if( userinside )
			{
				if( !random(2) )
					msg("$ME依循著標準的實驗程序，將編號 "+sprintf(HIG"%c%c"NOR"-"GRN"%d%d%d%d%d"NOR, range_random(65,90),range_random(65,90),random(10),random(10),random(10),random(10),random(10))+" 的實驗項目如期完成。(科技值 "HIG"+"+technology+NOR")\n", npc, 0, 1);
				else if( !random(3) )
					msg("$ME一時恍神，隨手將「"HIY+RANDOM_THING[random(sizeof(RANDOM_THING))]+NOR"」往化學原料裡扔，沒想到卻使得陷入僵局的實驗成功了！？(科技值 "HIG"+"+technology+NOR")\n", npc, 0, 1);
				else
					msg("$ME從失敗中獲取經驗，將原本錯誤的研究資料重新整理改寫。(科技值 "HIG"+"+technology+NOR")\n", npc, 0, 1);
			}
		}
	}
	
	addn("technology", technology, room);
	addn("total_technology", technology, room);
}

// 設定建築物內房間型態種類
nosave mapping action_info =
([
	"laboratory"	:
	([
		"short"	: HIC"研發"NOR CYN"實驗室"NOR,
		"heartbeat":60,	// 實際時間 1 秒為單位
		"job"	: RD,
		"master": 1,
		"help"	:
			([
"topics":
@HELP
    提供產品的製造與科技研發。
HELP,


"design":
@HELP
設計全新產品的指令，用法如下：[管理指令]
  design type car		- 生產汽車(car)類的產品，所有產品種類可用 plist 查詢
  design id porsche		- 將產品的 ID 設定為 porsche (可使用色碼)
  design name 保時捷		- 將產品的名稱設定為 保時捷 (可使用色碼)
  design inlay '物品'		- 將某些特殊物品試著鑲嵌到產品模型上，讓產品產生特殊功能
  design inlay 100 '物品'	- 將 100 個物品試著鑲嵌到產品模型上，讓產品產生特殊功能
  design inlay cancel		- 取回所有鑲嵌物品
  design delete 3		- 將研發產品列表中編號為 3 的產品資訊刪除
  design cancel			- 取消目前正在研發的產品
  design finish			- 完成產品設計，發表新產品
HELP,

"list":
@HELP
列出已研發設計的產品資訊，用法如下：
  list			 	- 列出以完成研發設計的產品資訊
HELP,

			]),
		"action":
			([
				"design"	: (: do_design_action($1, $2) :),
				"list"		: (: do_list_action($1, $2) :),
			]),
	]),
]);

string look_room(object env)
{
	string msg = "";
	object master = env->query_master();
	mapping design = query("design", master);

	if( mapp(design) )
	{
		object inlay;

		msg = "目前最新研發中的產品為：\n";
		msg += WHT"────────────────────────────────────\n"NOR;
		msg += HIW"產品種類"NOR WHT"(type)"NOR" ："NOR+(design["type"]||"未設計")+"\n";
		msg += HIR"產品英文"NOR RED"(id)"NOR"   ："NOR+(design["id"]||"未設計")+"\n";
		msg += HIC"產品中文"NOR CYN"(name)"NOR" ："NOR+(design["name"]||"未設計")+"\n";
		msg += HIY"鑲嵌物品"NOR YEL"(inlay)"NOR"：\n"NOR;
		
		if( mapp(design["inlay"]) )
		foreach(string file, int num in copy(design["inlay"]))
		{
			if( !file_exists(file) ) continue;

			inlay = load_object(file);
			
			if( !objectp(inlay) ) continue;
			
			msg += "                 "+QUANTITY_D->obj(inlay, num, 1)+"\n";
		}
		else
			msg += "                 未鑲嵌任何物品\n";

		
		if( design["type"] )
		{
			object product = load_object(PRODUCT_MODULE_PATH+replace_string(design["type"], " ", "_")+"_object.c");	
			
			if( mapp(design["inlay"]) && sizeof(design["inlay"]) )
			{
				set("design/inlay", copy(design["inlay"]), product);
				product->setup_inlay();
			}
			
			msg += HIM"\n產品資料"NOR MAG"(data) "NOR"：\n                 重量 "+query("mass", product)+"g、價值 "+query("value", product)+(product->query_description()||"")+"\n";
			msg += DESCRIPTION_D->query_object_buff_description(product, 15, 0);
			delete("design/inlay", product);
			destruct(product);
		}
		
		msg += WHT"────────────────────────────────────\n"NOR;
	}

	msg += "科技值 "HIC+query("technology", master)+NOR CYN"/"+query("total_technology", master)+NOR" ";
	msg += "平均研發力 "HIY+query("technologylevel", master)+NOR+" ";
	msg += "研發管理 "HIY+(sizeof(master->query_slave())+1)+"/"+query("researchmanage", master)+NOR+"\n";
	
	return msg;
}

// 設定建築物資料
nosave array building_info = ({
	
	// 建築物之中文名稱
	HIC"研發"NOR CYN"中心"NOR

	// 開張此建築物之最少房間限制
	,2

	// 城市中最大相同建築數量限制(0 代表不限制)
	,0

	// 建築物建築種類, GOVERNMENT(政府), ENTERPRISE(企業集團), INDIVIDUAL(私人)
	,INDIVIDUAL

	// 建築物可建築區域, AGRICULTURE_REGION(農業), INDUSTRY_REGION(工業), COMMERCE_REGION(商業)
	,COMMERCE_REGION

	// 開張儀式費用
	,30000000

	// 建築物關閉測試標記
	,0

	// 繁榮貢獻度
	,5
	
	// 最高可加蓋樓層
	,20
	
	// 最大相同建築物數量(0 代表不限制)
	,0
	
	// 建築物時代
	,2
});

