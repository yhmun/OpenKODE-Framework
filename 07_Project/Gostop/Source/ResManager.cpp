/*
 *
 *      File            ResManager.cpp
 *      Description     Resource Manager
 *      Version         0.95.1007
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (C) 2011 XMSoft.
 *      Copyright (C) 2011 Blue River Ltd.
 *      Copyright (C) 2011 Blueplay Ltd. All rights reserved.
 * 
 *      Contact Email: xmsoft77@gmail.com 
 *                     chris@blueplay.co.kr
 *                     	
 */ 

#include "Precompiled.h"

#if !defined ( SHP ) && defined ( _WIN32 )
#include "UTF8Bytes.h"
#else
#include "UTF8Texts.h"
#endif

CResManager*   g_pResManager = KD_NULL;

static const KDchar*    l_szPath [ ] = 
{	
	"",																					// ePath_Null

	"font/NanumGothicBold.ttf",															// eFNT_Gothic_Bold
    
    "sound/bgm/bgm_title.mp3",															// eSND_BGM_Main						
	"sound/bgm/bgm_select_stage.mp3",													// eSND_BGM_Stage	
	"sound/bgm/bgm_start_play.mp3",														// eSND_BGM_Play
	"sound/bgm/bgm_vs.mp3",																// eSND_BGM_Versus						
	"sound/bgm/bgm_cartoon.mp3",														// eSND_BGM_Cartoon					
	"sound/bgm/bgm_win.mp3",															// eSND_BGM_Win						
	"sound/bgm/bgm_big_win.mp3",														// eSND_BGM_Win2						
	"sound/bgm/bgm_lose.mp3",															// eSND_BGM_Lose						
	"sound/bgm/bgm_big_lose.mp3",														// eSND_BGM_Lose2						
	"sound/bgm/bgm_ending.mp3",															// eSND_BGM_Ending						
	"sound/bgm/bgm_ladder.mp3",															// eSND_BGM_Ladder						
    
    "sound/effect/eff_take_card.wav",													// eSND_Effect_Shuffle1
	"sound/effect/eff_take_card1.wav",													// eSND_Effect_Shuffle2	
	"sound/effect/eff_take_card2.wav",													// eSND_Effect_Shuffle3
	"sound/effect/eff_attach_1.wav",													// eSND_Effect_Attatch1			
	"sound/effect/eff_attach_2.wav",													// eSND_Effect_Attatch2			
	"sound/effect/eff_bbuck.wav",														// eSND_Effect_Buck				
	"sound/effect/eff_bomb.wav",														// eSND_Effect_Bomb				
	"sound/effect/eff_take_card.wav",													// eSND_Effect_Take				
	"sound/effect/eff_heading.wav",														// eSND_Effect_Heading				
	"sound/effect/eff_hurry_notice.wav",												// eSND_Effect_Warning				
	"sound/effect/eff_hurry_notice.wav",												// eSND_Effect_Clock				
	"sound/effect/eff_notice.wav",														// eSND_Effect_Notice				
	"sound/effect/eff_notice_myturn.wav",												// eSND_Effect_Turn				
	"sound/effect/eff_ssul.wav",														// eSND_Effect_SackSul				
	"sound/effect/eff_zzock.wav",														// eSND_Effect_Zock				
	"sound/effect/eff_success_gwang_gage.wav",											// eSND_Effect_Gage				
	"sound/effect/eff_chodan.wav",														// eSND_Effect_Dan					
	"sound/effect/eff_gwang_3.wav",														// eSND_Effect_3Gwang				
	"sound/effect/eff_gwang_5.wav",														// eSND_Effect_5Gwang			
	"sound/effect/eff_gwang_rain.wav",													// eSND_Effect_BGwang				
	"sound/effect/eff_goa_1_2.wav",														// eSND_Effect_1Go				
	"sound/effect/eff_go_3_5.wav",														// eSND_Effect_3Go					
	"sound/effect/eff_go_6_8.wav",														// eSND_Effect_6Go					
    
	"sound/attack/attack_success_gwang_gage1.wav",										// eSND_Attack_Gage_Success1		
	"sound/attack/attack_success_gwang_gage2.wav",										// eSND_Attack_Gage_Success2		
	"sound/attack/attack_fail_gwang_gage1.wav",											// eSND_Attack_Gage_Fail1			
	"sound/attack/attack_fail_gwang_gage2.wav",											// eSND_Attack_Gage_Fail2	
	"sound/attack/attack_heading.wav",													// eSND_Attack_Heading	
	"sound/attack/attack_bbuck.wav",													// eSND_Attack_Buck
	"sound/attack/attack_bbuck1.wav",													// eSND_Attack_Buck1			
	"sound/attack/attack_bbuck2.wav",													// eSND_Attack_Buck2				
	"sound/attack/attack_bbuck3.wav",													// eSND_Attack_Buck3				
	"sound/attack/attack_bbuckeat.wav",													// eSND_Attack_BuckEat			
	"sound/attack/attack_jabbuck.wav",													// eSND_Attack_JaBuck				
	"sound/attack/attack_ddadak.wav",													// eSND_Attack_DaDack				
	"sound/attack/attack_jjok.wav",														// eSND_Attack_Zock				
	"sound/attack/attack_ssl.wav",														// eSND_Attack_SackSul				
	"sound/attack/attack_shake.wav",													// eSND_Attack_Shake				
	"sound/attack/attack_bonus.wav",													// eSND_Attack_Bonus				
	"sound/attack/attack_bomb.wav",														// eSND_Attack_Bomb									
	"sound/attack/attack_gwang_3.wav",													// eSND_Attack_3Gwang				
	"sound/attack/attack_gwang_4.wav",													// eSND_Attack_4Gwang				
	"sound/attack/attack_gwang_5.wav",													// eSND_Attack_5Gwang				
	"sound/attack/attack_gwang_b.awv",													// eSND_Attack_BGwang				
	"sound/attack/attack_stop.wav",														// eSND_Attack_Stop				
	"sound/attack/attack_stop2.wav",													// eSND_Attack_Stop2	
	"sound/attack/attack_go_1.wav",														// eSND_Attack_1Go					
	"sound/attack/attack_go_2.wav",														// eSND_Attack_2Go					
	"sound/attack/attack_go_3.wav",														// eSND_Attack_3Go					
	"sound/attack/attack_go_4.wav",														// eSND_Attack_4Go					
	"sound/attack/attack_go_5.wav",														// eSND_Attack_5Go					
	"sound/attack/attack_go_6.wav",														// eSND_Attack_6Go					
	"sound/attack/attack_go_7.wav",														// eSND_Attack_7Go					
	"sound/attack/attack_go_8.wav",														// eSND_Attack_8Go					
	"sound/attack/attack_godori.wav",													// eSND_Attack_Godori							
	"sound/attack/attack_chodan.wav",													// eSND_Attack_ChoDan				
	"sound/attack/attack_hongdan.wav",													// eSND_Attack_HongDan				
	"sound/attack/attack_chungdan.wav",													// eSND_Attack_ChungDan		
	"sound/attack/attack_chongtong.wav",												// eSND_Attack_ChongTong
				
	"sound/harm/harm_bbuck.wav",														// eSND_Harm_Buck				
	"sound/harm/harm_bbuck1.wav",														// eSND_Harm_Buck1					
	"sound/harm/harm_bbuck2.wav",														// eSND_Harm_Buck2					
	"sound/harm/harm_bbuck3.wav",														// eSND_Harm_Buck3	
	"sound/harm/harm_bbuckeat.wav",														// eSND_Harm_BuckEat				
	"sound/harm/harm_jabbuck.wav",														// eSND_Harm_JaBuck				
	"sound/harm/harm_ddadak.wav",														// eSND_Harm_DaDack				
	"sound/harm/harm_jjok.wav",															// eSND_Harm_Zock					
	"sound/harm/harm_ssl.wav",															// eSND_Harm_SackSul				
	"sound/harm/harm_shake.wav",														// eSND_Harm_Shake					
	"sound/harm/harm_bonus.wav",														// eSND_Harm_Bonus					
	"sound/harm/harm_bomb.wav",															// eSND_Harm_Bomb						
	"sound/harm/harm_gwang_3.wav",														// eSND_Harm_3Gwang			
	"sound/harm/harm_gwang_4.wav",														// eSND_Harm_4Gwang				
	"sound/harm/harm_gwang_5.wav",														// eSND_Harm_5Gwang				
	"sound/harm/harm_gwang_b.wav",														// eSND_Harm_BGwang		
	"sound/harm/harm_go_1.wav",															// eSND_Harm_1Go					
	"sound/harm/harm_go_2.wav",															// eSND_Harm_2Go					
	"sound/harm/harm_go_3.wav",															// eSND_Harm_3Go					
	"sound/harm/harm_go_4.wav",															// eSND_Harm_4Go					
	"sound/harm/harm_go_5.wav",															// eSND_Harm_5Go					
	"sound/harm/harm_go_6.wav",															// eSND_Harm_6Go					
	"sound/harm/harm_go_7.wav",															// eSND_Harm_7Go					
	"sound/harm/harm_go_8.wav",															// eSND_Harm_8Go					
	"sound/harm/harm_godori.wav",														// eSND_Harm_Godori								
	"sound/harm/harm_chodan.wav",														// eSND_Harm_ChoDan				
	"sound/harm/harm_hongdan.wav",														// eSND_Harm_HongDan				
	"sound/harm/harm_chungdan.wav",														// eSND_Harm_ChungDan	
	"sound/harm/harm_chongtong.wav",													// eSND_Harm_ChongTong	

	"sound/etc/click.wav",																// eSND_Etc_Click					
	"sound/etc/focus.wav",																// eSND_Etc_Focus					
	"sound/etc/page.wav",																// eSND_Etc_Page			
	"sound/etc/notice1.wav",															// eSND_Etc_Notice1
	"sound/etc/notice2.wav",															// eSND_Etc_Notice2	
	"sound/etc/ladder.wav",																// eSND_Etc_Ladder					

	"image/",																			// eIMG_BG_Splah
	"image/bg/bg_start.jpg",															// eIMG_BG_Start
	"image/bg/bg_start2.jpg",															// eIMG_BG_Start2
	"image/bg/bg_select_stage.jpg",														// eIMG_BG_Stage
	"image/bg/bg_title_popup.png",														// eIMG_BG_Request1
	"image/bg/bg_nonetitle_popup.png",													// eIMG_BG_Request2
	"image/bg/bg_config.png",															// eIMG_BG_Dlg_Config
	"image/bg/bg_multi.png",															// eIMG_BG_Dlg_Multi1
	"image/bg/bg_myinfo_change_title.png",												// eIMG_BG_Dlg_Multi2
	"image/bg/myinfo_pop_box.png",														// eIMG_BG_Dlg_Info
	"image/bg/bg_popup2.png",															// eIMG_BG_Dlg_Rank
	"image/bg/bg_ranking_list.png",														// eIMG_BG_Dlg_List
	"image/bg/pop_box_i.png",															// eIMG_BG_Dlg_Shop	
	"image/bg/pop_box_shop.png",														// eIMG_BG_Dlg_Shop2
	"image/bg/bg_shop.png",																// eIMG_BG_Dlg_Shop3	
    "image/bg/game_dialog_box.png",                                                     // eIMG_BG_Dlg_Game
	"image/bg/bg_rank_my_info.png",														// eIMG_BG_List_Rank
	"image/bg/bg_play.jpg",																// eIMG_BG_Game
	"image/bg/result_bg.png",															// eIMG_BG_Result
	"image/bg/bg_ladder_game.png",														// eIMG_BG_Ladder

	"image/btn/btn_back_none.png",														// eIMG_BTN_Quit_Nor
	"image/btn/btn_back_press.png",														// eIMG_BTN_Quit_Sel
	"image/btn/btn_setup.png",															// eIMG_BTN_Config_Nor
	"image/btn/btn_setup_press.png",													// eIMG_BTN_Config_Sel
	"image/btn/btn_game_start.png",														// eIMG_BTN_Game_Nor
	"image/btn/btn_game_start_press.png",												// eIMG_BTN_Game_Sel
	"image/btn/btn_show_ranking.png",													// eIMG_BTN_Rank_Nor
	"image/btn/btn_show_ranking_press.png",												// eIMG_BTN_Rank_Sel
	"image/btn/btn_help.png",															// eIMG_BTN_Help_Nor
	"image/btn/btn_help_press.png",														// eIMG_BTN_Help_Sel
	"image/btn/map_return_button.png",													// eIMG_BTN_Main_Nor
	"image/btn/map_return_button_press.png",											// eIMG_BTN_Main_Sel
	"image/btn/map_myinfo_button.png",													// eIMG_BTN_Info_Nor
	"image/btn/map_myinfo_button_press.png",											// eIMG_BTN_Info_Sel
	"image/btn/mapicon_shop.png",														// eIMG_BTN_Shop_Nor
	"image/btn/mapicon_shop_press.png",													// eIMG_BTN_Shop_Sel		
	"image/btn/btn_back_none.png",														// eIMG_BTN_Close_Nor
	"image/btn/btn_back_press.png",														// eIMG_BTN_Close_Sel
	"image/btn/pop_btn_05_basic.png",													// eIMG_BTN_Prev_Nor
	"image/btn/pop_btn_05_touch.png",													// eIMG_BTN_Prev_Sel
	"image/btn/pop_btn_06_basic.png",													// eIMG_BTN_Next_Nor
	"image/btn/pop_btn_06_touch.png",													// eIMG_BTN_Next_Sel
	"image/btn/btn_yes.png",															// eIMG_BTN_Yes_Nor
	"image/btn/btn_yes_press.png",														// eIMG_BTN_Yes_Sel
	"image/btn/btn_no.png",																// eIMG_BTN_No_Nor
	"image/btn/btn_no_press.png",														// eIMG_BTN_No_Sel
	"image/btn/btn_confirm_none.png",													// eIMG_BTN_Confirm_Nor		
	"image/btn/btn_confirm_press.png",													// eIMG_BTN_Confirm_Sel		
	"image/btn/pop_btn_07_basic.png",													// eIMG_BTN_Reset_Nor		
	"image/btn/pop_btn_07_touch.png",													// eIMG_BTN_Reset_Sel
	"image/btn/pop_btn_18_basic.png",													// eIMG_BTN_Mini_Game_Nor
	"image/btn/pop_btn_18_touch.png",													// eIMG_BTN_Mini_Game_Sel
	"image/btn/pop_btn_08_on.png",														// eIMG_BTN_On_Nor			
	"image/btn/pop_btn_08_off.png",														// eIMG_BTN_Off_Nor	
	"image/btn/dialog_config_sound_btn1_none.png",										// eIMG_BTN_1_Nor					
	"image/btn/dialog_config_sound_btn1_press.png",										// eIMG_BTN_1_Sel					
	"image/btn/dialog_config_sound_btn2_none.png",										// eIMG_BTN_2_Nor					
	"image/btn/dialog_config_sound_btn2_press.png",										// eIMG_BTN_2_Sel					
	"image/btn/dialog_config_sound_btn3_none.png",										// eIMG_BTN_3_Nor			
	"image/btn/dialog_config_sound_btn3_press.png",										// eIMG_BTN_3_Sel					
	"image/btn/pop_btn_12_basic.png",													// eIMG_BTN_Circle_Nor		
	"image/btn/pop_btn_12_touch.png",													// eIMG_BTN_Circle_Sel		
	"image/btn/ranking_reg_none.png",													// eIMG_BTN_Connect_Nor
	"image/btn/ranking_reg_press.png",													// eIMG_BTN_Connect_Sel
	"image/btn/btn_buy_n.png",															// eIMG_BTN_Buy_Nor
	"image/btn/btn_buy_p.png",															// eIMG_BTN_Buy_Sel
	"image/btn/game_dialog_gostop_go_none.png",											// eIMG_BTN_Go_Nor					
	"image/btn/game_dialog_gostop_go_press.png",										// eIMG_BTN_Go_Sel					
	"image/btn/game_dialog_gostop_stop_none.png",										// eIMG_BTN_Stop_Nor				
	"image/btn/game_dialog_gostop_stop_press.png",										// eIMG_BTN_Stop_Sel	
	"image/btn/game_dialog_chongtong_4x_none.png",										// eIMG_BTN_ChongTong_Go_Nor				
	"image/btn/game_dialog_chongtong_4x_press.png",										// eIMG_BTN_ChongTong_Go_Sel				
	"image/btn/game_dialog_chongtong_10win_none.png",									// eIMG_BTN_ChongTong_Stop_Nor	
	"image/btn/game_dialog_chongtong_10win_press.png",									// eIMG_BTN_ChongTong_Stop_Sel		
	"image/btn/game_dialog_gukjinselect_yul_none.png",									// eIMG_BTN_Gukjin_10_Nor			
	"image/btn/game_dialog_gukjinselect_yul_press.png",									// eIMG_BTN_Gukjin_10_Sel			
	"image/btn/game_dialog_gukjinselect_pi_none.png",									// eIMG_BTN_Gukjin_2_Nor				
	"image/btn/game_dialog_gukjinselect_pi_press.png",									// eIMG_BTN_Gukjin_2_Sel				
	"image/btn/result_btn_confirm_none.png",											// eIMG_BTN_Continue_Nor			
	"image/btn/result_btn_confirm_press.png",											// eIMG_BTN_Continue_Sel			
	"image/btn/result_btn_mil_none.png",												// eIMG_BTN_Push_Nor				
	"image/btn/result_btn_mil_press.png",												// eIMG_BTN_Push_Sel				
	"image/btn/result_btn_stop_none.png",												// eIMG_BTN_Stop2_Nor				
	"image/btn/result_btn_stop_press.png",												// eIMG_BTN_Stop2_Sel				
	"image/btn/e_btn_menu_hide.png",													// eIMG_BTN_Game_Menu1			
	"image/btn/e_box_menu.png",															// eIMG_BTN_Game_Menu2			
	"image/btn/e_btn_skill_hide.png",													// eIMG_BTN_Game_Skill1		
	"image/btn/e_box_skill.png",														// eIMG_BTN_Game_Skill2		
	"image/btn/e_btn_menu01_basic.png",													// eIMG_BTN_Game_Help_Nor	
	"image/btn/e_btn_menu02_basic.png",													// eIMG_BTN_Game_Setting_Nor
	"image/btn/e_btn_menu03_basic.png",													// eIMG_BTN_Game_Shop_Nor
	"image/btn/e_btn_menu01_touch.png",													// eIMG_BTN_Game_Help_Sel			
	"image/btn/e_btn_menu02_touch.png",													// eIMG_BTN_Game_Setting_Sel		
	"image/btn/e_btn_menu03_touch.png",													// eIMG_BTN_Game_Shop_Sel			
	"image/btn/btn_shop_menu_n_01.png",													// eIMG_BTN_Shop_Menu_Nor
	"image/btn/btn_shop_menu_p_01.png",													// eIMG_BTN_Shop_Menu_Sel
	"image/btn/btn_coin_charge_n_01.png",												// eIMG_BTN_Shop_Coin_Nor
	"image/btn/btn_coin_charge_p_01.png",												// eIMG_BTN_Shop_Coin_Sel
	"image/btn/btn_money_charge_n_01.png",												// eIMG_BTN_Shop_Cash_Nor
	"image/btn/btn_money_charge_p_01.png",												// eIMG_BTN_Shop_Cash_Sel

	"image/etc/icon_coin.png",															// eIMG_ETC_Coin
	"image/etc/bg_text_input.png",														// eIMG_ETC_Text_Input
	"image/etc/help_gameinfo.png",														// eIMG_ETC_Game_Info
	"image/etc/pop_page_mark02.png",													// eIMG_ETC_Page_Mark_Nor
	"image/etc/pop_page_mark01.png",													// eIMG_ETC_Page_Mark_Sel
	"image/etc/pop_d_box.png",															// eIMG_ETC_Progress1
	"image/etc/pop_d_box_pix.png",														// eIMG_ETC_Progress2
	"image/etc/map_star_1.png",															// eIMG_ETC_Star1
	"image/etc/map_star_2.png",															// eIMG_ETC_Star2
	"image/etc/bg_talk_left.png",														// eIMG_ETC_Talk1
	"image/etc/bg_talk_right.png",														// eIMG_ETC_Talk2
	"image/etc/mapicon_border.png",														// eIMG_ETC_Stage_Menu_Border1
	"image/etc/mapicon_border_2.png",													// eIMG_ETC_Stage_Menu_Border2
	"image/etc/pop_f_02_t_touch.png",													// eIMG_ETC_Title_Menu_Border		
	"image/etc/pop_btn_13_touch.png",													// eIMG_ETC_Skill_Menu_Border1		
	"image/etc/skill_thumline.png",														// eIMG_ETC_Skill_Menu_Border2		
	"image/etc/skill_thumline_yellow.png",												// eIMG_ETC_Skill_Menu_Border3	
	"image/etc/e_mark_first_me.png",													// eIMG_ETC_First_Player1	
	"image/etc/e_mark_first_rival.png",													// eIMG_ETC_First_Player2
	"image/etc/e_big_p_guide_arrow01.png",												// eIMG_ETC_Player_Arrow1			
	"image/etc/e_big_p_guide_arrow02.png",												// eIMG_ETC_Player_Arrow2			
	"image/etc/e_mission_box_x.png",													// eIMG_ETC_Mission_X				
	"image/etc/e_mission_arrow01.png",													// eIMG_ETC_Mission_Arrow1			
	"image/etc/e_mission_arrow02.png",													// eIMG_ETC_Mission_Arrow2			
	"image/etc/e_mission_fail.png",														// eIMG_ETC_Mission_Fail			
	"image/etc/e_mission_success.png",													// eIMG_ETC_Mission_Success1		
	"image/etc/e_mission_rival.png",													// eIMG_ETC_Mission_Success2		
	"image/etc/on_mission_card_icon_b.png",												// eIMG_ETC_Mission_Mark			
	"image/etc/on_mission_card_icon_s.png",												// eIMG_ETC_Mission_Mark_Medium	
	"image/etc/game_result_win.png",													// eIMG_ETC_Win					
	"image/etc/game_result_lose.png",													// eIMG_ETC_Lose					
	"image/etc/game_result_pasan.png",													// eIMG_ETC_Bankrupted			
	"image/etc/result__txt.png",														// eIMG_ETC_Point_Times
	"image/etc/timing_bg.png",															// eIMG_ETC_Timing_Bar				
	"image/etc/timing_bar.png",															// eIMG_ETC_Timing_Gage		
	"image/etc/e_sno_box.png",															// eIMG_ETC_Number_Box	
	"image/etc/ladder_game_notice.png",													// eIMG_ETC_Ladder_Notice
	"image/etc/ladder_game_btn.png",													// eIMG_ETC_Ladder_Nor				
	"image/etc/ladder_game_btn_checked.png",											// eIMG_ETC_Ladder_Sel	
	"image/etc/ladder_game_kimhead.png",												// eIMG_ETC_Ladder_KBM

	"image/sprite/b_a_bg0001.jpg",														// eIMG_Sprite_Main
	"image/sprite/d_vs_ef0001.jpg",														// eIMG_Sprite_Versus
	"image/sprite/e_bet_01.png",														// eIMG_Sprite_Bet
	"image/sprite/skill_01.jpg",														// eIMG_Sprite_Skill
	"image/sprite/pop_f_02_a_t01.png",													// eIMG_Sprite_Title_Menu_Nor
	"image/sprite/pop_f_02_b_t01.png",													// eIMG_Sprite_Title_Menu_Dis
	"image/sprite/mapicon_nor_01.png",													// eIMG_Sprite_Stage_Menu_Nor
	"image/sprite/mapicon_dis_01.png",													// eIMG_Sprite_Stage_Menu_Dis
	"image/sprite/tab_rank_nor_01.png",													// eIMG_Sprite_Rank_Tab_Nor
	"image/sprite/tab_rank_sel_01.png",													// eIMG_Sprite_Rank_Tab_Sel
	"image/card/card_01.jpg",															// eIMG_Sprite_Card		
	"image/card/card_medium_01.jpg",													// eIMG_Sprite_Card_Medium		,
	"image/card/card_small_01.jpg",														// eIMG_Sprite_Card_Small			
	"image/card/card_mini_52.jpg",														// eIMG_Sprite_Card_Mini
	"image/card/border_b1.png",															// eIMG_Sprite_Select				
	"image/card/border_m1.png",															// eIMG_Sprite_Select_Medium		

	"image/font/x_a_no_00.png",															// eIMG_Font_A											
	"image/font/x_c_no_00.png",															// eIMG_Font_C						
	"image/font/x_d_no_00.png",															// eIMG_Font_D						
	"image/font/x_e_no_00.png",															// eIMG_Font_E											

	"image/effect/e_ef__crazy_zzac1.png",												// eIMG_Effect_Gage				
	"image/effect/eff_crazy_gage1.png",													// eIMG_Effect_GageS				
	"image/effect/e_ef_bbuck1.png",														// eIMG_Effect_Buck				
	"image/effect/eff_bbuck1.png",														// eIMG_Effect_Buck2				
	"image/effect/e_ef_heading1.png",													// eIMG_Effect_Heading				
	"image/effect/e_ef_zzac1.png",														// eIMG_Effect_Zack				
	"image/effect/e_ef_zzock1.png",														// eIMG_Effect_Zock				
	"image/effect/eff_zzock1.png",														// eIMG_Effect_Zock2				
	"image/effect/eff_daddac1.png",														// eIMG_Effect_Dadack				
	"image/effect/eff_ssul1.png",														// eIMG_Effect_SackSul				
	"image/effect/eff_bomb1.png",														// eIMG_Effect_Bomb				
	"image/effect/e_ef_bell1.png",														// eIMG_Effect_Bell				
	"image/effect/eff_pelight_b1.png",													// eIMG_Effect_Shine1				
	"image/effect/eff_pelight_m1.png",													// eIMG_Effect_Shine2				
	"image/effect/eff_stop1.png",														// eIMG_Effect_Stop				
	"image/effect/eff_1go1.png",														// eIMG_Effect_1Go					
	"image/effect/eff_2go1.png",														// eIMG_Effect_2Go					
	"image/effect/eff_3go1.png",														// eIMG_Effect_3Go					
	"image/effect/eff_4go1.png",														// eIMG_Effect_4Go					
	"image/effect/eff_5go1.png",														// eIMG_Effect_5Go					
	"image/effect/eff_6go1.png",														// eIMG_Effect_6Go					
	"image/effect/eff_7go1.png",														// eIMG_Effect_7Go					
	"image/effect/eff_8go1.png",														// eIMG_Effect_8Go					
	"image/effect/eff_3gwang1.png",														// eIMG_Effect_3Gwang				
	"image/effect/eff_4gwang1.png",														// eIMG_Effect_4Gwang				
	"image/effect/eff_5gwang1.png",														// eIMG_Effect_5Gwang				
	"image/effect/eff_bgwang1.png",														// eIMG_Effect_BGwang				
	"image/effect/eff_chodan1.png",														// eIMG_Effect_ChoDan				
	"image/effect/eff_hongdan1.png",													// eIMG_Effect_HongDan				
	"image/effect/eff_chungdan1.png",													// eIMG_Effect_ChungDan			
	"image/effect/eff_godori1.png",														// eIMG_Effect_Godori				
	"image/effect/eff_mission_o1.png",													// eIMG_Effect_MissionS			
	"image/effect/eff_mission_x1.png",													// eIMG_Effect_MissionF			
};

static const KDuint  l_uTalks [ ] [ 2 ] [ 10 ] =
{
    {  { 3, 3, 3, 3, 3, 3, 3, 3, 3, 3 },  { 1, 1, 1, 2, 1, 2, 1, 2, 2, 1 },  },         // eTALK_Take 
    {  { 0, 3, 3, 3, 3, 3, 3, 3, 3, 3 },  { 3, 1, 1, 1, 1, 2, 1, 1, 1, 2 },  },         // eTALK_Zock        
    {  { 3, 3, 3, 3, 3, 3, 3, 3, 3, 3 },  { 1, 1, 1, 1, 1, 2, 1, 1, 2, 1 },  },         // eTALK_SackSul        
    {  { 7, 3, 3, 3, 3, 3, 3, 3, 3, 3 },  { 1, 1, 2, 2, 1, 1, 1, 2, 2, 1 },  },         // eTALK_DaDack     
    {  { 1, 2, 1, 2, 1, 2, 1, 2, 1, 2 },  { 3, 3, 3, 3, 3, 0, 3, 0, 2, 0 },  },         // eTALK_Buck        
    {  { 3, 3, 3, 3, 3, 3, 3, 3, 3, 3 },  { 0, 1, 1, 2, 1, 2, 2, 1, 2, 2 },  },         // eTALK_JaBuck          
    {  { 3, 3, 3, 3, 3, 3, 3, 3, 3, 3 },  { 0, 1, 1, 2, 1, 2, 1, 1, 1, 2 },  },         // eTALK_Bomb         
    {  { 3, 3, 3, 3, 3, 3, 3, 3, 3, 3 },  { 1, 2, 1, 2, 1, 2, 1, 2, 2, 2 },  },         // eTALK_1Go        
    {  { 1, 3, 0, 3, 3, 3, 3, 3, 3, 3 },  { 2, 2, 1, 2, 1, 1, 1, 1, 2, 1 },  },         // eTALK_3Go       
    {  { 3, 3, 3, 3, 3, 3, 3, 3, 3, 3 },  { 1, 2, 2, 2, 2, 1, 1, 1, 2, 2 },  },         // eTALK_5Go       
    {  { 1, 3, 3, 3, 3, 3, 3, 3, 3, 3 },  { 2, 2, 2, 1, 2, 1, 2, 2, 2, 2 },  },         // eTALK_6Go       
    {  { 3, 3, 3, 3, 3, 3, 3, 3, 3, 3 },  { 6, 1, 1, 2, 1, 2, 2, 2, 1, 2 },  },         // eTALK_7Go         
    {  { 2, 3, 3, 3, 3, 3, 3, 3, 3, 3 },  { 2, 2, 2, 2, 2, 2, 2, 2, 2, 2 },  },         // eTALK_8Go         

    {  { 4, 3, 3, 3, 3, 3, 3, 3, 3, 3 },  { 0, 1, 1, 2, 1, 2, 1, 1, 2, 2 },  },         // eTALK_ChoDan       
    {  { 3, 3, 3, 3, 3, 3, 3, 3, 3, 3 },  { 0, 1, 1, 1, 1, 1, 1, 2, 1, 2 },  },         // eTALK_HongDan        
    {  { 3, 3, 3, 3, 3, 3, 3, 3, 3, 3 },  { 0, 1, 1, 2, 1, 1, 1, 1, 2, 2 },  },         // eTALK_ChungDan        
    {  { 3, 3, 3, 3, 3, 3, 3, 3, 3, 3 },  { 0, 2, 1, 2, 1, 2, 1, 2, 1, 1 },  },         // eTALK_Godori       
    {  { 3, 3, 3, 3, 3, 3, 3, 3, 3, 3 },  { 6, 1, 1, 1, 1, 1, 2, 1, 1, 2 },  },         // eTALK_3Gwang         
    {  { 3, 3, 3, 3, 3, 3, 3, 3, 3, 3 },  { 2, 2, 2, 2, 1, 2, 1, 2, 2, 2 },  },         // eTALK_5Gwang
    {  { 7, 3, 3, 3, 3, 3, 3, 3, 3, 3 },  { 6, 1, 1, 1, 1, 1, 1, 1, 1, 1 },  },         // eTALK_BGwang   
    {  { 3, 3, 3, 3, 3, 3, 3, 3, 3, 3 },  { 2, 2, 2, 2, 1, 2, 1, 2, 2, 2 },  },         // eTALK_ChongTong
    {  { 3, 3, 3, 3, 3, 3, 3, 3, 3, 3 },  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },  },         // eTALK_Stop 
    {  { 1, 2, 1, 1, 0, 1, 1, 1, 2, 2 },  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },  },         // eTALK_Heading  
    {  { 3, 0, 0, 0, 0, 0, 0, 0, 0, 0 },  { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 },  },         // eTALK_Gage1
    {  { 3, 0, 0, 0, 0, 0, 0, 0, 0, 0 },  { 2, 0, 0, 0, 0, 0, 0, 0, 0, 0 },  },         // eTALK_Gage2 
    {  { 3, 0, 0, 0, 0, 0, 0, 0, 0, 0 },  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },  },         // eTALK_Gage3      
    {  { 3, 0, 0, 0, 0, 0, 0, 0, 0, 0 },  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },  },         // eTALK_Slow     
    {  { 3, 0, 0, 0, 0, 0, 0, 0, 0, 0 },  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },  },         // eTALK_Limit 
};

CResManager::CResManager ( KDvoid )
{
    CC_ASSERT ( !g_pResManager );

	m_eBGM = ePath_Null;
    
#if defined ( __APPLE__ )   
/*    
    for ( KDuint i = 0; i != eTXT_Config_Game_Reset; i++ )
    {
        std::string  sText;
        
        const KDchar*  szText = l_szTexts [ i ];;
        KDchar  szLog [ 256 ] = "";
        
        for ( KDuint j = 0; j < kdStrlen ( szText ); j++ )
        {
            kdSprintfKHR ( szLog, "0x%02x, ", 0x000000ff & szText [ j ] );
            sText += szLog;
        }
        kdLogMessage ( l_szTexts [ i ] );
        kdLogMessage ( sText.c_str ( ) );  
        kdLogMessage ( "" );
    }
*/   
/*
    for ( KDuint i = 0; i < 27; i++ )
    {
        for ( KDuint j = 0; j < 2; j++ )
        {
            for ( KDuint k = 0; k < 10; k++ )
            {
                std::string  sText;
                
                const KDchar*  szText = (const KDchar*) l_szTalks [ i ][ j ][ k ];
                KDchar  szLog [ 256 ] = "";
                
                for ( KDuint l = 0; l < kdStrlen ( szText ); l++ )
                {
                    kdSprintfKHR ( szLog, "0x%02x, ", 0x000000ff & l_szTalks [ i ][ j ][ k ][ l ] );
                    sText += szLog;
                }
				
                kdLogMessage ( szText );
                //kdLogMessage ( sText.c_str ( ) );  
                kdLogMessage ( "" );
            }
        }
    }    
*/
#endif    
}

CResManager::~CResManager ( KDvoid )
{

}

KDvoid CResManager::create ( KDvoid )
{
    g_pResManager = new CResManager ( );
    
    CC_ASSERT ( g_pResManager );
}

KDvoid CResManager::release ( KDvoid )
{
	CC_SAFE_DELETE ( g_pResManager );
}

const KDchar* CResManager::getPath ( EPath eID )
{
	return l_szPath [ eID ];
}

const KDchar* CResManager::getPath ( EPath eID, KDuint uIndex )
{
	return l_szPath [ eID + uIndex ];
}

const KDchar* CResManager::getPathSprite ( EPath eID, KDuint uIndex )
{
	static KDchar  szPath [ 64 ];

	KDsize  uSize = 0;

	kdStrcpy ( szPath, l_szPath [ eID ] );
	
	uSize = kdStrlen ( szPath );
	uIndex += szPath [ uSize - 5 ] - '0';

	szPath [ uSize - 5 ] = '0' + uIndex % 10;
	if ( uIndex > 9 )
	{
		szPath [ uSize - 6 ] = '0' + uIndex / 10;
	}

	return szPath;
}

const KDchar* CResManager::getPathCartoon ( KDuint uStage, KDuint uIndex, KDbool bOpenning )
{
	static KDchar  szPath [ 64 ];

	if ( uStage == KDUINT_MAX )
	{
		uIndex++;
		kdSprintfKHR ( szPath, "image/cartoon/a_openning_%02d.jpg", uIndex );
	}
	else 
	{
		uStage++;
		uIndex++;

		if ( bOpenning )
		{
			kdSprintfKHR ( szPath, "image/cartoon/stage%02d_op_%02d.jpg", uStage, uIndex );
		}
		else
		{
			kdSprintfKHR ( szPath, "image/cartoon/stage%02d_ed_%02d.jpg", uStage, uIndex );
		}
	}

	return szPath;
}

const KDchar* CResManager::getPathCharacter ( KDuint uIndex, KDuint uState )
{
    static KDchar  szPath [ 64 ];
    
    kdSprintfKHR ( szPath, "image/character/x_character%02d_%d.png", uIndex, uState + 1 );
    
    return szPath;
}

const KDchar* CResManager::getPathTalk ( ETalk eID, KDuint uIndex, KDuint uState )
{
    return getPathCharacter ( uIndex, l_uTalks [ eID ] [ uState ] [ uIndex ] );
}

const KDchar* CResManager::getTextTalk ( ETalk eID, KDuint uIndex, KDuint uState )
{    
    return (const KDchar*) l_szTalks [ eID ] [ uState ] [ uIndex ];
}

const KDchar* CResManager::getText ( EText eID )
{    
    return (const KDchar*) l_szTexts [ eID ];
}

const KDchar* CResManager::getText ( EText eID, KDuint uIndex )
{
    return (const KDchar*) l_szTexts [ eID + uIndex ];
}

const KDchar* CResManager::getTextHelp ( KDuint uIndex )
{
    return (const KDchar*) l_szHelps [ uIndex ];
}

KDvoid CResManager::playSound ( EPath eID, KDint nMode )
{
#if !defined ( SHP )

	if ( eID != ePath_Null && g_pEnvManager->getSound ( ) )
	{
		if ( eID >= eSND_Attack_Gage_Success1 && eID <= eSND_Harm_ChongTong && !g_pEnvManager->getVoice ( ) )
		{
			return;
		}

		CCSound*  pSound = CCSound::sharedSound ( );
		KDuint    uSound = pSound->Open ( g_pResManager->getPath ( eID ), nMode );

		pSound->Rewind ( uSound );
	}

#endif
}

KDvoid CResManager::stopSound ( EPath eID )
{
#if !defined ( SHP )

	if ( eID != ePath_Null )
	{
		CCSound*  pSound = CCSound::sharedSound ( );
		KDuint    uSound = pSound->Open ( g_pResManager->getPath ( eID ) );

		pSound->Stop ( uSound );
	}

#endif
}

KDvoid CResManager::playBGM ( EPath eID )
{
	this->stopBGM ( );
	this->playSound ( eID, CCSoundRepeat );

	m_eBGM = eID;
}

KDvoid CResManager::stopBGM ( KDvoid )
{
	this->stopSound ( m_eBGM );

	m_eBGM = ePath_Null;
}

KDvoid CResManager::pauseBGM ( KDvoid )
{
	this->stopSound ( m_eBGM );
}

KDvoid CResManager::resumeBGM ( KDvoid )
{
	this->playBGM ( m_eBGM );
}

KDvoid CResManager::vibrate ( KDvoid )
{
	if ( g_pEnvManager->getVibrate ( ) )
	{
		xmVibrate ( 500 );
	}
}