// -*-c++-*-

/*!
  \file factory.cpp
  \brief parser factory utility Source File.
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

#include "factory.h"

#include "parser_v1.h"
#include "parser_v2.h"
#include "parser_v3.h"

#include "types.h"

namespace rcsc {
namespace rcg {

/*-------------------------------------------------------------------*/
/*!

*/
ParserPtr
make_parser( std::istream & is )
{
    char header[5];
    int version = REC_OLD_VERSION;
    ParserPtr p;

    is.read( header, 4 ); // read 'U', 'L', 'G', <version>

    if ( is.gcount() != 4 )
    {
        return p;
    }

    //std::cerr << "rcg header ["
    //          << header[0] << header[1] << header[2] << (int)header[3]
    //          << "]"
    //          << std::endl;
    if ( header[0] == 'U'
         && header[1] == 'L'
         && header[2] == 'G' )
    {
        version = static_cast< int >( header[3] );
    }

    switch ( version ) {
    case REC_OLD_VERSION:
        p = ParserPtr( new ParserV1 );
        break;
    case REC_VERSION_2:
        p = ParserPtr( new ParserV2 );
        break;
    case REC_VERSION_3:
        p = ParserPtr( new ParserV3 );
        break;
    default:
        // null pointer
        break;
    }

    return p;
}

} // end of namespace
} // end of namespace
