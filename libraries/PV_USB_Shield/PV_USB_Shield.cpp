#include "PV_USB_Shield.h"

PV_USB::PV_USB( byte rx_pin, byte tx_pin, unsigned long baud_rate ) 
  : rx( rx_pin ), tx( tx_pin ), baud( baud_rate ), com( rx_pin, tx_pin )
{
  openfile[0] = '\0';
}

void PV_USB::begin() {
  com.begin( baud );
}

char PV_USB::poll() {
  char msg;
  if( com.available() ) {
    msg = com.read();
    return msg;
  }
  return 0;
}

size_t PV_USB::write( uint8_t msg ) {
  return 1;
}

void PV_USB::sendRawChar( char msg ) {
#ifdef DEBUG_USB
  OUTPUT.print( "Writing ( " );
  OUTPUT.print( ((char)(msg)) );
  OUTPUT.print( " | " );
  OUTPUT.print( ((uint8_t)(msg)) );
  OUTPUT.println( " ) " );
#endif
  com.write( msg );
}

void PV_USB::command( char* c ) {
  byte i = 0;
  while( c[i] != '\0' && i < 32 ) {
    sendRawChar( c[i] );
    i++;
  }
  sendRawChar( '\r' );
}

boolean PV_USB::commandResponse( char* c, boolean show, long limit ) {
  command( c );
  return waitResponse( show, limit );
}

boolean PV_USB::waitResponse( boolean show, long limit ) {
  char response = '\0';
  long loops;

  for( loops = 0; loops < limit && response != '\r'; loops++ ) {
    response = poll();
    if( response && show ) {
      OUTPUT.write( response );
      if( response == '\r' ) OUTPUT.write( '\n' );
    }
  }

#ifdef DEBUG_USB
  OUTPUT.print( "waitResponse did " );
  OUTPUT.print( loops );
  OUTPUT.print( " loops\n" );
#endif
  return loops < limit;
}

unsigned long PV_USB::setBaud( unsigned long newBaud ) {
  char cmd[12] = "SBD $9C8000";

  switch( newBaud ) {
    case( 300 ):
      cmd[5] = '1';
      cmd[6] = '0';
      cmd[7] = '2';
      cmd[8] = '7';
      break;
    case( 600 ):
      cmd[5] = '8';
      cmd[6] = '8';
      cmd[7] = '1';
      cmd[8] = '3';
      break;
    case( 1200 ):
      cmd[5] = 'C';
      cmd[6] = '4';
      cmd[7] = '0';
      cmd[8] = '9';
      break;
    case( 2400 ):
      cmd[5] = 'E';
      cmd[6] = '2';
      cmd[7] = '0';
      cmd[8] = '4';
      break;
    case( 4800 ):
      cmd[5] = '7';
      cmd[6] = '1';
      cmd[7] = '0';
      cmd[8] = '2';
      break;
    case( 9600 ):
      cmd[5] = '3';
      cmd[6] = '8';
      cmd[7] = '4';
      cmd[8] = '1';
      break;
    case( 19200 ):
    default:
      newBaud = 19200;
      cmd[5] = '9';
      cmd[6] = 'C';
      cmd[7] = '8';
      cmd[8] = '0';
      break;
    case( 38400 ):
      cmd[5] = '4';
      cmd[6] = 'E';
      cmd[7] = 'C';
      cmd[8] = '0';
      break;
    case( 57600 ):
      cmd[5] = '3';
      cmd[6] = '4';
      cmd[7] = 'C';
      cmd[8] = '0';
      break;
    case( 115200 ):
      cmd[5] = '1';
      cmd[6] = 'A';
      cmd[7] = '0';
      cmd[8] = '0';
      break;
    case( 230400 ):
      cmd[5] = '0';
      cmd[6] = 'D';
      cmd[7] = '0';
      cmd[8] = '0';
      break;
    case( 460800 ):
      cmd[5] = '0';
      cmd[6] = '6';
      cmd[7] = '4';
      cmd[8] = '0';
      break;
    case( 921600 ):
      cmd[5] = '0';
      cmd[6] = '3';
      cmd[7] = '8';
      cmd[8] = '0';
      break;
    case( 1000000 ):
      cmd[5] = '0';
      cmd[6] = '3';
      cmd[7] = '0';
      cmd[8] = '0';
      break;
    case( 1500000 ):
      cmd[5] = '0';
      cmd[6] = '2';
      cmd[7] = '0';
      cmd[8] = '0';
      break;
    case( 2000000 ):
      cmd[5] = '0';
      cmd[6] = '1';
      cmd[7] = '0';
      cmd[8] = '0';
      break;
    case( 3000000 ):
      cmd[5] = '0';
      cmd[6] = '0';
      cmd[7] = '0';
      cmd[8] = '0';
      break;
  }

  commandResponse( cmd );
  com.begin( newBaud );
  waitResponse();

  baud = newBaud;
  return newBaud;
}

void PV_USB::ver() {
  commandResponse( "FWV", true );
  waitResponse( true );
  waitResponse( true );
  waitResponse( true );
}

void PV_USB::dir() {
  command( "DIR" );
  while( waitResponse( true ) );
}

void PV_USB::fileCommand( char* cmd, char* filename ) {
  int i;
  char full_cmd[17];
  int cmd_length;
  for( i = 0; cmd[i] != '\0'; i++ ) {
    full_cmd[i] = cmd[i];
  }
  full_cmd[i] = ' ';
  cmd_length = i + 1;

  for( i = 0; i < 12 && filename[i] != '\0'; i++ ) {
    full_cmd[i+cmd_length] = filename[i];
  }
  full_cmd[i+cmd_length] = '\0';
  commandResponse( full_cmd );
}

void PV_USB::dir( char* filename ) {
  fileCommand( "DIR", filename );
}

void PV_USB::cd( char* dirname ) {
  fileCommand( "CD", dirname );
}

void PV_USB::dump( char* filename ) {
  fileCommand( "RD", filename );
}

void PV_USB::mkdir( char* dirname ) {
  fileCommand( "MKD", dirname );
}

void PV_USB::del( char* filename ) {
  fileCommand( "DLF", filename );
}

unsigned long PV_USB::getTimestamp( int year, int month, int day, int hour, int minute, int second ){
  union mix_t {
    unsigned long out;
    struct dt_bit_t {
      int second2:5;
      int minute:6;
      int hour:5;
      int day:5;
      int month:4;
      int year:7;
    } in;
  } mix;

  mix.in.second2 = second / 2;
  mix.in.minute = minute;
  mix.in.hour = hour;
  mix.in.day = day;
  mix.in.month = month;
  mix.in.year = year - 1980;

  return mix.out;
}

void PV_USB::mkdir( char* dirname, int year, int month, int day, int hour, int minute, int second ) {
  unsigned long timestamp = getTimestamp( year, month, day, hour, minute, second );

  com.print( "MKD " );
  com.print( dirname );
  com.print( " " );
  com.print( timestamp );
  com.print( "\r" );
  waitResponse();
}

void PV_USB::rmdir( char* dirname ) {
  fileCommand( "DLD", dirname );
}

void PV_USB::openToAppend( char* filename ) {
  int i;
  for( i = 0; i < 13 && filename[i] != '\0'; i++ ) {
    openfile[i] = filename[i];
  }
  openfile[i] = '\0';
  fileCommand( "OPW", filename );
}

void PV_USB::openToAppend( char* filename, int year, int month, int day, int hour, int minute, int second ) {
  unsigned long timestamp = getTimestamp( year, month, day, hour, minute, second );

  int i;
  for( i = 0; i < 13 && filename[i] != '\0'; i++ ) {
    openfile[i] = filename[i];
  }
  openfile[i] = '\0';

  com.print( "OPW " );
  com.print( filename );
  com.print( " " );
  com.print( timestamp );
  com.print( "\r" );
  waitResponse();
}

void PV_USB::appendln() {
  unsigned chars = println();
  com.print( "WRF " );
  com.print( chars );
  com.print( "\r" );
  com.println();
  waitResponse();
}

void PV_USB::close() {
  fileCommand( "CLF", openfile );
  openfile[0] = '\0';
}

void PV_USB::openToRead( char* filename ) {
  int i;
  for( i = 0; i < 13 && filename[i] != '\0'; i++ ) {
    openfile[i] = filename[i];
  }
  openfile[i] = '\0';
  fileCommand( "OPR", filename );
}

void PV_USB::openToRead( char* filename, int year, int month, int day, int hour, int minute, int second ) {
  unsigned long timestamp = getTimestamp( year, month, day, hour, minute, second );

  int i;
  for( i = 0; i < 13 && filename[i] != '\0'; i++ ) {
    openfile[i] = filename[i];
  }
  openfile[i] = '\0';

  com.print( "OPR " );
  com.print( filename );
  com.print( " " );
  com.print( timestamp );
  com.print( "\r" );
  waitResponse();
}

void PV_USB::readBytes( unsigned long bytes ) {
  if( openfile[0] == '\0' ) return;
  com.print( "RDF " );
  com.print( bytes );
  com.print( "\r" );
  waitResponse( true );
}

void PV_USB::rename( char* oldname, char* newname ) {
  com.print( "REN " );
  com.print( oldname );
  com.print( " " );
  com.print( newname );
  com.print( "\r" );
}

void PV_USB::seek( unsigned long byteposition ) {
  com.print( "SEK " );
  com.print( byteposition );
  com.print( "\r" );
}

void PV_USB::freeSpace() {
  commandResponse( "FSE", true, 1000000 );
}

void PV_USB::diskInfo() {
  command( "IDD" );
  while( waitResponse( true ) );
}

void PV_USB::diskSerial() {
  commandResponse( "DSN", true );
}

void PV_USB::diskLabel() {
  commandResponse( "DVL", true );
}

void PV_USB::fileDates( char* filename ) {
  fileCommand( "DIRT", filename );
}



