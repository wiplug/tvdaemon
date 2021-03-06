$(document).ready( ready );

function ready( )
{
  Menu( "EPG" );
  t = ServerSideTable( 'epg', 'tvd?c=tvdaemon&a=get_epg', 10 );
  t["columns"] = {
    ""         : [ "Start", start ],
    "channel"  : "Channel",
    "duration" : [ "Duration", duration ],
    "name"     : [ "Name", function ( event ) { return "<b>" + event["name"] + "</b><br/>" + event["description"] } ]
  };
  t["click"] = function( event ) {
    if( confirm( "Record " + this["name"] + " ?" ))
      getJSON( 'tvd?c=channel&a=schedule&channel_id=' + this["channel_id"] + "&event_id=" + this["id"], schedule );
  };
  t.load( );
}

function schedule( data, errmsg )
{
  if( !data )
    return false;
  return true;
}

function duration( event )
{
  d = event["duration"] / 60;
  return Math.floor( d ) + "'";
}

function start( event )
{
  start = "";
  start_hour = event["start_hour"];
  start_min  = event["start_min"];
  start_day  = event["start_day"];
  start_month  = event["start_month"];
  if( start_hour < 10 ) start_hour = "0" + start_hour;
  if( start_min < 10 ) start_min = "0" + start_min;
  if( start_day < 10 ) start_day = "0" + start_day;
  if( start_month < 10 ) start_month = "0" + start_month;

  if( event["start_istoday"] == 1 )
    start = start_hour + ":" + start_min;
  else
    start = start_day + "." + start_month + " " + start_hour + ":" + start_min;
  return start;
}

