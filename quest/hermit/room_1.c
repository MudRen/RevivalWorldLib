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

#define BOOK_COST	3000000000

string help = @HELP
�b�o�ӥ��Z�L�_���ж����èS������S����T�C
HELP;

void create()
{
	set("short", HIC"��"NOR CYN"���~"NOR);
	set("long",@TEXT
    �o�̬O�@�����h���a�A���h���b�Фl���@���A���즳�H�i�Ӥ]���ݤ@���A�u�M
�`�b�ۤv��W���ѡC�Фl�����F���������y�H�~�X�G�S����L���F��F�A�ݨ�����
�h�������A�y�W�@�D�������M�ͱq�B�Y�k��e�ܤU�ڡA���G���g�O��^�i���Ԥh
�A�o�������󦹮ɪ��L�o�u�b�G�⤤���ѡA���G�@�ɤW���@�����w�g�P�L�L���F�C

TEXT);
	set("exits", ([ 
		"west" : ({ 38, 56, 0, "sixcastle", 3, 0, "38/56/0/sixcastle/3/0" }) ,
	]));
	
	set("objects", ([
		"/quest/hermit/npc_seller" : 1 ,
	]));
	::reset_objects();
}
void do_buy(object me, string arg)
{
	object ob;
	object seller = filter_array(all_inventory(this_object()), (: $1->is_seller_npc() :))[0];
	string moneyunit = MONEY_D->query_default_money_unit();
	
	if( !arg )
		return seller->do_command("say "+pnoun(2, me)+"�Q�n�R���اޯ�ѡH�n�R�N�֤@�I�A�R���������}�ڪ����u");

	if( !objectp(ob = new("/obj/book/skill/"+lower_case(replace_string(arg, " ", "-")))) )
		return seller->do_command("say ��A���n�H���ڤ����A�o�@�ɤW�ڥ��S�� "+arg+" �o�اޯ��...");

	if( !me->spend_money(moneyunit, BOOK_COST) )
	{
		destruct(ob);
		return seller->do_command("say "+pnoun(2, me)+"���W�����ڥ����� "HIY+money(moneyunit, BOOK_COST)+NOR"�C\n");
	}

	msg("$ME��O "HIY+money(moneyunit, BOOK_COST)+NOR" �V$YOU�ʶR"+ob->query_idname()+"�C\n", me, seller, 1);
	log_file("command/buy_skill_book", me->query_id(1)+" �ʶR"+ob->query_idname());

	ob->set_keep();
	
	if( !me->available_get(ob) )
	{
		tell(me, pnoun(2, me)+"������"+ob->query_idname()+"�F�A�u�n����b�a�W�C\n");
		ob->move_to_environment(me);
	}
	else
		ob->move(me);
	
	seller->do_command("say �R���N�����}�A���n�b�o�̤z�Z��");
}

mapping actions = 
([
	"buy" : (: do_buy :),
]);
