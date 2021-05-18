/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : lu_hua.c
 * Author : Kyo@RevivalWorld
 * Date   : 2007-09-27
 * Note   : 
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

/*
	系統預設之 NPC 反應函式
	void reply_emote(object me, string verb, string args);
	void reply_say(object me, string msg);
	void reply_catch_tell(object me, string verb, string args);
	void reply_get_object(object me, object ob, mixed amount);
	void reply_get_money(object me, string unit, mixed money);
*/

#include <ansi.h>
#include <inherit.h>
#include <gender.h>
#include <daemon.h>
#include <quest.h>

inherit STANDARD_PERSON;

string *keywords = ({ "鹿", "滑", "茸", "撞", "test", });

void do_command(string cmd);

int is_keyword(string msg)
{
	for(int i=0; i<sizeof(keywords); i++)
		if( msg && strsrch(msg, keywords[i]) != -1 ) return 1;
	return 0;
}

void set_behavior()
{
	set("behavior/random", 
	    ([
			//(: do_command("sweat") :)   : 50,
	    ]));
}


void delete_behavior()
{
	delete("behavior");
}

void reply_say(object me, string msg)
{
	if( !objectp(me) || !msg ) return;

	if( is_keyword(msg) )
	{
		do_command("point "+me->query_id(1)+" 你是不是很想進不來");
		do_command("slap "+me->query_id(1));
		me->faint();
		return;
	}

	if( strsrch(msg, "中秋") != -1 || strsrch(msg, "月兔") != -1)
		do_command("say "+pnoun(2, me)+"想要知道中秋禮物的秘密嗎？");
}

void reply_emote(object me, string verb, string args)
{
	/* 沒作用=.="
	if( is_keyword(args) )
	{
		do_command("point "+me->query_id(1)+" 你是不是很想進不來");
		do_command("slap "+me->query_id(1));
		me->faint();
		return;
	}
	*/
	if( objectp(me) ) {
		switch(verb) {
			case "hi":
				do_command("draw " +me->query_id(1)+ " 前途黯淡");
				break;
			case "nod":
				do_command("say 既然"+pnoun(2, me)+"想知道，只要"+pnoun(2, me)+"收集了重生的世界到目前為止的中秋禮物給神，神就告訴"+pnoun(2, me)+"。");
				break;
			default:
				do_command("arc " +me->query_id(1));
				break;
		}
	}
}

void destruct_self()
{
	object me, ob;
	mixed loc, test;
	string *where_name = CITY_D->query_cities();
	string where = where_name[random(sizeof(where_name))];

	loc = arrange_city_location(range_random(0,99), range_random(0,99), where, random(CITY_D->query_city_num(where)));

	test = loc_short(loc);

	ob = this_object();
	ob->move(loc);

	CHANNEL_D->channel_broadcast("sys", "中秋寶藏的Npc移動到了"+test, me);
	CHANNEL_D->channel_broadcast("news", "傳聞有個知道失落寶藏秘密的人出現在 "+where+" 。", me);
}

void reply_get_object(object me, object ob, mixed amount)
{
	string *moon_gifts = query("quest_temp/"+QUEST_MOON_RABBIT+"/gift", me) || allocate(0);
	string basename = base_name(ob);

	if( !objectp(ob) ) return;

	switch(basename)
	{
		case "/obj/festival/2006/mooncake/cake_glass":
		case "/obj/festival/2006/mooncake/rabbit_ear":
		case "/obj/festival/2006/mooncake/wukang_axe":
		case "/obj/festival/2006/mooncake/shaddock_hat":
		case "/obj/festival/2006/mooncake/hairpin":
		case "/obj/festival/2007/mooncake/rabbit_stick":
		case "/obj/festival/2007/mooncake/hoi_bow":

			moon_gifts |= ({ basename });

			if( sizeof(moon_gifts) == 7 )
			{
				CHANNEL_D->channel_broadcast("news", "據說"+me->query_idname()+"獲得了一張古代神留下來藏寶圖。", me);
				process_function(({
					1,
					(: do_command("say 倒站ㄅ...") :),
					1,
					(: do_command("say 祝"+pnoun(2, $(me))+"挖到大寶讓Rw倒站吧...-.-") :),
					(: delete("quest_temp/"+QUEST_MOON_RABBIT+"/gift", $(me)) :),
					(: new("/obj/festival/2007/mooncake/treasure_map")->move($(me)) :),
				}));
				//CHANNEL_D->channel_broadcast("news", "據說"+me->query_idname()+"獲得了一張古代神留下來藏寶圖。", me);
			}
			else
			{
				process_function(({
					1,
					(: do_command("say 還不快點去收集"+pnoun(2, $(me))+"是不是想暈到火星節 -.-") :),
					1,
					(: do_command("say 神要ㄑ下一個城市吃ㄒ提，"+pnoun(2, $(me))+"自己過來找神") :),
					//(: destruct_self :),
				}));
				set("quest_temp/"+QUEST_MOON_RABBIT+"/gift", moon_gifts, me);	
			}
			me->save();
			destruct(ob);
			break;
		default:
		{
			process_function(({
			1,
			(: do_command("say "+pnoun(2, $(me))+"給神這個東西幹嗎？神不需要這個東西...") :),
			1,
			(: do_command("drop all "+$(ob)->query_id(1)) :),
			}));
			break;
		}
	}
}

void create()
{
	set_idname("lu hua", HIY"陸"NOR+YEL"華"NOR);
	set_temp("status", HIY"沉"NOR+YEL"思"NOR);

	set("long", "  一位神秘的矮小書生，似乎知道天下的秘密，自稱自己為神。");
	set("unit", "位");
	set("age", 999);
	set("gender", MALE_GENDER);
	set("heartbeat", 1); // 永久性心跳

	set("attr/str", 999);
	set("attr/phy", 999);
	set("attr/int", 999);
	set("attr/agi", 999);
	set("attr/cha", 999);

	set("abi/stamina/max", 99999);
	set("abi/stamina/cur", 99999);
	set("abi/health/max", 99999);
	set("abi/health/cur", 99999);
	set("abi/energy/max", 99999);
	set("abi/energy/cur", 99999);

	set("no_fight", 1);

	//if( clonep() )
		//call_out( (: destruct_self :), 300 );

	// 啟動主動行為
	set_behavior();

	startup_living();
}
