/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : s_afterroom.c
 * Author : tzj@RevivalWorld
 * Date   : 2006-09-09
 * Note   : Ĭ��Ȯa���m����
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
	set("short", CYN"�m����"NOR);
	set("long",@TEXT
�@�@�ⰼ�s��F�X�󲱤�����δX�U���}�ơA�]���m�F�A�ѩһݭn�Ψ쪺�U�ؤu
��C�ѩ�g�L��z�A�ҥH���M���B�F������c�h�A���ݰ_�ӫo�@�I�]��ı�o����
�C���b���B���ɥi�D�쭹��������A�q�Τl�`�B�ƨӡA�ӫΥ~�]�g�`�Ǩ����s��
������s�n�C
TEXT);


	set("exits", ([
		//���F���O��
		"east"    : ({ 82, 66, 0, "lightforest", 0, 0, "82/66/0/lightforest/0/0" }),
		
		//���n���O��
		//"south" : ,
		
		//���訫
		"west"    : "/quest/suphia_and_benjamin/room/s_kitchen",
		
		//���_���O��
		"north" : ({ 81, 65, 0, "lightforest", 0, 0, "81/65/0/lightforest/0/0" }),
			
		//���B���q��2F���ӱ�
		//"up"    : ,
		
	]));

/*	�w�d��NPC�Ϊ��~��...
	set("objects", ([
		"/quest/suphia_and_benjamin/npc_suphia" : 1 ,
	]));
*/
	
	::reset_objects();
	replace_program(STANDARD_ROOM);
}
