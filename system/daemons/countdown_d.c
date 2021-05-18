/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : countdown_d.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-12-31
 * Note   : �˼ƺ��F
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>

#define TARGET_TIME	1199116800
#define TARGET_NAME	NOR CYN"�褸"HIC" 2008 "NOR CYN"�~"NOR

void create()
{
	set_heart_beat(1);
}

void heart_beat()
{
	int now = time();
	
	if( now > TARGET_TIME )
	{
		set_heart_beat(0);
		return;
	}
	
	// �ɶ���F
	if( now == TARGET_TIME )
	{
		string msg = "";
		
		msg += "\n";
		msg += HIM"������"HIC"2008"HIG" HAPPY"HIY" NEW"HIR" YEAR"HIM"������\n\a"NOR;
		msg += HIM"�@�@������"HIC"2008"HIG" HAPPY"HIY" NEW"HIR" YEAR"HIM"������\n\a"NOR;
		msg += HIM"�@�@�@�@������"HIC"2008"HIG" HAPPY"HIY" NEW"HIR" YEAR"HIM"������\n\a"NOR;
		msg += HIM"�@�@�@�@�@�@������"HIC"2008"HIG" HAPPY"HIY" NEW"HIR" YEAR"HIM"������\n\a"NOR;
		msg += HIM"�@�@�@�@�@�@�@�@������"HIC"2008"HIG" HAPPY"HIY" NEW"HIR" YEAR"HIM"������\n\a"NOR;
		msg += "\n";
		shout(msg);
	}
	// �C��˼�
	else if( TARGET_TIME - now <= 10 )
	{
		shout("\n�Z��"+TARGET_NAME+"�˼� "HIR+(TARGET_TIME-now)+NOR" ��C\a\n\n");
	}
	// �C 5 ��˼�
	else if( TARGET_TIME - now <= 60 )
	{
		if( !(now % 5 ) )
			shout("\n�Z��"+TARGET_NAME+"�u�� "HIY+(TARGET_TIME-now)+NOR" ��C\a\n\n");
	}
	// �C�����˼�
	else if( TARGET_TIME - now <= 30*60 )
	{
		if( !(now % 60 ) )
			shout("\n�Z��"+TARGET_NAME+"�٦� "+(TARGET_TIME-now)/60+" �����C\n\n");
	}
	// �C 5 �����˼�
	else if( TARGET_TIME - now <= 60*60 )
	{
		if( !(now % (5*60) ) )
			shout("\n�Z��"+TARGET_NAME+"�٦� "+(TARGET_TIME-now)/60+" �����C\n\n");
	}
	// �C�p�ɭ˼� 
	else if( TARGET_TIME - now <= 24*60*60 )
	{
		if( !(now % (60*60)) )
			shout("\n�Z��"+TARGET_NAME+"�٦� "+(TARGET_TIME-now)/(60*60)+" �p�ɡC\n\n");
	}
	// �C�ѭ˼�
	else
	{
		if( !(now % (24*60*60)) )
			shout("\n�Z��"+TARGET_NAME+"�٦� "+(TARGET_TIME-now)/(24*60*60)+" �ѡC\n\n");
	}
}

int query_lack_time()
{
	return TARGET_TIME - time();
}

string query_name()
{
	return "�˼ƨt��(COUNTDOWN_D)";
}
