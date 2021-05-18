/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : s_alvin_room.c
 * Author : tzj@RevivalWorld
 * Date   : 2006-09-10
 * Note   : Ĭ��Ȫ��̧̡Ъ����媺�׫�
 * Update :
 *  
 * 2007-08-17  ��z��Ƨ���m
 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>

inherit STANDARD_ROOM;

string help = @HELP
�b�o�ӥ��Z�L�_���ж����èS������S����T�C
HELP;

void create()
{
	set("short", HIC"�׫�"NOR);
	set("long",@TEXT
�@�@���}�b���������i���B�A�@�Ǹm��۱i�ൣ�ɡC�o�i�ɬݰ_�Ӧ����s�E�L�A
���]���W���H�N���\��n��B�p�T���ҫ��Χ�Ȥ����������A�]����o���ǭ��
�C�b�a������W�����Ǭ����]�l�A�@��ʻs�u�}�C�æ��i�x�֨k�ĻP�p�ժ����X
�ӳQ��m�b���ۮؤ��A���b��W�C
TEXT);


	set("exits", ([
		//���F���O��
		//"east"    : ,
		
		//���n���O��
		//"south" : ,
		
		//����^����U
		"west"    : "/quest/suphia_and_benjamin/room/s_frontroom",
		
		//���_���O��
		//"north" : ,
		
	]));

/*	�w�d��NPC�Ϊ��~��...
	set("objects", ([
		"/quest/suphia_and_benjamin/npc_suphia" : 1 ,
	]));
*/
	
	::reset_objects();
	replace_program(STANDARD_ROOM);
}
