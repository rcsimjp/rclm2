// -*-c++-*-

/*!
  \file resultprinter.cpp
  \brief game result printer program source File.
*/

/*
 *Copyright:

 Copyright (C) Hidehisa Akiyama

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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "util.h"
#include "gzfstream.h"
#include "parser.h"
#include "handler.h"
#include "factory.h"

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <cstring>

#ifdef HAVE_NETINET_IN_H
#include <netinet/in.h>
#endif
#ifdef HAVE_WINDOWS_H
#include <windows.h>
#endif

struct Point {
    double x;
    double y;

    Point()
        : x( 0.0 ), y( 0.0 )
      { }
};

class ResultPrinter
    : public rcsc::rcg::Handler {
private:

    static const double PITCH_LENGTH;
    static const double PITCH_WIDTH;

    static const double GOAL_POST_RADIUS;

    double M_goal_width;
    double M_ball_size;
    int M_half_time;

    rcsc::rcg::PlayMode M_playmode;
    int M_cycle;

    std::string M_left_team_name; //!< left teamname string
    std::string M_right_team_name; //!< right teamname string

    int M_left_score; //!< left team score
    int M_right_score; //!< right team score

    int M_left_penalty_taken; //!< total number of left team penalty trial
    int M_right_penalty_taken; //!< total number of left team penalty trial

    int M_left_penalty_score; //!< left team penalty kick score
    int M_right_penalty_score; //!< left team penalty kick score

    rcsc::rcg::SideID M_last_penalty_taker_side;

public:

    ResultPrinter();

    // v3 or older
    bool handleDispInfo( const rcsc::rcg::dispinfo_t & disp );
    bool handleShowInfo( const rcsc::rcg::showinfo_t & show );
    bool handleShortShowInfo2( const rcsc::rcg::short_showinfo_t2 & show );
    bool handleMsgInfo( rcsc::rcg::Int16,
                        const std::string & )
      {
          return true;
      }
    bool handlePlayMode( char playmode );
    bool handlePlayerType( const rcsc::rcg::player_type_t & )
      {
          return true;
      }
    bool handleServerParam( const rcsc::rcg::server_params_t & param );
    bool handlePlayerParam( const rcsc::rcg::player_params_t & )
      {
          return true;
      }
    bool handleTeamInfo( const rcsc::rcg::team_t & team_left,
                         const rcsc::rcg::team_t & team_right );

    // common
    bool handleEOF();

    // v4 or later
    bool handleShow( const int time,
                     const rcsc::rcg::ShowInfoT & show );
    bool handleMsg( const int time,
                    const int board,
                    const std::string & msg );
    bool handlePlayMode( const int time,
                         const rcsc::rcg::PlayMode pm );
    bool handleTeam( const int time,
                     const rcsc::rcg::TeamT & team_l,
                     const rcsc::rcg::TeamT & team_r );
    bool handleServerParam( const std::string & msg );
    bool handlePlayerParam( const std::string & msg );
    bool handlePlayerType( const std::string & msg );

private:

    bool crossGoalLine( const Point & ball_pos,
                        const Point & prev_ball_pos );

    void checkFinalPenaltyGoal( const Point & ball_pos );

};

const double ResultPrinter::PITCH_LENGTH = 105.0;
const double ResultPrinter::PITCH_WIDTH = 68.0;

const double ResultPrinter::GOAL_POST_RADIUS = 0.06;

/*-------------------------------------------------------------------*/
/*!

*/
ResultPrinter::ResultPrinter()
    : M_goal_width( 14.02 )
    , M_ball_size( 0.085 )
    , M_half_time( 3000 )
    , M_playmode( rcsc::rcg::PM_Null )
    , M_cycle( 0 )
    , M_left_team_name( "" )
    , M_right_team_name( "" )
    , M_left_score( 0 )
    , M_right_score( 0 )
    , M_left_penalty_taken( 0 )
    , M_right_penalty_taken( 0 )
    , M_left_penalty_score( 0 )
    , M_right_penalty_score( 0 )
    , M_last_penalty_taker_side( rcsc::rcg::NEUTRAL )
{

}

/*-------------------------------------------------------------------*/
/*!

*/
bool
ResultPrinter::handleDispInfo( const rcsc::rcg::dispinfo_t & disp )
{
    if ( rcsc::rcg::SHOW_MODE == htons( disp.mode ) )
    {
        return handleShowInfo( disp.body.show );
    }
    return true;
}

/*-------------------------------------------------------------------*/
/*!

*/
bool
ResultPrinter::crossGoalLine( const Point & ball_pos,
                              const Point & prev_ball_pos )
{
    double delta_x = ball_pos.x - prev_ball_pos.x;
    double delta_y = ball_pos.y - prev_ball_pos.y;

    double gradient = delta_y / delta_x;
    double offset = prev_ball_pos.y - gradient * prev_ball_pos.x;

    double x = PITCH_LENGTH*0.5 + M_ball_size;
    if ( ball_pos.x < 0.0 ) x *= -1.0;
    double y_intercept = gradient * x + offset;

    //     std::cout << ": prev = "
    //               << prev_ball_pos.x << ','
    //               << prev_ball_pos.y
    //               << std::endl;
    //     std::cout << ": curr = "
    //               << ball_pos.x << ','
    //               << ball_pos.y
    //               << std::endl;
    //     std::cout << ": delta_x = " << delta_x << std::endl;
    //     std::cout << ": delta_y = " << delta_y << std::endl;
    //     std::cout << ": grad = " << gradient << std::endl;
    //     std::cout << ": off = " << offset << std::endl;
    //     std::cout << ": x = " << x << std::endl;
    //     std::cout << ": y_inter = " << y_intercept << std::endl;

    return ( std::fabs( y_intercept ) <= ( M_goal_width*0.5 + GOAL_POST_RADIUS ) );
}

/*-------------------------------------------------------------------*/
/*!

*/
void
ResultPrinter::checkFinalPenaltyGoal( const Point & ball_pos )
{
    static Point s_prev_ball_pos;

    if ( M_playmode == rcsc::rcg::PM_TimeOver
         && crossGoalLine( ball_pos, s_prev_ball_pos ) )
    {
        if ( M_last_penalty_taker_side == rcsc::rcg::LEFT )
        {
            //std::cerr << "time_over -> penalty_score_l" << std::endl;
            ++M_left_penalty_score;
        }
        else
        {
            //std::cerr << "time_over -> penalty_score_r" << std::endl;
            ++M_right_penalty_score;
        }
    }

    s_prev_ball_pos = ball_pos;
}

/*-------------------------------------------------------------------*/
/*!

*/
bool
ResultPrinter::handleShowInfo( const rcsc::rcg::showinfo_t & show )
{
    M_cycle = rcsc::rcg::nstohi( show.time );

    handlePlayMode( show.pmode );
    handleTeamInfo( show.team[0], show.team[1] );

    if ( M_last_penalty_taker_side != rcsc::rcg::NEUTRAL )
    {
        Point ball_pos;

        ball_pos.x = rcsc::rcg::nstohd( show.pos[0].x );
        ball_pos.y = rcsc::rcg::nstohd( show.pos[0].y );

        checkFinalPenaltyGoal( ball_pos );
    }

    return true;
}

/*-------------------------------------------------------------------*/
/*!

*/
bool
ResultPrinter::handleShortShowInfo2( const rcsc::rcg::short_showinfo_t2 & show )
{
    M_cycle = rcsc::rcg::nstohi( show.time );

    if ( M_last_penalty_taker_side != rcsc::rcg::NEUTRAL )
    {
        Point ball_pos;

        ball_pos.x = rcsc::rcg::nltohd( show.ball.x );
        ball_pos.y = rcsc::rcg::nltohd( show.ball.y );

        checkFinalPenaltyGoal( ball_pos );
    }

    return true;
}

/*-------------------------------------------------------------------*/
/*!

*/
bool
ResultPrinter::handlePlayMode( char playmode )
{
    rcsc::rcg::PlayMode pm = static_cast< rcsc::rcg::PlayMode >( playmode );

    if ( M_playmode == pm )
    {
        return true;
    }

    M_playmode = pm;

    switch ( M_playmode ) {
    case rcsc::rcg::PM_PenaltySetup_Left:
        ++M_left_penalty_taken;
        M_last_penalty_taker_side = rcsc::rcg::LEFT;
        break;
    case rcsc::rcg::PM_PenaltySetup_Right:
        ++M_right_penalty_taken;
        M_last_penalty_taker_side = rcsc::rcg::RIGHT;
        break;
    case rcsc::rcg::PM_PenaltyMiss_Left:
        break;
    case rcsc::rcg::PM_PenaltyMiss_Right:
        break;
    case rcsc::rcg::PM_PenaltyScore_Left:
        ++M_left_penalty_score;
        break;
    case rcsc::rcg::PM_PenaltyScore_Right:
        ++M_right_penalty_score;
        break;
    case rcsc::rcg::PM_TimeOver:
        break;
    default:
        break;
    }

    return true;
}

/*-------------------------------------------------------------------*/
/*!

*/
bool
ResultPrinter::handleServerParam( const rcsc::rcg::server_params_t & param )
{
    M_goal_width = rcsc::rcg::nltohd( param.goal_width );
    M_ball_size = rcsc::rcg::nltohd( param.ball_size );
    M_half_time = rcsc::rcg::nstohi( param.half_time );

    return true;
}

/*-------------------------------------------------------------------*/
/*!

*/
bool
ResultPrinter::handleTeamInfo( const rcsc::rcg::team_t & team_left,
                               const rcsc::rcg::team_t & team_right )
{
    if ( M_left_team_name.empty() )
    {
        char buf[18];
        std::memset( buf, '\0', 18 );
        std::strncpy( buf, team_left.name, 16 );
        M_left_team_name = buf;
    }
    if ( M_right_team_name.empty() )
    {
        char buf[18];
        std::memset( buf, '\0', 18 );
        std::strncpy( buf, team_right.name, 16 );
        M_right_team_name = buf;
    }

    M_left_score = rcsc::rcg::nstohi( team_left.score );
    M_right_score = rcsc::rcg::nstohi( team_right.score );

    return true;
}

/*-------------------------------------------------------------------*/
/*!
  print result
  "<TeamNameL> <TeamNameR> <ScoreL> <ScoreR>
*/
bool
ResultPrinter::handleEOF()
{
    bool incomplete = false;

    if ( M_left_team_name.empty() )
    {
        M_left_team_name = "null";
        incomplete = true;
    }

    if ( M_right_team_name.empty() )
    {
        M_right_team_name = "null";
        incomplete = true;
    }

    std::cout << M_left_team_name << " " << M_right_team_name << " "
              << M_left_score << " " << M_right_score;

    if ( M_left_penalty_taken > 0
         && M_right_penalty_taken > 0 )
    {
        std::cout << " " << M_left_penalty_score
                  << " " << M_right_penalty_score;
    }

    if ( ! incomplete
         && M_playmode != rcsc::rcg::PM_TimeOver )
    {
        if ( M_cycle % M_half_time == 0 // just a half time
             || ( ( M_cycle + 1 ) % M_half_time == 0
                  && ( ( M_cycle / M_half_time ) % 2 == 0 // even number halves
                       || ( ( M_cycle + 1 ) / M_half_time ) % 2 == 0 )
                  && M_left_score == M_right_score ) ) // draw game
        {

        }
        else
        {
            incomplete = true;
        }
    }

    if ( incomplete )
    {
        std::cout << " (incomplete match : cycle="
                  << M_cycle << ")";
    }

    std::cout << std::endl;

    return true;
}


/*-------------------------------------------------------------------*/
/*!

*/
bool
ResultPrinter::handleShow( const int time,
                           const rcsc::rcg::ShowInfoT & show )
{
    return true;
}

/*-------------------------------------------------------------------*/
/*!

*/
bool ResultPrinter::handleMsg( const int time,
                               const int board,
                               const std::string & msg )
{
    return true;
}

/*-------------------------------------------------------------------*/
/*!

*/
bool ResultPrinter::handlePlayMode( const int time,
                                    const rcsc::rcg::PlayMode pm )
{
    M_playmode = pm;

    return true;
}

/*-------------------------------------------------------------------*/
/*!

*/
bool ResultPrinter::handleTeam( const int time,
                                const rcsc::rcg::TeamT & team_l,
                                const rcsc::rcg::TeamT & team_r )
{
    M_left_team_name = team_l.name_;
    M_left_score = team_l.score_;
    M_left_penalty_taken = team_l.pen_score_ + team_l.pen_miss_;
    M_left_penalty_score = team_l.pen_score_;

    M_right_team_name = team_r.name_;
    M_right_score = team_r.score_;
    M_right_penalty_taken = team_r.pen_score_ + team_r.pen_miss_;
    M_right_penalty_score = team_r.pen_score_;

    return true;
}

/*-------------------------------------------------------------------*/
/*!

*/
bool
ResultPrinter::handleServerParam( const std::string & msg )
{
    return true;
}

/*-------------------------------------------------------------------*/
/*!

*/
bool
ResultPrinter::handlePlayerParam( const std::string & msg )
{
    return true;
}

/*-------------------------------------------------------------------*/
/*!

*/
bool
ResultPrinter::handlePlayerType( const std::string & msg )
{
    return true;
}

////////////////////////////////////////////////////////////////////////

void
usage( const char * prog )
{
    std::cerr << "Usage: " << prog << " <RcgFile>[.gz]"
              << std::endl;
}


////////////////////////////////////////////////////////////////////////

int
main( int argc, char** argv )
{
    if ( argc < 2 )
    {
        usage( argv[0] );
        return 1;
    }

    for ( int i = 1; i < argc; ++i )
    {
        if ( argv[i][0] == '-' )
        {
            continue;
        }

        rcsc::gzifstream fin( argv[i] );

        if ( ! fin.is_open() )
        {
            std::cerr << "Failed to open file : " << argv[i]
                      << std::endl;
            continue;
        }

        rcsc::rcg::ParserPtr parser = rcsc::rcg::make_parser( fin );

        if ( ! parser )
        {
            std::cerr << "Failed to create rcg parser for "
                      << argv[i]
                      << std::endl;
            continue;
        }

        // create rcg handler instance
        ResultPrinter printer;

        if ( ! parser->parse( fin, printer ) )
        {
            std::cerr << "Failed to parse [" << argv[i] << "]"
                      << std::endl;
        }

        fin.close();
    }

    return 0;
}
