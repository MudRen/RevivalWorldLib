/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : labor.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-05-22
 * Note   : labor 指令
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
    這個指令能讓你查看目前已顧用的所有人力清單，以及徵調清單上的人力到你所
在的地方來，以方便命令他做工作或者其他事情。

labor			- 顯示所有員工列表
labor '員工編號'	- 將該編號之員工呼喚至您的面前(請利用 order 指令為員工編號)
labor salary 1000	- 儲存 1000 元的安全預備薪資(預設為該城市的貨幣)

HELP;

void labor_call(object me, object labor)
{
	if( !objectp(me) || !objectp(labor) || BATTLEFIELD_D->inside_battlefield(me) )
		return;

	msg("$ME收到了一封來自"+me->query_idname()+"的徵調訊息，立刻出發前往。\n", labor, 0, 1);
	
	labor->move_to_environment(me);
	
	msg("$ME氣喘虛虛地趕到了此處。\n", labor, 0, 1);
	tell(me, labor->query_idname()+"已到達目的地。\n");
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
		return me->query_idname()+"沒有雇用任何人力。\n";
			
	msg = pnoun(2, me)+"總共雇用了 "+sizeof(labors)+" 個人力：\n";
	msg += NOR WHT"─────────────────────────────────────\n"NOR;
	msg +=     HIW"編號 人力名稱          位置                         親密度 工作 月薪($"+moneyunit+")\n"NOR;
	msg += NOR WHT"─────────────────────────────────────\n"NOR;

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
			place = HIR"迷失在閉鎖空間之中"NOR;

		salary = SALARY_D->query_labor_salary(labor);
		totalsalary += salary;
	
		msg += sprintf(HIY" %-3d"NOR" %-18s%-30s%5s %-4s %s\n", query("number", labor), noansi_strlen(labor->query_idname())<=16?labor->query_idname():labor->query_name(), place, HIG+query("relationship/"+me->query_id(1), labor)+NOR, query("job/name", labor)||"無", HIY+NUMBER_D->number_symbol(salary)+NOR);
	}
    	
	salary_paid = to_int(query("salary_paid", me));
	
	msg += sprintf("\n "NOR YEL"每月應付薪水總額 "HIY"$%s %s"NOR YEL"，安全預備薪資 "HIY"$%s %s"NOR YEL" ("HIY"%s"NOR YEL" 個月)\n"NOR,  moneyunit, NUMBER_D->number_symbol(totalsalary), moneyunit, NUMBER_D->number_symbol(salary_paid), count(totalsalary, ">", 0) ? NUMBER_D->number_symbol(count(salary_paid, "/", totalsalary)) : "0");
	msg += NOR WHT"─────────────────────────────────────\n"NOR;
	
	return msg;
}

void labor_salary(object me, string arg)
{
	int money;
	string moneyunit = MONEY_D->city_to_money_unit(query("city", me)) || MONEY_D->query_default_money_unit();

	if( !arg )
		return tell(me, pnoun(2, me)+"想要儲存多少安全預備薪資？\n");

	money = to_int(big_number_check(arg));
	
	if( money <= 0 )
		return tell(me, "請輸入正確的金額數字。\n");
		
	if( !me->spend_money(moneyunit, money) )
		return tell(me, pnoun(2, me)+"身上沒那麼多 $"+moneyunit+" 錢了！。\n");
		
	set("salary_paid", to_int(query("salary_paid", me)) + money, me);
		
	msg("$ME儲存了 "HIY"$"+moneyunit+" "+NUMBER_D->number_symbol(money)+NOR" 的安全預備薪資。\n", me, 0, 1);	
	
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
			return tell(me, "戰場中無法召集員工。\n");

		foreach( object labor in labors )
		{
			if( query("number", labor) == num )
			{
				if( same_environment(me, labor) )
				{
					tell(me, labor->query_idname()+"已經在這裡了。\n");
					count++;
					continue;
				}

				if( query("adventure", labor) && file_exists(query("adventure/from", labor)) && ADVENTURE_D->is_doing_adventure(query("adventure/from", labor)) )
				{
					tell(me, labor->query_idname()+"正在執行探險任務中，無法召回。\n");
					count++;
					continue;
				}

				if( query("job/type", labor) == SPORTER && BASEBALL_D->is_playing(labor) )
				{
					tell(me, labor->query_idname()+"正在進行球賽，目前無法移動。\n");
					count++;
					continue;
				}
				
				if( environment(labor) && labor->is_faint() )
				{
					tell(me, labor->query_idname()+"昏迷中，目前無法移動。\n");
					count++;
					continue;
				}
					
				if( environment(labor) && labor->is_dead() )
				{
					tell(me, labor->query_idname()+"死亡了，目前無法移動。\n");
					count++;
					continue;
				}
					
				waiting_time = labor->query_loading()/1000;
				
				// 最少 5 秒鐘
				if( waiting_time < 5 )
					waiting_time = 5;
					
				if( wizardp(me) )
					waiting_time = 0;
				
				msg("$ME捎了一封信給了"+labor->query_idname()+"叫"+pnoun(3, labor)+"立刻趕來此處(需要 "+waiting_time+" 秒的時間)。\n", me, 0, 1);
				
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
		
		return tell(me, "沒有這個編號的員工。\n");
	}
	else if( sscanf(arg, "salary %s", salary) == 1 )
	{
		labor_salary(me, salary);
		return;
	}
	else if( wizardp(me) && find_player(arg) )
		return tell(me, labor_list(find_player(arg)));
	
	return tell(me, "請輸入正確的指令格式(help labor)。\n");
}
