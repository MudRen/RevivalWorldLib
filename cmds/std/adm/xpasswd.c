/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : xpasswd.c
 * Author : Msr@RevivalWorld
 * Date   : 2010-03-26
 * Note   : 修改玩家秘碼
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <daemon.h>
#include <feature.h>

inherit COMMAND;

string help = @HELP
指令格式 : xpasswd ID 新密碼

這個指令可以修改玩家的密碼，請謹慎使用。
HELP;

private void command(object me, string arg)
{
    object ob;
    string id,password;

    if( !arg || sscanf(arg,"%s %s",id, password)!=2 )
        return tell( me, "用法: xpasswd ID \n");

    if( find_player(id) )
        return tell( me, "目前 "+ id + " 正在線上, 無法修改 ... \n");

    if( file_size(user_data(id)) <= 0 )
        return tell( me, "沒有這個玩家。\n");
    else
    {
        ob = load_user(id);

        PASSWORD_D->set_password( id, crypt(password,0) );

        tell(me, "玩家" + ob->query_idname() + "的密碼已經更改為："+password+"。\n");

        log_file("command/xpasswd", sprintf( "%s 修改 %s 的密碼\n", me->query_idname(), ob->query_idname()));

        destruct(ob);
        return ;
    }
    return ;
}
