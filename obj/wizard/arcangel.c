/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : enterprise.c
 * Author : Clode@RevivalWorld
 * Date   : 2006-02-08
 * Note   : 企業號
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>
#include <feature.h>
#include <equipment.h>
#include <buff.h>
#include <daemon.h>

inherit STANDARD_OBJECT;
inherit EQUIPMENT;

mapping actions;

void fire_process(object me, object target, int combo, int sec)
{
	if( sec == 0 )
	{
		int damage;
		for(int i=0;i<combo;i++)
		{
			damage = range_random(15, 35);
			
			msg("$YOU被$ME發射的 12 型光炮陣列擊中，導致 "HIR+damage+NOR" 的生命傷害("NOR GRN+target->query_health_cur()+NOR"/"HIG+target->query_health_max()+NOR")。\n", me, target, 1);
			
			COMBAT_D->cause_damage(me, target, damage);
		}

		delete_temp("delay/fire");
	}
	else
	{
		msg("距離 12 型光炮陣列擊中$YOU還有 "+sec+" 秒鐘...\n", me, target, 1);
		call_out((: fire_process, me, target, combo, sec-1 :), 1);
	}
}

void fire(object me, string arg)
{
	int combo = random(11)+1;
	object target = find_player(arg) || present(arg);

	if( !me->is_equipping_object(this_object()) )
		return tell(me, pnoun(2, me)+"必須先裝備"+this_object()->query_idname()+"。\n");

	if( query_temp("delay/fire") > time() )
		return tell(me, "此武器無法連續發射，請等待武器系統冷卻。\n");

	if( !objectp(target) )
		return tell(me, "沒有 "+arg+" 這個人。\n");

	for(int i=0;i<combo;i++)
		msg("$ME操縱"+this_object()->query_idname()+"對著$YOU發射 12 型光炮陣列("HIY+combo+NOR YEL" 連發"NOR")。\n", me, target, 1);
	set_temp("delay/fire", time() + 5);
	call_out((: fire_process, me, target, combo, 5 :), 1);
}

void shield(object me, string arg)
{
	if( !me->is_equipping_object(this_object()) )
		return tell(me, pnoun(2, me)+"必須先裝備"+this_object()->query_idname()+"。\n");

	if( !query_temp("shield/on") )
	{
		msg("$ME啟動"+this_object()->query_idname()+"自動調頻防護罩系統。\n", me, 0, 1);
		set_temp("status", HIR"(防護罩)"NOR);
		set_temp("shield/on", 1);
	}
	else
	{
		msg("$ME關閉"+this_object()->query_idname()+"自動調頻防護罩系統。\n", me, 0, 1);
		delete_temp("status");
		delete_temp("shield/on");
	}
}

void antimatter_process(object me, object target, int sec)
{
	if( sec == 0 )
	{
		msg("$YOU被$ME所啟動的反物質散布口徹底毀滅，導致 "HIR+1000000+NOR" 的生命傷害("NOR RED+"-1000000"+NOR"/"HIG+target->query_health_max()+NOR")。\n", me, target, 1);

		target->faint();

		if( userp(target) )
			LOGOUT_D->quit(target);

		delete_temp("delay/antimatter");
	}
	else
	{
		msg("距離反物質散布口聚集能量完成還有 "+sec+" 秒鐘...\n", me, target, 1);
		call_out((: antimatter_process, me, target, sec-10 :), 10);
	}
}

// 反物質散布口
void antimatter(object me, string arg)
{
	object target = find_player(arg) || present(arg);

	if( !me->is_equipping_object(this_object()) )
		return tell(me, pnoun(2, me)+"必須先裝備"+this_object()->query_idname()+"。\n");

	if( query_temp("delay/antimatter") > time() )
		return tell(me, "此武器無法連續發射，請等待武器系統冷卻。\n");

	if( !objectp(target) )
		return tell(me, "沒有 "+arg+" 這個人。\n");

	msg("$ME操縱"+this_object()->query_idname()+"對著$YOU開啟反物質散布口。\n", me, target, 1);
	set_temp("delay/antimatter", time() + 60);
	call_out((: antimatter_process, me, target, 60 :), 1);
}

void create()
{
	set_idname(HIC"arc"NOR CYN"an"HIW"gel"NOR, HIC"大"NOR CYN"天使"HIW"號");
	set_temp("status", HIY"絕"NOR YEL"版");

	actions = ([
	    "fire" : (: fire :),
	    "shield" : (: shield :),
	    "antimatter": (: antimatter :),
	]);

	if( this_object()->set_shadow_ob() ) return;

	set("long", @LONG
  配備超光速飛行的曲速引擎之星際戰鑑，擁有徹底毀滅對手的能力
  
    武裝
      11 條 12 型光炮陣列，總輸出量為 8 萬 5 千兆瓦
      1 個高速量子魚雷管，裝備 50 枚量子魚雷
      4 個 4 型連發式光雷管，總共裝備有 250 枚光雷
      反物質散布口。
  
    防禦
      自動調頻防護罩系統，總容量為 459 萬兆焦
      重型二度複合密鎢鋼與三度複合密鎢鋼雙層外殼，加上 10 厘米的抗燒蝕裝甲
      高級結構保護盾。 
  
    曲速能力
      正常巡航曲速 8 級
      全速巡航曲速 9.9 級
      最大能力是以曲速 9.99 航行 24 小時。   
   
    曲速對照表
    曲速(級)     光速(每單位 299,792,458 公尺/秒)
    [1;37m1            [1;30m1
    [1;37m2            [1;30m8
    [1;37m3            [1;30m27
    [1;37m4            [1;30m64
    [1;37m5            [1;30m125
    [1;37m6            [1;30m216
    [1;37m7            [1;30m343
    [1;37m8            [1;30m512
    [1;37m9            [1;30m729      
    [1;37m10           [1;30m1000
    [1;37m11           [1;30m1331
    [1;37m12           [1;30m1728
    [1;37m13           [1;30m2197
    [1;37m14.1         [1;30m2803.221  [0m
LONG
	);

	set("unit", "顆");
	set("mass", 0);
	set("value", 12000000);
	set("flag/no_amount", 1);

	set(BUFF_STR, 100);
	set(BUFF_PHY, 100);
	set(BUFF_INT, 100);
	set(BUFF_AGI, 100);
	set(BUFF_CHA, 100);

	set(BUFF_STAMINA_MAX, 100);
	set(BUFF_HEALTH_MAX, 100);
	set(BUFF_ENERGY_MAX, 100);

	set(BUFF_STAMINA_REGEN, 100);
	set(BUFF_HEALTH_REGEN, 100);
	set(BUFF_ENERGY_REGEN, 100);

	set(BUFF_LOADING_MAX, 100);
	set(BUFF_SLOT_MAX, 100);
	set(BUFF_FOOD_MAX, 100);
	set(BUFF_DRINK_MAX, 100);

	set(BUFF_SOCIAL_EXP_BONUS, 1000);
	set(BUFF_COMBAT_EXP_BONUS, 1000);
	
	set("buff/status", HIC"大"NOR CYN"天使"HIW"號");

	::setup_equipment(EQ_MOUNT, HIG"駕"NOR GRN"駛"NOR);
}
