/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : suicide.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-01-31
 * Note   : 
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <daemon.h>
#include <feature.h>
#include <message.h>

inherit COMMAND;

string help = @HELP
�۱����O

�o�ӫ��O�i�H���t�ΧR���Ӫ��a���⪺���
�Y�z�w�g���¤F���ͪ��@�ɡA�z�i�H�ϥγo�ӫ��O�R������

�������O:
HELP;

private void redo_passwd(object me);

private void delay_suicide(object me)
{
	if( !objectp(me) ) return;

	set("password", PASSWORD_D->query_password(me->query_id(1)), me);
	
	me->save();

	if( CHAR_D->destruct_char(me->query_id(1)) )
	{
		CHANNEL_D->channel_broadcast("news", me->query_idname()+"�}�U����� C4 �˼Ƨ����u�F�v�A�u���@�ζ¬��ĤW���]�P�@����A�Х@�ܤ��w�� "+SERIAL_NUMBER_D->query_serial_number("suicide")+" �Ӧ۱��רҡC");
		
		catch( filter_array(all_inventory(me), (: !query_temp("keep", $1) :))->move(query_temp("location", me) || environment(me)) );

		destruct(me);
	}
	else
		CHANNEL_D->channel_broadcast("news", me->query_idname()+"�}�U����� C4 �˼Ƨ����u...�v�A�S�z���A�ݨӱo��Ův��s��s�C");
}

private void confirm_suicide(object me, string arg)
{
	/* �קK�Q�~�� */
	if( !is_command() ) return;

	if( lower_case(arg)[0] == 'y' )
	{
		CHANNEL_D->channel_broadcast("news", me->query_idname()+"�ש�Q�q�F�A�q�f�U�����_��� C4 �\�b�}�U�A�]�w�˼Ʈɶ��� 20 �����A�˼Ƨ�����N�R���Ҧ��b����ơC");
		
		tell(me, "�Y���ܥD�N�A�i�H�� Quit ���}�C�����s�n�J�A�K�����۱��˼ơC\n");
		
		call_out((: delay_suicide :), 1200, me);
	}
	else tell(me, "�b���R�������C\n");
}

private void check_passwd(object me, string arg)
{
	string old_passwd = PASSWORD_D->query_password(me->query_id(1));
	
	if( !old_passwd ) return tell(me, "�±K�X���j�M���G�o�Ϳ��~�A�гq���Ův�B�z�C\n");
	
	if( !arg || crypt(arg, old_passwd) != old_passwd )
	{
		tell(me, "�K�X��J���~�C\n");
		return redo_passwd(me);
	}
	
	tell(me, HIR"ĵ�i�G�۱���t�αN�R���Ҧ�"+me->query_idname()+"�����"NOR"\n�O�_�u���n�۱�(Yes/No)�H");
	input_to( (: confirm_suicide, me :), 3 );
}

private void confirm_redo_passwd(object me, string arg)
{
	if( !arg ) return redo_passwd(me);
	arg = lower_case(arg);
	
	if( arg[0] == 'y' )
	{
		tell(me, "�п�J"+pnoun(2, me)+"���K�X�G");
		input_to( (: check_passwd, me :), 3 );
	}
	else tell(me, "�b���R�������C\n");
}
	
private void redo_passwd(object me)
{
	tell(me, "�b���R�����ѡA�O�_���s��J(Yes/No)�H");
	input_to( (: confirm_redo_passwd, me :) );
}

private void do_command(object me, string arg)
{
	if( call_stack(1)[<1] != me || !is_command() ) return;

	if( wizardp(me) )
		return tell(me, "�Ův���o�۱��A�Ův�۱��e�������������a�C\n");

	tell(me, "�п�J"+pnoun(2, me)+"���K�X�G");
	input_to( (: check_passwd, me :), 3 );
}