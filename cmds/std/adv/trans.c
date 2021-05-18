/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : trans.c
 * Author : Msr@RevivalWorld
 * Date   : 2007-08-20
 * Note   : trans 指令
 * Update :
 *  o 2010-03-29 重寫 trans 指令, 加上可 trans 目錄或檔案  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <daemon.h>
#include <feature.h>
#include <message.h>

#define MS sprintf("%c%c",13,10 )
#define CC sprintf("%c",10 )

inherit COMMAND;

string help = @HELP
        可將 windows \r\n 改變成 unix 的 \n 指令。
HELP;

private void trans( string *files )
{
    string msg = "";

    foreach( string file in files )
    {
        if( !stringp(msg = read_file( file )) )
        {
            printf("無法轉換 %s 檔案...\n");
            continue;
        }

        msg = replace_string( msg, MS, CC );
        write_file( file, msg, 1);      
    }
}

private void command(object me, string arg)
{
    string *files;

    if(!arg) arg = query("current_work/file", me);
    if(!arg) return tell(me, "你要轉換什麼檔案或目錄﹖\n");

    arg = resolve_path(me, arg);

    switch( file_size(arg) ) {
    case -1 : 
        return tell( me, "並沒有 "+ arg +" 這個目錄或檔案。\n");
        break;
    case -2 : 
        files = allocate(0);

        if( arg[<1] != '/') arg += "/";

        foreach( string file in get_dir(arg) )
            if( file_size(arg + file) > 0 )
                files += ({ arg+file });
        break;
    default:
        files = ({ arg });
        break;
    }    

    tell( me, "轉換檔案中 ... 請稍後...\n");

    trans( files );

    if(sizeof(files))
        tell( me, HIC "總共有" + HIW + sizeof(files) + HIC "個檔案成功轉換格式！\n" NOR);
    else
        tell( me, HIC "沒有轉換任何檔案。\n" NOR);
    return;
}
