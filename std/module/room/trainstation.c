/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : trainstation.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-04-11
 * Note   : ������
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <map.h>
#include <feature.h>
#include <message.h>
#include <daemon.h>
#include <npc.h>
#include <location.h>
#include <delay.h>
#include <citydata.h>

inherit ROOM_ACTION_MOD;

#define TICKET_PRICE		100000
#define TICKET			"/obj/systemproducts/train_ticket"

string *check_trains(object room)
{
	object target;
	string* trains;
	string* deleted_trains;

	if( !sizeof(trains = query("trains", room)) )
		return 0;

	deleted_trains = allocate(0);

	foreach(string file in trains)
	{
		if( !objectp(target = load_module(file)) || query("function", target) != "platform" )
			deleted_trains |= ({ file });
	}
	
	if( sizeof(deleted_trains) )
	{
		trains -= deleted_trains;
		set("trains", trains, room);
		room->save();
	}
	
	return trains;
}

void do_setup(object me, string arg)
{
	string value;
	object env = environment(me);
	string city = env->query_city();
	string* trains;

	if( !arg )
		return tell(me, pnoun(2, me)+"�Q�n�]�w���򶵥ءH(help setup)\n");

	sscanf(arg, "%s %s", arg, value);
	
	if( !CITY_D->is_mayor_or_officer(city, me) )
		return tell(me, pnoun(2, me)+"���O�o�y�����������Ωx���A�L�k�]�w������x�Z���C\n");

	trains = check_trains(env) || allocate(0);

	switch(arg)
	{
		case "add":
		{
			int num, x, y;
			string file;
			object target;

			if( !value || (sscanf(value, "'%s' %d %d,%d", city, num, x, y) != 4 && sscanf(value, "%s %d %d,%d", city, num, x, y) != 4) )
				return tell(me, "�п�J���T���������y��(�Ҧp�Ganycity 1 21,33)�C\n");
			
			file = CITY_ROOM_MODULE(city, (num-1), (x-1), (y-1), "trainstation");

			if( !objectp(target = load_module(file)) || query("function", target) != "platform" )
				return tell(me, "�y��"+loc_short(city, num-1, x-1, y-1)+"�èS����������x�C\n");

			trains |= ({ file });
			set("trains", trains, env);
			
			env->save();
			env->startup_heart_beat();
			msg("$ME�W�[�F�e��"+CITY_D->query_city_name(city, num-1)+target->query_room_name()+"���Z���C\n", me, 0, 1);
			break;	
		}
		case "delete":
		{
			int num = to_int(value);
			
			if( num < 1 || num > sizeof(trains) )
				return tell(me, "�п�J���T���s���C\n");
				
			trains = trains[0..(num-2)] + trains[num..];
			
			set("trains", trains, env);
			
			env->save();
			msg("$ME�R���F�s�� "+num+" ���Z���C\n", me, 0, 1);
			break;
		}
		default:
			return tell(me, "�п�J���T�����O(help setup)�C\n");
	}
}

void do_list(object me, string arg)
{
	int num;
	string msg="";
	object env = environment(me);
	string* trains;
	object target;

	if( !sizeof(trains = check_trains(env)) )
		return tell(me, "�ثe�S����������Z���C\n");
	
	msg += WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
	msg += HIW"�Z��   �ت��a\n"NOR;
	msg += WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
	
	foreach(string file in trains)
	{
		target = load_module(file);
		msg += sprintf(HIY"%4d"NOR"   %s\n", ++num, CITY_D->query_city_name(target->query_city(), target->query_city_num())+target->query_room_name());
	}

	msg += WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
	
	me->more(msg);
}


// �ʶR����
void do_buy(object me, string arg)
{
	object ob;
	string money_unit = environment(me)->query_money_unit();
	
	if( !me->spend_money(money_unit, TICKET_PRICE) )
		return tell(me, pnoun(2, me)+"���W�������� "HIY+money(money_unit, TICKET_PRICE)+NOR"�C\n");

	ob = new(TICKET);
	msg("$ME��F "HIY+money(money_unit, TICKET_PRICE)+NOR+" �R�F$YOU�C\n", me, ob, 1);

	ob->move(me);
}


void do_board(object me, string arg)
{
	object room = environment(me);
	object train_ob = query_temp("train_ob", room);
	object* ob = filter_array(all_inventory(me), (: base_name($1) == TICKET :));

	if( environment(me) == this_object() )
		return tell(me, pnoun(2, me)+"�w�g�b�����W�F�C\n");
		
	if( !sizeof(ob) )
		return tell(me, pnoun(2, me)+"�������e�������j�U�ʶR�����C\n");

	if( !objectp(train_ob) )
		return tell(me, "�����|���i���A�Цb��x�W�y�ԡC\n");

	msg("$ME�N"+ob[0]->query_idname()+"�浹�F�������÷f�W�F"+train_ob->query_idname()+"�C\n", me, 0, 1);
	me->move(train_ob);
	broadcast(environment(me), me->query_idname() + "�f�W�F���Z�C���C\n", 0, me);
	destruct(ob, 1);
}

void heart_beat(object room)
{
	int tick;
	string* trains;
	string file;
	object target;
	
	if( !sizeof(trains = check_trains(room)) )
	{
		room->stop_heart_beat();
		return;
	}

	if( !stringp(file = query("train", room)) )
	{
		file = trains[addn_temp("next", 1, room)%sizeof(trains)];
			set("train", file, room);
	}

	if( !objectp(target = load_module(file)) )
		return;
	
	tick = addn_temp("tick", 1, room);

	// 0 ��
	if( !(tick%30) )
	{
		object train_ob;
		
		target = load_module(file);
		
		train_ob = new("/obj/etc/train", target);
		
		train_ob->move(room);

		broadcast(room, "�s�C���w�g�i���A���C���N�� 30 ���e��"+CITY_D->query_city_name(target->query_city(), target->query_city_num())+target->query_room_name()+"�A�б��f�����Z�C�������Ⱦ��t�W���C\n");
		
		set_temp("train_ob", train_ob, room);
	}
	// 20 ��
	else if( !((tick+26)%30) )
		broadcast(room, "�e��"+CITY_D->query_city_name(target->query_city(), target->query_city_num())+target->query_room_name()+"���C���b 10 ������Y�N�����A�б��f�����Z�C�������Ⱦ��t�W���C\n");
	// 30 ��
	else if( !((tick+24)%30) )
	{
		object train_ob = query_temp("train_ob", room);
		
		if( !objectp(train_ob) )
			return;

		broadcast(room, "�e��"+CITY_D->query_city_name(target->query_city(), target->query_city_num())+target->query_room_name()+"���C���w�g�����C\n");

		train_ob->start();

		delete("train", room);

		delete_temp("train_ob", room);
	}
}

string look_room(object room)
{
	string msg = "";

	if( query("function", room) == "platform" )
	{
		string file = query("train", room);
		object train_ob = query_temp("train_ob", room);
		int tick = query_temp("tick", room);
		object target;

		target = load_module(file);
		
		if( !objectp(target) )
			msg += HIG"�ثe��x�W�L����C���i�f\n\n"NOR;
		else if( objectp(train_ob) )
			msg += HIY"���Z�C��"NOR YEL"�ت��a�G"+CITY_D->query_city_name(target->query_city(), target->query_city_num())+target->query_room_name()+" ("WHT"�N�� "HIW+(30-((tick%30)*5))+WHT" ����������"NOR")\n\n";
		else
			msg += HIY"�U�Z�C��"NOR YEL"�ت��a�G"+CITY_D->query_city_name(target->query_city(), target->query_city_num())+target->query_room_name()+" ("WHT"�N�� "HIW+(150-((tick%30)*5))+WHT" ������i��"NOR")\n\n";
	}
	return msg;
}

// �]�w�ؿv�����ж����A����
nosave mapping action_info =
([
	"platform"	:
	([
		"short"	: HIC"����"NOR CYN"��x"NOR,
		"help"	:
			([
"topics":
@HELP
    �ȫȷf������x�C
HELP,

"setup":
@HELP
setup add anycity 1 21,33	- �W�[�y�� ancity 1 21,33 �����������C���ت��a
setup delete �s��		- �R���Y�Z�Z��(�̷� list �s���R��)
HELP,

"board":
@HELP
board			- ��X�����f�W����
HELP,

"list":
@HELP
list			- �C�X�o�Ӥ�x�q�����ت��a
HELP,

			]),
		"heartbeat":5,	// ��ڮɶ� 1 �����
		"master":1,
		"action":
			([
				"setup" :	(: do_setup :),
				"board" :	(: do_board :),
				"list"	:	(: do_list :),
			]),
	
	]),	
	"lobby"	:
	([
		"short"	: HIY"����"NOR YEL"�j�U"NOR,
		"help"	:
			([
"topics":
@HELP
    �ʶR�������j�U�C
HELP,
			]),
		"heartbeat":0,	// ��ڮɶ� 1 �����
		"master":1,
		"action":
			([
				"buy"	:	(: do_buy :),
			]),
	]),
]);

// �]�w�ؿv�����
nosave array building_info = ({
	
	// �ؿv��������W��
	HIY"������"NOR

	// �}�i���ؿv�����̤֩ж�����
	,3

	// �������̤j�ۦP�ؿv�ƶq����(0 �N������)
	,0

	// �ؿv���ؿv����, GOVERNMENT(�F��), ENTERPRISE(���~����), INDIVIDUAL(�p�H)
	,GOVERNMENT

	// �ؿv���i�ؿv�ϰ�, AGRICULTURE_REGION(�A�~), INDUSTRY_REGION(�u�~), COMMERCE_REGION(�ӷ~)
	,COMMERCE_REGION

	// �}�i�����O��
	,50000000
	
	// �ؿv���������ռаO
	,0

	// �c�a�^�m��
	,40
	
	// �̰��i�[�\�Ӽh
	,1
	
	// �̤j�ۦP�ؿv���ƶq(0 �N������)
	,0
	
	// �ؿv���ɥN
	,2
});
