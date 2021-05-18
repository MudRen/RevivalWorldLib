/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : Santa_Claus.c
 * Author : Kyo@RevivalWorld
 * Date   : 2007-12-20
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
	object ob;
	int quest = query_temp("christmas", me);
	int questgift = query_temp("christmasgift", me);
	int getgift = query("christmasgift", me);
	int test = range_random(1, 100);
	string msg1;

	if( !objectp(me) || !msg ) return;

	if( is_keyword(msg) )
	{
		do_command("point "+me->query_id(1)+" 你是不是很想進不來");
		do_command("slap "+me->query_id(1));
		//me->faint();
		return;
	}

	if( strsrch(msg, "幫忙") != -1 || strsrch(msg, "送禮物") != -1)
	{
		if( query("total_online_time",  me) < 24*60*60*2 )
			return tell( me,HIR"小朋友等"+pnoun(2, me)+"長大再來幫忙吧！\n"NOR);

		if(getgift == 1)
			return tell( me,HIR"領過禮物的無法領取第二次喔！！\n"NOR);

		if(questgift == 5)
			return tell( me,HIR"您已經幫忙送完五次，可以找聖誕老人領取(獎勵)了！！\n"NOR);

		if(quest > 1)
		{
			do_command("say "+pnoun(2, me)+"還在這邊幹嘛？不是說要幫忙神送禮物嗎？");
		} else {
			do_command("say 幫我拉雪橇的麋鹿跑光了，"+pnoun(2, me)+"願意幫忙神送禮物嗎？");
			set_temp("christmas", 1, me); // 第一步
		}
	}
	else if( strsrch(msg, "獎勵") != -1 )
	{
		if(getgift != 1)
		{
			if(questgift == 5)
			{
				set_temp("christmas", 0, me);
				set("christmasgift", 1, me); // 領過的標記
				if(test <= 15)
				{
					ob = new("/obj/festival/2007/christmas/deer_card");
					msg1 = ob->query_idname();
					log_file("command/christmas2007", me->query_idname()+"獲得"+msg1);
					ob->move(me);
					CHANNEL_D->channel_broadcast("news", me->query_idname()+"解開了聖誕節任務，領取到了聖誕禮物 "+ob->query_idname()+"。");
				}
				else if(test > 15 && test <= 30)
				{
					ob = new("/obj/festival/2007/christmas/deer_sled");
					msg1 = ob->query_idname();
					log_file("command/christmas2007", me->query_idname()+"獲得"+msg1);
					ob->move(me);
					CHANNEL_D->channel_broadcast("news", me->query_idname()+"解開了聖誕節任務，領取到了聖誕禮物 "+ob->query_idname()+"。");
				}
				else if(test > 30 && test <= 65)
				{
					ob = new("/obj/festival/2007/christmas/xmas_ring_hide.c");
					msg1 = ob->query_idname();
					log_file("command/christmas2007", me->query_idname()+"獲得"+msg1);
					ob->move(me);
					CHANNEL_D->channel_broadcast("news", me->query_idname()+"解開了聖誕節任務，領取到了聖誕禮物 "+ob->query_idname()+"。");
				}
				else {
					ob = new("/obj/festival/2007/christmas/xmas_ring.c");
					msg1 = ob->query_idname();
					log_file("command/christmas2007", me->query_idname()+"獲得"+msg1);
					ob->move(me);
					CHANNEL_D->channel_broadcast("news", me->query_idname()+"解開了聖誕節任務，領取到了聖誕禮物 "+ob->query_idname()+"。");
				}
				return;
			}
			else
			{
				do_command("say 還沒有幫忙就想領禮物，"+pnoun(2, me)+"想偷雞就重新來過吧！");
				set_temp("christmasgift", 0, me);
				set_temp("christmas", 0, me);
				//me->faint();
				return;
			}
			return;
		}
		return tell( me,HIR"領過禮物的無法領取第二次喔！！\n"NOR);
	}
}

void reply_emote(object me, string verb, string args)
{
	mixed loc, test;
	string *where_name = AREA_D->query_areas();
	string where = where_name[random(sizeof(where_name))];
	int num;
	int quest = query_temp("christmas", me);

	do
	{
  	where = where_name[random(sizeof(where_name))];
	} while ( where == "lostland" || where == "battlefield");

	num = random(AREA_D->query_area_nums(where));
	loc = arrange_city_location(range_random(0,99), range_random(0,99), where, num);

	test = loc_short(loc);

	// 沒作用=.="
	//if( is_keyword(args) )
	//{
	//	do_command("point "+me->query_id(1)+" 你是不是很想進不來");
	//	do_command("slap "+me->query_id(1));
	//	me->faint();
	//	return;
	//}

	if( objectp(me) ) {
		switch(verb) {
			case "hi":
				do_command("draw " +me->query_id(1)+ " 前途黯淡");
				break;
			case "nod":
				if(quest == 1)
				{
					object ob = new("/obj/festival/2007/christmas/gift_box.c");
					do_command("say 既然"+pnoun(2, me)+"願意幫忙，只要"+pnoun(2, me)+"幫我送完五件禮物給小朋友，神就給"+pnoun(2, me)+"一份獎勵。");
					do_command("say 這份禮物就麻煩"+pnoun(2, me)+"幫我送到"+test+"給那位在等待的小孩。");
					tell(me, this_object()->query_idname()+"拿給"+pnoun(2, me)+"一份"+ob->query_idname()+"。\n");
					ob->move(me);
					ob = new("/obj/festival/2007/christmas/child.c");
					set("santaid", me->query_id(1), ob);
					ob->move(loc);
					set_temp("christmas", 2, me); // 第二步
				} else {
					do_command("arc " +me->query_id(1));
				}
				break;
			default:
				do_command("arc " +me->query_id(1));
				break;
		}
	}
}

void reply_get_object(object me, object ob, mixed amount)
{
	string basename = base_name(ob);
	int check = query("christmasgift", me);
	//int quest = query_temp("christmasgift", me);

	if( !objectp(ob) ) return;

	if(basename == "/obj/festival/2007/christmas/xmas_ring" || basename == "/obj/festival/2007/christmas/xmas_ring_hide" 
	|| basename == "/obj/festival/2007/christmas/deer_sled" )
	{
		if(check == 1)
		{
			do_command("say "+pnoun(2, me)+"不想要這份禮物啊？如果"+pnoun(2, me)+"願意再幫神送五份禮物，神就重新給"+pnoun(2, me)+"一份獎勵！！");
			set("christmasgift", 0, me); // 次數歸0
			set_temp("christmasgift", 0, me); // 步驟歸0
			tell( me,HIR"任務重新啟動，請找聖誕老人重新接任務。\n"NOR);
			CHANNEL_D->channel_broadcast("news", me->query_idname()+"決定重新再幫忙聖誕老人一次，請大家給予加油的掌聲！！");
			log_file("command/christmas2007", me->query_idname()+"重新放棄上一個物品重新接任務");
			destruct(ob);
		}
		else
		{
			do_command("say "+pnoun(2, me)+"根本還沒領過獎勵吧！！");
			destruct(ob);
		}
	} else {
		do_command("say 這是"+pnoun(2, me)+"給神的聖誕禮物嗎？謝謝了！！");
		destruct(ob);
	}

}

void destruct_self()
{
	object me, ob;
	mixed loc, test;
	string *where_name = AREA_D->query_areas();
	string where = where_name[random(sizeof(where_name))];
	int num;

	do
	{
  	where = where_name[random(sizeof(where_name))];
	} while ( where == "lostland" || where == "battlefield");

	num = random(AREA_D->query_area_nums(where));
	loc = arrange_city_location(range_random(0,99), range_random(0,99), where, num);

	test = loc_short(loc);

	ob = this_object();
	ob->move(loc);

	//CHANNEL_D->channel_broadcast("news", "聖誕老人來到了。"+test, me);
	//CHANNEL_D->channel_broadcast("news", "聖誕老人來到了 "+AREA_D->query_area_idname(where, num)+" 尋找能幫忙他送禮物的人。", me);
	CHANNEL_D->channel_broadcast("news", "聖誕老人來到了 "+AREA_D->query_area_idname(where)+" "+test+" 尋找能幫忙他送禮物的人。", me);
	call_out( (: destruct_self :), 180 );
}

void create()
{
	set_idname("santa claus", HIR"聖誕"HIW"老人"NOR);

	set("long", "  一年一次的聖誕節才會出現的聖誕老人，和其他聖誕老人不同的是這位老人頭上有長
  角，而且鼻子是藍色的。");
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

	//set("no_fight", 1);

	if( clonep() )
		call_out( (: destruct_self :), 180 );

	// 啟動主動行為
	set_behavior();

	startup_living();
}
