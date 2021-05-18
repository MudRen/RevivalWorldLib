/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : xpasswd.c
 * Author : Msr@RevivalWorld
 * Date   : 2010-03-26
 * Note   : �ק缾�a���X
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <daemon.h>
#include <feature.h>

inherit COMMAND;

string help = @HELP
���O�榡 : xpasswd ID �s�K�X

�o�ӫ��O�i�H�ק缾�a���K�X�A���ԷV�ϥΡC
HELP;

private void command(object me, string arg)
{
    object ob;
    string id,password;

    if( !arg || sscanf(arg,"%s %s",id, password)!=2 )
        return tell( me, "�Ϊk: xpasswd ID \n");

    if( find_player(id) )
        return tell( me, "�ثe "+ id + " ���b�u�W, �L�k�ק� ... \n");

    if( file_size(user_data(id)) <= 0 )
        return tell( me, "�S���o�Ӫ��a�C\n");
    else
    {
        ob = load_user(id);

        PASSWORD_D->set_password( id, crypt(password,0) );

        tell(me, "���a" + ob->query_idname() + "���K�X�w�g��אּ�G"+password+"�C\n");

        log_file("command/xpasswd", sprintf( "%s �ק� %s ���K�X\n", me->query_idname(), ob->query_idname()));

        destruct(ob);
        return ;
    }
    return ;
}
