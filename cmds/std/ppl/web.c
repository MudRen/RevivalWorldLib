/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : web.c
 * Author : Clode@RevivalWorld
 * Date   : 2006-02-11
 * Note   : web ���O
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <feature.h>
#include <daemon.h>

inherit COMMAND;

string help = @HELP
    �]�w�ۤv���Y���Ϥ�������}�A���z�������b�Ӻ��}�W��m�z���Y��
�Ϥ��A�]�i�H������J�����W�w�g�s�b���U�عϤ����}�C
    
web avatar ���ɺ��}		�]�w�z���Y���Ϥ����}
web avatar -d			�����Y���]�w
web homepage �������}		�]�w�z���ӤH�������}
web homepage -d			���������]�w

�]�w����K�i�H�� http://www.revivalworld.org/rw/online/who �[�ݤj�a�����

HELP;

private void do_command(object me, string arg)
{
	string option, value;

	if( !arg )
	{
		tell(me, pnoun(2, me)+"���Y���Ϥ����}�G"+(query("web/avatar", me)||"���]�w")+"\n");
		tell(me, pnoun(2, me)+"���ӤH�������}�G"+(query("web/homepage", me)||"���]�w")+"\n");
		return;
	}
	
	if( sscanf(arg, "%s %s", option, value) != 2 )
		return tell(me, "�п�J���T�����O�榡�C\n");
		
	if( option == "avatar" )
	{
		if( value == "-d" )
		{
			delete("web/avatar", me);
	
			// �Y�ɧ�s
			MYSQL_SCHEDULE_D->refresh_who();
			
			return tell(me, pnoun(2, me)+"�N�Y���Ϥ������F�C\n");
		}
		
		if( strsrch(value, "'") != -1 )
			return tell(me, "���}������֦� ' �r���C\n");
	
		if( noansi_strlen(value) > 512 )
			return tell(me, "���}�̦h�u�঳ 512 �Ӧr����C\n");
		
		set("web/avatar", value, me);

		
		tell(me, pnoun(2, me)+"�N�Y���Ϥ����}�]�w���u"+value+"�v�C\n"NOR);
	
		// �Y�ɧ�s
		MYSQL_SCHEDULE_D->refresh_who();
	}
	else if( option == "homepage" )
	{
		if( value == "-d" )
		{
			delete("web/homepage", me);
	
			// �Y�ɧ�s
			MYSQL_SCHEDULE_D->refresh_who();
			
			return tell(me, pnoun(2, me)+"�N�ӤH���������F�C\n");
		}
		
		if( strsrch(value, "'") != -1 )
			return tell(me, "���}������֦� ' �r���C\n");
	
		if( noansi_strlen(value) > 512 )
			return tell(me, "���}�̦h�u�঳ 512 �Ӧr����C\n");
		
		set("web/homepage", value, me);

		
		tell(me, pnoun(2, me)+"�N�ӤH�������}�]�w���u"+value+"�v�C\n"NOR);
	
		// �Y�ɧ�s
		MYSQL_SCHEDULE_D->refresh_who();
	}
	else
		return tell(me, "�п�J���T�����O�ﶵ�C\n");
}