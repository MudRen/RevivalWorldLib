/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : cotton_seed.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-09-09
 * Note   : �֪�ؤl
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
	set_idname("cotton seed","�֪�ؤl");
	
	if( this_object()->set_shadow_ob() ) return;
	
	set("long","�i�ΨӥͲ��֪�C\n");
	set("unit", "�U");
	set("mass", 10);
	set("value", 180);
	set("badsell", 1);

	// �ؤl��T
	set("growth",
	([
		//����
		"type":		FARM,

		//�W��
		"idname":	this_object()->query_idname(),
		
		//�������خɪ��T��
		"start_msg":"�֪�ؤl����a�G������Цa�A�@���Y�Y�V�a���Pı�C\n" ,
			
	       	//�C���ؤl�ί��]�ݭn����Ƽƶq
	       	"material":
	       	([
	       		"/obj/materials/water":14,
	       	]),
			
	       	//���β��~���ƶq�]�w�P���~���|
	       	"harvest":
	       	([
	       		"/obj/farming/product/cotton":12,
	       	]),

		//�������ήɪ��T��
		"harvest_msg":"$ME�w�ߦa���γo���������֪�СC\n",

		//�شӦ������~�һݭn���ޯ�P�䵥��
		"skill":
		([
			"farming":90,
		]),

		//�����ɪ��T��
		"growing":
		([
			5:	"�֪�ФW�p�p�����]�w�g�������j�F�C\n",
			50:	"�u���֪�Ф@����N�s�M�A�R���F�ͮ�C\n",
			500:	"�֪�ФW���֪�w�g���������F�C\n",
			700:	"�u���@��������⪺�֪�A�w�g���ΦA��@�F�A�@�ߵ��@�}�l�K�i�H���ΡC\n",
			800:	"�o���֪�ФW���֪�w�g���������F�A�ש�i�H���ΤF�C\n",
		]),
	
		// �Ѯ�v�T�ԭz
		"weather":
		([
			SNOWY:	"�j���s�s�Y���ϱo�֪ᳰ��a�\��F�C\n",
			SHOWER:	"�s��Ӫ��B�q�ϱo�@�Ǵ֪�w���G��F�C\n",
			TYPHOON:"�䭷��ŧ�ϱo�����֪�Q�j���d�y���_�C\n",
			FOEHN:	"�I�����jŧ���������֪�ֳt���\���C\n",
		]),
	]));
}

