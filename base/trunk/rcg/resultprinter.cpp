// -*-c++-*-

/*!
  \file resultprinter.cpp
  \brief game result printer from rcg file
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
#include "config.h"
#endif

#include "gzfstream.h"
#include "factory.h"
#include "handler.h"
#include "util.h"

#include <cmath>
#include <cstring>
#include <iostream>
#include <fstream>
#include <string>

#ifdef HAVE_NETINET_IN_H
#include <netinet/in.h>
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

    static const double GOAL_WIDTH;
    static const double GOAL_POST_RADIUS;

    static const double BALL_SIZE;

    rcsc::PlayMode M_playmode;

    std::string M_left_team_name; //!< left teamname string
    std::string M_right_team_name; //!< right teamname string

    int M_left_score; //!< left team score
    int M_right_score; //!< right team score

    int M_left_penalty_taken; //!< total number of left team penalty trial
    int M_right_penalty_taken; //!< total number of left team penalty trial

    int M_left_penalty_score; //!< left team penalty kick score
    int M_right_penalty_score; //!< left team penalty kick score

    rcsc::SideID M_last_penalty_taker_side;

public:

    ResultPrinter();

    virtual
    bool handleDispInfo( const rcsc::rcg::dispinfo_t & disp );

    virtual
    bool handleShowInfo( const rcsc::rcg::showinfo_t & show );

    virtual
    bool handleShortShowInfo2( const rcsc::rcg::short_showinfo_t2 & show );

    virtual
    bool handleMsgInfo( short, const std::string & )
      {
          return true;
      }

    virtual
    bool handlePlayMode( char playmode );

    virtual
    bool handlePlayerType( const rcsc::rcg::player_type_t & )
      {
          return true;
      }
    virtual
    bool handleServerParam( const rcsc::rcg::server_params_t & )
      {
          return true;
      }
    virtual
    bool handlePlayerParam( const rcsc::rcg::player_params_t & )
      {
          return true;
      }

    virtual
    bool handleTeamInfo( const rcsc::rcg::team_t & team_left,
                         const rcsc::rcg::team_t & team_right );

    //! handle the end of file
    virtual
    bool handleEOF();

private:

    bool crossGoalLine( const Point & ball_pos,
                        const Point & prev_ball_pos );

    void checkFinalPenaltyGoal( const Point & ball_pos );

};

const double ResultPrinter::PITCH_LENGTH = 105.0;
const double ResultPrinter::PITCH_WIDTH = 68.0;

const double ResultPrinter::GOAL_WIDTH = 14.02;
const double ResultPrinter::GOAL_POST_RADIUS = 0.06;

const double ResultPrinter::BALL_SIZE = 0.085;

/*-------------------------------------------------------------------*/
/*!
  constructor
*/
ResultPrinter::ResultPrinter()
    : M_playmode( rcsc::PM_Null )
    , M_left_team_name( "" )
    , M_right_team_name( "" )
    , M_left_score( 0 )
    , M_right_score( 0 )
    , M_left_penalty_taken( 0 )
    , M_right_penalty_taken( 0 )
    , M_left_penalty_score( 0 )
    , M_right_penalty_score( 0 )
    , M_last_penalty_taker_side( rcsc::NEUTRAL )
{

}

/*-------------------------------------------------------------------*/
/*!

*/
bool
ResultPrinter::handleDispInfo( const rcsc::rcg::dispinfo_t & disp )
{
    return handleShowInfo( disp.body.show );
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

    double x = PITCH_LENGTH*0.5 + BALL_SIZE;
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

    return ( std::fabs( y_intercept ) <= ( GOAL_WIDTH*0.5 + GOAL_POST_RADIUS ) );
}

/*-------------------------------------------------------------------*/
/*!

*/
void
ResultPrinter::checkFinalPenaltyGoal( const Point & ball_pos )
{
    static Point s_prev_ball_pos;

    if ( M_playmode == rcsc::PM_TimeOver
         && crossGoalLine( ball_pos, s_prev_ball_pos ) )
    {
        if ( M_last_penalty_taker_side == rcsc::LEFT )
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
    if ( M_last_penalty_taker_side != rcsc::NEUTRAL )
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
    if ( M_last_penalty_taker_side != rcsc::NEUTRAL )
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
    M_playmode = static_cast< rcsc::PlayMode >( playmode );

    switch ( M_playmode ) {
    case rcsc::PM_PenaltySetup_Left:
        ++M_left_penalty_taken;
        M_last_penalty_taker_side = rcsc::LEFT;
        break;
    case rcsc::PM_PenaltySetup_Right:
        ++M_right_penalty_taken;
        M_last_penalty_taker_side = rcsc::RIGHT;
        break;
    case rcsc::PM_PenaltyMiss_Left:
        break;
    case rcsc::PM_PenaltyMiss_Right:
        break;
    case rcsc::PM_PenaltyScore_Left:
        ++M_left_penalty_score;
        break;
    case rcsc::PM_PenaltyScore_Right:
        ++M_right_penalty_score;
        break;
    case rcsc::PM_TimeOver:
        break;
    default:
        break;
    }

    return true;
}

/*-------------------------------------------------------------------*/
/*!
  read team name & score.
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
    if ( M_left_team_name.empty() )
    {
        M_left_team_name = "null";
    }
    if ( M_right_team_name.empty() )
    {
        M_right_team_name = "null";
    }

    std::cout << M_left_team_name << " " << M_right_team_name << " "
              << M_left_score << " " << M_right_score;

    if ( M_left_penalty_taken > 0
         && M_right_penalty_taken > 0 )
    {
        std::cout << " " << M_left_penalty_score
                  << " " << M_right_penalty_score;
    }

    std::cout << std::endl;

    return true;
}


////////////////////////////////////////////////////////////////////////

void
usage()
{
    std::cerr << "Usage: rclmresultprinter <RcgFile>[.gz]"
              << std::endl;
}


////////////////////////////////////////////////////////////////////////

int
main( int argc, char** argv )
{
    if ( argc < 2 )
    {
        usage();
        return 1;
    }

    for ( int i = 1; i < argc; ++i )
    {
        if ( argv[i][0] == '-' )
        {
            continue;
        }

        rcsc::gzifstream fin( argv[i] );
        //std::ifstream fin( argv[i] );

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
