/* This	program	is a part of RW	mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File	  : pig_fossil.c
 * Author : Clode@RevivalWorld
 * Date	  : 2009-12-24
 * Note	  : ���ޤƥ�
 * Update :
 *  o 2000-00-00  

 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>

inherit	STANDARD_OBJECT;

void create()
{
	set_idname(HIC"fossil "NOR CYN"pig"NOR, NOR CYN"����"HIC"�ƥ�"NOR);
	set_temp("status", HIC"�}"NOR CYN"��");

	if( this_object()->set_shadow_ob() ) return;

	set("long", "�o���ƥ������o�X�ť��A���G���د������O�q�ʦL�䤤�A�γ\����@�~�P�k���D�o�O����C");
	set("unit", "��");
	set("value", 100);
	set("mass", 100);
	set("flag/no_amount", 1);
}
