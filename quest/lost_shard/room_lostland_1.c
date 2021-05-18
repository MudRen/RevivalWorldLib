/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : room_lostland.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-08-18
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

void create()
{
	set("short", HIM"����"NOR MAG"���ӥ�"NOR);
	set("long",@TEXT
    �o�̬O�@�ӷ����j�Ѫ��ӥޡA����N�h���O�N�B�B�B���㪺���z�A�۾��W���G
�۪Z���A��L������A�a�W�]�G���ۦU���Z�������e�A�̲`�B��F�T�ƥ۴áA���}
�o�ǥ۴é��U�@�ݩ~�M���`�������A�u���@�}�G������_�q�`�B����ӨӡA��b��
�H���������A���o�T�ӳo��`���é��}�ޫo�۹�S�H�n�_�C

�@�@�����������������ߡ@�����������������ߡ@������������������
�@�@���@�@�@�@�@�@�@���@���@�@�@�@�@�@�@���@���@�@�@�@�@�@�@��
�@�@���@�@�@���@�@�@���@���@�@�@���@�@�@���@���@�@�@���@�@�@��
�@�@���@�@�@�@�@�@�@���@���@�@�@�@�@�@�@���@���@�@�@�@�@�@�@��
�@�@���@�@�@�j�@�@�@���@���@�@�@���@�@�@���@���@�@�@�j�@�@�@��
�@�@���@�@�@�@�@�@�@���@���@�@�@�@�@�@�@���@���@�@�@�@�@�@�@��
�@�@���@�@�@�M�@�@�@���@���@�@�@���@�@�@���@���@�@�@���@�@�@��
�@�@���@�@�@�@�@�@�@���@���@�@�@�@�@�@�@���@���@�@�@�@�@�@�@��
�@�@���@�@�@��@�@�@���@���@�@�@��@�@�@���@���@�@�@�q�@�@�@��
�@�@���@�@�@�@�@�@�@���@���@�@�@�@�@�@�@���@���@�@�@�@�@�@�@��
�@�@���@�@�@�@�@�@�@���@���@�@�@���@�@�@���@���@�@�@�@�@�@�@��
�@�@���@�@�@�@�@�@�@���@���@�@�@�@�@�@�@���@���@�@�@�@�@�@�@��
�@�@���@�����������@���@���@�����������@���@���@�����������@��
�@�@��������@��������@��������@��������@��������@��������
TEXT);
	set("exits", ([ "out" : ({ 34, 58, 0, "sixcastle", 1, 0, "34/58/0/sixcastle/1/0" }) ]));

	::reset_objects();
}

void do_jump(object me, string arg)
{
	if( !arg )
		return tell(me, pnoun(2, me)+"�Q�n���i���ӥ۴�(left�Bright�Bmiddle)�H\n");

	arg = lower_case(arg);

	if( arg == "left" )
	{
		msg("$ME����m�b���䪺�ç��̸��F�i�h...�C\n", me, 0, 1);
		
		if( !sizeof(filter_array(all_inventory(me), (: base_name($1) == "/quest/lost_shard/lostland_key" :))) )
		{
			msg("$ME�Q�@�D�L�Ϊ��O�q�ϼu�F�X�ӡA���G���@�D��צ�F�q�D...�C\n", me, 0, 1);
			return;
		}
		
		me->move(({ 0, 0, 0, "lostland", 0, 0, "0/0/0/lostland/0/0" }));
		msg("$ME��V���ª��q�D�A�Ө�"+AREA_D->query_area_idname("lostland", 0)+"�C\n", me, 0, 1);
		
		CHANNEL_D->channel_broadcast("combat", me->query_idname()+"��V���ª��q�D�A��F"+AREA_D->query_area_idname("lostland", 0)+"�C");
	}
	else if( arg == "right" )
	{
		msg("$ME����m�b�k�䪺�ç��̸��F�i�h...�C\n", me, 0, 1);
		
		if( !sizeof(filter_array(all_inventory(me), (: base_name($1) == "/quest/lost_shard/lostland_key_2" :))) )
		{
			msg("$ME�Q�@�D�L�Ϊ��O�q�ϼu�F�X�ӡA���G���@�D��צ�F�q�D...�C\n", me, 0, 1);
			return;
		}
		
		me->move(({ 28, 89, 0, "lostland", 1, 0, "28/89/0/lostland/1/0" }));
		msg("$ME��V���ª��q�D�A�Ө�"+AREA_D->query_area_idname("lostland", 1)+"�C\n", me, 0, 1);
		
		CHANNEL_D->channel_broadcast("combat", me->query_idname()+"��V���ª��q�D�A��F"+AREA_D->query_area_idname("lostland", 1)+"�C");
	}
	else if( arg == "middle" )
	{
		msg("$ME����m�b�������ç��̸��F�i�h...�C\n", me, 0, 1);
		
		if( !sizeof(filter_array(all_inventory(me), (: base_name($1) == "/quest/lost_shard/lostland_key_3" :))) )
		{
			msg("$ME�Q�@�D�L�Ϊ��O�q�ϼu�F�X�ӡA���G���@�D��צ�F�q�D...�C\n", me, 0, 1);
			return;
		}
		
		me->move(({ 0, 0, 0, "lostland", 2, 0, "0/0/0/lostland/2/0" }));
		msg("$ME��V���ª��q�D�A�Ө�"+AREA_D->query_area_idname("lostland", 2)+"�C\n", me, 0, 1);
		
		CHANNEL_D->channel_broadcast("combat", me->query_idname()+"��V���ª��q�D�A��F"+AREA_D->query_area_idname("lostland", 2)+"�C");
	}
	else
		tell(me, pnoun(2, me)+"�Q�n���i���ӥ۴�(left�Bright�Bmiddle)�H\n");
}

mapping actions = 
([
	"jump" : (: do_jump :),
]);
