/*
 *  tvdaemon
 *
 *  DVB Port class
 *
 *  Copyright (C) 2012 André Roth
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

#ifndef _Port_
#define _Port_

#include "ConfigObject.h"
#include "RPCObject.h"

#include <stdint.h>

class Frontend;
class Transponder;
class Source;
class Activity;

class Port : public ConfigObject, public RPCObject
{
  public:
    Port( Frontend &frontend, int config_id, std::string name = "Default Port", int id = 0 );
    Port( Frontend &frontend, std::string configfile );
    virtual ~Port( );

    virtual bool SaveConfig( );
    virtual bool LoadConfig( );

    int GetPortNum( ) { return port_num; }
    void SetSource( Source *source ) { this->source = source; }
    Source *GetSource( ) { return source; }

    bool Scan( Activity &act );
    bool ScanEPG( Activity &act );

    // RPC
    void json( json_object *entry ) const;
    bool RPC( const HTTPRequest &request, const std::string &cat, const std::string &action );

    bool Tune( Activity &act );

  private:
    Frontend &frontend;
    std::string name;
    int port_num;
    Source *source;

};

#endif
