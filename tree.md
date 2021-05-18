```
.
├── area
├── city
│   └── fallencity1
│       ├── 0
│       │   ├── data
│       │   ├── estate
│       │   ├── map
│       │   └── room
│       └── info
├── cmds
│   └── std
│       ├── adm
│       │   ├── eval_special.c
│       │   ├── imudtool.c
│       │   ├── ipuse.c
│       │   ├── save_all.c
│       │   ├── send_gift.c
│       │   ├── update_alluser.c
│       │   ├── wscheck.c
│       │   └── xpasswd.c
│       ├── adv
│       │   ├── allowmulti.c
│       │   ├── announce.c
│       │   ├── buildwizhall.c
│       │   ├── call.c
│       │   ├── channel_status.c
│       │   ├── check_memory.c
│       │   ├── clone.c
│       │   ├── config.c
│       │   ├── cp.c
│       │   ├── dellink.c
│       │   ├── dest.c
│       │   ├── dump.c
│       │   ├── edemote.c
│       │   ├── edit.c
│       │   ├── edit_help.c
│       │   ├── editnews.c
│       │   ├── eval.c
│       │   ├── force.c
│       │   ├── ftp_users.c
│       │   ├── fullheal.c
│       │   ├── gag_user.c
│       │   ├── givememoney.c
│       │   ├── header.c
│       │   ├── hyperlink.c
│       │   ├── infodata.c
│       │   ├── malloc_status.c
│       │   ├── mkdir.c
│       │   ├── mkroom.c
│       │   ├── mnldata.c
│       │   ├── mud_status.c
│       │   ├── mudstatus.c
│       │   ├── mv.c
│       │   ├── parsing.c
│       │   ├── promote.c
│       │   ├── purge.c
│       │   ├── recover.c
│       │   ├── reset_cmd.c
│       │   ├── reset_coor.c
│       │   ├── reset_include.c
│       │   ├── rm.c
│       │   ├── rmdir.c
│       │   ├── rmtree.c
│       │   ├── search.c
│       │   ├── setip.c
│       │   ├── setroom.c
│       │   ├── shell.c
│       │   ├── shutdown.c
│       │   ├── snoop.c
│       │   ├── summon.c
│       │   ├── terrain.c
│       │   ├── trans.c
│       │   ├── update.c
│       │   ├── warning.c
│       │   ├── whereis.c
│       │   ├── whois.c
│       │   ├── wizbuild.c
│       │   └── wizlock.c
│       ├── guest
│       │   ├── callouts.c
│       │   ├── callouts2.c
│       │   ├── cat.c
│       │   ├── cd.c
│       │   ├── children.c
│       │   ├── cmdusage.c
│       │   ├── config.c
│       │   ├── cost.c
│       │   ├── data.c
│       │   ├── exchange.c
│       │   ├── ff.c
│       │   ├── fgrep.c
│       │   ├── g2b.c
│       │   ├── goto.c
│       │   ├── gotomap.c
│       │   ├── grep.c
│       │   ├── home.c
│       │   ├── ident.c
│       │   ├── inherits.c
│       │   ├── lf.c
│       │   ├── ls.c
│       │   ├── map.c
│       │   ├── mem.c
│       │   ├── more.c
│       │   ├── nature.c
│       │   ├── nslookup.c
│       │   ├── qload.c
│       │   ├── shopping.c
│       │   ├── tail.c
│       │   ├── url.c
│       │   ├── userid.c
│       │   └── which.c
│       ├── npc
│       │   ├── assist.c
│       │   ├── drink.c
│       │   ├── drop.c
│       │   ├── eat.c
│       │   ├── element.c
│       │   ├── equip.c
│       │   ├── follow.c
│       │   ├── get.c
│       │   ├── give.c
│       │   ├── go.c
│       │   ├── group.c
│       │   ├── keep.c
│       │   ├── kill.c
│       │   ├── mjbeep.c
│       │   ├── nick.c
│       │   ├── remove.c
│       │   ├── say.c
│       │   ├── stance.c
│       │   └── use.c
│       ├── ppl
│       │   ├── alias.c
│       │   ├── baseball.c
│       │   ├── battle.c
│       │   ├── beep.c
│       │   ├── blist.c
│       │   ├── bug.c
│       │   ├── build.c
│       │   ├── buildbridge.c
│       │   ├── buildroad.c
│       │   ├── buildterrain.c
│       │   ├── buy.c
│       │   ├── check.c
│       │   ├── chfn.c
│       │   ├── close.c
│       │   ├── collect.c
│       │   ├── command.c
│       │   ├── date.c
│       │   ├── decorate.c
│       │   ├── dict.c
│       │   ├── enterprise.c
│       │   ├── estate.c
│       │   ├── fight.c
│       │   ├── finger.c
│       │   ├── grow.c
│       │   ├── help.c
│       │   ├── history.c
│       │   ├── home.c
│       │   ├── ignore.c
│       │   ├── im.c
│       │   ├── info.c
│       │   ├── inventory.c
│       │   ├── iplist.c
│       │   ├── klist.c
│       │   ├── labor.c
│       │   ├── land.c
│       │   ├── language.c
│       │   ├── levy.c
│       │   ├── localtime.c
│       │   ├── lock.c
│       │   ├── look.c
│       │   ├── map.c
│       │   ├── mayor.c
│       │   ├── mccp.c
│       │   ├── mnlmudlist.c
│       │   ├── msg.c
│       │   ├── mudlist.c
│       │   ├── occupy.c
│       │   ├── open.c
│       │   ├── order.c
│       │   ├── passwd.c
│       │   ├── plist.c
│       │   ├── quest.c
│       │   ├── quit.c
│       │   ├── random.c
│       │   ├── recall.c
│       │   ├── region.c
│       │   ├── save.c
│       │   ├── score.c
│       │   ├── sell.c
│       │   ├── semote.c
│       │   ├── set.c
│       │   ├── sit.c
│       │   ├── skill.c
│       │   ├── suicide.c
│       │   ├── taichi.c
│       │   ├── tax.c
│       │   ├── tell.c
│       │   ├── time.c
│       │   ├── title.c
│       │   ├── to.c
│       │   ├── top.c
│       │   ├── transfer.c
│       │   ├── tune.c
│       │   ├── unlock.c
│       │   ├── uptime.c
│       │   ├── version.c
│       │   ├── waitback.c
│       │   ├── web.c
│       │   ├── who.c
│       │   ├── windowsize.c
│       │   └── wizlist.c
│       ├── pplcmd_example.c
│       ├── wiz
│       │   ├── announce.c
│       │   ├── callouts.c
│       │   ├── callouts2.c
│       │   ├── cat.c
│       │   ├── cd.c
│       │   ├── check_memory.c
│       │   ├── children.c
│       │   ├── clone.c
│       │   ├── cmdusage.c
│       │   ├── cost.c
│       │   ├── cp.c
│       │   ├── data.c
│       │   ├── dellink.c
│       │   ├── dest.c
│       │   ├── edemote.c
│       │   ├── edit.c
│       │   ├── edit_help.c
│       │   ├── exchange.c
│       │   ├── ff.c
│       │   ├── fgrep.c
│       │   ├── fullheal.c
│       │   ├── g2b.c
│       │   ├── gag_user.c
│       │   ├── givememoney.c
│       │   ├── goto.c
│       │   ├── gotomap.c
│       │   ├── grep.c
│       │   ├── header.c
│       │   ├── hyperlink.c
│       │   ├── ident.c
│       │   ├── infodata.c
│       │   ├── inherits.c
│       │   ├── lf.c
│       │   ├── ls.c
│       │   ├── malloc_status.c
│       │   ├── map.c
│       │   ├── mem.c
│       │   ├── mkdir.c
│       │   ├── mkroom.c
│       │   ├── more.c
│       │   ├── mud_status.c
│       │   ├── mudstatus.c
│       │   ├── mv.c
│       │   ├── nature.c
│       │   ├── nslookup.c
│       │   ├── qload.c
│       │   ├── rm.c
│       │   ├── rmdir.c
│       │   ├── setroom.c
│       │   ├── shopping.c
│       │   ├── snoop.c
│       │   ├── summon.c
│       │   ├── tail.c
│       │   ├── update.c
│       │   ├── url.c
│       │   ├── userid.c
│       │   ├── warning.c
│       │   ├── whereis.c
│       │   ├── which.c
│       │   ├── whois.c
│       │   ├── wizbuild.c
│       │   └── wizlock.c
│       └── wizcmd_example.c
├── combat
│   └── handler
│       └── unarm.c
├── data
│   ├── backup
│   │   ├── deleted
│   │   └── user
│   ├── binaries
│   ├── board
│   │   ├── player
│   │   └── wizhall
│   ├── bug
│   ├── city
│   ├── daemon
│   │   ├── adventure.o
│   │   ├── auction.o
│   │   ├── enterprise.o
│   │   ├── exchange.o
│   │   ├── ip.o
│   │   ├── labor.o
│   │   ├── login.o
│   │   ├── mapload.o
│   │   ├── money.o
│   │   ├── mrtg
│   │   │   ├── mnlmudlist
│   │   │   └── stock
│   │   ├── news.o
│   │   ├── serial_number.o
│   │   ├── sysdatabase.o
│   │   ├── sysdatabase_bakcup.o
│   │   ├── time.o
│   │   └── top.o
│   ├── emotion
│   │   └── emotion.o
│   ├── finger
│   │   └── ip2name
│   ├── network
│   │   ├── flowrate.log
│   │   ├── i2d.o
│   │   └── i3d.o
│   ├── object
│   │   ├── lotto.o
│   │   └── mj112.o
│   └── user
│       ├── c
│       │   └── clode
│       │       └── data.o
│       └── m
│           └── msr
│               └── data.o
├── doc
│   ├── chat
│   │   └── company.txt
│   ├── docformat
│   │   └── brief
│   ├── driver
│   │   ├── modify
│   │   │   ├── detail_document
│   │   │   │   ├── add_action.c.doc
│   │   │   │   ├── backend.c.doc
│   │   │   │   ├── ed.c.doc
│   │   │   │   ├── efun_main.c.doc
│   │   │   │   ├── lex.c.doc
│   │   │   │   ├── package_ansi.c.doc
│   │   │   │   ├── package_arith_operator.c.doc
│   │   │   │   ├── package_chinese.c.doc
│   │   │   │   ├── package_contrib.c.doc
│   │   │   │   ├── package_database.c.doc
│   │   │   │   ├── package_encode.c.doc
│   │   │   │   ├── package_math.c.doc
│   │   │   │   └── sprintf.c.doc
│   │   │   └── source_files
│   │   │       ├── add_action.c
│   │   │       ├── backend.c
│   │   │       ├── ed.c
│   │   │       ├── efuns_main.c
│   │   │       ├── efuns_main.h
│   │   │       ├── lex.c
│   │   │       ├── local_options
│   │   │       ├── packages
│   │   │       │   ├── ansi.c
│   │   │       │   ├── ansi_spec.c
│   │   │       │   ├── arith_operator.c
│   │   │       │   ├── arith_operator_spec.c
│   │   │       │   ├── chinese.c
│   │   │       │   ├── chinese.h
│   │   │       │   ├── chinese_spec.c
│   │   │       │   ├── contrib.c
│   │   │       │   ├── contrib_spec.c
│   │   │       │   ├── database.c
│   │   │       │   ├── database_spec.c
│   │   │       │   ├── encode.c
│   │   │       │   ├── encode_spec.c
│   │   │       │   ├── math.c
│   │   │       │   └── math_spec.c
│   │   │       └── sprintf.c
│   │   └── perl
│   │       └── tran.pl
│   ├── help
│   │   ├── GPL
│   │   ├── PaxHeader
│   │   ├── ansicode
│   │   ├── ascii
│   │   ├── channels
│   │   ├── color
│   │   ├── copyright
│   │   ├── efuns
│   │   ├── newbie
│   │   ├── news
│   │   ├── rule
│   │   ├── topics
│   │   └── zhuyinfuhaobiao
│   ├── rwlib
│   │   ├── PaxHeader
│   │   ├── README
│   │   ├── efun
│   │   │   └── efuns_intro
│   │   └── simul_efun
│   │       ├── C_ab
│   │       ├── abs
│   │       ├── add_slash
│   │       ├── ansi_capitalize
│   │       ├── arrange_city_location
│   │       ├── arrange_location
│   │       ├── big_number_check
│   │       ├── big_number_pow
│   │       ├── broadcast
│   │       ├── city_coor_short
│   │       ├── city_roomfiles
│   │       ├── clear_screen
│   │       ├── cptree
│   │       ├── deep_path_list
│   │       ├── destruct
│   │       ├── file_exists
│   │       ├── int_array_addition
│   │       ├── intact_file_name
│   │       ├── intact_path
│   │       ├── is_chinese
│   │       ├── is_command
│   │       ├── is_daemon
│   │       ├── is_english
│   │       ├── load_cityroom
│   │       ├── load_user
│   │       ├── log_file
│   │       ├── msg
│   │       ├── multiwrap
│   │       ├── object_parse
│   │       ├── pnoun
│   │       ├── present
│   │       ├── present_objects
│   │       ├── probability
│   │       ├── process_function
│   │       ├── pythagorean_thm
│   │       ├── range_random
│   │       ├── reset_screen
│   │       ├── resolve_path
│   │       ├── reverse_array
│   │       ├── rmtree
│   │       ├── roff
│   │       ├── same_environment
│   │       ├── shout
│   │       ├── single_quote_parse
│   │       ├── sort_objects
│   │       ├── spec_line_msg
│   │       ├── startup_title_screen
│   │       ├── tell
│   │       ├── to_string
│   │       ├── user_backup_path
│   │       ├── user_data
│   │       ├── user_deleted_path
│   │       ├── user_path
│   │       └── wiz_home
│   └── v22doc
│       ├── README
│       ├── advance
│       │   └── optimization.html
│       ├── applies
│       │   ├── __INIT
│       │   ├── clean_up
│       │   ├── create
│       │   ├── heart_beat
│       │   ├── id
│       │   ├── init
│       │   ├── interactive
│       │   │   ├── catch_tell
│       │   │   ├── logon
│       │   │   ├── net_dead
│       │   │   ├── process_input
│       │   │   ├── receive_message
│       │   │   ├── receive_snoop
│       │   │   ├── telnet_suboption
│       │   │   ├── terminal_type
│       │   │   ├── window_size
│       │   │   └── write_prompt
│       │   ├── master
│       │   │   ├── author_file
│       │   │   ├── compile_object
│       │   │   ├── connect
│       │   │   ├── crash
│       │   │   ├── creator_file
│       │   │   ├── domain_file
│       │   │   ├── epilog
│       │   │   ├── error_handler
│       │   │   ├── flag
│       │   │   ├── get_bb_uid
│       │   │   ├── get_root_uid
│       │   │   ├── get_save_file_name
│       │   │   ├── log_error
│       │   │   ├── make_path_absolute
│       │   │   ├── object_name
│       │   │   ├── parse_command_all_word
│       │   │   ├── parse_command_prepos_list
│       │   │   ├── preload
│       │   │   ├── privs_file
│       │   │   ├── retrieve_ed_setup
│       │   │   ├── save_ed_setup
│       │   │   ├── slow_shutdown
│       │   │   ├── valid_bind
│       │   │   ├── valid_compile_to_c
│       │   │   ├── valid_hide
│       │   │   ├── valid_link
│       │   │   ├── valid_object
│       │   │   ├── valid_override
│       │   │   ├── valid_read
│       │   │   ├── valid_save_binary
│       │   │   ├── valid_seteuid
│       │   │   ├── valid_shadow
│       │   │   ├── valid_socket
│       │   │   └── valid_write
│       │   ├── move_or_destruct
│       │   ├── parsing
│       │   │   ├── parse_command_adjectiv_id_list
│       │   │   ├── parse_command_id_list
│       │   │   └── parse_command_plural_id_list
│       │   └── reset
│       ├── basic
│       │   ├── Contents
│       │   ├── Introduction
│       │   ├── chapter1
│       │   ├── chapter2
│       │   ├── chapter3
│       │   ├── chapter4
│       │   ├── chapter5
│       │   ├── chapter6
│       │   ├── chapter7
│       │   └── chapter8
│       ├── concepts
│       │   ├── MudOSdriver
│       │   ├── defines
│       │   ├── global_include_file
│       │   ├── lpc
│       │   ├── lpc.driver
│       │   ├── lpc.lpc
│       │   ├── lpc.object
│       │   ├── message_doc
│       │   ├── objects
│       │   ├── oop
│       │   ├── preprocessor
│       │   ├── simul_efun
│       │   └── socket_efuns
│       ├── driver
│       │   ├── adding_efuns
│       │   ├── done-lars
│       │   ├── done-mudos
│       │   ├── malloc
│       │   └── stackmachine
│       ├── efuns
│       │   ├── TODO
│       │   ├── arrays
│       │   │   ├── allocate
│       │   │   ├── filter_array
│       │   │   ├── map_array
│       │   │   ├── member_array
│       │   │   ├── sort_array
│       │   │   └── unique_array
│       │   ├── buffers
│       │   │   ├── allocate_buffer
│       │   │   ├── crc32
│       │   │   ├── read_buffer
│       │   │   └── write_buffer
│       │   ├── calls
│       │   │   ├── call_other
│       │   │   ├── call_out
│       │   │   ├── catch
│       │   │   ├── origin
│       │   │   ├── previous_object
│       │   │   ├── query_shadowing
│       │   │   ├── remove_call_out
│       │   │   ├── remove_shadow
│       │   │   ├── shadow
│       │   │   ├── this_object
│       │   │   └── throw
│       │   ├── compile
│       │   │   └── generate_source
│       │   ├── filesystem
│       │   │   ├── cp
│       │   │   ├── file_size
│       │   │   ├── get_dir
│       │   │   ├── link
│       │   │   ├── mkdir
│       │   │   ├── read_bytes
│       │   │   ├── read_file
│       │   │   ├── rename
│       │   │   ├── rm
│       │   │   ├── rmdir
│       │   │   ├── stat
│       │   │   ├── tail
│       │   │   ├── write_bytes
│       │   │   └── write_file
│       │   ├── floats
│       │   │   ├── acos
│       │   │   ├── asin
│       │   │   ├── atan
│       │   │   ├── ceil
│       │   │   ├── cos
│       │   │   ├── exp
│       │   │   ├── floor
│       │   │   ├── log
│       │   │   ├── pow
│       │   │   ├── sin
│       │   │   ├── sqrt
│       │   │   ├── tan
│       │   │   └── to_int
│       │   ├── functions
│       │   │   ├── evaluate
│       │   │   └── function_owner
│       │   ├── general
│       │   │   ├── filter
│       │   │   ├── map
│       │   │   ├── restore_variable
│       │   │   ├── save_variable
│       │   │   ├── sizeof
│       │   │   ├── store_variable
│       │   │   └── typeof
│       │   ├── interactive
│       │   │   ├── add_action
│       │   │   ├── command
│       │   │   ├── commands
│       │   │   ├── disable_commands
│       │   │   ├── disable_wizard
│       │   │   ├── ed
│       │   │   ├── enable_commands
│       │   │   ├── enable_wizard
│       │   │   ├── exec
│       │   │   ├── find_player
│       │   │   ├── get_char
│       │   │   ├── in_edit
│       │   │   ├── in_input
│       │   │   ├── input_to
│       │   │   ├── interactive
│       │   │   ├── message
│       │   │   ├── notify_fail
│       │   │   ├── printf
│       │   │   ├── query_host_name
│       │   │   ├── query_idle
│       │   │   ├── query_ip_name
│       │   │   ├── query_ip_number
│       │   │   ├── query_ip_port
│       │   │   ├── query_notify_fail
│       │   │   ├── query_snoop
│       │   │   ├── query_snooping
│       │   │   ├── receive
│       │   │   ├── remove_action
│       │   │   ├── remove_interactive
│       │   │   ├── resolve
│       │   │   ├── say
│       │   │   ├── set_this_player
│       │   │   ├── shout
│       │   │   ├── snoop
│       │   │   ├── this_interactive
│       │   │   ├── this_player
│       │   │   ├── this_user
│       │   │   ├── userp
│       │   │   ├── users
│       │   │   ├── wizardp
│       │   │   └── write
│       │   ├── internals
│       │   │   ├── cache_stats
│       │   │   ├── debug_info
│       │   │   ├── debugmalloc
│       │   │   ├── dump_file_descriptors
│       │   │   ├── dump_prog
│       │   │   ├── dump_socket_status
│       │   │   ├── dumpallobj
│       │   │   ├── get_config
│       │   │   ├── malloc_status
│       │   │   ├── memory_info
│       │   │   ├── mud_status
│       │   │   ├── opcprof
│       │   │   ├── query_load_average
│       │   │   ├── refs
│       │   │   ├── rusage
│       │   │   ├── set_debug_level
│       │   │   ├── set_malloc_mask
│       │   │   ├── swap
│       │   │   ├── time_expression
│       │   │   ├── trace
│       │   │   └── traceprefix
│       │   ├── mappings
│       │   │   ├── allocate_mapping
│       │   │   ├── filter_mapping
│       │   │   ├── keys
│       │   │   ├── m_delete
│       │   │   ├── m_indices
│       │   │   ├── m_values
│       │   │   ├── map_delete
│       │   │   ├── map_mapping
│       │   │   ├── match_path
│       │   │   └── values
│       │   ├── mixed
│       │   │   ├── arrayp
│       │   │   ├── bufferp
│       │   │   ├── classp
│       │   │   ├── floatp
│       │   │   ├── functionp
│       │   │   ├── intp
│       │   │   ├── mapp
│       │   │   ├── nullp
│       │   │   ├── objectp
│       │   │   ├── pointerp
│       │   │   ├── stringp
│       │   │   └── undefinedp
│       │   ├── mudlib
│       │   │   ├── author_stats
│       │   │   ├── domain_stats
│       │   │   ├── export_uid
│       │   │   ├── find_living
│       │   │   ├── geteuid
│       │   │   ├── getuid
│       │   │   ├── living
│       │   │   ├── livings
│       │   │   ├── query_privs
│       │   │   ├── set_author
│       │   │   ├── set_light
│       │   │   ├── set_living_name
│       │   │   ├── set_privs
│       │   │   └── seteuid
│       │   ├── new
│       │   │   ├── call_stack
│       │   │   ├── copy
│       │   │   ├── fetch_variable
│       │   │   ├── file_length
│       │   │   ├── flush_messages
│       │   │   ├── functions
│       │   │   ├── heart_beats
│       │   │   ├── iconv_string
│       │   │   ├── lpc_info
│       │   │   ├── memory_summary
│       │   │   ├── named_livings
│       │   │   ├── pluralize
│       │   │   ├── program_info
│       │   │   ├── terminal_colour
│       │   │   └── view_errors
│       │   ├── numbers
│       │   │   ├── random
│       │   │   └── to_float
│       │   ├── objects
│       │   │   ├── all_inventory
│       │   │   ├── children
│       │   │   ├── clone_object
│       │   │   ├── clonep
│       │   │   ├── deep_inventory
│       │   │   ├── destruct
│       │   │   ├── environment
│       │   │   ├── file_name
│       │   │   ├── find_object
│       │   │   ├── first_inventory
│       │   │   ├── load_object
│       │   │   ├── master
│       │   │   ├── move_object
│       │   │   ├── new
│       │   │   ├── next_inventory
│       │   │   ├── objects
│       │   │   ├── present
│       │   │   ├── query_heart_beat
│       │   │   ├── reload_object
│       │   │   ├── restore_object
│       │   │   ├── save_object
│       │   │   ├── set_heart_beat
│       │   │   ├── set_hide
│       │   │   ├── tell_object
│       │   │   ├── tell_room
│       │   │   └── virtualp
│       │   ├── parsing
│       │   │   ├── TODO
│       │   │   └── query_verb
│       │   ├── sockets
│       │   │   ├── socket_accept
│       │   │   ├── socket_acquire
│       │   │   ├── socket_address
│       │   │   ├── socket_bind
│       │   │   ├── socket_close
│       │   │   ├── socket_connect
│       │   │   ├── socket_create
│       │   │   ├── socket_error
│       │   │   ├── socket_listen
│       │   │   ├── socket_release
│       │   │   └── socket_write
│       │   ├── strings
│       │   │   ├── B2G
│       │   │   ├── G2B
│       │   │   ├── bg5cc
│       │   │   ├── capitalize
│       │   │   ├── clear_bit
│       │   │   ├── crypt
│       │   │   ├── cwrap
│       │   │   ├── explode
│       │   │   ├── implode
│       │   │   ├── lower_case
│       │   │   ├── reg_assoc
│       │   │   ├── regexp
│       │   │   ├── repeat_string
│       │   │   ├── replace_string
│       │   │   ├── set_bit
│       │   │   ├── sprintf
│       │   │   ├── sscanf
│       │   │   ├── strcmp
│       │   │   ├── strlen
│       │   │   ├── strsrch
│       │   │   ├── terminal_colour
│       │   │   ├── test_bit
│       │   │   └── upper_case
│       │   ├── system
│       │   │   ├── all_previous_objects
│       │   │   ├── call_out_info
│       │   │   ├── ctime
│       │   │   ├── deep_inherit_list
│       │   │   ├── error
│       │   │   ├── eval_cost
│       │   │   ├── find_call_out
│       │   │   ├── function_exists
│       │   │   ├── function_profile
│       │   │   ├── inherit_list
│       │   │   ├── inherits
│       │   │   ├── localtime
│       │   │   ├── max_eval_cost
│       │   │   ├── reclaim_objects
│       │   │   ├── replace_program
│       │   │   ├── replaceable
│       │   │   ├── reset_eval_cost
│       │   │   ├── set_eval_limit
│       │   │   ├── set_reset
│       │   │   ├── shallow_inherit_list
│       │   │   ├── shutdown
│       │   │   ├── time
│       │   │   ├── uptime
│       │   │   └── variables
│       │   └── v21
│       │       ├── apply
│       │       ├── bind
│       │       ├── break_string
│       │       ├── each
│       │       ├── ed_cmd
│       │       ├── ed_start
│       │       ├── errorp
│       │       ├── moncontrol
│       │       ├── parse_command
│       │       ├── process_string
│       │       ├── process_value
│       │       ├── query_ed_mode
│       │       └── unique_mapping
│       ├── html
│       │   ├── break.html
│       │   ├── class.html
│       │   ├── continue.html
│       │   ├── do_while.html
│       │   ├── for.html
│       │   ├── foreach.html
│       │   ├── implicit_pointer.html
│       │   ├── profiling.html
│       │   └── while.html
│       ├── intermediate
│       │   ├── Contents
│       │   ├── Copyright
│       │   ├── chapter1
│       │   ├── chapter2
│       │   ├── chapter3
│       │   ├── chapter4
│       │   ├── chapter5
│       │   ├── chapter6
│       │   └── chapter7
│       ├── lpc
│       │   ├── constructs
│       │   │   ├── for
│       │   │   ├── function
│       │   │   ├── if
│       │   │   ├── include
│       │   │   ├── inherit
│       │   │   ├── prototypes
│       │   │   ├── switch
│       │   │   └── while
│       │   ├── preprocessor
│       │   │   ├── README
│       │   │   ├── define
│       │   │   └── include
│       │   └── types
│       │       ├── array.2d
│       │       ├── buffer
│       │       ├── float
│       │       ├── function
│       │       ├── general
│       │       ├── lpc.2darray
│       │       ├── lpc.function
│       │       ├── mappings
│       │       ├── strings
│       │       └── substructures
│       ├── other_mud_doc
│       │   └── fs_doc
│       │       ├── History.MudOS
│       │       ├── Install.mudlib
│       │       ├── README
│       │       ├── Running.MudOS
│       │       ├── applies
│       │       │   ├── __INIT
│       │       │   ├── clean_up
│       │       │   ├── create
│       │       │   ├── id
│       │       │   ├── init
│       │       │   ├── interactive
│       │       │   │   ├── catch_tell
│       │       │   │   ├── logon
│       │       │   │   ├── net_dead
│       │       │   │   ├── process_input
│       │       │   │   ├── receive_message
│       │       │   │   ├── receive_snoop
│       │       │   │   ├── telnet_suboption
│       │       │   │   ├── terminal_type
│       │       │   │   └── write_prompt
│       │       │   ├── master
│       │       │   │   ├── author_file
│       │       │   │   ├── compile_object
│       │       │   │   ├── connect
│       │       │   │   ├── crash
│       │       │   │   ├── creator_file
│       │       │   │   ├── domain_file
│       │       │   │   ├── epilog
│       │       │   │   ├── error_handler
│       │       │   │   ├── flag
│       │       │   │   ├── get_bb_uid
│       │       │   │   ├── get_root_uid
│       │       │   │   ├── get_save_file_name
│       │       │   │   ├── log_error
│       │       │   │   ├── make_path_absolute
│       │       │   │   ├── object_name
│       │       │   │   ├── preload
│       │       │   │   ├── privs_file
│       │       │   │   ├── retrieve_ed_setup
│       │       │   │   ├── save_ed_setup
│       │       │   │   ├── slow_shutdown
│       │       │   │   ├── valid_asm
│       │       │   │   ├── valid_bind
│       │       │   │   ├── valid_compile_to_c
│       │       │   │   ├── valid_hide
│       │       │   │   ├── valid_link
│       │       │   │   ├── valid_object
│       │       │   │   ├── valid_override
│       │       │   │   ├── valid_read
│       │       │   │   ├── valid_save_binary
│       │       │   │   ├── valid_seteuid
│       │       │   │   ├── valid_shadow
│       │       │   │   ├── valid_socket
│       │       │   │   ├── valid_write
│       │       │   │   └── view_errors
│       │       │   ├── move_or_destruct
│       │       │   └── reset
│       │       ├── build
│       │       │   ├── armor_types
│       │       │   ├── char_apply
│       │       │   ├── char_prop
│       │       │   ├── item_prop
│       │       │   ├── npc
│       │       │   ├── party_npc
│       │       │   ├── room
│       │       │   ├── room_prop
│       │       │   └── weapon_prop
│       │       ├── build.zip
│       │       ├── concepts
│       │       │   ├── MudOSdriver
│       │       │   ├── global_include_file
│       │       │   ├── lpc
│       │       │   ├── message_doc
│       │       │   ├── objects
│       │       │   ├── oop
│       │       │   ├── preprocessor
│       │       │   ├── simul_efun
│       │       │   └── socket_efuns
│       │       ├── defines
│       │       │   ├── BSDMALLOC
│       │       │   ├── CHECK_MEMORY
│       │       │   ├── DEBUGMALLOC
│       │       │   ├── DEBUGMALLOC_EXTENSIONS
│       │       │   ├── DO_MSTATS
│       │       │   ├── HAS_STATUS_TYPE
│       │       │   ├── README-DEFINES
│       │       │   ├── SMALLOC
│       │       │   ├── SYSMALLOC
│       │       │   ├── WRAPPEDMALLOC
│       │       │   └── lpc
│       │       │       ├── LPC3
│       │       │       ├── MUDOS
│       │       │       ├── MUD_NAME
│       │       │       ├── README
│       │       │       ├── SAVE_EXTENSION
│       │       │       ├── USE_EUID
│       │       │       ├── __ARCH__
│       │       │       ├── __DIR__
│       │       │       ├── __FILE__
│       │       │       ├── __PORT__
│       │       │       └── __VERSION__
│       │       ├── driver
│       │       │   ├── adding_efuns
│       │       │   ├── done-lars
│       │       │   ├── done-mudos
│       │       │   ├── malloc
│       │       │   └── stackmachine
│       │       ├── efuns
│       │       │   ├── acos
│       │       │   ├── add_action
│       │       │   ├── all_inventory
│       │       │   ├── all_previous_objects
│       │       │   ├── allocate
│       │       │   ├── allocate_buffer
│       │       │   ├── allocate_mapping
│       │       │   ├── apply
│       │       │   ├── arrayp
│       │       │   ├── asin
│       │       │   ├── atan
│       │       │   ├── author_stats
│       │       │   ├── bind
│       │       │   ├── break_string
│       │       │   ├── bufferp
│       │       │   ├── cache_stats
│       │       │   ├── call_other
│       │       │   ├── call_out
│       │       │   ├── call_out_info
│       │       │   ├── capitalize
│       │       │   ├── catch
│       │       │   ├── ceil
│       │       │   ├── children
│       │       │   ├── clear_bit
│       │       │   ├── clone_object
│       │       │   ├── clonep
│       │       │   ├── command
│       │       │   ├── commands
│       │       │   ├── cos
│       │       │   ├── cp
│       │       │   ├── crc32
│       │       │   ├── crypt
│       │       │   ├── ctime
│       │       │   ├── debug_info
│       │       │   ├── debugmalloc
│       │       │   ├── deep_inherit_list
│       │       │   ├── deep_inventory
│       │       │   ├── destruct
│       │       │   ├── disable_commands
│       │       │   ├── disable_wizard
│       │       │   ├── domain_stats
│       │       │   ├── dump_file_descriptors
│       │       │   ├── dump_prog
│       │       │   ├── dump_socket_status
│       │       │   ├── dumpallobj
│       │       │   ├── each
│       │       │   ├── ed
│       │       │   ├── ed_cmd
│       │       │   ├── ed_start
│       │       │   ├── enable_commands
│       │       │   ├── enable_wizard
│       │       │   ├── environment
│       │       │   ├── error
│       │       │   ├── errorp
│       │       │   ├── eval_cost
│       │       │   ├── evaluate
│       │       │   ├── exec
│       │       │   ├── exp
│       │       │   ├── explode
│       │       │   ├── export_uid
│       │       │   ├── file_name
│       │       │   ├── file_size
│       │       │   ├── filter_array
│       │       │   ├── filter_mapping
│       │       │   ├── find_call_out
│       │       │   ├── find_living
│       │       │   ├── find_object
│       │       │   ├── find_player
│       │       │   ├── first_inventory
│       │       │   ├── floatp
│       │       │   ├── floor
│       │       │   ├── function_exists
│       │       │   ├── function_profile
│       │       │   ├── functionp
│       │       │   ├── generate_source
│       │       │   ├── get_char
│       │       │   ├── get_config
│       │       │   ├── get_dir
│       │       │   ├── geteuid
│       │       │   ├── getuid
│       │       │   ├── implode
│       │       │   ├── in_edit
│       │       │   ├── in_input
│       │       │   ├── inherit_list
│       │       │   ├── inherits
│       │       │   ├── input_to
│       │       │   ├── interactive
│       │       │   ├── intp
│       │       │   ├── keys
│       │       │   ├── link
│       │       │   ├── living
│       │       │   ├── livings
│       │       │   ├── load_object
│       │       │   ├── localtime
│       │       │   ├── log
│       │       │   ├── lower_case
│       │       │   ├── malloc_status
│       │       │   ├── map
│       │       │   ├── map_array
│       │       │   ├── map_delete
│       │       │   ├── map_mapping
│       │       │   ├── mapp
│       │       │   ├── master
│       │       │   ├── match_path
│       │       │   ├── max_eval_cost
│       │       │   ├── member_array
│       │       │   ├── memory_info
│       │       │   ├── message
│       │       │   ├── mkdir
│       │       │   ├── moncontrol
│       │       │   ├── move_object
│       │       │   ├── mud_status
│       │       │   ├── new
│       │       │   ├── next_inventory
│       │       │   ├── notify_fail
│       │       │   ├── nullp
│       │       │   ├── objectp
│       │       │   ├── objects
│       │       │   ├── opcprof
│       │       │   ├── origin
│       │       │   ├── parse_command
│       │       │   ├── pointerp
│       │       │   ├── pow
│       │       │   ├── present
│       │       │   ├── previous_object
│       │       │   ├── printf
│       │       │   ├── process_string
│       │       │   ├── process_value
│       │       │   ├── query_ed_mode
│       │       │   ├── query_heart_beat
│       │       │   ├── query_host_name
│       │       │   ├── query_idle
│       │       │   ├── query_ip_name
│       │       │   ├── query_ip_number
│       │       │   ├── query_load_average
│       │       │   ├── query_privs
│       │       │   ├── query_shadowing
│       │       │   ├── query_snoop
│       │       │   ├── query_snooping
│       │       │   ├── query_verb
│       │       │   ├── random
│       │       │   ├── read_buffer
│       │       │   ├── read_bytes
│       │       │   ├── read_file
│       │       │   ├── receive
│       │       │   ├── reclaim_objects
│       │       │   ├── refs
│       │       │   ├── reg_assoc
│       │       │   ├── regexp
│       │       │   ├── reload_object
│       │       │   ├── remove_action
│       │       │   ├── remove_call_out
│       │       │   ├── rename
│       │       │   ├── replace_program
│       │       │   ├── replace_string
│       │       │   ├── reset_eval_cost
│       │       │   ├── resolve
│       │       │   ├── restore_object
│       │       │   ├── restore_variable
│       │       │   ├── rm
│       │       │   ├── rmdir
│       │       │   ├── rusage
│       │       │   ├── save_object
│       │       │   ├── save_variable
│       │       │   ├── say
│       │       │   ├── set_author
│       │       │   ├── set_bit
│       │       │   ├── set_debug_level
│       │       │   ├── set_eval_limit
│       │       │   ├── set_heart_beat
│       │       │   ├── set_hide
│       │       │   ├── set_light
│       │       │   ├── set_living_name
│       │       │   ├── set_malloc_mask
│       │       │   ├── set_privs
│       │       │   ├── set_reset
│       │       │   ├── set_this_player
│       │       │   ├── seteuid
│       │       │   ├── shadow
│       │       │   ├── shout
│       │       │   ├── shutdown
│       │       │   ├── sin
│       │       │   ├── sizeof
│       │       │   ├── snoop
│       │       │   ├── socket_accept
│       │       │   ├── socket_acquire
│       │       │   ├── socket_address
│       │       │   ├── socket_bind
│       │       │   ├── socket_close
│       │       │   ├── socket_connect
│       │       │   ├── socket_create
│       │       │   ├── socket_error
│       │       │   ├── socket_listen
│       │       │   ├── socket_release
│       │       │   ├── socket_write
│       │       │   ├── sort_array
│       │       │   ├── sprintf
│       │       │   ├── sqrt
│       │       │   ├── sscanf
│       │       │   ├── stat
│       │       │   ├── strcmp
│       │       │   ├── stringp
│       │       │   ├── strlen
│       │       │   ├── strsrch
│       │       │   ├── swap
│       │       │   ├── tail
│       │       │   ├── tan
│       │       │   ├── tell_object
│       │       │   ├── tell_room
│       │       │   ├── test_bit
│       │       │   ├── this_interactive
│       │       │   ├── this_object
│       │       │   ├── this_player
│       │       │   ├── throw
│       │       │   ├── time
│       │       │   ├── time_expression
│       │       │   ├── to_float
│       │       │   ├── to_int
│       │       │   ├── trace
│       │       │   ├── traceprefix
│       │       │   ├── tt
│       │       │   ├── typeof
│       │       │   ├── undefinedp
│       │       │   ├── unique_array
│       │       │   ├── unique_mapping
│       │       │   ├── uptime
│       │       │   ├── userp
│       │       │   ├── users
│       │       │   ├── values
│       │       │   ├── virtualp
│       │       │   ├── wizardp
│       │       │   ├── write
│       │       │   ├── write_buffer
│       │       │   ├── write_bytes
│       │       │   └── write_file
│       │       ├── etc
│       │       │   ├── class.es3
│       │       │   └── stat.es3
│       │       ├── help
│       │       │   ├── FS
│       │       │   ├── about
│       │       │   ├── area
│       │       │   ├── areas
│       │       │   ├── armor
│       │       │   ├── armor.tmp
│       │       │   ├── armor_rule
│       │       │   ├── armor_rule.bak
│       │       │   ├── attitude
│       │       │   ├── balance
│       │       │   ├── board
│       │       │   ├── bonze
│       │       │   ├── callout
│       │       │   ├── channels
│       │       │   ├── class
│       │       │   ├── class_attr
│       │       │   ├── cmds
│       │       │   ├── combat
│       │       │   ├── command
│       │       │   ├── convert
│       │       │   ├── cure
│       │       │   ├── daemon_dragon
│       │       │   ├── damage
│       │       │   ├── deposit
│       │       │   ├── dremagic
│       │       │   ├── dying
│       │       │   ├── edit_example
│       │       │   ├── fighter
│       │       │   ├── five_spells
│       │       │   ├── force
│       │       │   ├── ftp
│       │       │   ├── ftp.old
│       │       │   ├── gift
│       │       │   ├── health
│       │       │   ├── helps
│       │       │   ├── inherit
│       │       │   ├── list
│       │       │   ├── ls.doc
│       │       │   ├── magic
│       │       │   ├── main_map
│       │       │   ├── main_map2
│       │       │   ├── map
│       │       │   ├── map.es2
│       │       │   ├── martial
│       │       │   ├── material
│       │       │   ├── medicine
│       │       │   ├── mud_idea
│       │       │   ├── newbie
│       │       │   ├── officer
│       │       │   ├── party_npc
│       │       │   ├── pk_rules
│       │       │   ├── quest
│       │       │   ├── quest_func
│       │       │   ├── quest_rule
│       │       │   ├── quest_rules
│       │       │   ├── rank
│       │       │   ├── roommaker
│       │       │   ├── rules
│       │       │   ├── sec_kee
│       │       │   ├── sell
│       │       │   ├── settings
│       │       │   ├── setup
│       │       │   ├── sha-array
│       │       │   ├── sha_kee
│       │       │   ├── shasword
│       │       │   ├── skill
│       │       │   ├── skill_rules
│       │       │   ├── start
│       │       │   ├── stats
│       │       │   ├── story
│       │       │   ├── sword-array
│       │       │   ├── t_spells
│       │       │   ├── tendo
│       │       │   ├── theme
│       │       │   ├── tiandaopai
│       │       │   ├── tintin
│       │       │   ├── topics
│       │       │   ├── trade
│       │       │   ├── value
│       │       │   ├── weapon
│       │       │   ├── weapon_rule
│       │       │   ├── withdraw
│       │       │   ├── wiz_rules
│       │       │   ├── wiz_rules.wade
│       │       │   ├── wizard
│       │       │   ├── xianjianpai
│       │       │   └── zhaohuanshu
│       │       ├── hskill
│       │       ├── lpc
│       │       │   ├── constructs
│       │       │   │   ├── for
│       │       │   │   ├── function
│       │       │   │   ├── if
│       │       │   │   ├── include
│       │       │   │   ├── inherit
│       │       │   │   ├── prototypes
│       │       │   │   ├── switch
│       │       │   │   └── while
│       │       │   ├── preprocessor
│       │       │   │   ├── README
│       │       │   │   ├── define
│       │       │   │   └── include
│       │       │   └── types
│       │       │       ├── array.2d
│       │       │       ├── buffer
│       │       │       ├── float
│       │       │       ├── function
│       │       │       ├── general
│       │       │       ├── mappings
│       │       │       ├── strings
│       │       │       └── substructures
│       │       ├── mudlib
│       │       │   ├── action
│       │       │   ├── daemon
│       │       │   │   ├── chard
│       │       │   │   ├── chinesed
│       │       │   │   ├── cmdd
│       │       │   │   ├── combatd
│       │       │   │   ├── fingerd
│       │       │   │   ├── human
│       │       │   │   ├── monster
│       │       │   │   ├── rankd
│       │       │   │   └── securityd
│       │       │   ├── feature
│       │       │   │   ├── README
│       │       │   │   ├── action
│       │       │   │   ├── alias
│       │       │   │   ├── apprentice
│       │       │   │   ├── autoload
│       │       │   │   ├── clean_up
│       │       │   │   ├── command
│       │       │   │   ├── damage
│       │       │   │   ├── dbase
│       │       │   │   ├── edit
│       │       │   │   ├── equip
│       │       │   │   ├── finance
│       │       │   │   ├── message
│       │       │   │   ├── more
│       │       │   │   ├── name
│       │       │   │   ├── save
│       │       │   │   ├── skill
│       │       │   │   ├── team
│       │       │   │   └── vendor
│       │       │   ├── inheritance
│       │       │   ├── skill
│       │       │   └── std
│       │       │       ├── bank
│       │       │       ├── char
│       │       │       ├── equip
│       │       │       ├── item
│       │       │       │   └── combined
│       │       │       ├── money
│       │       │       ├── npc
│       │       │       ├── skill
│       │       │       └── weapon
│       │       ├── pf
│       │       ├── simul_efuns
│       │       │   ├── article
│       │       │   ├── atoi
│       │       │   ├── base_name
│       │       │   ├── blink
│       │       │   ├── bold
│       │       │   ├── clear_line
│       │       │   ├── clear_screen
│       │       │   ├── creator_file
│       │       │   ├── data_dir
│       │       │   ├── data_file
│       │       │   ├── destruct
│       │       │   ├── directory_exists
│       │       │   ├── dump_variable
│       │       │   ├── emote
│       │       │   ├── erase_line
│       │       │   ├── exclude_array
│       │       │   ├── exec
│       │       │   ├── file_exists
│       │       │   ├── find_object_or_load
│       │       │   ├── format_string
│       │       │   ├── format_time
│       │       │   ├── get_char
│       │       │   ├── get_object
│       │       │   ├── get_objects
│       │       │   ├── getoid
│       │       │   ├── hiddenp
│       │       │   ├── identify
│       │       │   ├── index
│       │       │   ├── inverse
│       │       │   ├── member_group
│       │       │   ├── merge
│       │       │   ├── mkdirs
│       │       │   ├── objective
│       │       │   ├── path_file
│       │       │   ├── pluralize
│       │       │   ├── pluralize_verb
│       │       │   ├── possessive
│       │       │   ├── ppossessive
│       │       │   ├── query_idle_string
│       │       │   ├── resolv_path
│       │       │   ├── say
│       │       │   ├── shout
│       │       │   ├── shutdown
│       │       │   ├── slice_array
│       │       │   ├── snoop
│       │       │   ├── subjective
│       │       │   ├── substr
│       │       │   ├── tell_group
│       │       │   ├── tell_object
│       │       │   ├── tell_room
│       │       │   ├── temp_file
│       │       │   ├── to_object
│       │       │   ├── underscore
│       │       │   ├── uniq_array
│       │       │   ├── up_line
│       │       │   ├── user_exists
│       │       │   ├── user_path
│       │       │   ├── visible
│       │       │   ├── wrap
│       │       │   ├── write
│       │       │   └── writef
│       │       ├── skill
│       │       │   ├── badforce
│       │       │   ├── badstrike
│       │       │   ├── celestial
│       │       │   ├── chaos-steps
│       │       │   ├── chen-steps
│       │       │   ├── cloud-magic
│       │       │   ├── cloudy-fly
│       │       │   ├── coldpoison
│       │       │   ├── crazy-blade
│       │       │   ├── dragon-blade
│       │       │   ├── dragon-dagger
│       │       │   ├── earth_spells
│       │       │   ├── fire_spells
│       │       │   ├── fonxansword
│       │       │   ├── god-plan
│       │       │   ├── god-seven
│       │       │   ├── godcure
│       │       │   ├── gold_spells
│       │       │   ├── gouyee
│       │       │   ├── haoforce
│       │       │   ├── honest-kan
│       │       │   ├── iceforce
│       │       │   ├── knowpen
│       │       │   ├── konforce
│       │       │   ├── linpo-steps
│       │       │   ├── liuh-ken
│       │       │   ├── lotusforce
│       │       │   ├── lungshan
│       │       │   ├── necromancy
│       │       │   ├── oneforce
│       │       │   ├── poetforce
│       │       │   ├── poisonforce
│       │       │   ├── rain-throwing
│       │       │   ├── seaforce
│       │       │   ├── security
│       │       │   ├── six-chaos-sword
│       │       │   ├── six-fingers
│       │       │   ├── snow-martial
│       │       │   ├── snowforce
│       │       │   ├── sunforce
│       │       │   ├── taoism
│       │       │   ├── taoist.chun
│       │       │   ├── training-force
│       │       │   ├── water_spells
│       │       │   ├── winter-steps
│       │       │   └── wood_spells
│       │       ├── skill.misc
│       │       ├── wiz
│       │       │   ├── new_wiz
│       │       │   ├── newwiz.doc
│       │       │   ├── nnwiz
│       │       │   ├── quality_check
│       │       │   └── wiz_rules
│       │       └── wizhelp
│       │           ├── edit_cmd
│       │           ├── edit_file
│       │           ├── edit_note
│       │           ├── emote
│       │           ├── file_oper
│       │           ├── mob_set
│       │           ├── npc_example
│       │           ├── wiz_groups.812688783
│       │           ├── wiz_init
│       │           ├── wiz_jobs
│       │           ├── wiz_mind
│       │           ├── wiz_settings
│       │           ├── wiz_works
│       │           └── wizcmds
│       └── ref
│           ├── lpc.2darray
│           ├── lpc.driver
│           ├── lpc.function
│           ├── lpc.lpc
│           ├── lpc.mud
│           └── lpc.object
├── include
│   ├── ansi.h
│   ├── areadata.h
│   ├── autokill.h
│   ├── backup
│   │   └── daemon.h
│   ├── bank.h
│   ├── baseball.h
│   ├── buff.h
│   ├── building.h
│   ├── citycondition.h
│   ├── citydata.h
│   ├── combat.h
│   ├── condition.h
│   ├── daemon.h
│   ├── delay.h
│   ├── disaster.h
│   ├── emotion.h
│   ├── envvar.h
│   ├── equipment.h
│   ├── estate.h
│   ├── eval.h
│   ├── feature.h
│   ├── fly.h
│   ├── function.h
│   ├── futures.h
│   ├── gender.h
│   ├── globals.h
│   ├── group.h
│   ├── help.h
│   ├── i2d.h
│   ├── i3.h
│   ├── inherit.h
│   ├── inputto.h
│   ├── ip.h
│   ├── localtime.h
│   ├── location.h
│   ├── lock.h
│   ├── login.h
│   ├── map.h
│   ├── material.h
│   ├── message.h
│   ├── money.h
│   ├── mudlib.h
│   ├── nature.h
│   ├── nature_data.h
│   ├── npc.h
│   ├── object.h
│   ├── origin.h
│   ├── pronoun.h
│   ├── quest.h
│   ├── roommodule.h
│   ├── runtime_config.h
│   ├── secure.h
│   ├── skill.h
│   ├── socket.h
│   ├── socket_err.h
│   ├── status.h
│   ├── stuck.h
│   ├── terrain.h
│   ├── time.h
│   ├── type.h
│   ├── use.h
│   └── weapon.h
├── log
│   ├── MudOS
│   ├── channel
│   ├── command
│   │   ├── etc
│   │   └── snoop_log
│   ├── daemon
│   ├── system
│   │   └── preload
│   └── wizhall
├── npc
├── obj
│   ├── adventure
│   │   └── map.c
│   ├── armor
│   │   ├── ancient_belt.c
│   │   ├── ancient_boots.c
│   │   ├── ancient_bracers.c
│   │   ├── ancient_breast_plate.c
│   │   ├── ancient_cloak.c
│   │   ├── ancient_gloves.c
│   │   ├── ancient_helm.c
│   │   ├── ancient_leggings.c
│   │   ├── caerulea_belt.c
│   │   ├── caerulea_boots.c
│   │   ├── caerulea_bracers.c
│   │   ├── caerulea_breast_plate.c
│   │   ├── caerulea_cloak.c
│   │   ├── caerulea_gloves.c
│   │   ├── caerulea_helm.c
│   │   ├── caerulea_leggings.c
│   │   ├── chi_lord_belt.c
│   │   ├── chi_lord_boots.c
│   │   ├── chi_lord_bracers.c
│   │   ├── chi_lord_breast_plate.c
│   │   ├── chi_lord_cloak.c
│   │   ├── chi_lord_gloves.c
│   │   ├── chi_lord_helm.c
│   │   ├── chi_lord_leggings.c
│   │   ├── coarse_belt.c
│   │   ├── coarse_boots.c
│   │   ├── coarse_bracers.c
│   │   ├── coarse_breast_plate.c
│   │   ├── coarse_cloak.c
│   │   ├── coarse_gloves.c
│   │   ├── coarse_helm.c
│   │   ├── coarse_leggings.c
│   │   ├── gigas_belt.c
│   │   ├── gigas_boots.c
│   │   ├── gigas_bracers.c
│   │   ├── gigas_breast_plate.c
│   │   ├── gigas_cloak.c
│   │   ├── gigas_gloves.c
│   │   ├── gigas_helm.c
│   │   ├── gigas_leggings.c
│   │   ├── hydra_belt.c
│   │   ├── hydra_boots.c
│   │   ├── hydra_bracers.c
│   │   ├── hydra_breast_plate.c
│   │   ├── hydra_cloak.c
│   │   ├── hydra_gloves.c
│   │   ├── hydra_helm.c
│   │   ├── hydra_leggings.c
│   │   ├── mactans_belt.c
│   │   ├── mactans_boots.c
│   │   ├── mactans_bracers.c
│   │   ├── mactans_breast_plate.c
│   │   ├── mactans_cloak.c
│   │   ├── mactans_gloves.c
│   │   ├── mactans_helm.c
│   │   ├── mactans_leggings.c
│   │   ├── mydas_belt.c
│   │   ├── mydas_boots.c
│   │   ├── mydas_bracers.c
│   │   ├── mydas_breast_plate.c
│   │   ├── mydas_cloak.c
│   │   ├── mydas_gloves.c
│   │   ├── mydas_helm.c
│   │   ├── mydas_leggings.c
│   │   ├── nebulosa_belt.c
│   │   ├── nebulosa_boots.c
│   │   ├── nebulosa_bracers.c
│   │   ├── nebulosa_breast_plate.c
│   │   ├── nebulosa_cloak.c
│   │   ├── nebulosa_gloves.c
│   │   ├── nebulosa_helm.c
│   │   ├── nebulosa_leggings.c
│   │   ├── nessiteras_belt.c
│   │   ├── nessiteras_boots.c
│   │   ├── nessiteras_bracers.c
│   │   ├── nessiteras_breast_plate.c
│   │   ├── nessiteras_cloak.c
│   │   ├── nessiteras_gloves.c
│   │   ├── nessiteras_helm.c
│   │   ├── nessiteras_leggings.c
│   │   ├── poseidon_cloak.c
│   │   ├── thibetanus_belt.c
│   │   ├── thibetanus_boots.c
│   │   ├── thibetanus_bracers.c
│   │   ├── thibetanus_breast_plate.c
│   │   ├── thibetanus_cloak.c
│   │   ├── thibetanus_gloves.c
│   │   ├── thibetanus_helm.c
│   │   ├── thibetanus_leggings.c
│   │   ├── zen_ancient_belt.c
│   │   ├── zen_ancient_boots.c
│   │   ├── zen_ancient_bracers.c
│   │   ├── zen_ancient_breast_plate.c
│   │   ├── zen_ancient_cloak.c
│   │   ├── zen_ancient_gloves.c
│   │   ├── zen_ancient_helm.c
│   │   ├── zen_ancient_leggings.c
│   │   ├── zen_caerulea_belt.c
│   │   ├── zen_caerulea_boots.c
│   │   ├── zen_caerulea_bracers.c
│   │   ├── zen_caerulea_breast_plate.c
│   │   ├── zen_caerulea_cloak.c
│   │   ├── zen_caerulea_gloves.c
│   │   ├── zen_caerulea_helm.c
│   │   ├── zen_caerulea_leggings.c
│   │   ├── zen_gigas_belt.c
│   │   ├── zen_gigas_boots.c
│   │   ├── zen_gigas_bracers.c
│   │   ├── zen_gigas_breast_plate.c
│   │   ├── zen_gigas_cloak.c
│   │   ├── zen_gigas_gloves.c
│   │   ├── zen_gigas_helm.c
│   │   ├── zen_gigas_leggings.c
│   │   ├── zen_hydra_belt.c
│   │   ├── zen_hydra_boots.c
│   │   ├── zen_hydra_bracers.c
│   │   ├── zen_hydra_breast_plate.c
│   │   ├── zen_hydra_cloak.c
│   │   ├── zen_hydra_gloves.c
│   │   ├── zen_hydra_helm.c
│   │   ├── zen_hydra_leggings.c
│   │   ├── zen_mactans_belt.c
│   │   ├── zen_mactans_boots.c
│   │   ├── zen_mactans_bracers.c
│   │   ├── zen_mactans_breast_plate.c
│   │   ├── zen_mactans_cloak.c
│   │   ├── zen_mactans_gloves.c
│   │   ├── zen_mactans_helm.c
│   │   ├── zen_mactans_leggings.c
│   │   ├── zen_mydas_belt.c
│   │   ├── zen_mydas_boots.c
│   │   ├── zen_mydas_bracers.c
│   │   ├── zen_mydas_breast_plate.c
│   │   ├── zen_mydas_cloak.c
│   │   ├── zen_mydas_gloves.c
│   │   ├── zen_mydas_helm.c
│   │   ├── zen_mydas_leggings.c
│   │   ├── zen_nebulosa_belt.c
│   │   ├── zen_nebulosa_boots.c
│   │   ├── zen_nebulosa_bracers.c
│   │   ├── zen_nebulosa_breast_plate.c
│   │   ├── zen_nebulosa_cloak.c
│   │   ├── zen_nebulosa_gloves.c
│   │   ├── zen_nebulosa_helm.c
│   │   ├── zen_nebulosa_leggings.c
│   │   ├── zen_nessiteras_belt.c
│   │   ├── zen_nessiteras_boots.c
│   │   ├── zen_nessiteras_bracers.c
│   │   ├── zen_nessiteras_breast_plate.c
│   │   ├── zen_nessiteras_cloak.c
│   │   ├── zen_nessiteras_gloves.c
│   │   ├── zen_nessiteras_helm.c
│   │   ├── zen_nessiteras_leggings.c
│   │   ├── zen_poseidon_cloak.c
│   │   ├── zen_thibetanus_belt.c
│   │   ├── zen_thibetanus_boots.c
│   │   ├── zen_thibetanus_bracers.c
│   │   ├── zen_thibetanus_breast_plate.c
│   │   ├── zen_thibetanus_cloak.c
│   │   ├── zen_thibetanus_gloves.c
│   │   ├── zen_thibetanus_helm.c
│   │   └── zen_thibetanus_leggings.c
│   ├── baseball
│   │   └── champion_ring.c
│   ├── battlefield
│   │   ├── battle_insigne.c
│   │   ├── battlefield_fear_statue.c
│   │   └── battlefield_glory_statue.c
│   ├── board
│   │   ├── announcement.c
│   │   ├── bugs.c
│   │   ├── cityhall_center.c
│   │   ├── cityhall_darkless.c
│   │   ├── cityhall_dreamaker.c
│   │   ├── cityhall_fantasycity.c
│   │   ├── cityhall_formosa.c
│   │   ├── cityhall_robotworld.c
│   │   ├── cityhall_vicecity.c
│   │   ├── class_design.c
│   │   ├── client.c
│   │   ├── combat_design.c
│   │   ├── cookys.c
│   │   ├── enterprise_design.c
│   │   ├── government_design.c
│   │   ├── idea.c
│   │   ├── manufacture_design.c
│   │   ├── map_design.c
│   │   ├── master.c
│   │   ├── meeting.c
│   │   ├── mj_idea.c
│   │   ├── msr.c
│   │   ├── player.c
│   │   ├── progress.c
│   │   ├── quest_design.c
│   │   ├── story.c
│   │   ├── war_design.c
│   │   ├── wizard.c
│   │   ├── wizard_clode.c
│   │   ├── wizard_cookys.c
│   │   ├── wizard_msr.c
│   │   ├── wizard_tzj.c
│   │   └── wizhall.c
│   ├── book
│   │   ├── product
│   │   │   ├── _product_plan.c
│   │   │   └── gold.c
│   │   ├── skill
│   │   │   ├── _skill_book.c
│   │   │   ├── agility-adv.c
│   │   │   ├── architectonic-adv.c
│   │   │   ├── architectonic-large_a.c
│   │   │   ├── architectonic-large_b.c
│   │   │   ├── architectonic-large_c.c
│   │   │   ├── charisma-adv.c
│   │   │   ├── energy-adv.c
│   │   │   ├── fatalblow.c
│   │   │   ├── health-adv.c
│   │   │   ├── intelligence-adv.c
│   │   │   ├── leadership-adv.c
│   │   │   ├── leadership-advance_a.c
│   │   │   ├── leadership-advance_b.c
│   │   │   ├── leadership-advance_c.c
│   │   │   ├── physique-adv.c
│   │   │   ├── stamina-adv.c
│   │   │   └── strength-adv.c
│   │   └── virtual_product_book.c
│   ├── collection
│   │   └── tool
│   │       ├── coarse_bucket.c
│   │       ├── coarse_drill.c
│   │       ├── coarse_pickax.c
│   │       ├── coarse_saw.c
│   │       └── coarse_shovel.c
│   ├── equipment
│   │   ├── body.c
│   │   ├── head.c
│   │   ├── lfoot.c
│   │   ├── lhand.c
│   │   ├── rfoot.c
│   │   ├── rhand.c
│   │   └── xmas_ring.c
│   ├── etc
│   │   ├── board.c
│   │   ├── bug_red_envelope.c
│   │   ├── edit_shadow.c
│   │   ├── firework.c
│   │   ├── honor_card.c
│   │   ├── land_dominium.c
│   │   ├── lotto.c
│   │   ├── mine.c
│   │   ├── mine.old.c
│   │   ├── mine.rec.c
│   │   ├── mj
│   │   │   ├── mj.c
│   │   │   ├── mj.h
│   │   │   └── mjai.c
│   │   ├── money.c
│   │   ├── newspaper.c
│   │   ├── purewater.c
│   │   ├── snoop_shadow.c
│   │   ├── train.c
│   │   └── void.c
│   ├── farming
│   │   ├── barley.c
│   │   ├── barley_seed.c
│   │   ├── lily.c
│   │   ├── mung_bean.c
│   │   ├── product
│   │   │   ├── barley.c
│   │   │   ├── corn.c
│   │   │   ├── cotton.c
│   │   │   ├── herb.c
│   │   │   ├── herbage.c
│   │   │   ├── lily.c
│   │   │   ├── rice.c
│   │   │   ├── rose.c
│   │   │   ├── sunflower.c
│   │   │   └── wheat.c
│   │   ├── red_bean.c
│   │   ├── rice.c
│   │   ├── rice_seedling.c
│   │   ├── rose.c
│   │   ├── seed
│   │   │   ├── barley_seed.c
│   │   │   ├── barley_seedling.c
│   │   │   ├── corn_seed.c
│   │   │   ├── cotton_seed.c
│   │   │   ├── herb_seed.c
│   │   │   ├── herbage_seed.c
│   │   │   ├── lily_seed.c
│   │   │   ├── rice_seedling.c
│   │   │   ├── rose_seed.c
│   │   │   ├── sunflower_seed.c
│   │   │   └── wheat_seedling.c
│   │   ├── seed_of_lily.c
│   │   ├── seed_of_rose.c
│   │   ├── soy_bean.c
│   │   └── treasure
│   │       ├── beauty_hibiscus.c
│   │       ├── blood_orchid.c
│   │       ├── buddha_wood.c
│   │       ├── dashen.c
│   │       ├── liangyi.c
│   │       ├── night_lotus.c
│   │       ├── presl_pine.c
│   │       ├── rice_god.c
│   │       ├── sky.c
│   │       ├── stem_lotus.c
│   │       ├── thousandleaf_lotus.c
│   │       └── wo_grass.c
│   ├── festival
│   │   ├── 2005
│   │   │   └── christmas
│   │   │       └── xmas_ring.c
│   │   ├── 2006
│   │   │   ├── christmas
│   │   │   │   ├── sock.c
│   │   │   │   ├── xmas_ring.c
│   │   │   │   └── xmas_ring_hide.c
│   │   │   ├── mooncake
│   │   │   │   ├── cake_glass.c
│   │   │   │   ├── hairpin.c
│   │   │   │   ├── moon_cake.c
│   │   │   │   ├── rabbit_ear.c
│   │   │   │   ├── shaddock_hat.c
│   │   │   │   └── wukang_axe.c
│   │   │   └── newyear
│   │   │       ├── red_envelope_2006.c
│   │   │       └── red_envelope_2006_no_function.c
│   │   ├── 2007
│   │   │   ├── christmas
│   │   │   │   ├── child.c
│   │   │   │   ├── deer_card.c
│   │   │   │   ├── deer_sled.c
│   │   │   │   ├── gift_box.c
│   │   │   │   ├── santa_claus.c
│   │   │   │   ├── xmas_ring.c
│   │   │   │   └── xmas_ring_hide.c
│   │   │   └── mooncake
│   │   │       ├── armor
│   │   │       │   ├── moonrabbit_belt.c
│   │   │       │   ├── moonrabbit_boots.c
│   │   │       │   ├── moonrabbit_bracers.c
│   │   │       │   ├── moonrabbit_cloak.c
│   │   │       │   ├── moonrabbit_cloth.c
│   │   │       │   ├── moonrabbit_gloves.c
│   │   │       │   └── moonrabbit_helm.c
│   │   │       ├── hoi_bow.c
│   │   │       ├── lu_hua.c
│   │   │       ├── moon_cake.c
│   │   │       ├── moon_deer.c
│   │   │       ├── moonrabbit_card.c
│   │   │       ├── rabbit_stick.c
│   │   │       └── treasure_map.c
│   │   ├── 2008
│   │   │   └── christmas
│   │   │       ├── armor
│   │   │       │   ├── xmas_antlers.c
│   │   │       │   ├── xmas_belt.c
│   │   │       │   ├── xmas_boots.c
│   │   │       │   ├── xmas_bracers.c
│   │   │       │   ├── xmas_cloak.c
│   │   │       │   ├── xmas_cloth.c
│   │   │       │   ├── xmas_gloves.c
│   │   │       │   └── xmas_helm.c
│   │   │       └── xmas_ring.c
│   │   ├── 2009
│   │   │   ├── christmas
│   │   │   │   ├── armor
│   │   │   │   │   └── xmas_cloak.c
│   │   │   │   ├── top
│   │   │   │   │   └── xmas_antlers.c
│   │   │   │   └── xmas_ring.c
│   │   │   ├── forum_gift
│   │   │   │   └── cheer_cd.c
│   │   │   ├── midwinter
│   │   │   │   ├── peanut_glue_pudding.c
│   │   │   │   └── sesame_glue_pudding.c
│   │   │   └── mooncake
│   │   ├── 2012
│   │   │   └── christmas
│   │   │       ├── xmas_ring.c
│   │   │       └── xmas_ring_hide.c
│   │   ├── 2013
│   │   │   └── christmas
│   │   │       ├── xmas_ring.c
│   │   │       └── xmas_ring_hide.c
│   │   ├── 2014
│   │   │   └── christmas
│   │   │       ├── xmas_ring.c
│   │   │       └── xmas_ring_hide.c
│   │   ├── 2015
│   │   │   └── christmas
│   │   │       ├── xmas_ring.c
│   │   │       └── xmas_ring_hide.c
│   │   ├── 2016
│   │   │   └── christmas
│   │   │       ├── xmas_ring.c
│   │   │       └── xmas_ring_hide.c
│   │   ├── 2017
│   │   │   └── christmas
│   │   │       ├── box.c
│   │   │       └── gift
│   │   │           ├── ring_1.c
│   │   │           ├── ring_2.c
│   │   │           ├── ring_3.c
│   │   │           ├── ring_4.c
│   │   │           ├── ring_5.c
│   │   │           └── ring_hide.c
│   │   ├── 2018
│   │   │   └── christmas
│   │   │       ├── box.c
│   │   │       └── gift
│   │   │           ├── ring_1.c
│   │   │           ├── ring_2.c
│   │   │           ├── ring_3.c
│   │   │           └── ring_hide.c
│   │   ├── christmas
│   │   │   ├── sock.c
│   │   │   ├── xmas_ring.c
│   │   │   └── xmas_ring_hide.c
│   │   ├── mooncake
│   │   │   ├── cake_glass.c
│   │   │   ├── hairpin.c
│   │   │   ├── moon_cake.c
│   │   │   ├── rabbit_ear.c
│   │   │   ├── shaddock_hat.c
│   │   │   └── wukang_axe.c
│   │   ├── red_envelope_2006.c
│   │   └── red_envelope_2006_no_function.c
│   ├── festivalarmor
│   ├── festivaltop
│   ├── fishfarm
│   │   ├── breed
│   │   │   ├── clam.c
│   │   │   ├── young_crab.c
│   │   │   └── young_shrimp.c
│   │   ├── product
│   │   │   ├── big_crab.c
│   │   │   ├── big_shrimp.c
│   │   │   └── pearl.c
│   │   └── treasure
│   │       ├── bagua.c
│   │       ├── bamboo_stone.c
│   │       ├── integrity_stone.c
│   │       ├── intelligence_stone.c
│   │       ├── jintu.c
│   │       ├── king_stone.c
│   │       ├── maleh_gobi.c
│   │       ├── mars_stone.c
│   │       ├── namay.c
│   │       ├── null_stone.c
│   │       ├── purple_stone.c
│   │       ├── terra.c
│   │       └── zen_stone.c
│   ├── fishing
│   │   ├── adv_fish
│   │   │   ├── fish0.c
│   │   │   ├── fish1.c
│   │   │   ├── fish2.c
│   │   │   └── fish3.c
│   │   ├── bait
│   │   │   ├── bread_bug.c
│   │   │   ├── prawn.c
│   │   │   ├── tsou_chip.c
│   │   │   └── worm.c
│   │   ├── fish
│   │   │   ├── fish0.c
│   │   │   ├── fish1.c
│   │   │   ├── fish2.c
│   │   │   ├── fish3.c
│   │   │   ├── fish4.c
│   │   │   ├── fish5.c
│   │   │   ├── fish6.c
│   │   │   ├── fish7.c
│   │   │   └── fish8.c
│   │   ├── other
│   │   │   └── box.c
│   │   ├── rod
│   │   │   ├── bamboo_rod.c
│   │   │   ├── carbon_fiber_rod.c
│   │   │   ├── coarse_rod.c
│   │   │   ├── golden_rod.c
│   │   │   ├── microelectronic_rod.c
│   │   │   ├── plastic_rod.c
│   │   │   ├── standard_rod.c
│   │   │   ├── toy_rod.c
│   │   │   └── wood_rod.c
│   │   └── sp_fish
│   │       ├── fish0.c
│   │       ├── fish1.c
│   │       └── fish2.c
│   ├── gem
│   │   ├── _gem_object.c
│   │   ├── gem0.c
│   │   ├── gem1.c
│   │   ├── gem2.c
│   │   ├── gem3.c
│   │   ├── gem4.c
│   │   ├── gem5.c
│   │   └── gem6.c
│   ├── inlay
│   │   ├── ancient_blade_secret_art_1st.c
│   │   ├── ancient_blade_secret_art_2nd.c
│   │   ├── faith_halberd_secret_art_1st.c
│   │   ├── faith_halberd_secret_art_2nd.c
│   │   ├── hydra_stick_secret_art_1st.c
│   │   ├── hydra_stick_secret_art_2nd.c
│   │   ├── lord_sword_secret_art_1st.c
│   │   ├── lord_sword_secret_art_2nd.c
│   │   ├── nessiteras_fist_secret_art_1st.c
│   │   ├── nessiteras_fist_secret_art_2nd.c
│   │   ├── sky_1st_secret_art.c
│   │   ├── sky_2nd_secret_art.c
│   │   ├── sky_3rd_secret_art.c
│   │   ├── truth_axe_secret_art_1st.c
│   │   └── truth_axe_secret_art_2nd.c
│   ├── materials
│   │   ├── fuel.c
│   │   ├── gold.c
│   │   ├── iron.c
│   │   ├── metal.c
│   │   ├── stone.c
│   │   ├── water.c
│   │   └── wood.c
│   ├── npc
│   │   ├── area
│   │   │   ├── giant_elephant.c
│   │   │   ├── giant_lion.c
│   │   │   ├── giant_orang.c
│   │   │   ├── giant_python.c
│   │   │   ├── giant_scolopendrid.c
│   │   │   ├── giant_snake.c
│   │   │   ├── giant_spider.c
│   │   │   ├── little_rabbit.c
│   │   │   ├── wild_cat.c
│   │   │   ├── wild_dog.c
│   │   │   ├── wild_panther.c
│   │   │   ├── wild_pig.c
│   │   │   └── wild_tiger.c
│   │   ├── battlefield
│   │   │   ├── berserker.c
│   │   │   ├── fiend.c
│   │   │   ├── mars.c
│   │   │   └── minotaur.c
│   │   ├── boss
│   │   │   ├── caretta_gigas.c
│   │   │   ├── chelonia_mydas.c
│   │   │   ├── crime_knight.c
│   │   │   ├── latrodectus_mactans.c
│   │   │   ├── neofelis_nebulosa.c
│   │   │   ├── poseidon.c
│   │   │   ├── salvation_knight.c
│   │   │   ├── sky_1st_statue.c
│   │   │   ├── sky_2nd_statue.c
│   │   │   ├── sky_3rd_statue.c
│   │   │   ├── small_statue.c
│   │   │   ├── urocissa_caerulea.c
│   │   │   └── ursus_thibetanus_formosanus.c
│   │   ├── etc
│   │   │   ├── girl.c
│   │   │   ├── richman.c
│   │   │   └── user_simulator.c
│   │   ├── factory
│   │   │   ├── manufacture_worker.c
│   │   │   └── rd_engineer.c
│   │   ├── lostland
│   │   │   ├── blainville's_beaked_whale.c
│   │   │   ├── forgotten_ancient_knight.c
│   │   │   ├── forgotten_warrior.c
│   │   │   ├── hydra.c
│   │   │   └── nessiteras_rhombopteryx.c
│   │   └── shopping
│   │       ├── customer.c
│   │       ├── japangirlname
│   │       └── rich_customer.c
│   ├── pasture
│   │   ├── breed
│   │   │   ├── cattle_stud.c
│   │   │   ├── chicken_stud.c
│   │   │   ├── deer_stud.c
│   │   │   ├── horse_stud.c
│   │   │   ├── pig_stud.c
│   │   │   └── sheep_stud.c
│   │   ├── feed
│   │   │   ├── alfalfa_grass.c
│   │   │   ├── pangola_grass.c
│   │   │   └── timothy_grass.c
│   │   ├── product
│   │   │   ├── antler.c
│   │   │   ├── beef.c
│   │   │   ├── chicken.c
│   │   │   ├── egg.c
│   │   │   ├── horseflesh.c
│   │   │   ├── leather.c
│   │   │   ├── mutton.c
│   │   │   ├── pork.c
│   │   │   ├── raw_milk.c
│   │   │   ├── venison.c
│   │   │   └── wool.c
│   │   └── treasure
│   │       ├── fly_horse.c
│   │       ├── giant_eagle.c
│   │       ├── huge_lord.c
│   │       ├── justice.c
│   │       ├── kylin.c
│   │       ├── mammoth.c
│   │       ├── nine_tail_fox.c
│   │       ├── pack_animal.c
│   │       ├── people.c
│   │       ├── sheep.c
│   │       ├── sishiang.c
│   │       ├── true_fly_horse.c
│   │       ├── true_giant_eagle.c
│   │       ├── true_horse.c
│   │       ├── true_kylin.c
│   │       ├── true_mammoth.c
│   │       ├── true_pack_animal.c
│   │       ├── tsouni_horse.c
│   │       └── wuju.c
│   ├── room
│   │   ├── arearoom.c
│   │   ├── cityroom.c
│   │   ├── room.c
│   │   └── zoneroom.c
│   ├── rwparty
│   │   ├── _action_dignose.c
│   │   ├── gift_of_clode_1st.c
│   │   ├── gift_of_cookys_1st.c
│   │   ├── gift_of_dragonku_1st.c
│   │   ├── gift_of_kyo_1st.c
│   │   ├── gift_of_lak_1st.c
│   │   ├── gift_of_msr_1st.c
│   │   ├── gift_of_sinji_1st.c
│   │   ├── gift_of_tzj_1st.c
│   │   ├── gift_of_whatup_1st.c
│   │   ├── red_envelope_1st_rwparty.c
│   │   └── red_evelope_justingkidding.c
│   ├── systemproducts
│   │   ├── architectonic_high.c
│   │   ├── dark_plum.c
│   │   ├── flatbed_tricycle.c
│   │   ├── fortune_bag.c
│   │   ├── pill_of_agility.c
│   │   ├── pill_of_charisma.c
│   │   ├── pill_of_deadly_contract.c
│   │   ├── pill_of_extreme_silence.c
│   │   ├── pill_of_intelligence.c
│   │   ├── pill_of_invincibility.c
│   │   ├── pill_of_lost_agility.c
│   │   ├── pill_of_lost_charisma.c
│   │   ├── pill_of_lost_energy.c
│   │   ├── pill_of_lost_health.c
│   │   ├── pill_of_lost_intelligence.c
│   │   ├── pill_of_lost_physique.c
│   │   ├── pill_of_lost_stamina.c
│   │   ├── pill_of_lost_strength.c
│   │   ├── pill_of_physique.c
│   │   ├── pill_of_purification.c
│   │   ├── pill_of_strength.c
│   │   ├── pure_water.c
│   │   ├── souvenir.c
│   │   ├── steamed_roll.c
│   │   ├── taichi.c
│   │   └── train_ticket.c
│   ├── trash
│   │   ├── trash0.c
│   │   ├── trash1.c
│   │   ├── trash2.c
│   │   ├── trash3.c
│   │   ├── trash4.c
│   │   └── trash5.c
│   ├── weapon
│   │   ├── ancient_blade.c
│   │   ├── caerulea_fist.c
│   │   ├── chair.c
│   │   ├── coarse_blade.c
│   │   ├── coarse_fist.c
│   │   ├── coarse_stick.c
│   │   ├── faith_halberd.c
│   │   ├── gigas_fist.c
│   │   ├── hydra_stick.c
│   │   ├── landmine.c
│   │   ├── lord_sword.c
│   │   ├── mactans_fist.c
│   │   ├── mydas_fist.c
│   │   ├── nebulosa_fist.c
│   │   ├── nessiteras_fist.c
│   │   ├── nuclear_bomb.c
│   │   ├── schair.c
│   │   ├── thibetanus_fist.c
│   │   └── truth_axe.c
│   └── wizard
│       ├── arcangel.c
│       ├── enterprise.c
│       ├── golftdi.c
│       ├── sky_body.c
│       └── sky_sword.c
├── product
│   └── gold
├── quest
│   ├── abour
│   │   ├── npc_seller.c
│   │   └── room_1.c
│   ├── bohr
│   │   ├── bohr.c
│   │   ├── broken_rod.c
│   │   ├── fishing_note.c
│   │   └── john_barton.c
│   ├── city_walker
│   │   ├── city_walker.c
│   │   └── land_essence.c
│   ├── crystalcave
│   │   ├── npc_seller.c
│   │   └── room_1.c
│   ├── epic_fishfarm
│   │   └── heart_of_ocean.c
│   ├── godsland
│   │   ├── room_godsland_1.c
│   │   ├── room_godsland_2.c
│   │   ├── room_godsland_3.c
│   │   ├── room_godsland_4.c
│   │   ├── room_godsland_5.c
│   │   └── room_godsland_6.c
│   ├── hermit
│   │   ├── npc_seller.c
│   │   └── room_1.c
│   ├── lost_shard
│   │   ├── letter.c
│   │   ├── lost_shard_1.c
│   │   ├── lost_shard_2.c
│   │   ├── lost_shard_3.c
│   │   ├── lost_shard_4.c
│   │   ├── lost_shard_5.c
│   │   ├── lost_shard_6.c
│   │   ├── lostland_key.c
│   │   ├── lostland_key_2.c
│   │   ├── lostland_key_3.c
│   │   ├── mountain_taoist.c
│   │   └── room_lostland_1.c
│   ├── monster_fossil
│   │   ├── cat_fossil.c
│   │   ├── deep_sea_thorn.c
│   │   ├── dog_fossil.c
│   │   ├── eagle_feather.c
│   │   ├── elephant_fossil.c
│   │   ├── eye_claw.c
│   │   ├── gulen.c
│   │   ├── jade_emerald.c
│   │   ├── lava_meteor.c
│   │   ├── lion_fossil.c
│   │   ├── monster_gall.c
│   │   ├── orang_fossil.c
│   │   ├── panther_fossil.c
│   │   ├── pig_fossil.c
│   │   ├── python_fossil.c
│   │   ├── rabbit_fossil.c
│   │   ├── scolopendrid_fossil.c
│   │   ├── snake_fossil.c
│   │   ├── spider_fossil.c
│   │   └── tiger_fossil.c
│   ├── old_farmer
│   │   ├── fuck_robotking.c
│   │   ├── npc_old_farmer.c
│   │   ├── room_1.c
│   │   └── room_2.c
│   ├── suphia_and_benjamin
│   │   ├── npc
│   │   │   ├── ada.c
│   │   │   ├── alvin.c
│   │   │   ├── foreman.c
│   │   │   └── suphia.c
│   │   ├── npc_foreman.c
│   │   ├── npc_suphia.c
│   │   ├── room
│   │   │   ├── s_afterroom.c
│   │   │   ├── s_alvin_room.c
│   │   │   ├── s_frontroom.c
│   │   │   └── s_kitchen.c
│   │   ├── room_s_afterroom.c
│   │   ├── room_s_alvin_room.c
│   │   ├── room_s_frontroom.c
│   │   └── room_s_kitchen.c
│   └── yin_ling_yu
│       ├── grandam.c
│       ├── liang_fan_huan.c
│       ├── mu_qing_yi.c
│       ├── room_1.c
│       ├── room_2.c
│       ├── room_3.c
│       ├── room_4.c
│       ├── room_5.c
│       ├── room_6.c
│       ├── room_7.c
│       ├── room_8.c
│       ├── room_9.c
│       ├── tear_wild_ginseng.c
│       ├── wild_ginseng.c
│       ├── yi_ke_wen.c
│       └── yin_ling_yu.c
├── std
│   ├── inherit
│   │   ├── feature
│   │   │   ├── command
│   │   │   │   ├── _command.c
│   │   │   │   └── _personal_command.c
│   │   │   ├── living
│   │   │   │   ├── _attribution_liv.c
│   │   │   │   ├── _combat_liv.c
│   │   │   │   ├── _condition_liv.c
│   │   │   │   ├── _delay_liv.c
│   │   │   │   ├── _equipment_liv.c
│   │   │   │   ├── _group_liv.c
│   │   │   │   ├── _heart_beat_liv.c
│   │   │   │   ├── _inventory_liv.c
│   │   │   │   ├── _living_liv.c
│   │   │   │   ├── _money_liv.c
│   │   │   │   ├── _move_liv.c
│   │   │   │   ├── _quest_liv.c
│   │   │   │   ├── _skill_liv.c
│   │   │   │   ├── _stuck_liv.c
│   │   │   │   ├── npc
│   │   │   │   │   ├── _action_npc.c
│   │   │   │   │   ├── _behavior_message_npc.c
│   │   │   │   │   ├── _behavior_walking_npc.c
│   │   │   │   │   ├── _command_npc.c
│   │   │   │   │   ├── _heart_beat_npc.c
│   │   │   │   │   ├── _idname_npc.c
│   │   │   │   │   └── _message_npc.c
│   │   │   │   └── usr
│   │   │   │       ├── _action_usr.c
│   │   │   │       ├── _command_usr.c
│   │   │   │       ├── _edit_usr.c
│   │   │   │       ├── _heart_beat_usr.c
│   │   │   │       ├── _ident_usr.c
│   │   │   │       ├── _idname_usr.c
│   │   │   │       ├── _input_usr.c
│   │   │   │       ├── _message_usr.c
│   │   │   │       ├── _more_usr.c
│   │   │   │       ├── _restore_usr.c
│   │   │   │       ├── ppl
│   │   │   │       │   └── _command_ppl.c
│   │   │   │       └── wiz
│   │   │   │           └── _command_wiz.c
│   │   │   ├── module
│   │   │   │   ├── condition
│   │   │   │   │   └── _condition_mod.c
│   │   │   │   ├── npc
│   │   │   │   │   └── _npc_mod.c
│   │   │   │   ├── product
│   │   │   │   │   ├── _product_action_mod.c
│   │   │   │   │   └── _product_mod.c
│   │   │   │   ├── room
│   │   │   │   │   ├── _room_action_mod.c
│   │   │   │   │   └── _room_mod.c
│   │   │   │   └── skill
│   │   │   │       └── _skill_mod.c
│   │   │   ├── object
│   │   │   │   ├── _action.c
│   │   │   │   ├── _amount.c
│   │   │   │   ├── _bulletin_board.c
│   │   │   │   ├── _clean_up.c
│   │   │   │   ├── _database.c
│   │   │   │   ├── _drink.c
│   │   │   │   ├── _equipment.c
│   │   │   │   ├── _fly.c
│   │   │   │   ├── _food.c
│   │   │   │   ├── _idname.c
│   │   │   │   ├── _keep.c
│   │   │   │   ├── _move.c
│   │   │   │   ├── _save.c
│   │   │   │   ├── _unique.c
│   │   │   │   └── _weapon.c
│   │   │   └── room
│   │   │       ├── _inventory_room.c
│   │   │       ├── _remove_room.c
│   │   │       └── _reset_room.c
│   │   └── standard
│   │       ├── bulletin_board.c
│   │       ├── interactive.c
│   │       ├── interactive_ppl.c
│   │       ├── interactive_wiz.c
│   │       ├── standard_armor.c
│   │       ├── standard_bulletin_board.c
│   │       ├── standard_drink.c
│   │       ├── standard_food.c
│   │       ├── standard_heavyarmor.c
│   │       ├── standard_longrange.c
│   │       ├── standard_maproom.c
│   │       ├── standard_module_npc.c
│   │       ├── standard_module_product.c
│   │       ├── standard_module_room.c
│   │       ├── standard_monster.c
│   │       ├── standard_npc.c
│   │       ├── standard_object.c
│   │       ├── standard_person.c
│   │       ├── standard_room.c
│   │       ├── standard_shortrange.c
│   │       └── standard_skill.c
│   └── module
│       ├── combat
│       ├── condition
│       │   ├── air_conditioner.c
│       │   ├── attackdefend.c
│       │   ├── audio.c
│       │   ├── battlefield_experience.c
│       │   ├── battlefield_fear.c
│       │   ├── battlefield_glory.c
│       │   ├── battlefield_murderer.c
│       │   ├── battlefield_player_1.c
│       │   ├── boss_hunter.c
│       │   ├── cheer_coffee.c
│       │   ├── christmas_2007.c
│       │   ├── city_stamina.c
│       │   ├── deadly_contract.c
│       │   ├── die.c
│       │   ├── dignose.c
│       │   ├── extreme_silence.c
│       │   ├── financeknowledge.c
│       │   ├── fishingnote.c
│       │   ├── flute_weak.c
│       │   ├── gibaman.c
│       │   ├── gods_transformer.c
│       │   ├── guitar_weak.c
│       │   ├── honor.c
│       │   ├── invincibility.c
│       │   ├── knowledgebless.c
│       │   ├── knowledgesanctuary.c
│       │   ├── life_miracle.c
│       │   ├── lost_agility.c
│       │   ├── lost_charisma.c
│       │   ├── lost_energy.c
│       │   ├── lost_health.c
│       │   ├── lost_intelligence.c
│       │   ├── lost_physique.c
│       │   ├── lost_stamina.c
│       │   ├── lost_strength.c
│       │   ├── lostland.c
│       │   ├── lostseal.c
│       │   ├── magnetic_stone.c
│       │   ├── medicated_wine.c
│       │   ├── milk.c
│       │   ├── mooncake_2007.c
│       │   ├── newbie.c
│       │   ├── newbie_adv.c
│       │   ├── peanut_glue_pudding.c
│       │   ├── perfume_herb.c
│       │   ├── perfume_herbage.c
│       │   ├── perfume_lily.c
│       │   ├── perfume_rose.c
│       │   ├── perfume_sunflower.c
│       │   ├── poison_giant_spider.c
│       │   ├── power_agility.c
│       │   ├── power_charisma.c
│       │   ├── power_intelligence.c
│       │   ├── power_physique.c
│       │   ├── power_strength.c
│       │   ├── radiation.c
│       │   ├── seehumans.c
│       │   ├── selfcharge.c
│       │   ├── sesame_glue_pudding.c
│       │   ├── sightseebless.c
│       │   ├── sightseebless_adv1.c
│       │   ├── sightseebless_adv2.c
│       │   ├── sightseebless_adv3.c
│       │   ├── sightseebless_adv4.c
│       │   ├── sightseebless_adv5.c
│       │   ├── sightseebless_adv6.c
│       │   ├── sightseebless_adv7.c
│       │   ├── sky_1st_statue.c
│       │   ├── sky_2nd_statue.c
│       │   ├── sky_3rd_statue.c
│       │   ├── steamed_roll_heal.c
│       │   ├── television.c
│       │   └── whiskey.c
│       ├── object
│       │   ├── furniture
│       │   │   ├── chair.c
│       │   │   ├── chest.c
│       │   │   └── stool.c
│       │   ├── tools
│       │   │   ├── ring.c
│       │   │   └── rod.c
│       │   └── vehicle
│       │       └── car.c
│       ├── product
│       │   ├── air_conditioner_module.c
│       │   ├── air_conditioner_object.c
│       │   ├── antidote_module.c
│       │   ├── antidote_object.c
│       │   ├── antimissile_module.c
│       │   ├── antimissile_object.c
│       │   ├── audio_module.c
│       │   ├── audio_object.c
│       │   ├── axe_module.c
│       │   ├── axe_object.c
│       │   ├── bandage_module.c
│       │   ├── bandage_object.c
│       │   ├── barrel_module.c
│       │   ├── barrel_object.c
│       │   ├── bat_module.c
│       │   ├── bat_object.c
│       │   ├── bbq_corn_module.c
│       │   ├── bbq_corn_object.c
│       │   ├── bed_module.c
│       │   ├── bed_object.c
│       │   ├── beer_module.c
│       │   ├── beer_object.c
│       │   ├── beer_shrimp_module.c
│       │   ├── beer_shrimp_object.c
│       │   ├── belt_module.c
│       │   ├── belt_object.c
│       │   ├── blade_module.c
│       │   ├── blade_object.c
│       │   ├── board_module.c
│       │   ├── board_object.c
│       │   ├── boat_body_module.c
│       │   ├── boat_body_object.c
│       │   ├── boots_module.c
│       │   ├── boots_object.c
│       │   ├── bracers_module.c
│       │   ├── bracers_object.c
│       │   ├── breast_plate_module.c
│       │   ├── breast_plate_object.c
│       │   ├── bucket_module.c
│       │   ├── bucket_object.c
│       │   ├── car_body_module.c
│       │   ├── car_body_object.c
│       │   ├── car_module.c
│       │   ├── car_object.c
│       │   ├── chicken_rice_ball_module.c
│       │   ├── chicken_rice_ball_object.c
│       │   ├── cloak_module.c
│       │   ├── cloak_object.c
│       │   ├── compact_disk_module.c
│       │   ├── compact_disk_object.c
│       │   ├── copper_module.c
│       │   ├── copper_object.c
│       │   ├── distilled_water_module.c
│       │   ├── distilled_water_object.c
│       │   ├── drill_module.c
│       │   ├── drill_object.c
│       │   ├── engine_module.c
│       │   ├── engine_object.c
│       │   ├── fishing_rod_module.c
│       │   ├── fishing_rod_object.c
│       │   ├── fist_module.c
│       │   ├── fist_object.c
│       │   ├── flashlight_module.c
│       │   ├── flashlight_object.c
│       │   ├── flute_module.c
│       │   ├── flute_object.c
│       │   ├── gem_jewel_module.c
│       │   ├── gem_jewel_object.c
│       │   ├── gem_module.c
│       │   ├── gem_object.c
│       │   ├── glass_module.c
│       │   ├── glass_object.c
│       │   ├── glider_module.c
│       │   ├── glider_object.c
│       │   ├── gloves_module.c
│       │   ├── gloves_object.c
│       │   ├── gold
│       │   │   ├── module.c
│       │   │   └── object.c
│       │   ├── guitar_module.c
│       │   ├── guitar_object.c
│       │   ├── halberd_module.c
│       │   ├── halberd_object.c
│       │   ├── helm_module.c
│       │   ├── helm_object.c
│       │   ├── herb_perfume_module.c
│       │   ├── herb_perfume_object.c
│       │   ├── herbage_perfume_module.c
│       │   ├── herbage_perfume_object.c
│       │   ├── high_timber_module.c
│       │   ├── high_timber_object.c
│       │   ├── integrated_circuit_module.c
│       │   ├── integrated_circuit_object.c
│       │   ├── landmine_module.c
│       │   ├── landmine_object.c
│       │   ├── leggings_module.c
│       │   ├── leggings_object.c
│       │   ├── lily_perfume_module.c
│       │   ├── lily_perfume_object.c
│       │   ├── liquid_fuel_module.c
│       │   ├── liquid_fuel_object.c
│       │   ├── man_made_fibers_module.c
│       │   ├── man_made_fibers_object.c
│       │   ├── mechanical_structure_module.c
│       │   ├── mechanical_structure_object.c
│       │   ├── medicated_wine_module.c
│       │   ├── medicated_wine_object.c
│       │   ├── milk_module.c
│       │   ├── milk_object.c
│       │   ├── missile_module.c
│       │   ├── missile_object.c
│       │   ├── mitt_module.c
│       │   ├── mitt_object.c
│       │   ├── mold_module.c
│       │   ├── mold_object.c
│       │   ├── natural_gas_module.c
│       │   ├── natural_gas_object.c
│       │   ├── normal_timber_module.c
│       │   ├── normal_timber_object.c
│       │   ├── nuclear_bomb_module.c
│       │   ├── nuclear_bomb_object.c
│       │   ├── pearl_jewel_module.c
│       │   ├── pearl_jewel_object.c
│       │   ├── petroleum_module.c
│       │   ├── petroleum_object.c
│       │   ├── pickax_module.c
│       │   ├── pickax_object.c
│       │   ├── plane_body_module.c
│       │   ├── plane_body_object.c
│       │   ├── plastic_module.c
│       │   ├── plastic_object.c
│       │   ├── precious_metal_module.c
│       │   ├── precious_metal_object.c
│       │   ├── refined_timber_module.c
│       │   ├── refined_timber_object.c
│       │   ├── rocket_module.c
│       │   ├── rocket_object.c
│       │   ├── rose_perfume_module.c
│       │   ├── rose_perfume_object.c
│       │   ├── rowboat_module.c
│       │   ├── rowboat_object.c
│       │   ├── rubber_module.c
│       │   ├── rubber_object.c
│       │   ├── saw_module.c
│       │   ├── saw_object.c
│       │   ├── shovel_module.c
│       │   ├── shovel_object.c
│       │   ├── silicon_module.c
│       │   ├── silicon_object.c
│       │   ├── steamed_crab_module.c
│       │   ├── steamed_crab_object.c
│       │   ├── steamed_roll_module.c
│       │   ├── steamed_roll_object.c
│       │   ├── steel_module.c
│       │   ├── steel_object.c
│       │   ├── stick_module.c
│       │   ├── stick_object.c
│       │   ├── sulfur_module.c
│       │   ├── sulfur_object.c
│       │   ├── sunflower_perfume_module.c
│       │   ├── sunflower_perfume_object.c
│       │   ├── sword_module.c
│       │   ├── sword_object.c
│       │   ├── tap_water_module.c
│       │   ├── tap_water_object.c
│       │   ├── television_module.c
│       │   ├── television_object.c
│       │   ├── tent_module.c
│       │   ├── tent_object.c
│       │   ├── tire_module.c
│       │   ├── tire_object.c
│       │   ├── truck_module.c
│       │   ├── truck_object.c
│       │   ├── whiskey_module.c
│       │   ├── whiskey_object.c
│       │   ├── wire_module.c
│       │   └── wire_object.c
│       ├── room
│       │   ├── adventure.c
│       │   ├── aircraft.c
│       │   ├── airport.c
│       │   ├── armor.c
│       │   ├── auction.c
│       │   ├── bank.c
│       │   ├── baseball.c
│       │   ├── chemical.c
│       │   ├── church.c
│       │   ├── cityhall.c
│       │   ├── clothing.c
│       │   ├── defendtower.c
│       │   ├── drink.c
│       │   ├── electrics.c
│       │   ├── enterprise.c
│       │   ├── entertainment.c
│       │   ├── expedition.c
│       │   ├── food.c
│       │   ├── fuel.c
│       │   ├── furniture.c
│       │   ├── futures.c
│       │   ├── hardware.c
│       │   ├── heavyarmor.c
│       │   ├── hospital.c
│       │   ├── house.c
│       │   ├── inherit
│       │   │   ├── _action_auction.c
│       │   │   ├── _action_buy.c
│       │   │   ├── _action_export.c
│       │   │   ├── _action_gain.c
│       │   │   ├── _action_import.c
│       │   │   ├── _action_list.c
│       │   │   ├── _action_sell.c
│       │   │   ├── _action_setup.c
│       │   │   ├── _action_transport.c
│       │   │   ├── _heart_beat_store.c
│       │   │   ├── _mod_factory.c
│       │   │   └── _object_database_mod.c
│       │   ├── instrument.c
│       │   ├── jail.c
│       │   ├── labor.c
│       │   ├── longrange.c
│       │   ├── machinery.c
│       │   ├── magic.c
│       │   ├── medicine.c
│       │   ├── metal.c
│       │   ├── nuclear.c
│       │   ├── perfume.c
│       │   ├── postoffice.c
│       │   ├── prodigy.c
│       │   ├── recycle.c
│       │   ├── research.c
│       │   ├── resource.c
│       │   ├── shortrange.c
│       │   ├── space.c
│       │   ├── stock.c
│       │   ├── stone.c
│       │   ├── store.c
│       │   ├── trading_post.c
│       │   ├── trainstation.c
│       │   ├── transportation.c
│       │   ├── university.c
│       │   ├── warehouse.c
│       │   ├── water.c
│       │   └── wood.c
│       └── skill
│           ├── adventure-fac.c
│           ├── agility-adv.c
│           ├── agility.c
│           ├── aircraft-fac.c
│           ├── architectonic-adv.c
│           ├── architectonic-high.c
│           ├── architectonic.c
│           ├── armor-fac.c
│           ├── attack-stance-adv.c
│           ├── attack-stance.c
│           ├── axe.c
│           ├── blade.c
│           ├── charisma-adv.c
│           ├── charisma.c
│           ├── chemical-fac.c
│           ├── clothing-fac.c
│           ├── consciousness.c
│           ├── curveball.c
│           ├── defend-stance-adv.c
│           ├── defend-stance.c
│           ├── dodge.c
│           ├── drink-fac.c
│           ├── drink.c
│           ├── electrics-fac.c
│           ├── eloquence.c
│           ├── energy-adv.c
│           ├── energy.c
│           ├── entertainment-fac.c
│           ├── estaterebate.c
│           ├── exchange.c
│           ├── factorymanage.c
│           ├── farming.c
│           ├── fatalblow.c
│           ├── feature.c
│           ├── fishfarm.c
│           ├── fishing.c
│           ├── fldaccuracy.c
│           ├── fldrange.c
│           ├── flute.c
│           ├── food-fac.c
│           ├── food.c
│           ├── forkball.c
│           ├── fourseam.c
│           ├── fuel.c
│           ├── fuelclassify.c
│           ├── furniture-fac.c
│           ├── guitar.c
│           ├── halberd.c
│           ├── hardware-fac.c
│           ├── health-adv.c
│           ├── health.c
│           ├── heavyarmor-fac.c
│           ├── hitaccuracy.c
│           ├── hitpower.c
│           ├── hitrange.c
│           ├── idle.c
│           ├── instrument-fac.c
│           ├── intelligence-adv.c
│           ├── intelligence.c
│           ├── leadership-adv.c
│           ├── leadership.c
│           ├── lefthand.c
│           ├── longrange-fac.c
│           ├── machinery-fac.c
│           ├── magic-fac.c
│           ├── medic-stance-adv.c
│           ├── medic-stance.c
│           ├── medicine-fac.c
│           ├── metal.c
│           ├── metalclassify.c
│           ├── pasture.c
│           ├── perfume-fac.c
│           ├── physique-adv.c
│           ├── physique.c
│           ├── price.c
│           ├── researchmanage.c
│           ├── righthand.c
│           ├── security.c
│           ├── shortrange-fac.c
│           ├── sinker.c
│           ├── slider.c
│           ├── speed-stance-adv.c
│           ├── speed-stance.c
│           ├── sport.c
│           ├── staff.c
│           ├── stamina-adv.c
│           ├── stamina.c
│           ├── stick.c
│           ├── stock.c
│           ├── stone.c
│           ├── stoneclassify.c
│           ├── storemanage.c
│           ├── strength-adv.c
│           ├── strength.c
│           ├── sword.c
│           ├── taichi.c
│           ├── teach.c
│           ├── technology.c
│           ├── transportation-fac.c
│           ├── twohands.c
│           ├── twoseam.c
│           ├── unarmed.c
│           ├── water.c
│           ├── waterclassify.c
│           ├── wood.c
│           └── woodclassify.c
├── system
│   ├── daemons
│   │   ├── adventure_d.c
│   │   ├── algorithm_path_d.c
│   │   ├── area_d.c
│   │   ├── area_d_main.c
│   │   ├── areadata_d.c
│   │   ├── auction_d.c
│   │   ├── baseball_d.c
│   │   ├── battlefield_d.c
│   │   ├── birthday_d.c
│   │   ├── boss_d.c
│   │   ├── bug_d.c
│   │   ├── building_d.c
│   │   ├── captcha_d.c
│   │   ├── channel_d.c
│   │   ├── char_d.c
│   │   ├── chinese_d.c
│   │   ├── city_d.c
│   │   ├── city_d_main.c
│   │   ├── citydata_d.c
│   │   ├── clean_d.c
│   │   ├── combat_d.c
│   │   ├── combat_d_main.c
│   │   ├── command_d.c
│   │   ├── countdown_d.c
│   │   ├── description_d.c
│   │   ├── dict_d.c
│   │   ├── distributed_computing_d.c
│   │   ├── emote_d.c
│   │   ├── enterprise_d.c
│   │   ├── estate_d.c
│   │   ├── etc
│   │   │   ├── logind_motd
│   │   │   ├── logind_welcome_msg0
│   │   │   ├── logind_welcome_msg1
│   │   │   ├── logind_welcome_msg2
│   │   │   ├── logind_welcome_msg3
│   │   │   ├── logind_welcome_msg4
│   │   │   ├── logind_welcome_msg5
│   │   │   ├── questions
│   │   │   ├── weather_autumn
│   │   │   ├── weather_spring
│   │   │   ├── weather_summer
│   │   │   └── weather_winter
│   │   ├── exchange_d.c
│   │   ├── finger_d.c
│   │   ├── ftp_d.c
│   │   ├── futures_d.c
│   │   ├── graph_d.c
│   │   ├── growth_d.c
│   │   ├── history_d.c
│   │   ├── html_d.c
│   │   ├── http_client_d.c
│   │   ├── im_d.c
│   │   ├── intermud2_d.c
│   │   ├── ip_d.c
│   │   ├── labor_d.c
│   │   ├── language_d.c
│   │   ├── login_d.c
│   │   ├── logout_d.c
│   │   ├── mail_d.c
│   │   ├── map_d.c
│   │   ├── mapnpc
│   │   │   ├── _mapnpc.c
│   │   │   ├── all_area.c
│   │   │   ├── area_atlantis_0.c
│   │   │   ├── area_atlantis_1.c
│   │   │   ├── area_atlantis_10.c
│   │   │   ├── area_atlantis_2.c
│   │   │   ├── area_atlantis_3.c
│   │   │   ├── area_atlantis_4.c
│   │   │   ├── area_atlantis_5.c
│   │   │   ├── area_atlantis_6.c
│   │   │   ├── area_atlantis_7.c
│   │   │   ├── area_atlantis_8.c
│   │   │   ├── area_atlantis_9.c
│   │   │   ├── area_blueflowers_0.c
│   │   │   ├── area_blueflowers_1.c
│   │   │   ├── area_blueflowers_10.c
│   │   │   ├── area_blueflowers_2.c
│   │   │   ├── area_blueflowers_3.c
│   │   │   ├── area_blueflowers_4.c
│   │   │   ├── area_blueflowers_5.c
│   │   │   ├── area_blueflowers_6.c
│   │   │   ├── area_blueflowers_7.c
│   │   │   ├── area_blueflowers_8.c
│   │   │   ├── area_blueflowers_9.c
│   │   │   ├── area_cnossus_0.c
│   │   │   ├── area_cnossus_1.c
│   │   │   ├── area_cnossus_10.c
│   │   │   ├── area_cnossus_2.c
│   │   │   ├── area_cnossus_3.c
│   │   │   ├── area_cnossus_4.c
│   │   │   ├── area_cnossus_5.c
│   │   │   ├── area_cnossus_6.c
│   │   │   ├── area_cnossus_7.c
│   │   │   ├── area_cnossus_8.c
│   │   │   ├── area_cnossus_9.c
│   │   │   ├── area_godsland_0.c
│   │   │   ├── area_heritage_0.c
│   │   │   ├── area_heritage_1.c
│   │   │   ├── area_heritage_2.c
│   │   │   ├── area_heritage_3.c
│   │   │   ├── area_heritage_4.c
│   │   │   ├── area_heritage_5.c
│   │   │   ├── area_heritage_6.c
│   │   │   ├── area_heritage_7.c
│   │   │   ├── area_heritage_8.c
│   │   │   ├── area_lightforest_0.c
│   │   │   ├── area_lightforest_1.c
│   │   │   ├── area_lightforest_2.c
│   │   │   ├── area_lightforest_3.c
│   │   │   ├── area_lightforest_4.c
│   │   │   ├── area_lightforest_5.c
│   │   │   ├── area_lightforest_6.c
│   │   │   ├── area_lightforest_7.c
│   │   │   ├── area_lightforest_8.c
│   │   │   ├── area_lightforest_9.c
│   │   │   ├── area_lostland_0.c
│   │   │   ├── area_lostland_1.c
│   │   │   ├── area_lostland_2.c
│   │   │   ├── area_memoryland_0.c
│   │   │   ├── area_memoryland_1.c
│   │   │   ├── area_memoryland_2.c
│   │   │   ├── area_memoryland_3.c
│   │   │   ├── area_memoryland_4.c
│   │   │   ├── area_memoryland_5.c
│   │   │   ├── area_memoryland_6.c
│   │   │   ├── area_memoryland_7.c
│   │   │   ├── area_memoryland_8.c
│   │   │   ├── area_memoryland_9.c
│   │   │   ├── area_sixcastle_0.c
│   │   │   ├── area_sixcastle_1.c
│   │   │   ├── area_sixcastle_2.c
│   │   │   ├── area_sixcastle_3.c
│   │   │   ├── area_sixcastle_4.c
│   │   │   └── area_sixcastle_5.c
│   │   ├── mapnpc_d.c
│   │   ├── mapob_d.c
│   │   ├── material_d.c
│   │   ├── mnlmudlist_d.c
│   │   ├── money_d.c
│   │   ├── mrtg_d.c
│   │   ├── mud_d.c
│   │   ├── mysql_d.c
│   │   ├── mysql_schedule_d.c
│   │   ├── nature_d.c
│   │   ├── network_d.c
│   │   ├── news_d.c
│   │   ├── number_d.c
│   │   ├── password_d.c
│   │   ├── ppl_login_d.c
│   │   ├── product_d.c
│   │   ├── quantity_d.c
│   │   ├── quest_d.c
│   │   ├── question_d.c
│   │   ├── realnews_d.c
│   │   ├── rwlibcomm_d.c
│   │   ├── salary_d.c
│   │   ├── schedule_d.c
│   │   ├── secure_d.c
│   │   ├── serial_number_d.c
│   │   ├── shopping_d.c
│   │   ├── skill_d.c
│   │   ├── smtp_d.c
│   │   ├── socket_d.c
│   │   ├── socket_ob.c
│   │   ├── stance_d.c
│   │   ├── stock_d.c
│   │   ├── sysdatabase_d.c
│   │   ├── system_d.c
│   │   ├── tax_d.c
│   │   ├── terrain_d.c
│   │   ├── time_d.c
│   │   ├── top_d.c
│   │   ├── translate_d.c
│   │   ├── treasure_d.c
│   │   ├── unique_d.c
│   │   ├── virtual_d.c
│   │   ├── whois_d.c
│   │   ├── wiz_login_d.c
│   │   └── zone_d.c
│   ├── kernel
│   │   ├── creator.c
│   │   ├── data
│   │   │   ├── password.o
│   │   │   ├── password.o_backup
│   │   │   ├── secure.o
│   │   │   └── secure.o_bak
│   │   ├── etc
│   │   │   └── preload
│   │   ├── master
│   │   │   ├── compile_object.c
│   │   │   ├── crash.c
│   │   │   ├── ed.c
│   │   │   ├── error.c
│   │   │   ├── file.c
│   │   │   ├── flag.c
│   │   │   ├── object.c
│   │   │   ├── parse.c
│   │   │   ├── preload.c
│   │   │   ├── valid_asm.c
│   │   │   ├── valid_author.c
│   │   │   ├── valid_bind.c
│   │   │   ├── valid_compile_to_c.c
│   │   │   ├── valid_database.c
│   │   │   ├── valid_domain.c
│   │   │   ├── valid_hide.c
│   │   │   ├── valid_link.c
│   │   │   ├── valid_object.c
│   │   │   ├── valid_override.c
│   │   │   ├── valid_readwrite.c
│   │   │   ├── valid_save_binary.c
│   │   │   ├── valid_shadow.c
│   │   │   └── valid_socket.c
│   │   ├── master.c
│   │   ├── secure.o
│   │   ├── simul_efun
│   │   │   ├── ansi.c
│   │   │   ├── area.c
│   │   │   ├── array.c
│   │   │   ├── bit.c
│   │   │   ├── boolean.c
│   │   │   ├── city.c
│   │   │   ├── file.c
│   │   │   ├── int.c
│   │   │   ├── location.c
│   │   │   ├── log.c
│   │   │   ├── math.c
│   │   │   ├── message.c
│   │   │   ├── object.c
│   │   │   ├── parse.c
│   │   │   ├── path.c
│   │   │   ├── person.c
│   │   │   ├── process.c
│   │   │   ├── secure.c
│   │   │   ├── socket.c
│   │   │   ├── string.c
│   │   │   ├── title_screen.c
│   │   │   └── zone.c
│   │   └── simul_efun.c
│   ├── object
│   │   ├── client_ob.c
│   │   ├── login_ob.c
│   │   ├── ppl_ob.c
│   │   └── wiz_ob.c
│   └── tmp
│       └── swapfile
├── tree.txt
├── wiz
│   ├── home
│   │   └── clode
│   └── wizhall
│       ├── npc_aristotle.c
│       ├── npc_einstein.c
│       ├── npc_fermat.c
│       ├── npc_mundell.c
│       ├── room_wizhall_1.c
│       ├── room_wizhall_10.c
│       ├── room_wizhall_11.c
│       ├── room_wizhall_12.c
│       ├── room_wizhall_13.c
│       ├── room_wizhall_14.c
│       ├── room_wizhall_15.c
│       ├── room_wizhall_16.c
│       ├── room_wizhall_17.c
│       ├── room_wizhall_2.c
│       ├── room_wizhall_3.c
│       ├── room_wizhall_4.c
│       ├── room_wizhall_5.c
│       ├── room_wizhall_6.c
│       ├── room_wizhall_7.c
│       ├── room_wizhall_8.c
│       └── room_wizhall_9.c
├── www
│   ├── bugrss
│   │   ├── LpcData.php
│   │   ├── make_rss_feed.php
│   │   ├── make_rss_feed.phps
│   │   └── rss.php
│   ├── map
│   ├── map.html
│   ├── mrtg
│   │   ├── default
│   │   │   ├── 0mudos_cpumem
│   │   │   │   ├── 0mudos_cpumem-day.png
│   │   │   │   ├── 0mudos_cpumem-month.png
│   │   │   │   ├── 0mudos_cpumem-week.png
│   │   │   │   ├── 0mudos_cpumem-year.png
│   │   │   │   ├── 0mudos_cpumem.html
│   │   │   │   ├── 0mudos_cpumem.log
│   │   │   │   └── 0mudos_cpumem.old
│   │   │   ├── 0mudos_mem
│   │   │   │   ├── 0mudos_mem-day.png
│   │   │   │   ├── 0mudos_mem-month.png
│   │   │   │   ├── 0mudos_mem-week.png
│   │   │   │   ├── 0mudos_mem-year.png
│   │   │   │   ├── 0mudos_mem.html
│   │   │   │   ├── 0mudos_mem.log
│   │   │   │   └── 0mudos_mem.old
│   │   │   ├── 0system_cpu
│   │   │   │   ├── 0system_cpu-day.png
│   │   │   │   ├── 0system_cpu-month.png
│   │   │   │   ├── 0system_cpu-week.png
│   │   │   │   ├── 0system_cpu-year.png
│   │   │   │   ├── 0system_cpu.html
│   │   │   │   ├── 0system_cpu.log
│   │   │   │   └── 0system_cpu.old
│   │   │   ├── 0system_mem
│   │   │   │   ├── 0system_mem-day.png
│   │   │   │   ├── 0system_mem-month.png
│   │   │   │   ├── 0system_mem-week.png
│   │   │   │   ├── 0system_mem-year.png
│   │   │   │   ├── 0system_mem.html
│   │   │   │   ├── 0system_mem.log
│   │   │   │   └── 0system_mem.old
│   │   │   └── 0system_netflow
│   │   │       ├── 0system_netflow-day.png
│   │   │       ├── 0system_netflow-month.png
│   │   │       ├── 0system_netflow-week.png
│   │   │       ├── 0system_netflow-year.png
│   │   │       ├── 0system_netflow.html
│   │   │       ├── 0system_netflow.log
│   │   │       └── 0system_netflow.old
│   │   ├── icons
│   │   │   ├── Thumbs.db
│   │   │   ├── favicon.ico
│   │   │   ├── mrtg-l.gif
│   │   │   ├── mrtg-l.png
│   │   │   ├── mrtg-m.gif
│   │   │   ├── mrtg-m.png
│   │   │   ├── mrtg-r.gif
│   │   │   ├── mrtg-r.png
│   │   │   ├── mrtg-ti.gif
│   │   │   └── mrtg-ti.png
│   │   ├── mnlmudlist
│   │   └── stock
│   ├── rwmap.php
│   ├── rwmapappletviewer
│   │   ├── ColorMapLabel.class
│   │   ├── ColorMapLabel_this_mouseAdapter.class
│   │   ├── ColorMapLabel_this_mouseMotionAdapter.class
│   │   ├── ColorMapLabel_this_mouseWheelAdapter.class
│   │   └── MapApplet.class
│   └── stock.html
└── zone
    ├── generator
    │   └── test.c
    └── quest

301 directories, 2991 files
```
