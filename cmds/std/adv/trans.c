/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : trans.c
 * Author : Msr@RevivalWorld
 * Date   : 2007-08-20
 * Note   : trans ���O
 * Update :
 *  o 2010-03-29 ���g trans ���O, �[�W�i trans �ؿ����ɮ�  
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
        �i�N windows \r\n ���ܦ� unix �� \n ���O�C
HELP;

private void trans( string *files )
{
    string msg = "";

    foreach( string file in files )
    {
        if( !stringp(msg = read_file( file )) )
        {
            printf("�L�k�ഫ %s �ɮ�...\n");
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
    if(!arg) return tell(me, "�A�n�ഫ�����ɮשΥؿ��S\n");

    arg = resolve_path(me, arg);

    switch( file_size(arg) ) {
    case -1 : 
        return tell( me, "�èS�� "+ arg +" �o�ӥؿ����ɮסC\n");
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

    tell( me, "�ഫ�ɮפ� ... �еy��...\n");

    trans( files );

    if(sizeof(files))
        tell( me, HIC "�`�@��" + HIW + sizeof(files) + HIC "���ɮצ��\�ഫ�榡�I\n" NOR);
    else
        tell( me, HIC "�S���ഫ�����ɮסC\n" NOR);
    return;
}
