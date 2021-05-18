/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : child.c
 * Author : Kyo@RevivalWorld
 * Date   : 2007-12-21
 * Note   : 小孩
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
#include <npc.h>
#include <condition.h>
#include <feature.h>

inherit STANDARD_PERSON;

void reply_get_object(object me, object ob, mixed amount)
{
	string basename = base_name(ob);
	string checkid = query("santaid", this_object());
	//int quest = query_temp("christmasgift", me);

	if( !objectp(ob) ) return;

	if(basename == "/obj/festival/2007/christmas/gift_box")
	{
		if(checkid == me->query_id(1))
		{
			destruct(ob);
			do_command("jump " +me->query_id(1));
			do_command("say 謝謝"+pnoun(2, me)+"幫我送來這份禮物，我要拿回家去給媽媽看！！");
			set_temp("christmas", 0, me); // 步驟歸0
			addn_temp("christmasgift", 1, me); // 增加一次
			tell( me,HIR"任務完成請繼續幫忙聖誕老人送下一次的禮物。\n"NOR);
			destruct(this_object());
		}
		else
		{
			do_command("say 這個禮物不是人家要的啦！！");
			do_command("drop all "+ob->query_id(1)+"");
		}
	} else {
		do_command("say 人家要聖誕老公公給的禮物不是這個啦！！");
		do_command("drop all "+ob->query_id(1)+"");
	}

}

void destruct_self()
{
        //shout(HIB"遠方"NOR" - 等不到禮物的小孩失望的回家了。\n");
	destruct(this_object());
}

void create()
{
	set_idname("child", "小孩");

	set("unit", "隻");
	set("age", range_random(1, 5));
	set("gender", MALE_GENDER);

	set("long", "　等待聖誕老人送禮物的小孩。");
	set("no_fight", 1);

	startup_living();

	if( clonep() )
		call_out( (: destruct_self :), 180 ); // 三分鐘內沒送到就消失
}

int remove()
{
	destruct(all_inventory(this_object()));
}

