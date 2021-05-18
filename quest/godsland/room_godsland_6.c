/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : room_godsland_6.c
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
	if( arg == "�۾�" )
		return "�o�ӥ۾��۷����A���γ\�����|�k���L�h(climb)�C\n";
	
	return 0;
}

void create()
{
	set("short", WHT"����}��"NOR);
	set("long",@TEXT
�@�@�o�̬O�}�ު��̲`�B�F�A���e�X�{�@�D���q���۾��A���W�蹺���Ϲ����M�O��
�o�۷���A�Ϥ��yø�ۤj�q���H���ǳƤF�j�q�����~���b�������ѯ��A�u������
�e���ѯ����q�J���A��A�իD�`�_��H�ߡC�۾����̳��������o�ۥե��A�n������
�X�f�b���A�γ\�i�H�յ��k���W�h�C
TEXT);

	set("exits", ([ 
		"south" : "/quest/godsland/room_godsland_5",
	 ]));

	::reset_objects();
}

void do_climb(object me, string arg)
{
	int *quest = query_temp("godsland_enter", me) || allocate(0);

	if( sizeof(quest) != 5 )
		return msg("$ME�յ��k�����D�۾��A���O��b�O�Ӱ��F�A$ME�h�^���U�Į��F�@�n�C\n", me, 0, 1);
	
	delete_temp("godsland_enter", me);
	
	msg("$ME���\�k���W���D�ܸG�F���۾��A�ܧ֪�$ME�w�g�����b�۾��᪺�s�}����...�C\n", me, 0, 1);
		
	me->move(({ 49, 99, 0, "godsland", 0, 0, "49/99/0/godsland/0/0" }));
	msg("$ME��V���ª��q�D�A�Ө�"+AREA_D->query_area_idname("godsland", 0)+"�C\n", me, 0, 1);
	
	CHANNEL_D->channel_broadcast("combat", me->query_idname()+"��V���ª��q�D�A��F"+AREA_D->query_area_idname("godsland", 0)+"�C");
}

mapping actions = 
([
	"climb" : (: do_climb :),
]);

