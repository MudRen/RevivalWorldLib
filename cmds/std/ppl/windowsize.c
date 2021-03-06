/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : pplcmd_example.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-00-00
 * Note   : 玩家指令範例
 * Update :
 *  o 2003-00-00  
 *
 -----------------------------------------
 */

#include <feature.h>
#include <message.h>
inherit COMMAND;

string help = @HELP
自訂畫面大小

系統會自動偵測使用者的 Client 視窗大小，以做最佳化的訊息格式輸出
若您的 Client 不支援則系統預設的螢幕高與寬分別為 64 行與 80 個字元寬
若您不習慣該設定值，可自行手動修改

範例及用法如下:

> windowsize h64
設定螢幕高度為 64 行

> windowsize w80
設定螢幕寬度為 80 個字元

> windowsize h64 w80
同時設定高度 80 與寬度 64

指令格式:
windowsize			顯示目前設定
windowsize h<行數>		設定螢幕高度 (行)
windowsize w<字元數>		設定螢幕寬度 (字元)
windowsize h<行數> w<字元數>	同時設定高度與寬度

相關指令:
HELP;

private void do_command(object me, string arg)
{
        int h, w;

        h = query_temp("windowsize/height", me);
        w = query_temp("windowsize/width", me);

        if( !arg )
                return tell(me, pnoun(2, me)+"目前設定的畫面大小為：高 "+h+" 行，寬 "+w+" 字元。\n");
        if( arg == "-unset" ) {
                delete_temp("windowsize", me);
                return tell(me, pnoun(2, me)+"取消自訂畫面大小，將由系統自動偵測。\n");
        }

        foreach(arg in explode(arg, " "))
        {
                if( sscanf(arg, "h%d", h) )
                        set_temp("windowsize/height", h, me);

                if( sscanf(arg, "w%d", w) )
                        set_temp("windowsize/width", w, me);
        }

        return tell(me, pnoun(2, me)+"目前設定的畫面大小為：高 "+h+" 行，寬 "+w+" 字元。\n");
}
