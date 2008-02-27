// -*-c++-*-

/*!
  \file rcsc/rcg/types.h
  \brief rcg data format types.
*/

/*
 *Copyright:

 Copyright (C) Hidehisa AKIYAMA

 This code is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

 *EndCopyright:
 */

/////////////////////////////////////////////////////////////////////

#ifndef RCSC_RCG_TYPES_H
#define RCSC_RCG_TYPES_H

#include <boost/cstdint.hpp>

namespace rcsc {


//! max player number in one team
const int MAX_PLAYER = 11;

//! uniform number that represents the unknown player
const int Unum_Unknown = -1;

/*!
  \enum SideID
  \brief side type definition
*/
enum SideID {
    LEFT = 1,
    NEUTRAL = 0,
    RIGHT = -1,
};

/*!
  \enum MarkerID
  \brief marker type defintiion
 */
enum MarkerID {
    Goal_L, Goal_R, // 1

    Flag_C,
    Flag_CT, Flag_CB,
    Flag_LT, Flag_LB,
    Flag_RT,  Flag_RB, // 8

    Flag_PLT, Flag_PLC, Flag_PLB,
    Flag_PRT, Flag_PRC, Flag_PRB, // 14

    Flag_GLT, Flag_GLB,
    Flag_GRT, Flag_GRB, // 18

    Flag_TL50, Flag_TL40, Flag_TL30, Flag_TL20, Flag_TL10, // 23
    Flag_T0,
    Flag_TR10, Flag_TR20, Flag_TR30, Flag_TR40, Flag_TR50, // 29

    Flag_BL50, Flag_BL40, Flag_BL30, Flag_BL20, Flag_BL10,
    Flag_B0,
    Flag_BR10, Flag_BR20, Flag_BR30, Flag_BR40, Flag_BR50, // 40

    Flag_LT30, Flag_LT20, Flag_LT10 , // 43
    Flag_L0,
    Flag_LB10, Flag_LB20, Flag_LB30, // 47

    Flag_RT30, Flag_RT20, Flag_RT10, // 50
    Flag_R0,
    Flag_RB10, Flag_RB20, Flag_RB30, // 54

    Marker_Unknown = 55
};

/*!
  \enum LineID
  \brief line type definition
 */
enum LineID {
    Line_Left, Line_Right, Line_Top, Line_Bottom,
    Line_Unknown
};

//! Id of the unknown player type
const int Hetero_Unknown = -1;
//! Id of the default player type
const int Hetero_Default = 0;

/*!
  \enum PlayMode
  \brief playmode types defined in rcssserver/src/types.h
 */
enum PlayMode {
    PM_Null,
    PM_BeforeKickOff,
    PM_TimeOver,
    PM_PlayOn,
    PM_KickOff_Left,
    PM_KickOff_Right,
    PM_KickIn_Left,
    PM_KickIn_Right,
    PM_FreeKick_Left,
    PM_FreeKick_Right,
    PM_CornerKick_Left,
    PM_CornerKick_Right,
    PM_GoalKick_Left,
    PM_GoalKick_Right,
    PM_AfterGoal_Left,
    PM_AfterGoal_Right, // - sserver-2.94
    PM_Drop_Ball, // - sserver-3.29
    PM_OffSide_Left,
    PM_OffSide_Right, // untill sserver-5.27
    // [I.Noda:00/05/13] added for 3D viewer/commentator/small league
    PM_PK_Left,
    PM_PK_Right,
    PM_FirstHalfOver,
    PM_Pause,
    PM_Human,
    PM_Foul_Charge_Left,
    PM_Foul_Charge_Right,
    PM_Foul_Push_Left,
    PM_Foul_Push_Right,
    PM_Foul_MultipleAttacker_Left,
    PM_Foul_MultipleAttacker_Right,
    PM_Foul_BallOut_Left,
    PM_Foul_BallOut_Right, // until sserver-7.11
    PM_Back_Pass_Left, // after rcssserver-8.05-rel
    PM_Back_Pass_Right,
    PM_Free_Kick_Fault_Left,
    PM_Free_Kick_Fault_Right,
    PM_CatchFault_Left,
    PM_CatchFault_Right,
    PM_IndFreeKick_Left, // after rcssserver-9.2.0
    PM_IndFreeKick_Right,
    PM_PenaltySetup_Left,  // after rcssserver-9.3.0
    PM_PenaltySetup_Right,
    PM_PenaltyReady_Left,
    PM_PenaltyReady_Right,
    PM_PenaltyTaken_Left,
    PM_PenaltyTaken_Right,
    PM_PenaltyMiss_Left,
    PM_PenaltyMiss_Right,
    PM_PenaltyScore_Left,
    PM_PenaltyScore_Right,
    PM_MAX
};

//! playmode string table defined in rcssserver.
#define PLAYMODE_STRINGS {"",\
			"before_kick_off",\
			"time_over",\
			"play_on",\
			"kick_off_l",\
			"kick_off_r",\
			"kick_in_l",\
			"kick_in_r",\
			"free_kick_l",\
			"free_kick_r",\
			"corner_kick_l",\
			"corner_kick_r",\
			"goal_kick_l",\
			"goal_kick_r",\
			"goal_l",\
			"goal_r",\
			"drop_ball",\
			"offside_l",\
			"offside_r",\
			"penalty_kick_l",\
			"penalty_kick_r",\
			"first_half_over",\
			"pause",\
			"human_judge",\
			"foul_charge_l",\
			"foul_charge_r",\
			"foul_push_l",\
			"foul_push_r",\
			"foul_multiple_attack_l",\
			"foul_multiple_attack_r",\
			"foul_ballout_l",\
			"foul_ballout_r",\
      "back_pass_l", \
      "back_pass_r", \
      "free_kick_fault_l", \
      "free_kick_fault_r", \
      "catch_fault_l", \
      "catch_fault_r", \
      "indirect_free_kick_l", \
      "indirect_free_kick_r",\
      "penalty_setup_l", \
      "penalty_setup_r",\
      "penalty_ready_l",\
      "penalty_ready_r", \
      "penalty_taken_l", \
      "penalty_taken_r", \
      "penalty_miss_l", \
      "penalty_miss_r", \
      "penalty_score_l", \
      "penalty_score_r", \
      "", \
      "", \
      "", \
      "", \
      "" \
}

// available characters in player's say or coach's freeform message
// [-0-9a-zA-Z ().+*/?<>_]
// 74 characters

//! character set that player can say.
#define SAY_CHARACTERS \
"0123456789"\
"abcdefghijklmnopqrstuvwxyz"\
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"\
" ().+*/?<>_-"


/*!
  \enum BallStatus
  \brief ball position status for coach/trainer
 */
enum BallStatus {
    Ball_Null,
    Ball_InField,
    Ball_GoalL,
    Ball_GoalR,
    Ball_OutOfField,
    Ball_MAX
} ;

//! ball status string table for trainer.
#define BALL_STATUS_STRINGS { "",\
    "in_field",\
    "goal_l",\
    "goal_r",\
    "out_of_field",\
}


namespace rcg {

//! type of the short value
typedef boost::int16_t Int16;
//! type of the long value
typedef boost::int32_t Int32;

/*!
  \brief max length of color name string.

  Actually, this variable is not used.
*/
const int COLOR_NAME_MAX = 64;

/*!
  \enum DispInfoMode
  \brief monitor data type enumeration.
 */
enum DispInfoMode {
    NO_INFO     = 0, //!< dummy type
    SHOW_MODE   = 1, //!< showinfo_t
    MSG_MODE    = 2, //!< msg info
    DRAW_MODE   = 3, //!< drawinfo_t
    BLANK_MODE  = 4, //!< dummy. until sserver-6.07
    PM_MODE     = 5, //!< playmode
    TEAM_MODE   = 6, //!< team_t (team name & score)
    PT_MODE     = 7, //!< player_type_t
    PARAM_MODE  = 8, //!< server_params_t
    PPARAM_MODE = 9  //!< player_params_t
};

/*!
  \enum MsgInfoMode
  \brief message data type id enumeration.
 */
enum MsgInfoMode {
    MSG_BOARD = 1,
    LOG_BOARD = 2
};

/*!
  \enum PlayerStatus
  \brief player status bit mask.
 */
enum PlayerStatus {
    DISABLE         = 0x0000,
    STAND           = 0x0001,
    KICK            = 0x0002,
    KICK_FAULT      = 0x0004,
    GOALIE          = 0x0008,
    CATCH           = 0x0010,
    CATCH_FAULT     = 0x0020,
    BALL_TO_PLAYER  = 0x0040,
    PLAYER_TO_BALL  = 0x0080,
    DISCARD         = 0x0100,
    LOST            = 0x0200,
    BALL_COLLIDE    = 0x0400,
    PLAYER_COLLIDE  = 0x0800,
    TACKLE          = 0x1000,
    TACKLE_FAULT    = 0x2000,
    BACK_PASS       = 0x4000,
    FREE_KICK_FAULT = 0x8000
};


/*-------------------------------------------------------------------*/
// monitor protocol version 1
// game log format versin 1 & version 2

const double SHOWINFO_SCALE = 16.0; //!< scaling parameter
const int REC_OLD_VERSION = 1; //!< version number of rcg v1
const int REC_VERSION_2 = 2; //!< recorded value of rcg v2

/*!
  \struct pos_t
  \brief object data
*/
struct pos_t {
    Int16 enable; //!< determines if this object is in field.
    Int16 side;   //!< side type of this object
    Int16 unum;   //!< uniform number
    Int16 angle;  //!< player's body angle. This is degree data.
    Int16 x;      //!< x-coordinate
    Int16 y;      //!< y-coordinate
};

/*!
  \struct team_t
  \brief team data
*/
struct team_t {
    char name[16]; //!< team name string
    Int16 score;   //!< score
};

/*!
  \struct showinfo_t
  \brief view data of rcssmonitor v1 protocol
*/
struct showinfo_t {
    char pmode;     //!< playmode id
    team_t team[2]; //!< team date
    pos_t pos[MAX_PLAYER * 2 + 1]; //!< all movable objects
    Int16 time; //!< game time
};

/*!
  \struct msginfo_t
  \brief message data
*/
struct msginfo_t {
    Int16 board;  //!< message type id
    char message[2048]; //!< message body
};

/*!
  \struct pointinfo_t
  \brief point data in drawinfo_t
*/
struct pointinfo_t {
    Int16 x; //!< x pos
    Int16 y; //!< y pos
    char color[COLOR_NAME_MAX]; //!< color name
};

/*!
  \struct circleinfo_t
  \brief circle data in drawinfo_t
*/
struct circleinfo_t {
    Int16 x; //!< x pos
    Int16 y; //!< y pos
    Int16 r; //!< radius
    char color[COLOR_NAME_MAX]; //!< color name
};

/*!
  \struct lineinfo_t
  \brief line data in drawinfo_t
*/
struct lineinfo_t {
    Int16 x1; //!< x of point1
    Int16 y1; //!< y of point1
    Int16 x2; //!< x of point2
    Int16 y2; //!< y of point2
    char color[COLOR_NAME_MAX]; //!< color name
};

/*!
  \struct drawinfo_t
  \brief drawing data
*/
struct drawinfo_t {
    Int16 mode; //!< data type id
    union {
        pointinfo_t pinfo;
        circleinfo_t cinfo;
        lineinfo_t linfo;
    } object; //!< union variable
};

/*!
  \struct dispinfo_t
  \brief data block for rcssmonitor v1 protocol and rcg v1/v2
*/
struct dispinfo_t {
    Int16 mode; //!< data type id
    union {
        showinfo_t show; //!< view data
        msginfo_t msg; //!< message data
        drawinfo_t draw; //!< draw data
    } body; //!< union variable
};

/*-------------------------------------------------------------------*/
// monitor protocol version 2
// game log format version 3

//! scaling variable
const double SHOWINFO_SCALE2 = 65536.0;
//! version number
const int REC_VERSION_3 = 3;

/*!
  \struct ball_t
  \brief ball data
*/
struct ball_t {
    Int32 x; //!< pos x scaled by SHOWINFO_SCALE2
    Int32 y; //!< pos y scaled by SHOWINFO_SCALE2
    Int32 deltax; //!< velocity x scaled by SHOWINFO_SCALE2
    Int32 deltay; //!< velocity y scaled by SHOWINFO_SCALE2
};

/*!
  \struct player_t
  \brief player data
*/
struct player_t {
    Int16 mode; //!< status flag
    Int16 type; //!< player type id
    Int32 x; //!< scaled pos x
    Int32 y; //!< scaled pos y
    Int32 deltax; //!< scaled velocity x
    Int32 deltay; //!< scaled velocity y
    Int32 body_angle; //!< scaled body angle. radian data
    Int32 head_angle; //!< scaled heading angle relative to body. radian data
    Int32 view_width; //!< scaled view width. radian data
    Int16 view_quality; //!< view quality id
    Int32 stamina; //!< scaled stamina value.
    Int32 effort; //!< scaled effort value
    Int32 recovery; //!< scaled recover value
    Int16 kick_count; //!< executed kick command count
    Int16 dash_count; //!< executed dash command count
    Int16 turn_count; //!< executed turn command count
    Int16 say_count; //!< executed say command count
    Int16 turn_neck_count; //!< executed turn_neck command count
    Int16 catch_count; //!< executed catch command count
    Int16 move_count; //!< executed move command count
    Int16 change_view_count; //!< executed change_view command count
};

/*!
  \struct showinfo_t2
  \brief view data of rcssmonitor v2 protocol
*/
struct showinfo_t2 {
    char pmode; //!< playmode id
    team_t team[2]; //!< team info
    ball_t ball; //!< ball info
    player_t pos[MAX_PLAYER * 2]; //!< all player info
    Int16 time; //!< game time
};

/*!
  \struct short_showinfo_t2
  \brief view data.

  rcg v3 files include only this as view data.
*/
struct short_showinfo_t2 {
    ball_t ball; //!< ball info
    player_t pos[MAX_PLAYER * 2]; //!< player info
    Int16 time; //!< game time
};

/*!
  \struct player_type_t
  \brief player type parameters
 */
struct player_type_t {
    Int16 id; //!< type id
    Int32 player_speed_max; //!< max speed
    Int32 stamina_inc_max; //!< max stamina increment value
    Int32 player_decay; //!< speed decay
    Int32 inertia_moment; //!< inertia moment
    Int32 dash_power_rate; //!< dash power rate
    Int32 player_size; //!< body radius
    Int32 kickable_margin; //!< kickable margin
    Int32 kick_rand; //!< random factor for kick
    Int32 extra_stamina; //!< extra stamina value when stamina is 0.
    Int32 effort_max; //!< max(initial) effort value
    Int32 effort_min; //!< min effort value

    Int32 sparelong1; //!< spare variable
    Int32 sparelong2; //!< spare variable
    Int32 sparelong3; //!< spare variable
    Int32 sparelong4; //!< spare variable
    Int32 sparelong5; //!< spare variable
    Int32 sparelong6; //!< spare variable
    Int32 sparelong7; //!< spare variable
    Int32 sparelong8; //!< spare variable
    Int32 sparelong9; //!< spare variable
    Int32 sparelong10; //!< spare variable
};

/*!
  \struct server_params_t
  \brief server parametors
 */
struct server_params_t {
    Int32 goal_width; //!< goal width
    Int32 inertia_moment; //!< related to player's turn action
    Int32 player_size; //!< normal player size
    Int32 player_decay; //!< normal player speed decay
    Int32 player_rand; //!< noize added to normal player movement
    Int32 player_weight; //!< normal player weight
    Int32 player_speed_max; //!< normal player speed max
    Int32 player_accel_max; //!< normal player accel max
    Int32 stamina_max; //!< normal player stamina max
    Int32 stamina_inc; //!< normal player stamina inc
    Int32 recover_init; //!< normal player recovery init
    Int32 recover_dec_thr; //!< normal player recovery decriment threshold
    Int32 recover_min; //!< normal player recovery min
    Int32 recover_dec; //!< normal player recovery decriment
    Int32 effort_init; //!< normal player dash effort init
    Int32 effort_dec_thr; //!< normal player dash effort decriment threshold
    Int32 effort_min; //!< normal player dash effrot min
    Int32 effort_dec; //!< normal player dash effort decriment
    Int32 effort_inc_thr; //!< normal player dash effort incriment threshold
    Int32 effort_inc; //!< normal player dash effort incriment
    Int32 kick_rand;  //!< noise added directly to normal player's kick
    Int16 team_actuator_noise; //!< flag whether to use team specific actuator noise
    Int32 player_rand_factor_l; //!< factor to multiple prand for left team
    Int32 player_rand_factor_r; //!< factor to multiple prand for right team
    Int32 kick_rand_factor_l; //!< factor to multiple kick_rand for left team
    Int32 kick_rand_factor_r; //!< factor to multiple kick_rand for right team
    Int32 ball_size; //!< ball size
    Int32 ball_decay; //!< ball speed decay
    Int32 ball_rand; //!< noise added to ball movement
    Int32 ball_weight; //!< ball weight
    Int32 ball_speed_max; //!< ball speed max
    Int32 ball_accel_max; //!< ball acceleration max
    Int32 dash_power_rate; //!< normal player's dash power rate
    Int32 kick_power_rate; //!< normal player's kick power rate
    Int32 kickable_margin; //!< normal player's kickable margin
    Int32 control_radius; //!< control radius
    Int32 control_radius_width; //!< (control radius) - (plyaer size)
    Int32 max_power; //!< max power
    Int32 min_power; //!< min power
    Int32 max_moment; //!< max moment
    Int32 min_moment; //!< min moment
    Int32 max_neck_moment; //!< max neck moment
    Int32 min_neck_moment; //!< min neck moment
    Int32 max_neck_angle; //!< max neck angle
    Int32 min_neck_angle; //!< min neck angle
    Int32 visible_angle; //!< visible angle
    Int32 visible_distance; //!< visible distance
    Int32 wind_dir; //!< wind direction
    Int32 wind_force; //!< wind force
    Int32 wind_ang; //!< wind angle for rand
    Int32 wind_rand; //!< wind noise for force
    Int32 kickable_area; //!< kickable_area
    Int32 catch_area_l; //!< goalie catchable area length
    Int32 catch_area_w; //!< goalie catchable area width
    Int32 catch_probability; //!< goalie catchable possibility
    Int16 goalie_max_moves; //!< goalie max moves after a catch
    Int32 corner_kick_margin; //!< corner kick margin
    Int32 offside_active_area; //!< offside active area size
    Int16 wind_none; //!< wind factor is none
    Int16 use_wind_random; //!< wind factor is random
    Int16 coach_say_count_max; //!< max count of coach's freeform say
    Int16 coach_say_msg_size; //!< max length of coach's freeform say message
    Int16 clang_win_size; //!< window of coach language window
    Int16 clang_define_win; //!< window of coach language define message
    Int16 clang_meta_win; //!< window of coach lauguage meta message
    Int16 clang_advice_win; //!< window of coach language advice message
    Int16 clang_info_win; //!< window of coach language info message
    Int16 clang_mess_delay; //!< coach language advise message is delayed this cycle
    Int16 clang_mess_per_cycle; //!< online coach can send CLang advise per this cycle
    Int16 half_time; //!<  half time cycle
    Int16 simulator_step; //!< simulator step interval msec
    Int16 send_step; //!< udp send step interval msec
    Int16 recv_step; //!< udp recv step interval msec
    Int16 sense_body_step; //!< sense_body interval step msec
    Int16 lcm_step; //!< lcm of all the above steps msec
    Int16 player_say_msg_size; //!< string size of say message
    Int16 player_hear_max; //!< player hear_capacity_max
    Int16 player_hear_inc; //!< player hear_capacity_inc
    Int16 player_hear_decay; //!< player hear_capacity_decay
    Int16 catch_ban_cycle; //!< goalie catch ban cycle
    Int16 slow_down_factor; //!< factor to slow down simulator and send intervals
    Int16 use_offside; //!< flag for using off side rule
    Int16 kickoff_offside; //!< flag for permit kick off offside
    Int32 offside_kick_margin; //!< offside kick margin
    Int32 audio_cut_dist; //!< audio cut off distance
    Int32 dist_quantize_step; //!< quantize step of distance
    Int32 landmark_dist_quantize_step; //!< quantize step of distance for landmark
    Int32 dir_quantize_step; //!< quantize step of direction
    Int32 dist_quantize_step_l; //!< team right quantize step of distance
    Int32 dist_quantize_step_r; //!< team left quantize step of distance
    Int32 landmark_dist_quantize_step_l; //!< team right quantize step of distance for landmark
    Int32 landmark_dist_quantize_step_r; //!< team left quantize step of distance for landmark
    Int32 dir_quantize_step_l; //!< team left quantize step of direction
    Int32 dir_quantize_step_r; //!< team right quantize step of direction
    Int16 coach_mode; //!< coach mode
    Int16 coach_with_referee_mode; //!< coach with referee mode
    Int16 use_old_coach_hear; //!< old format for hear command (coach)
    Int16 online_coach_look_step; //!< online coach's look interval step
    Int32 slowness_on_top_for_left_team;  //!< flag for left team dash power rate reduction on field top area
    Int32 slowness_on_top_for_right_team; //!< flag for right team dash power rate reduction on field top area

    Int32 ka_length; //!< keep away region length
    Int32 ka_width;  //!< keep away region width

    Int32 ball_stuck_area; //!< spare variable

    Int32 sparelong6; //!< spare variable
    Int32 sparelong7; //!< spare variable
    Int32 sparelong8; //!< spare variable
    Int32 sparelong9; //!< spare variable
    Int32 sparelong10; //!< spare variable

    Int16 start_goal_l; //!< initial left team score
    Int16 start_goal_r; //!< initial right team score
    Int16 fullstate_l; //!< flag for left team's fullstate info receive
    Int16 fullstate_r; //!< flag for right team's fullstate info receive
    Int16 drop_ball_time; //!< automatically drop_ball waiting cycle
    Int16 synch_mode; //!< flag for using synchronized mode
    Int16 synch_offset; //!< wheh sync_mode, server decide the message sent to clients before this number[ms]
    Int16 synch_micro_sleep; //!< the number of microseconds to sleep while waiting for players */
    Int16 point_to_ban; //!< player continue to point to the same point at least during this cycle
    Int16 point_to_duration; //!< point_to command has effectiveness during this cycle

    /*
      Int16 pen_before_setup_wait;
      Int16 pen_setup_wait;
      Int16 pen_ready_wait;
      Int16 pen_taken_wait;
      Int16 pen_nr_kicks;
      Int16 pen_max_extra_kicks;
      Int32 pen_dist_x;
      Int16 pen_random_winner;
      Int32 pen_max_goalie_dist_x;
      Int16 pen_allow_mult_kicks;
    */
};

/*!
  \struct player_params_t
  \brief heterogenious player trade-off parametors
*/
struct player_params_t {
    Int16 player_types; //!< the number of player types
    Int16 substitute_max; //!< max player substitution
    Int16 pt_max; //!< max each player type on field
    Int32 player_speed_max_delta_min; //!< minimal value wighin delta range
    Int32 player_speed_max_delta_max; //!< maximal value wighin delta range
    Int32 stamina_inc_max_delta_factor; //!< tradeoff parameter
    Int32 player_decay_delta_min; //!< minimal value wighin delta range
    Int32 player_decay_delta_max; //!< maximal value wighin delta range
    Int32 inertia_moment_delta_factor; //!< tradeoff parameter
    Int32 dash_power_rate_delta_min; //!< minimal value wighin delta range
    Int32 dash_power_rate_delta_max; //!< maximal value wighin delta range
    Int32 player_size_delta_factor; //!< tradeoff parameter
    Int32 kickable_margin_delta_min; //!< minimal value wighin delta range
    Int32 kickable_margin_delta_max; //!< maximal value wighin delta range
    Int32 kick_rand_delta_factor; //!< tradeoff parameter
    Int32 extra_stamina_delta_min; //!< minimal value wighin delta range
    Int32 extra_stamina_delta_max; //!< maximal value wighin delta range
    Int32 effort_max_delta_factor; //!< tradeoff parameter
    Int32 effort_min_delta_factor; //!< tradeoff parameter
    Int32 random_seed; //!< random seed to generate parameters
    Int32 new_dash_power_rate_delta_min; //!< minimal value wighin delta range
    Int32 new_dash_power_rate_delta_max; //!< maximal value wighin delta range
    Int32 new_stamina_inc_max_delta_factor; //!< tradeoff parameter

    Int32 sparelong5; //!< spare variable
    Int32 sparelong6; //!< spare variable
    Int32 sparelong7; //!< spare variable
    Int32 sparelong8; //!< spare variable
    Int32 sparelong9; //!< spare variable
    Int32 sparelong10; //!< spare variable

    Int16 spareshort1; //!< spare variable
    Int16 spareshort2; //!< spare variable
    Int16 spareshort3; //!< spare variable
    Int16 spareshort4; //!< spare variable
    Int16 spareshort5; //!< spare variable
    Int16 spareshort6; //!< spare variable
    Int16 spareshort7; //!< spare variable
    Int16 spareshort8; //!< spare variable
    Int16 spareshort9; //!< spare variable
    Int16 spareshort10; //!< spare variable
};

/*!
  \struct dispinfo_t2
  \brief data block for rcssmonitor v2 protocol.

  This data format is used only for monitor protocol and not used in rcg.
*/
struct dispinfo_t2 {
    Int16 mode; //!< data type id
    union {
        showinfo_t2 show; //!< view data
        msginfo_t msg; //!< message data
        player_type_t ptinfo; //!< player type data
        server_params_t sparams; //!< server param data
        player_params_t pparams; //!< payer trade-off param data
    } body; //!< union variable
};

//! default rcg version
const int DEFAULT_LOG_VERSION = REC_VERSION_3;

} // end namespace
} // end namespace

#endif
