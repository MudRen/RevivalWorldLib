/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : house.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-05-03
 * Note   : 住宅動作繼承物件
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <map.h>
#include <daemon.h>
#include <feature.h>
#include <condition.h>
#include <delay.h>
#include <material.h>

inherit ROOM_ACTION_MOD;

#define REGEN		60
#define REGEN_WITH_BED	90

void do_sethome(object me, string arg)
{
    object env = environment(me);

    if( query("owner", env) != me->query_id(1) )
    	return tell(me, "這裡不是"+pnoun(2, me)+"的家。\n");
    	
    msg("$ME將此地設定為回家的地方，以後使用 home 指令便可以回到此地。\n", me, 0, 1);
    
    set("home", base_name(env), me);
    
    me->save();
}

// 製作紀念品
void do_make(object me, string arg)
{
	int num;
	object env = environment(me);
	string option;

    if( query("owner", env) != me->query_id(1) )
    	return tell(me, "這裡不是"+pnoun(2, me)+"的家。\n");
        	
	if( !arg )
		return tell(me, pnoun(2, me)+"要製作什麼手工藝品？(souvenir)\n");
	
	if( sscanf(arg, "%d %s", num, option) == 2 )
	{
		if( num <= 0 )
			return tell(me, "數量不能小於零。\n");
	}
	else
	{
		num = 1;
		option = arg;
	}

	if( option == "souvenir" )
	{
		object ob;
		object metal;
		object stone;
		object wood;
		object water;
		object fuel;

		foreach(ob in all_inventory(me) | all_inventory(env) )
		{
			switch(base_name(ob))
			{
				case MATERIAL(METAL):	if( !objectp(metal) ) metal = ob;	break;
				case MATERIAL(STONE):	if( !objectp(stone) ) stone = ob;	break;
				case MATERIAL(WOOD):	if( !objectp(wood) ) wood = ob;		break;
				case MATERIAL(WATER):	if( !objectp(water) ) water = ob;	break;
				case MATERIAL(FUEL):	if( !objectp(fuel) ) fuel = ob;		break;
				default: break;
			}
		}
		
		if( !objectp(metal) || metal->query_amount() < num*5 )
			return tell(me, pnoun(2, me)+"附近沒有足夠的"+MATERIAL(METAL)->query_idname()+"。\n");
		if( !objectp(stone) || stone->query_amount() < num*5 )
			return tell(me, pnoun(2, me)+"附近沒有足夠的"+MATERIAL(STONE)->query_idname()+"。\n");
		if( !objectp(wood) || wood->query_amount() < num*5 )
			return tell(me, pnoun(2, me)+"附近沒有足夠的"+MATERIAL(WOOD)->query_idname()+"。\n");
		if( !objectp(water) || water->query_amount() < num*5 )
			return tell(me, pnoun(2, me)+"附近沒有足夠的"+MATERIAL(WATER)->query_idname()+"。\n");
		if( !objectp(fuel) || fuel->query_amount() < num*5 )
			return tell(me, pnoun(2, me)+"附近沒有足夠的"+MATERIAL(FUEL)->query_idname()+"。\n");
			
		if( !me->cost_energy(100 * num) )
			return tell(me, pnoun(2, me)+"沒有足夠的精神可以用來製作紀念品了。\n");
			
		destruct(metal, num*5);
		destruct(stone, num*5);
		destruct(wood, num*5);
		destruct(water, num*5);
		destruct(fuel, num*5);
		
		ob = new("/product/souvenir_"+env->query_city());

		if( num > 1 )
			ob->set_amount(num);

		msg("$ME靠著細膩的手工仔細地製作了 "+num+" 個"+ob->query_idname()+"。\n", me, 0, 1);
		
		ob->move(me);
	}
	else
		return tell(me, pnoun(2, me)+"要製作什麼手工藝品？(souvenir)\n");
}

// 閱讀雜誌
void do_readmagazine(object me, string arg)
{
    object env = environment(me);

    if( query("owner", env) != me->query_id(1) )
    	return tell(me, "這裡不是"+pnoun(2, me)+"的家。\n");
    	
    msg("$ME坐在客廳裡翻開雜誌，輕鬆地閱讀來自世界各地的消息，充實了許多知識，也讓一身僵硬的肌肉輕鬆下來。\n", me, 0, 1);

	me->start_condition(SELFCHARGE);
}

// 睡覺
void do_sleep(object me, string arg)
{
	int stamina_cost;
	int health_cost;
	int energy_cost;
	int stamina_time;
	int health_time;
	int energy_time;
	int max_time;
	int regen;
	object env = environment(me);
	object* beds = filter_array(all_inventory(env), (: $1->query_module() == "bed" && query_temp("decorated", $1) :));

    if( query("owner", env) != me->query_id(1) )
    	return tell(me, "這裡不是"+pnoun(2, me)+"的家。\n");
       
	if( me->is_delaying() )
		return tell(me, me->query_delay_msg());
		
	if( sizeof(beds) && !random(5000) )
	{
		msg("$ME飛身跳上$YOU，不知如何突然「碰」的一聲，$YOU的床腳整個裂開，壞掉了！！\n", me, beds[0], 1);
		destruct(beds[0], 1);
		return;
	}

	stamina_cost = me->query_stamina_max() - me->query_stamina_cur();
	health_cost = me->query_health_max() - me->query_health_cur();
	energy_cost = me->query_energy_max() - me->query_energy_cur();

	if( sizeof(beds) )
	{
		regen = query("effect", beds[0]);
		msg("$ME舒服地躺在$YOU上，一眨眼就睡著了(恢復 "+stamina_cost+" 體力、"+health_cost+" 生命、"+energy_cost+" 精神)。\n", me, beds[0], 1);
	}
	else
	{
		regen = REGEN;
		msg("$ME舒服地躺在臥室地上，一眨眼就睡著了(恢復 "+stamina_cost+" 體力、"+health_cost+" 生命、"+energy_cost+" 精神)。\n", me, 0, 1);
	}

	stamina_time = 2 * stamina_cost / (me->stamina_regen() + regen) + 1;
	health_time = 2 * health_cost / (me->health_regen() + regen) + 1;
	energy_time = 2 * energy_cost / (me->energy_regen() + regen)+ 1;
	
	max_time = stamina_time;
	if( health_time > max_time ) max_time = health_time;
	if( energy_time > max_time ) max_time = energy_time;
		
	if( max_time <= 0 )
		tell(me, pnoun(2, me)+"睜開眼睛站了起來，感覺精神百倍。\n");
	else
	{
		if( me->in_condition(EXTREME_SILENCE) )
		{
			max_time /= 2;
			
			if( max_time < 1 )
				max_time = 1;
		}
		
		set_temp("rest_regen/stamina", regen, me);
		set_temp("rest_regen/health", regen, me);
		set_temp("rest_regen/energy", regen, me);
		me->start_delay(REST_DELAY_KEY, max_time, pnoun(2, me)+"正躺在"+(sizeof(beds)>0?beds[0]->query_idname():"地")+"上睡覺。\n", pnoun(2, me)+"睜開眼睛站了起來，感覺精神百倍。\n", bind((: delete_temp("rest_regen", $(me)), $(me)->set_stamina_full(), $(me)->set_health_full(), $(me)->set_energy_full() :), me));
	}
}

// OGC
void do_ogc(object me, string arg)
{
	object env = environment(me);

    if( query("owner", env) != me->query_id(1) )
    	return tell(me, "這裡不是"+pnoun(2, me)+"的家。\n");

	if( me->is_delaying() )
		return tell(me, me->query_delay_msg());

    msg("$ME從抽屜拿出奇怪的道具，跑到床上開始做起奇怪的動作...喔...哦...哇...呦...耶。\n", me, 0, 1);
    
    me->faint();
}


// 設定建築物內房間型態種類
nosave mapping action_info =
([
        "livingroom"    :
        ([
                "short" : HIW"客廳"NOR,
                "help"  :
                        ([
"topics":
@HELP
    客廳可以提供傢俱的擺設與閱讀刊物。
HELP,

"readmagazine":
@HELP
閱讀雜誌的指令，用法如下：
 
readmagazine		- 輕鬆地閱讀雜誌

HELP,

"make":
@HELP
製作工藝品的指令，用法如下：
 
make souvenir		- 製作純手工城市紀念品(需耗費五大原料各五個)
make '數量' souvenir	- 一次製作大量的紀念品

HELP,

"sethome":
@HELP
設定回家的地點，用法如下：
 
sethome			- 將此地設定為回家地點

HELP,

                        ]),
                "action":
                        ([
                                "readmagazine"	: (: do_readmagazine :),
                                "make"		: (: do_make :),
                                "sethome"	: (: do_sethome :),
                        ]),
        ]),
        "bedroom"       :
        ([
                "short" : HIW"臥室"NOR,
                "help"  :
                        ([
"topics":
@HELP
    臥室可以用來好好地睡一覺。
HELP,

"sleep":
@HELP
睡覺的指令，用法如下：

sleep			- 躺到床上呼呼大睡，有較高的休息效率

HELP,

                        ]),
                "action":
                        ([
                                "sleep"       : (: do_sleep :),
                                "ogc"	      : (: do_ogc :),
                        ]),
        ]),
]);

// 設定建築物資料
nosave array building_info = ({

        // 建築物之中文名稱
        HIG"住"NOR GRN"宅"NOR

        // 開張此建築物之最少房間限制
        ,1

        // 城市中最大相同建築數量限制(0 代表不限制)
        ,0

        // 建築物建築種類, GOVERNMENT(政府), ENTERPRISE(企業集團), INDIVIDUAL(私人)
        ,INDIVIDUAL

        // 建築物可建築區域, AGRICULTURE_REGION(農業), INDUSTRY_REGION(工業), COMMERCE_REGION(商業)
        ,AGRICULTURE_REGION | INDUSTRY_REGION | COMMERCE_REGION

        // 開張儀式費用
        ,100000

        // 建築物關閉測試標記
        ,0

        // 繁榮貢獻度
        ,1

        // 最高可加蓋樓層
        ,6

        // 最大相同建築物數量(0 代表不限制)
        ,1

        // 建築物時代
        ,1
});
