/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : herb_seed.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-07-22
 * Note   : �į�ؤl
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
	set_idname("herb seed","�į�ؤl");
	
	if( this_object()->set_shadow_ob() ) return;
	
	set("long","�i�Ψӧ@���U���ī~�C\n");
	set("unit", "�U");
	set("mass", 10);
	set("value", 100);
	set("badsell", 1);

	// �ؤl��T
	set("growth",
	([
		//����
		"type":		FARM,

		//�W��
		"idname":	this_object()->query_idname(),
		
		//�������خɪ��T��
		"start_msg":"�į󯱭]����a�G������Цa�A�@���Y�Y�V�a���Pı�C\n" ,
			
	       	//�C���ؤl�ί��]�ݭn����Ƽƶq
	       	"material":
	       	([
	       		"/obj/materials/water":10,
	       	]),
			
	       	//���β��~���ƶq�]�w�P���~���|
	       	"harvest":
	       	([
	       		"/obj/farming/product/herb":18,
	       	]),

		//�������ήɪ��T��
		"harvest_msg":"$ME�w�ߦa���γo���������į�СC\n",

		//�شӦ������~�һݭn���ޯ�P�䵥��
		"skill":
		([
			"farming":50,
		]),

		//�����ɪ��T��
		"growing":
		([
			5:	"�į�ФW�p�p�����]�w�g�������j�F�C\n",
			50:	"�u���į�Ф@����N�s�M�A�R���F�ͮ�C\n",
			300:	"�į�ФW���į�w�g���������F�C\n",
			450:	"�u���@����`��⪺�į�A�w�g���ΦA��@�F�A�@�ߵ��@�}�l�K�i�H���ΡC\n",
			550:	"�o���į�ФW���į�w�g���������F�A�ש�i�H���ΤF�C\n",
		]),
	
		// �Ѯ�v�T�ԭz
		"weather":
		([
			SNOWY:	"�j���s�s�Y���ϱo�į���a�\��F�C\n",
			SHOWER:	"�s��Ӫ��B�q�ϱo�@���į�w���G��F�C\n",
			TYPHOON:"�䭷��ŧ�ϱo�����į�Q�j���d�y���_�C\n",
			FOEHN:	"�I�����jŧ���������į�ֳt���\���C\n",
		]),
	]));
}

