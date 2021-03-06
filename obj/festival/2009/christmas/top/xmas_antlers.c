/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : xmas_antlers.c
 * Author : Msr@RevivalWorld
 * Date   : 2009-12-03
 * Note   : 黃金鹿茸
 * Update :
 *  o 2000-00-00
 *    2009-12-21 加入 bmw 想的一些招式
 *    2009-12-24 修正 bug / 調整部份數值 by Clode
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>
#include <feature.h>
#include <equipment.h>
#include <buff.h>
#include <daemon.h>

inherit STANDARD_OBJECT;
inherit EQUIPMENT;

void create()
{
    set_idname("gold antlers", HIY"黃金"NOR YEL"鹿"NOR HIY"茸"NOR);
    set_temp("status", HIY"2009"NOR HIC" 勇"NOR CYN"士" NOR HIC"版");

    if( this_object()->set_shadow_ob() ) return;

    set("long", "一對在重生世界中所向披靡的超級武器");
    set("unit", "對");
    set("mass", 0);
    set("value", 10);
    set("flag/no_amount", 1);
    set("skill", "stick");

    set(BUFF_STR, 1);
    set(ATTACK, 30);
    set(BUFF_SLIP_CHANCE, 10);

    ::setup_equipment(EQ_TWOHAND, HIY"頭"NOR YEL"頂"NOR);
}

string *attack =
({
  "$ME背後出現「[1;35m麥[0;35m可[1;35m傑[0;35m克[1;35m滑[m」的身影，頂著%s，秀起了招牌「[37m空中滑步[m」，繞著$YOU不停地滑來滑去！\n",
  "$ME搖茸晃腦的甩動%s，大喊著：「萬能的鹿神，請賜予我神奇的力量！」瞬間[0;36m失[m[1;36m控暴[m[0;36m衝[m地向$YOU滑了過去！\n",
  "$ME頭頂著%s，發揮「[1;31m惡[m[0;31m鹿鬥[m[1;31m勇者[0m」的精神，往$YOU [1;37m�繺�[0m 了過去！\n",
  "$ME緊握%s，叫了一聲：好屌！隨即變身成「[1;30m摑臉[m魔[1;30m鹿[m」，對$YOU使出「[1;32m摑[m[0;32m臉攻[m[1;32m擊[m」！\n"
});

//
// 武器自動特殊攻擊
//
int special_attack(object attacker, object defender)
{
    if( !random(10) )
    {
        int damage = range_random(50, 100);

        msg( sprintf(attack[random(sizeof(attack))], this_object()->query_idname()), attacker, defender, 1);

        return damage;
    }
    return 0;
}
