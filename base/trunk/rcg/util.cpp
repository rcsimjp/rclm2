// -*-c++-*-

/*!
  \file util.cpp
  \brief utilities for rcg data Source File.
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
#include "config.h"
#endif

#include "types.h"

#include <cmath>

#ifdef HAVE_NETINET_IN_H
#include <netinet/in.h>
#endif

namespace rcsc {
namespace rcg {

/*-------------------------------------------------------------------*/
/*!

*/
int
nstohi( const Int16 val )
{
    return static_cast< int >( static_cast< Int16 >( ntohs( val ) ) );
}

/*-------------------------------------------------------------------*/
/*!

*/
Int16
hitons( const int val )
{
    return htons( static_cast< Int16 >( val ) );
}

/*-------------------------------------------------------------------*/
/*!

*/
bool
nstohb( const Int16 val )
{
    return static_cast< bool >( static_cast< Int16 >( ntohs( val ) ) );
}

/*-------------------------------------------------------------------*/
/*!

*/
Int16
hbtons( const bool val )
{
    return htons( val ? 1 : 0 );
}

/*-------------------------------------------------------------------*/
/*!

*/
double
nstohd( const Int16 val )
{
    return ( static_cast< double >( static_cast< Int16 >( ntohs( val ) ) )
             / SHOWINFO_SCALE );
}

/*-------------------------------------------------------------------*/
/*!

*/
Int16
hdtons( const double & val )
{
    return htons( static_cast< Int16 >( rint( val * SHOWINFO_SCALE ) ) );
}

/*-------------------------------------------------------------------*/
/*!

*/
double
nltohd( const Int32 & val )
{
    return ( static_cast< double >( static_cast< Int32 >( ntohl( val ) ) )
             / SHOWINFO_SCALE2 );
}

/*-------------------------------------------------------------------*/
/*!

*/
Int32
hdtonl( const double & val )
{
    return htonl( static_cast< Int32 >( rint( val * SHOWINFO_SCALE2 ) ) );
}

/*-------------------------------------------------------------------*/
/*!

*/
Int32
nstonl( const Int16 val )
{
    return ( static_cast< Int32 >
             ( htonl( static_cast< Int32 >( nstohd( val ) * SHOWINFO_SCALE2 ) ) ) );
}

/*-------------------------------------------------------------------*/
/*!

*/
Int16
nltons( const Int32 & val )
{
    return ( static_cast< Int16 >
             ( htons( static_cast< Int16 >( nltohd( val ) * SHOWINFO_SCALE ) ) ) );
}

} // end namespace
} // end namespace