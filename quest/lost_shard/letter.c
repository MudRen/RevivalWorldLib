/* This	program	is a part of RW	mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File	  : letter.c
 * Author : Clode@RevivalWorld
 * Date	  : 2007-08-18
 * Note	  : 穆青依所撰寫的信
 * Update :
 *  o 2000-00-00  

 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>

inherit	STANDARD_OBJECT;

void create()
{
	set_idname("letter", "穆青依所撰寫的信");

	if( this_object()->set_shadow_ob() ) return;

	set("long", "穆青依寫給一位老道士的信，信封裡面還放著所有收集來的遠古武器碎片。");
	set("unit", "封");
	set("value", 100);
	set("mass", 10);
}
