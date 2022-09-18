#ifndef __COMMON_H
#define __COMMON_H


//Action -> ボタンで設定するやつ
//Command -> ToFセンサ、傾けてなんちゃらも含める

#define NUM_MODE 3
#define NUM_ACTION 5

// モード：背景の設定
enum Mode{
    Mode_PP=0,
    Mode_Monitor=1,
    Mode_PlainImg=2
};


#define Cmd_cam_on 1
#define Cmd_cam_off 2
#define Cmd_del_bk 3
#define Cmd_not_del_bk 0
    
#define Cmd_set_volume 4
#define Cmd_set_mic_level 5
#define Cmd_set_mute 6
#define Cmd_release_mute 7
    
#define Cmd_Start_Draw 8
#define Cmd_Stop_Draw 9

#define Cmd_load_stl 10
#define Cmd_zoom_img 11
#define Cmd_next_slide 12
#define Cmd_prev_slide 13
#define Cmd_show_slide_archive 14
#define Cmd_Exit 15
#define Cmd_mode_blank 16
#define Cmd_mode_pp 17
#define Cmd_mode_monitor 18



///////////////////////////////////////////////////////////////////////////////

#define SndData_PRY     0
#define SndDAta_Gyro    1
#define SndData_Acc     2
#define SndData_vol_out 4
#define SndData_vol_mic 5
#define SndData_battery 6
#define SndData_Cmd     7


#endif
