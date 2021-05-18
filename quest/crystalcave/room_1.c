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

string help = @HELP
�b�o�ӥ��Z�L�_���ж����èS������S����T�C
HELP;

void create()
{
	set("short", HIW"��"HIC"��"HIG"�}"HIY"�]"NOR);
	set("long",@TEXT
    �o�̬O�@�ӷ������K���}�]�A���}���������G�A��ӬO�]���}�]���G���F�U��
�U�ˤ��m�}�ɪ������A����ΡB��ΡB�٧ε����A�U�ص������覡���i�H�b�o�̨�
��A�p���_�۪��a���b���H���ص��ޡC�u�i���o�Ǥ�����b�ӹL��w�A�Q�a����
�ܦ��G���O����²�檺�ơC
TEXT);
	set("exits", ([ "out" : ({ 69, 53, 0, "cnossus", 2, 0, "69/53/0/cnossus/2/0" }) ]));

	set("objects", ([
		"/quest/crystalcave/npc_seller" : 1 ,
	]));

	::reset_objects();
}
void do_buy(object me, string arg)
{
	int num;
	object ob;
	object seller = filter_array(all_inventory(this_object()), (: $1->is_seller_npc() :))[0];
	string file;
	string moneyunit = MONEY_D->query_default_money_unit();
	int money;

	if( !arg )
		return seller->do_command("say "+pnoun(2, me)+"�Q�n�R�����C�⪺����(red�Borange�Byellow�Bgreen�Bblue�Bindigo�Bpurple)�H");
	
	sscanf(arg, "%d %s", num, arg);

	switch(arg)
	{
		case "red":
			file = "/obj/gem/gem"+0;
			break;
		case "orange":
			file = "/obj/gem/gem"+1;
			break;
		case "yellow":
			file = "/obj/gem/gem"+2;
			break;
		case "green":
			file = "/obj/gem/gem"+3;
			break;
		case "blue":
			file = "/obj/gem/gem"+4;
			break;
		case "indigo":
			file = "/obj/gem/gem"+5;
			break;
		case "purple":
			file = "/obj/gem/gem"+6;
			break;
		default:
			return seller->do_command("say "+pnoun(2, me)+"�Q�n�R�����C�⪺����(red�Borange�Byellow�Bgreen�Bblue�Bindigo�Bpurple)�H");
			break;
	}

	if( !num )
		num = 1;

	if( num < 1 || num > 1000 )
		return seller->do_command("say �п�J���`���ƶq(1~1000)�C");

	money = num*100000000;

	if( !me->spend_money(moneyunit, money) )
		return seller->do_command("say "+pnoun(2, me)+"���W���������� "HIY+money(moneyunit, money)+NOR"�C\n");

	ob = new(file);
	ob->set_amount(num);
	
	msg("$ME��O "HIY+money(moneyunit, money)+NOR" �V$YOU�ʶR "+num+" ��"+ob->query_idname()+"�C\n", me, seller, 1);
	log_file("command/buy_element", me->query_id(1)+" �ʶR "+num+" ��"+ob->query_idname());

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
