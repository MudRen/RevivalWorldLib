/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : gulen.c
 * Author : Clode@RevivalWorld
 * Date   : 2009-12-24
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

inherit STANDARD_MONSTER;

mapping defeat_titles =
([
	10 		:	HIC"葛"NOR CYN"蘭"HIY"獵"NOR YEL"人"NOR,
	100		:	HIC"葛"NOR CYN"蘭"HIB"剋"NOR BLU"星"NOR,
	1000	:	HIC"葛"NOR CYN"蘭"HIR"殺"NOR RED"手"NOR,
	2000	:	HIC"葛"NOR CYN"蘭"HIM"噬"NOR MAG"者"NOR,
	5000	:	HIC"葛"NOR CYN"蘭"WHT"死"HIW"神"NOR,
]);

void do_command(string cmd);

void set_behavior()
{
	set("behavior/random", 
	    ([
			(: do_command("sweat") :)   : 50,
	    ]));
}


void delete_behavior()
{
	delete("behavior");
}

void reply_say(object me, string msg)
{
	if( !objectp(me) ) return;

	if( !msg || !msg[0] )
		return;
		
	if( strsrch(msg, "化石") != -1 )
	{
		do_command("say 什麼！？"+pnoun(2, me)+"手上有那些化石嗎？快拿給我看看！");
		return;
	}
	
	do_command("say "+pnoun(2, me)+"在說些什麼？");
}

void reply_emote(object me, string verb, string args)
{
	if( !objectp(me) ) return;
		
	do_command("? "+me->query_id(1));
}

// 死亡時的事件
void reply_die()
{

}

// 被擊敗時的事件
void reply_defeat(object* attackers)
{
	::reply_defeat(attackers);
}

void change_to_fighter()
{
	string file;
	string *random_drop = ({
		"/quest/monster_fossil/deep_sea_thorn",
		"/quest/monster_fossil/lava_meteor",
		"/quest/monster_fossil/jade_emerald",
		"/quest/monster_fossil/monster_gall",
		"/quest/monster_fossil/eagle_feather",
		"/quest/monster_fossil/eye_claw",
	});
	
	set("no_fight", 0);
	
	file = random_drop[random(sizeof(random_drop))];
	new(file)->move(this_object());
}

void reply_get_object(object me, object ob, mixed amount)
{
	string *fossils = query("quest/"+QUEST_FOSSIL+"/peice", me) || allocate(0);
	string basename;

	if( !objectp(ob) ) return;

	basename = base_name(ob);

	switch(basename)
	{
		case "/quest/monster_fossil/cat_fossil":
		case "/quest/monster_fossil/dog_fossil":
		case "/quest/monster_fossil/elephant_fossil":
		case "/quest/monster_fossil/lion_fossil":
		case "/quest/monster_fossil/orang_fossil":
		case "/quest/monster_fossil/panther_fossil":
		case "/quest/monster_fossil/pig_fossil":
		case "/quest/monster_fossil/python_fossil":
		case "/quest/monster_fossil/rabbit_fossil":
		case "/quest/monster_fossil/scolopendrid_fossil":
		case "/quest/monster_fossil/snake_fossil":
		case "/quest/monster_fossil/spider_fossil":
		case "/quest/monster_fossil/tiger_fossil":
		{
			destruct(ob);

			if( member_array(basename, fossils) != -1 )
			{
				do_command("say 我已經有這塊化石了，但"+pnoun(2, me)+"既然要給我的話我就不客氣啦...");
				break;
			}			
			
			fossils |= ({ basename });
			
			if( sizeof(fossils) == 13 )
			{
				process_function(({
					1,
					(: do_command("grin") :),
					(: do_command("say 太好了...") :),
					3,
					(: do_command("say 還記得當初我說要給"+pnoun(2, $(me))+"的好東西嗎？") :),
					2,
					(: do_command("say 那個根本就是耍"+pnoun(2, $(me))+"的！誰叫"+pnoun(2, $(me))+"那麼好騙！活該！哈哈哈...") :),
					(: change_to_fighter :),
					(: delete("quest/"+QUEST_FOSSIL+"/peice", $(me)) :),
					(: !wizardp($(me)) && CHANNEL_D->channel_broadcast("city", $(me)->query_idname()+" 解開了「"+QUEST_FOSSIL_NAME+"」", $(me)) :),
					(: $(me)->set_quest_finish(QUEST_FOSSIL, QUEST_FOSSIL_NAME, "幫考古學家找到了所有的化石，但他卻食言了！！") :),
				}));
			}
			else
			{
				if( me->query_quest_step(QUEST_FOSSIL) == 0 )
				{
					process_function(({
						(: do_command("say 想不到"+pnoun(2, $(me))+"可以找到這些化石...") :),
						1,
						(: do_command("hehe") :),
						(: do_command("say "+pnoun(2, $(me))+"如果可以給我這塊大地上所有種類的化石，我這裡有一個好東西可以送給你。") :),
						(: $(me)->set_quest_note(QUEST_FOSSIL, QUEST_FOSSIL_NAME, pnoun(2, $(me))+"已經收集到了 "+sizeof($(fossils))+" 塊化石，似乎還有 "+(13-sizeof($(fossils)))+" 種化石尚未找到。") :),
						(: $(me)->set_quest_step(QUEST_FOSSIL, 1) :),
					}));
				}
				else
				{
					process_function(({
						(: do_command("say 太好了，應該還有 "+(13-sizeof($(fossils)))+" 種化石...") :),
						(: $(me)->set_quest_note(QUEST_FOSSIL, QUEST_FOSSIL_NAME, pnoun(2, $(me))+"已經收集到了 "+sizeof($(fossils))+" 塊化石，似乎還有 "+(13-sizeof($(fossils)))+" 種化石尚未找到。") :),
						(: $(me)->set_quest_step(QUEST_FOSSIL, 1) :),
					}));
				}

				set("quest/"+QUEST_FOSSIL+"/peice", fossils, me);
			}
			me->save();
			break;
		}
		default:
		{
			process_function(({
				1,
				(: do_command("say 給我這個幹嘛？我不需要這個東西...") :),
				1,
				(: do_command("drop all "+$(ob)->query_id(1)) :),
			}));
			break;
		}
	}
}

void create()
{
	set_idname("gulen", HIC"葛"NOR CYN"蘭"NOR);
	set_temp("status", ([HIW"考古"NOR WHT"學家"NOR : -1]));

	set("long", "  一位從外地來的考古學家，一直在地面上敲敲打打，他身上好像有很多稀奇古怪的物品。");
	set("unit", "位");
	set("age", 42);
	set("gender", MALE_GENDER);
	set("heartbeat", 1); // 永久性心跳

	set("attr/str", 100);
	set("attr/phy", 50);
	set("attr/int", 50);
	set("attr/agi", 100);
	set("attr/cha", 50);

	set("abi/stamina/max", 20000);
	set("abi/stamina/cur", 20000);
	set("abi/health/max", 20000);
	set("abi/health/cur", 20000);
	set("abi/energy/max", 20000);
	set("abi/energy/cur", 20000);
	set("skills/unarmed/level", 130);
	set("skills/dodge/level", 130);
	set("skills/fatalblow/level", 50);

	set("no_fight", 1);

	// 啟動主動行為
	set_behavior();

	startup_living();
}
