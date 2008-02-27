// -*-c++-*-

/*!
  \file parser_v2.cpp
  \brief rcg v2 parser Source File.
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "parser_v2.h"

#include "handler.h"
#include "types.h"

#include <iostream>

#ifdef HAVE_NETINET_IN_H
#include <netinet/in.h>
#endif

namespace rcsc {
namespace rcg {

/*-------------------------------------------------------------------*/
/*!

*/
bool
ParserV2::parse( std::istream & is,
                 Handler & handler ) const
{
    // streampos must be the first point!!!
    is.seekg( 0 );

    if ( ! is.good() )
    {
        return false;
    }

    // skip header
    char header[4];
    is.read( header, 4 ); // read 'U', 'L', 'G', <version>

    // register log version
    handler.handleLogVersion( REC_VERSION_2 );

    // main loop
    while ( is.good() )
    {
        // read data
        if ( ! parseData( is, handler ) )
        {
            return false;
        }
    }

    if ( is.eof() )
    {
        return handler.handleEOF();
    }

    return false;
}

/*-------------------------------------------------------------------*/
/*!

*/
bool
ParserV2::parseData( std::istream & is,
                     Handler & handler ) const
{
    // chedk data mode.
    dispinfo_t info;
    is.read( reinterpret_cast< char* >( &info.mode ), sizeof( short ) );

    if ( ! is.good() )
    {
        if ( is.eof() )
        {
            return true;
        }
        return false;
    }

    // read each data block
    switch ( ntohs( info.mode ) ) {
    case NO_INFO:
        return true;
    case SHOW_MODE:
        is.read( reinterpret_cast< char* >( &info.body.show ),
                 sizeof( showinfo_t ) );
        if ( is.gcount() == sizeof( showinfo_t ) )
        {
            return handler.handleShowInfo( info.body.show );
        }
        break;
    case MSG_MODE:
        is.read( reinterpret_cast< char* >( &info.body.msg ),
                 sizeof( msginfo_t ) );
        if ( is.gcount() == sizeof( msginfo_t ) )
        {
            return handler.handleShowInfo( info.body.show );
        }

        break;
    case DRAW_MODE:
        is.read( reinterpret_cast< char* >( &info.body.draw ),
                 sizeof( drawinfo_t ) );
        if ( is.gcount() == sizeof( drawinfo_t ) )
        {
            return true;
        }
        break;
    default:
        std::cerr << __FILE__ << ':' << __LINE__
                  << " Unknown mode" << htons( info.mode )
                  << std::endl;;
        break;
    }

    return false;
}

} // end of namespace
} // end of namespace
