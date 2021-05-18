/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : deer_stud.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-09-09
 * Note   : �س�
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>
#include <nature.h>
#include <map.h>

inherit STANDARD_OBJECT;

void create()
{
        set_idname("deer stud","�س�");
        
        if( this_object()->set_shadow_ob() ) return;
        
        set("long", "�Ψӥ�t�Ͳ����دb�C\n");
        set("unit", "��");
        set("mass", 1000);
        set("value", 8000);
        set("badsell", 1);

	// �ؤl��T
	set("growth",
	([
		//����
		"type":		PASTURE,

		//�W��
		"idname":	this_object()->query_idname(),
		
		//�}�l�Ͳ��ɪ��T��
		"start_msg":"�������س��s�|�B���B�M���Ϊ���A�R�����O�P�®�C\n" ,
			
	       	//�C���ݭn����ƺ����P�ƶq
	       	"material":
	       	([
	       		"/obj/materials/water":5,
	       		"/obj/farming/product/herbage":25,
	       	]),
			
	       	//���~���ƶq�]�w�P���~���|
	       	"harvest":
	       	([
	       		"/obj/pasture/product/leather":3,
	       		"/obj/pasture/product/antler":3,
	       	]),

		//�ͦ��ɪ��T��
		"harvest_msg":"$ME�w�ߦa�O�_�è��X�s�A������P�֭��C\n",

		//�شӦ������~�һݭn���ޯ�P�䵥��
		"skill":
		([
			"pasture":80,
		]),

		//�����ɪ��T��
		"growing":
		([
			5:	"�i�J�F�@���e�N����t���A�س������}�l���ۥ�t�C\n",
			50:	"�س��̫�_��L�h���ͬ��A���i�H�`�N�쳡���������欰���Ǥ��P�C\n",
			200:	"�����������{�l�����j�F�_�ӡA���G�O�h���F�C\n",
			500:	"�����̤��Y���U�F�\�h���d���p���A�u�������̲Ӥߪ���¶�b�p�����ǨûQ���e�̪�����C\n",
			800:	"�p���v���������A�U���U���C\n",
			900:	"�p���̥������d�����j���������A�i�H�i�榬���F�C\n",
		]),
	
		// �Ѯ�v�T�ԭz
		"weather":
		([
			SNOWY:	"�j���s�s�Y���ϱo���s�̳��򪺭ᦺ�C\n",
			SHOWER:	"�s��Ӫ��B�q�ϱo���������s�̨��D�f���C\n",
			TYPHOON:"�䭷��ŧ�ϱo���������s�̳Q�j���j�Ӫ����Y�{���C\n",
			FOEHN:	"�I�����jŧ�����������s�̹L�������Ӧ��C\n",
		]),
	]));
}
