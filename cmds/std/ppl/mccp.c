/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : pplcmd_example.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-00-00
 * Note   : ���a���O�d��
 * Update :
 *  o 2005-00-00  
 *
 -----------------------------------------
 */

#include <feature.h>
#include <ansi.h>

inherit COMMAND;

string help = @HELP
���O����:
    �ˬd���a���{���O�_���Ұ� MCCP(Mud Client Compression Protocol)�\��
���\��i�H�[�t�����ǿ�t��
���O²��:

���O�榡:

�������:
   
�������O:

HELP;

#define MCCP_SUPPORT	1

private void do_command(object me, string arg)
{
#ifdef MCCP_SUPPORT

	if( wizardp(me) && arg == "-l")
	{
		string m="\n";
		int j=1,i=0;
		foreach(object ob in users()) 
			m+=sprintf("%3d.%-30s%3s\n", j++, ob->query_idname(), compressedp(ob)?(i++,HIG"ON"NOR):HIR"OFF"NOR); 
		tell(me,m+"\n\nMCCP COUNT:"+HIG+ i +NOR+"\n");
	} else{
		tell(me,"�A�� MCCP ���A���G" + (compressedp(me)?(HIG"ON"NOR):HIR"OFF"NOR) +"\n");
	}
	tell(me,"�{�b�@ " +HIC+ sizeof(filter(users(),(: compressedp($1) :))) + NOR+"/"+ sizeof(users()) +" �ӨϥΪ̶}�� MCCP�C\n");
#else
	tell(me, "�ثe Driver ���䴩���\��C\n");

#endif
}	
