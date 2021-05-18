/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : ignore.c
 * Author : Msr@RevivalWorld
 * Date   : 2001-07-02
 * Note   : ignore ���O
 * Update :
 *  o 2000-00-00  
 -----------------------------------------
 */

#include <ansi.h>
#include <feature.h>
#include <message.h>
#include <daemon.h>

inherit COMMAND;

string help = @HELP
���O����:

�����O�i�H�L�o�Y�ǤH����ͤΪ��ʧ@�åB�ڵ�����H

���O�榡 : ignore        �C�X�Ҧ����ڵ��z�|���¦W��
           ignore -a id  �W�[�s�� id �i�J�¦W�椺
           ignore -d id  �N�Y�ӤH�� id �q�¦W�椤����

�������O:
	tell, channels
HELP;

private void do_command(object me, string str)
{
	int i = 0;
	string id, *ignore, msg;

	if( !is_command() ) return;

	if( !arrayp( ignore = query("ignore", me) ) )
		ignore = allocate(0);

	if( !stringp(str) )
	{
		if( !sizeof(ignore) ) 
			return tell( me, pnoun(2, me)+"�ثe�S������¦W��C\n");

		msg = pnoun(2, me)+"���¦W��p�U :\n";

		for( i=0 ; i < sizeof(ignore); i++)
			msg += sprintf( "[ %d ]\t%s\n", i+1, ignore[i] );

		return tell( me, msg);
	}

	if( sscanf( str,"-d %s",id) )
	{
		id = remove_ansi(lower_case(id));

		if( member_array( id, ignore ) == -1 )
			return tell(me, id+" �쥻�N���b�¦W�椤�C\n");

		ignore -= ({ id });

		if( !sizeof(ignore) )
			delete("ignore", me);
		else
			set("ignore", ignore, me);

		me->save();
		return tell(me, pnoun(2, me)+"�N "+capitalize(id)+" �q"+pnoun(2, me)+"���¦W�椤�����F�C\n");
	}

	if( sscanf(str, "-a %s",id) )
	{
		object *followers = query_temp("follower", me);

		id = remove_ansi(lower_case(id));

		if( !user_exists(id) )
			return tell(me, "���C�����èS�� "+capitalize(id)+" �o�쪱�a����C\n");

		if( member_array( id, ignore ) !=-1 ) 
			return tell(me, capitalize(id)+" �w�g�b"+pnoun(2, me)+"���¦W�椤�C\n");

		if( id == me->query_id(1) )
			return tell(me, pnoun(2, me)+"����N�ۤv�[�J�¦W�椤�C\n");

		ignore |= ({ id });

		set("ignore", ignore, me );
		me->save();
		
		if( arrayp(followers) )
		{
			object target_ob = find_player(id);
			
			if( objectp(target_ob) && member_array(target_ob, followers) != -1 )
			{
				followers -= ({ target_ob });
				
				set_temp("follower", followers, me);
				delete_temp("following", target_ob);
				
				msg("$ME������H$YOU�C\n", target_ob, me, 1);
			}
		}

		return tell(me, pnoun(2, me)+"�N "+capitalize(id)+" �[�J�¦W�椤�C\n");
	}

	return tell(me, help);

}
