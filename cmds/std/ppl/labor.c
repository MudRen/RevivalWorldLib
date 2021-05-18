/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : labor.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-05-22
 * Note   : labor ���O
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <feature.h>
#include <message.h>
#include <location.h>
#include <daemon.h>
#include <npc.h>
#include <buff.h>

inherit COMMAND;

string help = @HELP
    �o�ӫ��O�����A�d�ݥثe�w�U�Ϊ��Ҧ��H�O�M��A�H�μx�ղM��W���H�O��A��
�b���a��ӡA�H��K�R�O�L���u�@�Ϊ̨�L�Ʊ��C

labor			- ��ܩҦ����u�C��
labor '���u�s��'	- �N�ӽs�������u�I��ܱz�����e(�ЧQ�� order ���O�����u�s��)
labor salary 1000	- �x�s 1000 �����w���w���~��(�w�]���ӫ������f��)

HELP;

void labor_call(object me, object labor)
{
	if( !objectp(me) || !objectp(labor) || BATTLEFIELD_D->inside_battlefield(me) )
		return;

	msg("$ME����F�@�ʨӦ�"+me->query_idname()+"���x�հT���A�ߨ�X�o�e���C\n", labor, 0, 1);
	
	labor->move_to_environment(me);
	
	msg("$ME��ݵ��a����F���B�C\n", labor, 0, 1);
	tell(me, labor->query_idname()+"�w��F�ت��a�C\n");
}

string labor_list(object me)
{
	object env;
   	string place;
	string msg = "";
	object *labors = LABOR_D->get_labors(me);
	int salary, salary_paid, totalsalary;
	string moneyunit = MONEY_D->city_to_money_unit(query("city", me)) || MONEY_D->query_default_money_unit();

	if( !sizeof(labors) )
		return me->query_idname()+"�S�����Υ���H�O�C\n";
			
	msg = pnoun(2, me)+"�`�@���ΤF "+sizeof(labors)+" �ӤH�O�G\n";
	msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
	msg +=     HIW"�s�� �H�O�W��          ��m                         �˱K�� �u�@ ���~($"+moneyunit+")\n"NOR;
	msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;

	foreach( object labor in sort_array(labors, (: query("number", $1) - query("number", $2) :)) )
	{		
		if( query("adventure", labor) )
			place = AREA_D->query_area_name(query("adventure/area", labor), query("adventure/num", labor));
		else if( objectp(env = environment(labor)) )
		{
			array loc = query_temp("location", labor);

			if( env->is_maproom() )
				place = loc_short(loc);
			else
		{
			string roomname = env->query_room_name();
			
			if( roomname &&  noansi_strlen(roomname) > 30 )
				place = loc_short(env->query_location());
			else
				place = roomname || query("short", env);
		}
		}
		else
			place = HIR"�g���b����Ŷ�����"NOR;

		salary = SALARY_D->query_labor_salary(labor);
		totalsalary += salary;
	
		msg += sprintf(HIY" %-3d"NOR" %-18s%-30s%5s %-4s %s\n", query("number", labor), noansi_strlen(labor->query_idname())<=16?labor->query_idname():labor->query_name(), place, HIG+query("relationship/"+me->query_id(1), labor)+NOR, query("job/name", labor)||"�L", HIY+NUMBER_D->number_symbol(salary)+NOR);
	}
    	
	salary_paid = to_int(query("salary_paid", me));
	
	msg += sprintf("\n "NOR YEL"�C�����I�~���`�B "HIY"$%s %s"NOR YEL"�A�w���w���~�� "HIY"$%s %s"NOR YEL" ("HIY"%s"NOR YEL" �Ӥ�)\n"NOR,  moneyunit, NUMBER_D->number_symbol(totalsalary), moneyunit, NUMBER_D->number_symbol(salary_paid), count(totalsalary, ">", 0) ? NUMBER_D->number_symbol(count(salary_paid, "/", totalsalary)) : "0");
	msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
	
	return msg;
}

void labor_salary(object me, string arg)
{
	int money;
	string moneyunit = MONEY_D->city_to_money_unit(query("city", me)) || MONEY_D->query_default_money_unit();

	if( !arg )
		return tell(me, pnoun(2, me)+"�Q�n�x�s�h�֦w���w���~��H\n");

	money = to_int(big_number_check(arg));
	
	if( money <= 0 )
		return tell(me, "�п�J���T�����B�Ʀr�C\n");
		
	if( !me->spend_money(moneyunit, money) )
		return tell(me, pnoun(2, me)+"���W�S����h $"+moneyunit+" ���F�I�C\n");
		
	set("salary_paid", to_int(query("salary_paid", me)) + money, me);
		
	msg("$ME�x�s�F "HIY"$"+moneyunit+" "+NUMBER_D->number_symbol(money)+NOR" ���w���w���~��C\n", me, 0, 1);	
	
	me->save();
}

private void do_command(object me, string arg)
{
	int num;
	string salary;
	int waiting_time;

	if( !arg )
		return tell(me, labor_list(me));

	else if( sscanf(arg, "%d", num)==1 && num >= 0 )
	{
		int count = 0;
		object *labors = LABOR_D->get_labors(me);
			
		if( BATTLEFIELD_D->inside_battlefield(me) )
			return tell(me, "�Գ����L�k�l�����u�C\n");

		foreach( object labor in labors )
		{
			if( query("number", labor) == num )
			{
				if( same_environment(me, labor) )
				{
					tell(me, labor->query_idname()+"�w�g�b�o�̤F�C\n");
					count++;
					continue;
				}

				if( query("adventure", labor) && file_exists(query("adventure/from", labor)) && ADVENTURE_D->is_doing_adventure(query("adventure/from", labor)) )
				{
					tell(me, labor->query_idname()+"���b���汴�I���Ȥ��A�L�k�l�^�C\n");
					count++;
					continue;
				}

				if( query("job/type", labor) == SPORTER && BASEBALL_D->is_playing(labor) )
				{
					tell(me, labor->query_idname()+"���b�i��y�ɡA�ثe�L�k���ʡC\n");
					count++;
					continue;
				}
				
				if( environment(labor) && labor->is_faint() )
				{
					tell(me, labor->query_idname()+"���g���A�ثe�L�k���ʡC\n");
					count++;
					continue;
				}
					
				if( environment(labor) && labor->is_dead() )
				{
					tell(me, labor->query_idname()+"���`�F�A�ثe�L�k���ʡC\n");
					count++;
					continue;
				}
					
				waiting_time = labor->query_loading()/1000;
				
				// �̤� 5 ����
				if( waiting_time < 5 )
					waiting_time = 5;
					
				if( wizardp(me) )
					waiting_time = 0;
				
				msg("$ME���F�@�ʫH���F"+labor->query_idname()+"�s"+pnoun(3, labor)+"�ߨ軰�Ӧ��B(�ݭn "+waiting_time+" ���ɶ�)�C\n", me, 0, 1);
				
				if( query_temp("labor_call_out_handler", labor) )
				{
					remove_call_out(query_temp("labor_call_out_handler", labor));
					delete_temp("labor_call_out_handler", labor);
				}
				set_temp("labor_call_out_handler", call_out((: labor_call, me, labor:), waiting_time), labor);
					
				count++;
			}
		}
		
		if( count > 0 ) return;
		
		return tell(me, "�S���o�ӽs�������u�C\n");
	}
	else if( sscanf(arg, "salary %s", salary) == 1 )
	{
		labor_salary(me, salary);
		return;
	}
	else if( wizardp(me) && find_player(arg) )
		return tell(me, labor_list(find_player(arg)));
	
	return tell(me, "�п�J���T�����O�榡(help labor)�C\n");
}
