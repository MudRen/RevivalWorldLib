/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : module.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-04-06
 * Note   : 
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <feature.h>
#include <ansi.h>
#include <delay.h>
#include <map.h>
#include <daemon.h>

inherit PRODUCT_ACTION_MOD;

mapping inlay_effect =
([
	"/obj/materials/wood" : (: addn_temp("endurance", ($1 > 200 ? 200 : $1) * 1000, $2), addn_temp("original_endurance", ($1 > 200 ? 200 : $1) * 1000, $2) :),
	"/obj/materials/metal" : (: addn("rod_level", ($1 > 60 ? 60 : $1), $2) :),
]);

// �B�z�񳽻�
void do_hook(object me, string arg, object rod)
{
	object bait;

	if ( !arg )
		return tell(me,	pnoun(2, me)+"�Q�n�񤰻�F��b"+rod->short(1)+"�����_�W�H\n");

	if( !objectp(bait = present(arg, me)) )
		return tell(me,	pnoun(2, me)+"���W�S�� "+arg+" �o�تF��\n");

	if( !bait->is_bait() )
		return tell(me,	rod->query_idname()+"�������@����C\n");

	if( me->is_delaying() )
		return tell(me,	me->query_delay_msg());
		
	if( !query_temp("bait", rod) )
	{
		msg("$ME��"+bait->query_idname()+"���"+rod->query_idname()+"�����_�W�C\n", me,	0, 1);
		set_temp("bait", query("bait", bait), rod);
		set_temp("status", NOR"- "HIY"����"+bait->query_name()+NOR, rod);
		destruct(bait, 1);
	}
	else
		return tell(me,	rod->query_idname()+"�W�w�g������F�C\n");
}

// �B�z�_�c�ƥ�
void check_yn(object me, object rod, string arg)
{
	object ob;
	int i;

	if( !objectp(me) )
		return;
		
	me->remove_delay(FISHING_DELAY_KEY);

	if( !objectp(rod) )
		return;

	if( arg == "" || lower_case(arg) == "y" )
	{
		switch( random(7) )
		{
			case 0:
				i = range_random(4, 10);
				ob = new("/obj/fishing/sp_fish/fish"+random(3));
				ob->set_amount(i);
				msg("\n$ME�ߤ@��A�ĤO���@�ԡA��"+ob->short(1)+"�q�����ԤF�_�ӡA��"+rod->query_name()+"�~�M�@�v���ˡC\n", me, 0, 1);
				ob->move(me);
				break;
			case 1:
				i = range_random(8, 16);
				ob = new("/obj/fishing/sp_fish/fish"+random(3));
				ob->set_amount(i);
				msg("\n$ME�ߤ@��A�ĤO���@�ԡA��"+ob->short(1)+"�q�����ԤF�_�ӡA��"+rod->query_name()+"�o�X�y�ԡz���@�n�Ať�_�Ӧ��G����ܤj���ˮ`�C\n", me, 0, 1);
				if( addn_temp("endurance", -1000, ob) < 1 )
					set_temp("endurance", 10, ob);
				ob->move(me);
				break;
			case 2:
				i = range_random(4, 10);
				ob = new("/obj/fishing/adv_fish/fish"+random(4));
				ob->set_amount(i);
				msg("\n$ME�ߤ@��A�ĤO���@�ԡA��"+ob->short(1)+"�q�����ԤF�_�ӡA��"+rod->query_name()+"�~�M�@�v���ˡC\n", me, 0, 1);
				ob->move(me);
				break;
			case 3:
				i = range_random(8, 16);
				ob = new("/obj/fishing/adv_fish/fish"+random(4));
				ob->set_amount(i);
				msg("\n$ME�ߤ@��A�ĤO���@�ԡA��"+ob->short(1)+"�q�����ԤF�_�ӡA��"+rod->query_name()+"�o�X�y�ԡz���@�n�Ať�_�Ӧ��G����ܤj���ˮ`�C\n", me, 0, 1);
				if( addn_temp("endurance", -600, ob) < 1 )
					set_temp("endurance", 10, ob);
				ob->move(me);
				break;
			case 4:
				if( query_temp("endurance", rod) > 1000 )
				{
					check_yn(me, rod, "y");
					return;
				}
				ob = new("/obj/fishing/other/box");
				msg("\n$ME�ߤ@��A�ĤO���@�ԡA��"+ob->short(1)+"�q�����ԤF�_�ӡA��"+rod->query_name()+"�o�]���L�k�Ө����O���_���F�C\n", me, 0, 1);
				set_temp("endurance", -1, ob);
				ob->move(me);
				break;
			case 5:
				ob = new("/obj/fishing/other/box");
				msg("\n$ME�ߤ@��A�ĤO���@�ԡA��"+ob->short(1)+"�q�����ԤF�_�ӡA��"+rod->query_name()+"�o�X�y�ԡz���@�n�Ať�_�Ӧ��G����ܤj���ˮ`�C\n", me, 0, 1);
				if( addn_temp("endurance", -600, ob) < 1 )
					set_temp("endurance", 10, ob);
				ob->move(me);
				break;
			case 6:
				ob = new("/obj/fishing/other/box");
				msg("\n$ME�ߤ@��A�ĤO���@�ԡA��"+ob->short(1)+"�q�����ԤF�_�ӡA��"+rod->query_name()+"�~�M�@�v���ˡC\n", me, 0, 1);
				ob->move(me);
				break;
		}
	}
	else
		tell(me, pnoun(2, me)+"���ϫl�ΤO�ԡA�⳽�u���P�A����b���_�W�������]�F�C\n");
}
// �B�z�N�n����F��
void get_fish(object me, object rod)
{
	int rod_int, bait_int, fish_sk, break_int;
	object ob;

	if( !objectp(me) )
		return;

	if( !objectp(rod) )
	{
		me->remove_delay(FISHING_DELAY_KEY);
		msg("��$ME�Ԩ�@�b�ɡA��M�Pı����L�O�A�u�n��󳨰_�o�����F�C\n", me, 0, 1);
		return;
	}

	me->remove_delay(FISHING_DELAY_KEY);

	rod_int = query("rod_level", rod);
	bait_int = query_temp("bait", rod);
	fish_sk	= to_int(me->query_skill_level("fishing")/1.5);

	if( !random(200) )
		fish_sk += 50;

	// �ü�(���񵥯�) + �����ޯ൥�� + ���絥��
	switch(random(rod_int)+fish_sk+bait_int)
	{
		case 0..19:
			ob = new("/obj/trash/trash"+random(6));
			msg("\n$ME�@��"+rod->query_name()+"�_�Ӥ@�ݡA�~�M����F�D�D�D�D"+ob->query_idname()+"�C\n", me, 0, 1);
			break;
		case 20..39:
			msg("\n$ME�@��"+rod->query_name()+"�_�Ӥ@�ݡA�~�M���򳣨S����C\n", me, 0, 1);
			break;
		case 40..89:
			ob = new("/obj/fishing/fish/fish"+random(9));
			msg("\n$ME�@��"+rod->query_name()+"�_�Ӥ@�ݡA�~�M����F�D�D�D�D"+ob->query_idname()+"�C\n", me, 0, 1);
			break;
		case 90..109:
			ob = new("/obj/fishing/adv_fish/fish"+random(4));
			msg("\n$ME�@��"+rod->query_name()+"�_�Ӥ@�ݡA�~�M����F�D�D�D�D"+ob->query_idname()+"�C\n", me, 0, 1);
			break;
		case 110..129:
			ob = new("/obj/fishing/sp_fish/fish"+random(3));
			msg("\n�@�ѫe�ҥ������j�O�ԫl��$ME�t�I���h���ųQ�ԤJ���̡A��$ME�g�L�\�[�񰫡A�ש��"+ob->query_idname()+"�԰_�ӤF�C\n",	me, 0, 1);
			break;
		case 130..132:
      msg("\n�u��$ME�@���ϫl�ΤO���ԡA�i�O�٬O�����Ԥ��_�ӳo���쪺�F��C\n", me, 0, 1);
			if( userp(me) )
			{
				tell(me, pnoun(2, me)+"�n�Ϻɥ��O�ΤO���ԶܡH("HIY"Y"NOR"/n)\n");
				input_to( (: check_yn, me, rod :) );
			}
			else
			{
				check_yn(me, rod, "y");
			}
				
			break;
		case 133..9999:
			ob = new("/obj/fishing/sp_fish/fish"+random(3));
			msg("\n�@�ѫe�ҥ������j�O�ԫl��$ME�t�I���h���ųQ�ԤJ���̡A��$ME�g�L�\�[�񰫡A�ש��"+ob->query_idname()+"�԰_�ӤF�C\n",	me, 0, 1);
			break;
	}

	delete_temp("bait", rod);
	delete_temp("status", rod);
	
	if( objectp(ob) )
	{
		if( !me->available_get(ob) )
		{
			msg("$ME�����ʨ���h�F��F�A�u�n����b�a�W�C\n", me, 0, 1);
			ob->move_to_environment(me);
		}
		else
			ob->move(me);
	}
	// �B�z����O�_�n�_�����v
	break_int = (100/(fish_sk||1));
	if( break_int < 5 ) break_int = 5;
	if( break_int > 15 ) break_int = 15;
	
	if( addn_temp("endurance", range_random(-break_int, 1), rod) < 0 )
	{
		msg("\n$ME��W��"+rod->query_name()+"�o�X�y�ԡz���@�n�A�@�a���ݵo�{���w�g�_�F�C\n", me, 0, 1);
		set_temp("endurance", copy(query_temp("original_endurance", rod)), rod);
		call_out( (: destruct, rod, 1 :), 0);
	}	
}

void fishing(object me, object rod)
{
	if( !objectp(me) ) return;
	
	if( !me->cost_stamina(50) )
	{
		me->remove_delay(FISHING_DELAY_KEY);
		return tell(me,	pnoun(2, me)+"�Ӳ֤F�A�L�k�~��M�ߪ������C\n");
	}

	me->add_social_exp(range_random(150, 300));

	// �B�z�����ƥ�
	switch(	random(10) ) {
		case 0..2:
			msg("\e[1;"+3+random(8)+"m�B�Цb���W�ư��ƪ��A���G�S�����򲧪��o�͡D�D�D\n"NOR,	me, 0, 1);
			call_out( (: fishing, me, rod :), 6);
			break;
		case 3..5:
			msg("\e[1;"+3+random(8)+"m�B�Ц��G�L�L���ʤF�A���S�h�[�S�����\�ʤF�D�D�D\n"NOR,	me, 0, 1);
			call_out( (: fishing, me, rod :), 6);
			break;
		case 6:
			if(!query_temp("bait", rod))
				msg("\n\e[1;"+3+random(8)+"m�B�Ь�M�r�O���I�J�����A�i�O$ME�o�Ӥ��ΩԤW����A���N�o�˶]���F�D�D�D\n"NOR, me, 0,	1);
			else 
			{
				msg("\n\e[1;"+3+random(8)+"m�B�Ь�M�r�O���I�J�����A�i�O$ME�o�Ӥ��ΩԤW����A����N�o�˳Q�Y���F�D�D�D\n"NOR, me, 0, 1);
				delete_temp("bait", rod);
				delete_temp("status", rod);
			}
			
			me->remove_delay(FISHING_DELAY_KEY);
			break;
		case 7..9:
			msg("��M���A�B�Ф������E�P�W�U�\�ʵۡA$ME����ΤO�@�ԡD�D�D\n", me, 0,	1);
			if( !me->cost_stamina(50) )	
			{
				me->remove_delay(FISHING_DELAY_KEY);
				msg("��$ME�Ԩ�@�b�ɡA��M�Pı����L�O�A�u�n��󳨰_�o�����F�C\n", me, 0, 1);
			}
			else
				call_out( (:get_fish ,me, rod :), 2);
			break;
	}
}

void do_fishing(object me, string arg, object rod)
{
	object env = environment(me);
	array loc = query_temp("location", me);

	// �L�k�b�D�a�Ϥ�����	
	if( !env->is_maproom() )
		return tell(me,	pnoun(2, me)+"�L�k�b�ж��̳����C\n");

	if( environment(rod) != me )
		return tell(me, pnoun(2, me)+"�����N"+rod->query_name()+"���b��W�~��}�l�����C\n");

	// Delay ������U���O
	if( me->is_delaying() )
		return tell(me,	me->query_delay_msg());

	if( member_array( RIVER	, values((MAP_D->query_map_system(loc))->query_coor_range(loc, TYPE, 1))) != -1	)
	{
		msg("$ME���X"+rod->query_name()+"�A���e�誺�e���ΤO�@�ϥX��C\n", me, 0, 1);
		call_out( (: fishing, me, rod :), 5);
		me->start_delay(FISHING_DELAY_KEY, 999, pnoun(2, me)+"�����۳����C\n");
	}
	else 
		return tell(me,	"�o����S���i�H�������a��C\n");
}

// �]�w����ʧ@
nosave mapping action_info =
([
	"help"	: "�|���غc�����C",
	"action":
		([
			"fishing" 	: (: do_fishing :),
			"hook" 		: (: do_hook :)
		]),
]);

// �]�w������
nosave mapping product_info = 
([	
	//����W��
	"name":		"����",
	
	//�^��W��
	"id":		"fishing rod",

	//���ƻݨD
	"material":	([ "man_made_fibers":1, "plastic":1 ]),

	//�ޯ�ݨD
	"skill":	([ "hardware-fac":70, "technology":70 ]),

	//�Ͳ��u�t
	"factory":	"hardware",
]);
