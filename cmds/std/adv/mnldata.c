/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : mnldata.c
 * Author : Msr@RevivalWorld
 * Date   : 2004-12-30
 * Note   : 修改 Mnlmudlist 列表指令。
 * Update :
 *  o 2005-08-12 Blueshadow 修改操作介面   
 *  o 2006-08-01 Msr 修改操作介面
 *  o 2019-03-03 Msr 增加操作方式 
 *  o 
 -----------------------------------------
 */

#define ADDMUD              1
#define CHINESE_NAME		2
#define ENGLISH_NAME		3
#define USER_COUNT          4

#define SKIP            "~q"
#define SKIP2           ""
#define END             "."

#include <ansi.h>
#include <feature.h>
#include <daemon.h>
#include <message.h>

inherit COMMAND;

private nosave string mud;

string help = @HELP
    新增移除 Mnlmudlist 列表中 Mud 的各項資料。

目前的參數有

-a      新增 Mud 資訊到 mnlmudlist
-r      移除 mnlmudlist 上的 Mud 資訊
-q      查詢某 Mud 的資料
-u      修改查詢某 Mud 人數的字串
ex: mnldata -u rw.muds.net 4000 to 目前共有 %d 位巫師 %d 位玩家在線上﹐以及 %d 位使用者嘗試連線中。

-c      更改 Mud ip port 連線資訊
ex: mnldata -c rw.muds.net 4000 to rw.muds.net 5000

HELP;

private nomask varargs void input_to_mud( int type, mapping mnldata, string arg);

int check_mnlmudlist( string arg )
{
    mapping mnl;

    if(!arg) return 0;

    mnl = MNLMUDLIST_D->query_mnlmudlist();

    if( !mapp(mnl[arg]) )
        return 0;

    return 1;
}

mapping query_mnlmudlist( string arg )
{
    mapping mnl;

    if(!arg) return 0;

    mnl = MNLMUDLIST_D->query_mnlmudlist();

    return mnl[arg];
}

private void do_command(object me, string arg)
{
    string ip, port, oldip, oldport;

    if( !is_command() ) return;

    if( !arg )
        return tell(me, "請輸入要修改的項目( help mnldata )。\n");

    // 移除 mnlmudlist 的 Mud 資訊
    if( sscanf(arg, "-r %s %s", ip, port)==2 ) 
    {
        string mud = ip + " " + port;

        if(check_mnlmudlist(mud))
        {
            MNLMUDLIST_D->remove_mud(mud);
            return tell(me, "移除 Mud "+mud+"。\n"); 

        } else return tell(me, sprintf("沒有 %s 這個 Mud 的資料。\n", mud ) );
    }

    // 新增 mnlmudlist 的 Mud 資訊
    if( arg == "-a" )
    {
        tell(me, "請輸入要新增 Mud，以下用 . 表示結束，用 ~q 跳過此項。\n"); 
        tell(me, "請輸入要新增的 Mud。(ex: mud.revivalworld.org 4000)\n");
        input_to( (: input_to_mud, ADDMUD, me :));
        return ;
    }

    // 修改 mnlmudlist 的 ip 資訊
    if( sscanf( arg, "-c %s %s to %s %s", oldip, oldport, ip, port ) == 4)
    {
        string mud, oldmud;

        oldmud = oldip + " " + oldport;

        if(check_mnlmudlist(oldmud))
        {
            mud = ip + " " + port;

            MNLMUDLIST_D->change_ipport(oldmud, mud);
            return tell(me, sprintf("Mud %s 已改成 %s。\n", oldmud, mud)); 
        } else return tell(me, sprintf("沒有 %s 這個 Mud 的資料。\n", mud ) );
    }

    if( sscanf( arg, "-q %s %s", ip, port ) == 2 )
    {
        string mud = ip + " " + port;

        if( check_mnlmudlist(mud) )
            return tell( me, sprintf("Mud 資訊如下\n%O\n", query_mnlmudlist(mud)));
        else
            return tell(me, sprintf("沒有 %s 這個 Mud 的資料。\n", mud ) );
    }

    if( sscanf( arg, "-u %s %s to %s", ip, port, oldip ) == 3 )
    {
        string mud = ip + " " + port;

        if( check_mnlmudlist(mud) )
        {
            MNLMUDLIST_D->add_mud_info(mud, "USERS_COUNT_PARSE", oldip);
            return tell( me, sprintf("Mud %s 修改人數查詢語法更改為 %s\n", mud, oldip));
        }
        else
            return tell(me, sprintf("沒有 %s 這個 Mud 的資料。\n", mud ) );
    }     

    return tell( me, help );
}

private nomask varargs void input_to_mud( int type, object me, string arg)
{
    string ip, port;

    switch(type) 
    {
    case ADDMUD :
        {
            if( arg == SKIP || arg == SKIP2 )
            {
                tell(me, "請輸入 MUD_CHINESE_NAME。(ex: 重生的小鹿)\n");
                input_to( (: input_to_mud, ADDMUD, me :));
                return;
            }
            if( arg == END )
                return me->finish_input();

            if( sscanf(arg, "%s %s", ip, port)==2 ) 
            {
                mud = arg;
                MNLMUDLIST_D->add_mud(mud);
                tell(me, "請輸入 MUD_CHINESE_NAME。(ex: 重生的世界)\n");
                input_to( (: input_to_mud, CHINESE_NAME, me :) );
            } 
            else
            {
                tell(me, "請輸入要新增的 Mud。(ex: mud.revivalworld.org 4000)\n"); 
                input_to( (: input_to_mud, ADDMUD, me :));
            }
            break;
        }

    case CHINESE_NAME :
        {        
            if( !arg || arg == SKIP || arg == SKIP2 )
            {
                tell(me, "請輸入 MUD_ENGLISH_NAME。(ex: revival world)\n");
                input_to( (: input_to_mud, ENGLISH_NAME, me :) );
                return;
            }

            if( arg == END )
                return me->finish_input();

            if( is_chinese(arg) ) 
            {
                MNLMUDLIST_D->add_mud_info( mud, "MUD_CHINESE_NAME", arg );
                tell(me, "請輸入 MUD_ENGLISH_NAME。(ex: revival world)\n");
                input_to( (: input_to_mud, ENGLISH_NAME, me :) );
            } 
            else
            {
                tell(me, "請輸入 MUD_CHINESE_NAME。(ex: 重生的世界)\n"); 
                input_to( (: input_to_mud, CHINESE_NAME, me :));
            }
            break;
        }

    case ENGLISH_NAME :
        {
            if( arg == SKIP || arg == SKIP2)
            {
                tell(me, "請輸入人數判定語法(ex: 目前共有 %d 位巫師、%d 位玩家在線上)。\n");
                input_to( (: input_to_mud, USER_COUNT, me :) );
                return;

            }

            if( arg == END )
                return me->finish_input();

            if( is_english(arg) )
            {			
                MNLMUDLIST_D->add_mud_info( mud, "MUD_ENGLISH_NAME", arg );
                tell(me, "請輸入人數判定語法(ex: 目前共有 %d 位巫師、%d 位玩家在線上)。\n");

                input_to( (: input_to_mud, USER_COUNT, me :) );
            }
            else
            {
                tell(me, "請輸入 MUD_ENGLISH_NAME。(ex: revival world)\n"); 
                input_to( (: input_to_mud, ENGLISH_NAME, me :));
            }
            break;
        }

    case USER_COUNT :
        {
            if( arg == SKIP || arg == SKIP2 || arg == END ) 
                return me->finish_input();

            if(arg)
            {
                MNLMUDLIST_D->add_mud_info(mud, "USERS_COUNT_PARSE", arg );
                tell(me, "新增 Mud 資訊完成。\n");

                return me->finish_input();
            }
            else
            {
                tell(me, "請輸入人數判定語法(ex: 目前共有 %d 位巫師、%d 位玩家在線上)。\n");
                input_to( (: input_to_mud, USER_COUNT, me :));
            }
            break;
        }
    default: 
        return me->finish_input();
    }
}
