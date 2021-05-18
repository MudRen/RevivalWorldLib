/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : virtual_d.c
 * Author : Cookys@RevivalWorld
 * Date   : 2000-12-18
 * Note   : ����������F
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <object.h>
#include <daemon.h>
#include <areadata.h>
#include <citydata.h>
#include <inherit.h>

void create()
{
	if( clonep() ) 
		destruct(this_object());
}

nomask varargs object compile_object(string file, int cloning)
{
	string filename, option;
	object ob;

	if( !file || !file[0] || file[<2..<1] == __SAVE_EXTENSION__ ) return 0;

	// �����έ��Ϭ�������
	if( sscanf(file, CITY_ROOT"%*s/%*d/%s/%s", option, filename) == 4 || sscanf(file, AREA_ROOT"%*s/%*d/%s/%s", option, filename) == 4 )
	{
		switch(option)	
		{
			// �����ж���������
			case "room":
			{
				string type;
				
				// �ж������\���ͽƻs����
				if( cloning || (sscanf(filename, "%*d_%*d_%*d_%s", type) != 4 && sscanf(filename, "%*d_%*d_%s", type) != 3) ) 
					return 0;

				if( catch(ob = new(STANDARD_MODULE_ROOM, file, type)) )
				{
					destruct(ob);
					return 0;
				}
				else
					return ob;
			}
			default:
				return 0;
		}
	}

	// �ӫ~
	if( sscanf(file, "/product/%s/%*d", option) == 2 )
	{
		if( !file_exists(file) && previous_object(1) != find_object("/std/module/room/research") )
			return 0;

		if( catch(ob = new("/std/module/product/"+option+"_object.c", file, option)) )
		{
			destruct(ob);
			return 0;
		}
		else
			return ob;
	}

	// �����~(souvenir)
	if( sscanf(file, "/product/souvenir_%s", option) == 1 )
	{
		if( catch(ob = new("/obj/systemproducts/souvenir.c", file, option)) )
		{
			destruct(ob);
			return 0;
		}
		else
			return ob;
	}
				
	// �Ҥu
	if( sscanf(file, "/npc/%*d") )
	{
		if( cloning ) return 0;

		if( catch(ob = new(STANDARD_MODULE_NPC, file)) )
		{
			destruct(ob);
			return 0;
		}
		else
			return ob;
	}

	if( sscanf(file, "/obj/book/product/%s", option) == 1 )
		return new("/obj/book/virtual_product_book", option);
		
	// �ϥΪ̪���
	if( !cloning && sscanf(file, "/data/user/%s/%s", option, filename) == 2 )
	{
		// �ĥθ��Y�Ԫ��ˬd
		if( !is_english(filename) || filename[0..0] != option ) return 0;
		
		if( SECURE_D->is_wizard(filename) && (call_stack(1)[<1]==load_object(WIZ_LOGIN_D) || (call_stack(1)[<2]==load_object(LOGIN_D))) )
			return new(WIZ_OB);
		
		return new(PPL_OB);
	}
	
	if( sscanf(file, "/obj/adventure/map_%s", option) == 1 )
		return new("/obj/adventure/map", option);

	if( sscanf(file, "/obj/baseball/champion_ring_%s", option) == 1 )
		return new("/obj/baseball/champion_ring", option);
		
	return 0;
}

function query_compile_object_fp()
{
	return (: compile_object($1, $2) :);
}

string query_name()
{
	return "��������t��(VIRTUAL_D)";
}
