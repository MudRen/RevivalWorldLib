/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : room_8.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-12-19
 * Note   : 標準房間
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>
#include <quest.h>
#include <daemon.h>

inherit STANDARD_ROOM;

string help = @HELP
在這個平凡無奇的房間內並沒有任何特殊的資訊。
HELP;

void create()
{
	set("short", HIW"隱密"NOR WHT"山洞洞底"NOR);
	set("long",@TEXT
    這裡是一個極為隱密的山洞，裡面一片漆黑，往手邊摸去都是冷冰冰且潮濕的
石璧，頭頂上也不時地滴下水來，不過往裡頭一看，似乎格局還挺方正的，地上也
可以偶爾見到幾株正在生長的野山人參，而在這個方正的山洞正中央則有一個很顯
著的白玉石臺，石臺的上方似乎可以放一些東西。
TEXT);
	set("exits", ([ "out" : ({ 10, 70, 0, "lightforest", 7, 0, "10/70/0/lightforest/7/0" }) ]));

	::reset_objects();
}

void do_put(object me, string arg)
{
	object ob;
	
	if( !arg || !arg[0] )
		return tell(me, pnoun(2, me)+"想要放什麼東西在石臺上？\n");
		
	ob = present(arg);
	
	if( !objectp(ob) )
		return tell(me, "沒有這樣東西。\n");
		
	if( base_name(ob) != "/quest/yin_ling_yu/tear_wild_ginseng" )
		return tell(me, pnoun(2, me)+"將"+ob->query_idname()+"放到石臺上好一陣子，似乎沒啥反應因此又拿了下來。\n");

	tell(me, pnoun(2, me)+"將"+ob->query_idname()+"放到石臺上。\n");
	
	destruct(ob, 1);
	ob = 0;

	tell(me, WHT+@TEXT

    房間裡突然傳了一個細細的聲音：人間的至情至愛便是如此，有人能為了愛而
放棄對方，有人能為了愛而拿起血腥的武器，更有人能為了愛而失去生命，雖然最
後只留下了悔淚的眼淚，但這就是人生...
    我的至靈參已經完成，非常感謝您做的一切，我這裡沒有什麼特別的東西，就
允許我將一部份的至靈參拿來餵食您的座騎吧，如果您沒有座騎的話我就將我利用
野山人參長期飼養的座騎送給您吧

TEXT+NOR);

	foreach(object inv in all_inventory(me))
	{
		switch(base_name(inv))
		{	
			case "/obj/pasture/treasure/giant_eagle":
				ob = new("/obj/pasture/treasure/true_giant_eagle");
				destruct(inv);
				break;
			case "/obj/pasture/treasure/fly_horse":
				ob = new("/obj/pasture/treasure/true_fly_horse");
				destruct(inv);
				break;
			case "/obj/pasture/treasure/kylin":
				ob = new("/obj/pasture/treasure/true_kylin");
				destruct(inv);
				break;
			case "/obj/pasture/treasure/mammoth":
				ob = new("/obj/pasture/treasure/true_mammoth");
				destruct(inv);
				break;
			case "/obj/pasture/treasure/pack_animal":
				ob = new("/obj/pasture/treasure/true_pack_animal");
				destruct(inv);
				break;
		}
		
		if( objectp(ob) )
			break;
	}
	
	if( !objectp(ob) )
		ob = new("/obj/pasture/treasure/true_horse");

	msg("$ME得到了$YOU。\n", me, ob, 1);
	ob->set_keep();
	ob->move(me);
	
	me->set_quest_finish(QUEST_YIN_LING_YU_2, QUEST_YIN_LING_YU_2_NAME, "通過了重重考驗，將至靈參交回主人手中，並得到一些獎勵。");
	
	if( !wizardp(me) )
		CHANNEL_D->channel_broadcast("city", me->query_idname()+"解開了「"+QUEST_YIN_LING_YU_2_NAME+"」", me);
}

mapping actions = 
([
	"put" : (: do_put :),
]);
