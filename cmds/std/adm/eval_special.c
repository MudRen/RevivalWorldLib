/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : eval_special.c
 * Author : Clode@RevivalWorld
 * Date   : 2006-01-21
 * Note   : eval_special 指令。
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <daemon.h>
#include <feature.h>
#include <object.h>
#include <map.h>

inherit COMMAND;

string help = @HELP
        標準 eval_special 指令。
HELP;

private void eval_special()
{
	/*
	// 重新設定所有員工屬性
	foreach(object npc in objects( (: $1->is_module_npc() :)))
	{
		if( base_name(npc)[0..4] != "/npc/" ) continue;
			
		if( npc->query_skill_level("strength")/5 + 10 != query("attr/str", npc) )
		{
			tell(find_player("clode"), sprintf("%O = %d %d\n", npc, npc->query_skill_level("strength"), query("attr/str", npc)));
			set("attr/str", copy(npc->query_skill_level("strength"))/5 + 10, npc);
			npc->save();
		}

		if( npc->query_skill_level("physique")/5 + 10 != query("attr/phy", npc) )
		{
			tell(find_player("clode"), sprintf("%O = %d %d\n", npc, npc->query_skill_level("physique"), query("attr/phy", npc)));
			set("attr/phy", copy(npc->query_skill_level("physique"))/5 + 10, npc);
			npc->save();
		}

		if( npc->query_skill_level("intelligence")/5 + 10 != query("attr/int", npc) )
		{
			tell(find_player("clode"), sprintf("%O = %d %d\n", npc, npc->query_skill_level("intelligence"), query("attr/int", npc)));
			set("attr/int", copy(npc->query_skill_level("intelligence"))/5 + 10, npc);
			npc->save();
		}

		if( npc->query_skill_level("agility")/5 + 10 != query("attr/agi", npc) )
		{
			tell(find_player("clode"), sprintf("%O = %d %d\n", npc, npc->query_skill_level("agility"), query("attr/agi", npc)));
			set("attr/agi", copy(npc->query_skill_level("agility"))/5 + 10, npc);
			npc->save();
		}	

		if( npc->query_skill_level("charisma")/5 + 10 != query("attr/cha", npc) )
		{
			tell(find_player("clode"), sprintf("%O = %d %d\n", npc, npc->query_skill_level("charisma"), query("attr/cha", npc)));
			set("attr/cha", copy(npc->query_skill_level("charisma"))/5 + 10, npc);
			npc->save();
		}
	}
	*/		
			
			
	/*
	foreach(string area in AREA_D->query_areas())
	for(int num=0;num<AREA_D->query_area_nums(area);num++)
	{
		for(int i=0;i<5;i++)
		{
			int x = random(100);
			int y = random(100);
			
			array loc = 	
			
		}
	}
	*/
		
	// 統計產品數量
	/*
	int num;
	mapping data = allocate_mapping(0);
	foreach(object room in objects((: $1->is_module_room() && $1->query_master() == $1 && query("products", $1) :)))
	{
		reset_eval_cost();

		for(int i=0;i<7;i++)
		{
			num = to_int(room->query_module_file()->query_object_amount(room, "products", "/obj/gem/gem"+i));

			if( num > 4000 )
				tell(find_player("clode"), sprintf("%O\n", room));

			data["/obj/gem/gem"+i] += num;
		}
	}
	
	tell(find_player("clode"), sprintf("%O\n", data));
	*/
	
	/* 轉移開發產品資料至玩家身上 
	string city;
	int num;
	string file;
	object room;
	string *productslist;
	string owner;
	object user_ob;

	foreach(city in CITY_D->query_cities())
	{
		num = CITY_D->query_city_num(city);
		
		while(num--)
		{
			foreach(file in get_dir("/city/"+city+"/"+num+"/room/"))
			{
				reset_eval_cost();
				
				if( sscanf(file, "%*s_research.o") == 1 )
				{
					room = load_module("/city/"+city+"/"+num+"/room/"+file[0..<3]);
					
					if( !objectp(room) || query("function", room) != "laboratory" || !sizeof(productslist = query("productslist", room)) )
						continue;
				
					owner = query("owner", room);

					user_ob = load_user(owner);

					set("products", copy(productslist) | (copy(query("products", user_ob)) || allocate(0)), user_ob);
					
					tell(find_player("clode"), sprintf("%s\n%O\n", user_ob->query_idname(), productslist));
					
					user_ob->save();

					if( !interactive(user_ob) )
						destruct(user_ob);
				}
			}				
		}
	}
	*/

	/* 重新設定所有員工資料
	object ob;
	foreach(string number in get_dir("/npc/"))
	{
		catch( ob = load_object("/npc/"+number[0..<3]) );
		
		if( !objectp(ob) ) continue;

		set("abi/stamina/max", 500 + ob->query_skill_level("stamina")*10, ob);
		set("abi/health/max", 500 + ob->query_skill_level("health")*10, ob);
		set("abi/energy/max", 500 + ob->query_skill_level("energy")*10, ob);
		set("abi/stamina/cur", 500 + ob->query_skill_level("stamina")*10, ob);
		set("abi/health/cur", 500 + ob->query_skill_level("health")*10, ob);
		set("abi/energy/cur", 500 + ob->query_skill_level("energy")*10, ob);

		ob->save();
		ob->startup_living();
	}
	*/

	/* 查詢錯誤的房地產資料
	int x, y;
	array loc;
	string owner;
	string city;
	int num;
	
	foreach(city in CITY_D->query_cities())
	{
		for(num=0;num<CITY_D->query_city_num(city);num++)
		for(x=0;x<100;x++)
		for(y=0;y<100;y++)
		{
			loc = arrange_city_location(x, y, city, num);
			
			owner = CITY_D->query_coor_data(loc, "owner");
			
			if( !owner || belong_to_government(owner) ) continue;
			
			reset_eval_cost();

			if( !ESTATE_D->whose_estate(loc) )
			{
				
				CHANNEL_D->channel_broadcast("sys", save_variable(loc)+" error("+owner+").");
				ESTATE_D->remove_unknown_estate(loc);
			}
		}	
	}*/
	

}

private void startup_eval_special()
{
	int costtime;
	
	costtime = time_expression { eval_special(); };

        CHANNEL_D->channel_broadcast("news", sprintf("系統特殊更新完畢，共花費 %.3f 秒。", costtime/1000000.));
}
	
private void command(object me, string arg)
{
        if( !is_command() ) return;

        CHANNEL_D->channel_broadcast("news", "五秒鐘後系統進行特殊更新，更新過程可能耗費數秒鐘至數分鐘，請稍候。");

	call_out((: startup_eval_special :), 5);
}
