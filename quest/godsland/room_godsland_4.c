/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : room_godsland_4.c
 * Author : Clode@RevivalWorld
 * Date   : 2009-12-30
 * Note   : �зǩж�
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>
#include <quest.h>
#include <daemon.h>

inherit STANDARD_ROOM;

string help = @HELP
�b�o�ӥ��Z�L�_���ж����èS������S����T�C
HELP;

string query_look_message(string arg)
{
	if( arg == "�۬W" )
		return "�o�ӥ۬W�ܹ��O�H�y���A���G�O�@�Ӷ}���A�γ\�ΤO�@�I�i�H����(push)���C\n";
	
	return 0;
}

void create()
{
	set("short", WHT"����}��"NOR);
	set("long",@TEXT
�@�@�o�Ӽ��㪺�}�ަ��G�ܤ[�S���H�ӹL�F�A�����i���۾��W�観�\�h�Ϥ�A���]
�~�N�[���w�g�L�k���ѡA�u���a���@�Ӭ�X���諬�۬W�A���G�O�Y�ض}�������C��
�`�B�����G�٦��@�Ǥe���A�����j�������w�g�Q���۩ҫʳ��C
TEXT);

	set("exits", ([ 
		"south" : "/quest/godsland/room_godsland_3",
		"west" : "/quest/godsland/room_godsland_5",
	 ]));

	::reset_objects();
}

void do_push(object me, string arg)
{
	int *quest = query_temp("godsland_enter", me) || allocate(0);
	
	if( !sizeof(quest) || quest[<1] != 1 )
	{
		msg("$ME�ĤO���ʥ۬W�A���۬W�o�S�ܧ֪��h�^���A���G������a��˿��F�C\n", me, 0, 1);
		delete_temp("godsland_enter", me);
	}
	else
	{
		msg("$ME�ĤO���ʥ۬W�A�a�O�Ǧ^�F�@�Ǿ_�ʡC\n", me, 0, 1);
		set_temp("godsland_enter", quest + ({ 4 }), me);
	}
}

mapping actions = 
([
	"push" : (: do_push :),
]);
