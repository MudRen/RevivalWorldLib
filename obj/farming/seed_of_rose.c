/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : seed_of_rose.c
 * Author : Tzj@RevivalWorld
 * Date   : 2003-5-22
 * Note   : 玫瑰種子
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>

inherit STANDARD_OBJECT;

void create()
{
        set_idname("seed of rose",HIR"玫瑰"NOR+RED"種子"NOR);
        
        if( this_object()->set_shadow_ob() ) return;
        
        set("unit", "粒");
        set("mass", 2);
        set("value", 100);
        set("long","需要小心呵護才能長成[0m[31;1m玫瑰[0m花朵。\n");
        set("badsell", 1);
 
        // 種子資訊
        set("seed_data",
                ([
                        //插秧播種, 灌溉, 收割所耗體力設定
                        "str_cost": ([
                                "sow"           :3, //插秧或播種時需要的體力
                                "irrigate"      :7, //灌溉每升水需要的體力 
                                "reap"          :10 //收割時需要的體力
                                ]),
                        
                        //種子種類
                        "seed":"玫瑰種子",
                        
                        //設定插秧或是播種
                        "sow_type":"播種",
                        
                        //收割產品的路徑與數量設定
                        "reap":({ 5, "/obj/farming/rose.c"}),
                        
                        //各個步驟的農作訊息
                        "msg":
                        ([
                                // 插秧播種時的訊息
                                "sow":
                                        "玫瑰種子散播在田裡，還看不出幼苖。\n" ,
                                
                                // 灌溉時的訊息
                                "irrigate":
                                ({
                                        2,"玫瑰種子萌出一根根嫩芽漸漸生長。\n",
                                        3,"玫瑰幼苖慢慢生長出有刺藤蔓，上面的綠葉一一舒展開來。\n",
                                        5,"玫瑰田冒出了許多玫瑰花苞，再過不久就會開花了。\n",
                                        7,"只見一朵又一朵綻開的鮮紅色玫瑰在綠葉的映襯之下明艷動人，再等一下子就可以採收了。\n",
                                }),
                                
                                // 成熟收割時的訊息
                                "reap":
                                ({
                                        5,
                                        "玫瑰田上的玫瑰花已全部綻放，已經可以採收了。\n",
                                        "這片玫瑰田在細心呵護之下，綻放了許多玫瑰。\n共採收了$NUM朵的玫瑰。\n"
                                }),
                                
                                // 荒廢時的訊息
                                "neglect":
                                ({      
                                        50,
                                        "這片玫瑰田因為長久沒有澆水，綠葉一片片枯黃凋零。\n",
                                        "這片玫瑰田因久未善加呵護，已經全部荒廢了。\n",
                                }),
                                
                                "weather":
                                ([
                                        "RAINY":"大雨使得玫瑰田的根泡水腐爛了。\n",
                                        "SNOWY":"大雪使玫瑰田遭受到嚴重的冰害。\n",
                                        "WINDY":"狂風使的一部分的玫瑰幼苖被攔腰吹斷。\n",
                                ]),
                        ]),
                ])
        );
}


