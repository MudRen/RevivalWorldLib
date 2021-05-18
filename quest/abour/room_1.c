/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : room_1.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-12-19
 * Note   : �зǩж�
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

#define BOOK_COST	5000000000

string help = @HELP
�b�o�ӥ��Z�L�_���ж����èS������S����T�C
HELP;

void create()
{
	set("short", HIG"�A�~"NOR GRN"�D�F"NOR);
	set("long",@TEXT
    �o�̬O�@�ӹA�������p�D�F�A�D�F�̧��ۤ@�촣�ۥ��]���ȫȡA���G���b���a
�𮧡C���|�g�ݥh���񪺹A�a���b�V�O�ѺصۡA�Ů��ƨӰ}�}���_�󭻡A�_���H
���n���ҵo�X�Ӫ��P�P�n�]���_�a�ǤF�i�ӡA�o�˪��������H���m���ɡA��Ӥ߱�
���ѦۥD�a�r���_�ӡC

TEXT);
	set("exits", ([ 
		"east" : ({ 76, 47, 0, "lightforest", 0, 0, "76/47/0/lightforest/0/0" }) ,
		"west" : ({ 74, 47, 0, "lightforest", 0, 0, "74/47/0/lightforest/0/0" }) ,
		"north" : ({ 75, 46, 0, "lightforest", 0, 0, "75/46/0/lightforest/0/0" }) ,
		"south" : ({ 75, 48, 0, "lightforest", 0, 0, "75/48/0/lightforest/0/0" }) ,
	]));
	
	set("objects", ([
		"/quest/abour/npc_seller" : 1 ,
	]));
	::reset_objects();
}
void do_buy(object me, string arg)
{
	object ob;
	object seller = filter_array(all_inventory(this_object()), (: $1->is_seller_npc() :))[0];
	string moneyunit = MONEY_D->query_default_money_unit();
	
	if( !arg )
		return seller->do_command("say "+pnoun(2, me)+"�Q�n�R����t���(�аѦ� plist)�H");

	ob = new("/obj/book/product/"+lower_case(replace_string(arg, " ", "_")));

	if( !ob->query_idname() )
	{
		destruct(ob);
		return seller->do_command("say �@�A�ڨS�� "+arg+" �o�ذt��ѭC...�A�Ȥ��P�u�O��p...");
	}
	
	if( !me->spend_money(moneyunit, BOOK_COST) )
	{
		destruct(ob);
		return seller->do_command("say "+pnoun(2, me)+"���W���������� "HIY+money(moneyunit, BOOK_COST)+NOR"�C");
	}

	msg("$ME��O "HIY+money(moneyunit, BOOK_COST)+NOR" �V$YOU�ʶR"+ob->query_idname()+"�C\n", me, seller, 1);
	log_file("command/buy_product_book", me->query_id(1)+" �ʶR"+ob->query_idname());

	ob->set_keep();
	
	if( !me->available_get(ob) )
	{
		tell(me, pnoun(2, me)+"������"+ob->query_idname()+"�F�A�u�n����b�a�W�C\n");
		ob->move_to_environment(me);
	}
	else
		ob->move(me);
}

mapping actions = 
([
	"buy" : (: do_buy :),
]);
