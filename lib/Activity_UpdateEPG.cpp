/*
 *  tvdaemon
 *
 *  Activity_UpdateEPG class
 *
 *  Copyright (C) 2013 André Roth
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "Activity_UpdateEPG.h"

#include "Log.h"
#include "Frontend.h"
#include "Channel.h"

#include "descriptors/eit.h"
#include "descriptors/mgt.h"
#include "dvb-scan.h"
#include "dvb-demux.h"

Activity_UpdateEPG::Activity_UpdateEPG( ) : Activity( )
{
}

Activity_UpdateEPG::~Activity_UpdateEPG( )
{
}

std::string Activity_UpdateEPG::GetName( ) const
{
  std::string t = "Update EPG";
  if( transponder )
    t += " - " + transponder->toString( );
  return t;
}


bool Activity_UpdateEPG::Perform( )
{
  int time = 5;
  int fd_demux;

  if(( fd_demux = frontend->OpenDemux( )) < 0 )
  {
    frontend->LogError( "unable to open adapter demux" );
    goto fail;
  }
  if( transponder->HasMGT( ))
  {
    frontend->Log( "Reading MGT" );
    struct dvb_table_mgt *mgt = NULL;
    dvb_read_section( frontend->GetFE( ), fd_demux, DVB_TABLE_MGT, 0x1FFB, (uint8_t **) &mgt, time );
    if( mgt )
    {
      dvb_table_mgt_print( frontend->GetFE( ), mgt );
      const struct dvb_table_mgt_table *table = mgt->table;
      while( table )
      {
        if( table->type >= 0x200 and table->type < 0x300 )
        {
          frontend->Log( "Reading EIT %x", table->type );
          struct dvb_table_eit *eit = NULL;
          dvb_read_section( frontend->GetFE( ), fd_demux, 0xCB, table->pid, (uint8_t **) &eit, time );
          if( eit )
          {
            //transponder->ReadEPG( eit->event );
            dvb_table_eit_print( frontend->GetFE( ), eit );
            dvb_table_eit_free( eit );
          }

        }
        table = table->next;
      }
    }
    dvb_dmx_close( fd_demux );
    return true;
  }
  else
  {
    frontend->Log( "Reading EIT" );
    struct dvb_table_eit *eit = NULL;
    dvb_read_section( frontend->GetFE( ), fd_demux, DVB_TABLE_EIT_SCHEDULE, DVB_TABLE_EIT_PID, (uint8_t **) &eit, time );
    if( eit )
    {
      transponder->ReadEPG( eit->event );
      dvb_table_eit_free( eit );
    }

    dvb_dmx_close( fd_demux );
    return eit != NULL;
  }

fail:
  return false;
}
