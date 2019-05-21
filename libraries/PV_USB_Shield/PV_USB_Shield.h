#ifndef PV_USB_SHIELD
#define PV_USB_SHIELD

#include <Arduino.h>
#include <SoftwareSerial.h>

#define OUTPUT         Serial ///< Set the port to send the output from the USB shield.
#define SHOW_RESPONSES false  ///< Set if you want command responses from the USB shield to be sent to the OUTPUT port defined above.
//#define DEBUG_USB             ///< Uncomment for dubugging

/** \mainpage ProtoVoltaics USB Flash Drive Shield for Arduino
    \section intro_sec Introduction
    Thanks for taking a look at our shield! We value your feedback and contributions.  Contact us at
    <A HREF="mailto:support@protovoltaics.com">support@protovoltaics.com</A>.

    This Arduino shield has multiple USB type A ports to interface with USB flash drives.

    \section quick_start_sec Quick Start
        1. Download and extract the interface library for communicating with the shield.
           <A HREF="http://prods.protovoltaics.com/usb_shield/lib/PV_USB_Shield.zip">Download link for Arduino USB Flash Drive interface library.</A>
        2. Install the library.
           <A HREF="http://arduino.cc/en/Guide/Libraries" target="_blank">Instructions for installing an Arduino library.</A>
        3. Create a new Arduino sketch.
        4. Add the USB library to the sketch: Sketch | Import Library... | PV_USB_Shield
        5. Create a minimal program to write to a USB flash drive:
           <A HREF="http://prods.protovoltaics.com/usb_shield/examples/simple/USB_Shield_Minimal_Test.ino">Download the code below</A>
    \code
		#include <SoftwareSerial.h>
		#include <PV_USB_Shield.h>
		
		// Create our USB shield variable.
		// The jumper in the "TX" row on the shield is connected to the Arduino's D2 pin.
		// The jumper in the "RX" row on the shield is connected to the Arduino's D3 pin.
		// The default baud rate to the shield is 19200.
		PV_USB my_usb( 2, 3, 19200 );
		
		void setup() {
		  Serial.begin( 115200 );
		  Serial.println( "Starting up..." );
		  
		  // Start the USB shield.
		  my_usb.begin();
		  
		  // Open or create a new file on the USB flash drive connected to port to "USB-A2" on the shield.
		  my_usb.openToAppend( "new_file.txt" );
		  
		  // The "append" and "appendln" methods work just like "Serial.print" and "Serial.println".
		  my_usb.appendln( "Hello world" );
		  my_usb.appendln( 12345 );
		  my_usb.append( "254 in hex is 0x" );
		  my_usb.appendln( 254, HEX );
		  my_usb.append( "pi is almost " );
		  my_usb.appendln( 3.141592, 4 );
		  
		  // Always close the file when you are done with it to avoid data corruption.
		  my_usb.close();
		  
		  Serial.println( "Done writing to file." );
		}
		
		void loop() {
		  // See if there is any output coming from the USB shield.
		  char msg = my_usb.poll();
		  
		  // If there was any output, write it to the serial monitor.
		  if( msg ) {
		    Serial.write( msg );
		    // The shield uses carriage returns (\r) at the end of its replies.
		    // If we get a carriage return, tack on a new line to make things look pretty.
		    if( msg == '\r' ) Serial.write( '\n' );
		  }
		  
		  // If we enter a command manually on the serial monitor it must end with a carriage return.  
		  // Do not forget to select "carriage return" as the line ending character on the serial monitor!  
		  // The following will send our message from the serial monitor to the USB shield:
		  while( Serial.available() ) {
		    msg = Serial.read();
		    my_usb.sendRawChar( msg );
		  }
		}
    \endcode
        6. Use a computer to confirm that you have a file on the USB flash drive named "new_file.txt" with
           the correct contents:
	   \image html usb_minimal_output.png

    \section hw_sec Arduino USB Flash Drive Hardware Overview
    \subsection hw_com_sec Arduino to USB Shield Communication Pins
    You may want to use other pins to communicate with your Arduino USB flash drive shield.  You can change which 
    pins are used by changing the connections to the shield and altering the parameters used when you create your USB 
    shield variable.  

    The image below shows the default setting.  
    \image html USB_RX_TX.jpg
    You would use 
    \code
    	PV_USB my_usb( 2, 3, 19200 );
    \endcode
    for this hardware setting.  The first parameter to the PV_USB constructor is where the jumper is on the 
    shield's "TX" row.  The second parameter to the PV_USB constructor is where the jumper is on the shield's 
    "RX" row of headers.

    Current versions of the shield come with the connections to D2 and D3 hardwired: they are connected
    with the copper traces on the circuit board.  If you want to change the connections you will have to cut these small
    traces on the printed circuit board.  Then, you'll have to connect a jumper wire to the terminals that you 
    do want to use.  The image below shows the traces that make the default connections to D2 and D3:
    \image html rx_tx_con.JPG

    \subsection hw_usb_ports_sec USB Host Ports
    The shield has two USB type A ports, which allow the shield to act as a USB host and interact with USB flash 
    drives.  A USB flash drive on the port labeled "USB-A2" can be read from and written to using the software 
    commands described below.  A USB flash drive on the port labeled "USB-A1" <b>cannot</b> be read from or written 
    to using the software commands described below.
    
    <b>The "USB-A1" port is only used for data retrieval and firmware upgrades. Port "USB-A1" only performs 
    operations when there is another USB flash drive on port "USB-A2".</b>  If port "USB-A2" has no USB flash drive 
    in it then the shield will not do anything with the USB flash drive on the "USB-A1" port: it will not look for a 
    firmware update file.  If there is a USB flash drive on the "USB-A2" port, then when a USB flash drive is inserted 
    into the "USB-A1" port the following actions will take place:
        1. A new directory will be created at the root level on the flash drive on port "USB-A1".  The directory
           will be named "LOGS0001".  If a directory named "LOGS0001" already exists, then a directory named 
	   "LOGSxxxx" will be created where <i>xxxx</i> represents a four digit number for a directory 
	   that does not yet exist on the flash drive.
        2. All of the contents of the flash drive on port "USB-A2" will be copied over to the newly created 
           directory on port "USB-A1".
        3. If a new firmware file is present (named "USB_FWF.ROM") on the flash drive on port "USB-A1", then it will 
           be used to update the shield's firmware.

    The "USB-A2" port is used for reading and writing files using the software routines described in the software 
    section below.  When a USB flash drive is inserted into port "USB-A2" it first checks for a firmware updated file 
    (named "USB_FWF.ROM") and updates the shield's firmware if the version number in the file is different than the 
    version number of the shield's firmware.

    \subsection hw_firmware_sec USB Shield Firmware Changes
    The shield has a bootloader that can be used to change the firmware running on the shield.  To develop firmware 
    for the USB chip you will want to use the 
    <A HREF="http://www.ftdichip.com/Firmware/VNC2tools.htm#VNC2Toolchain" target="_blank">Vinculum II Toolchain from FTDI</A>.  
    To burn the new firmware on the USB chip you can use a 
    <A HREF="http://www.ftdichip.com/Products/Modules/DevelopmentModules.htm#VNC2%20Debugger-Programmer%20Module" target="_blank">VNC2 Debugger/Programmer Module</A> 
    or you can take the ROM file generated by the Vinculum II IDE, rename it "USB_FWF.ROM", place the file on a 
    USB memory stick, and insert the stick on port "USB-A2" of the shield.  Remember to change the version number 
    of your file in the Vinculum IDE: Build | Project | Options | Version Info.

    \subsection hw_bootload_sec Arduino Reprogramming from USB Flash Drive Pins
    <B>THE FOLLOWING FUNCTIONALITY DOES NOT EXIST!</B>

    The pins between the "USB-A1" socket and the row of "TX" and "RX" selection headers could be used to connect 
    the shield's IO3, IO2, and IO1 pins to the Arduino's D13, D12, and D11 pins.  This could potentially allow 
    the shield to reprogram the Arduino from a USB flash drive using the "Arduino as ISP" functionality.

    This feature is not implemented.  To accomplish this the firmware on the USB chip would have to be 
    modified to perform the ISP functionality when a flash drive is inserted.  This code would have to be written 
    using the 
    <A HREF="http://www.ftdichip.com/Firmware/VNC2tools.htm#VNC2Toolchain" target="_blank">Vinculum II Toolchain from FTDI</A>.  
    The firmware would then have to be uploaded to the USB chip on the shield.

    At the bottom of the shield, next to the "POWER" header, is a jumper to select which direction the "RESET" 
    pin from the Arduino is going.  If the jumper is in the "IN" position, then the Arduino's RESET pin will 
    act as an input to the USB shield and the shield will reset when the Arduino's RESET pin is triggered.  If 
    the jumper is in the "OUT" position, then the USB shield will be able to control the Arduino's reset line. 
    This might be useful/required if you desire to implement the capability described in this section.

    <B>THIS CAPABILITY IS NOT IMPLEMENTED!</B>

    \subsection hw_jumper_sec Jumper Pin Explanations
    The jumpers labeled "B1 ID" and "B2 ID" are used to connect the ID pads of the USB mini connectors to ground. 
    This will set the direction (host/slave) of the mini-USB ports.  mini-USB ports are not populated by default, 
    so you will never need to use these jumpers unless you are an expert hardware hacker.

    The jumpers labeled "LEVEL SHIFT BYPASS" will allow the outputs of the USB chip to bypass the voltage level 
    shifters on the USB shield.  Voltage shifters are present on the shield to allow the 3.3V output of the USB 
    chip to communicate with 5V Arduino's.  You will never need to use these jumpers unless you are an expert 
    hardware hacker.

    The jumper labeled "ICSP 5V" will allow the 5V supply of a connected 
    <A HREF="http://www.ftdichip.com/Products/Modules/DevelopmentModules.htm#VNC2%20Debugger-Programmer%20Module" target="_blank">VNC2 Debugger/Programmer Module</A> 
    to be directly connected to the 5V supply of the shield.  You will never need to use this jumper unless you 
    are an expert hardware hacker.  There is also an unpopulated set of pads on the back side of the shield that 
    you could use to connect a diode between the 5V supply of the VNC2 Debugger/Programmer Module and the 5V line 
    on the USB shield.

    The jumpers at the bottom of the shield, just above the "POWER" and "ANALOG IN" headers bring out all of the 
    input/output lines from the USB chip.  The pins closest to the Arduino headers are directly connected to the 
    USB chip's I/O lines.  The jumpers would allow you to connect those lines to the LED pads on the other side of 
    a given jumper.  This would allow you to drive LEDs based on different criteria (though to do this you would 
    have to alter the USB chip's firmware).  You will never need to use these jumpers unless you are an expert 
    hardware hacker.

    \subsection hw_icsp_sec FTDI Debugger-Programmer Module Interface
    Vias are present between the two USB type A sockets that can be used to connect to a 
    <A HREF="http://www.ftdichip.com/Products/Modules/DevelopmentModules.htm#VNC2%20Debugger-Programmer%20Module" target="_blank">VNC2 Debugger/Programmer Module</A>.
    The vias are labeled "USB ICSP".  The square via is pin 1.  This could be used to change the firmware on the 
    USB shield if needed.  You will never need to use this port unless you are an expert hardware hacker.


    \section sw_sec Arduino USB Flash Drive Software Overview
    Methods are available to perform common USB flash drive interactions.  You may need to alter the library to 
    perform certain tasks.  For further information you may want to refer to the 
    <A HREF="http://www.ftdichip.com/Firmware/Precompiled/UM_VinculumFirmware_V205.pdf" target="_blank">FTDI Vinculum Firmware User Manual</A>.
    \subsection sw_disk_sec File and Directory Methods
    The following methods follow the typical naming convention of the DOS terminal:
    - PV_USB::dir() - to list the files and folders in the current directory.
    - PV_USB::dir( char* ) - to list the file and its size in bytes.
    - PV_USB::cd( char* ) - to change the directory.
    - PV_USB::dump( char* ) - to write all of the contents of a file to the serial monitor.
    - PV_USB::mkdir( char* ) - to create a folder with the name given.
    - PV_USB::rmdir( char* ) - to delete the folder with the name given.
    - PV_USB::del( char* ) - to delete the file given.
    - PV_USB::openToAppend( char* ) - to open the given file for writing.  There is also a version that will set the file's creation timestamp.
    - PV_USB::append() - used just like Serial.print() to write to a previously opened file.
    - PV_USB::appendln() - used just like Serial.println() to write to a previously opened file.
    - PV_USB::openToRead( char* ) - to open the given file for reading.
    - PV_USB::readBytes( unsigned long ) - to write the given number of bytes in the previously opened file to the serial monitor.
    - PV_USB::seek( unsigned long ) - to move to a byte position within the previously opened file.
    - PV_USB::close() - to close a previously opened file.
    - PV_USB::rename( char*, char* ) - to rename a file.
    - PV_USB::freeSpace() - to get the available free space left on the drive.
    - PV_USB::diskInfo() - to get vendor and product information on the drive.
    - PV_USB::diskSerial() - to get the serial number of the drive.
    - PV_USB::diskLabel() - to get the logical volume label of the drive.
    - PV_USB::fileDates( char* ) - to get the creation, modify, and access dates on the file given. 

    There are also some low level methods available:
    - PV_USB::poll() - reads from the serial port connected to the USB shield and returns a byte if present. 
    - PV_USB::sendRawChar( char ) - sends a single character to the USB shield.
    - PV_USB::command( char* ) - sends a command to the USB shield.
    - PV_USB::waitResponse( boolean, long ) - waits for a response from the USB shield.
    - PV_USB::commandResponse( char*, boolean, long ) - sends a command and waits for the response.
    - PV_USB::fileCommand( char*, char* ) - executes a command with a given file/folder name as a parameter.
*/


/// ProtoVoltaics USB Class.
/** Class object for communicating with the ProtoVoltaics USB Shield for Arduino.
*/
class PV_USB : public Print {
  public:

    /// Class constructor.
    PV_USB( byte rx_pin = 2, byte tx_pin = 3, unsigned long baud_rate = 19200 );

    /// Open the communication connection to the USB shield.
    void begin();
    
    /// Poll the USB shield for new data.
    /** Checks to see if new data is available from the USB shield and returns the data if present.
     *  \return The value sent by the USB shield, 0 if no new data.
     */
    char poll();

    /// Routine to implement inheritance from Print superclass.
    /** This is used to count how many characters need to be written to the file since we have to 
     *  tell the shield how many characters we are going to send before we actually send them.
     *  \param msg An ignored message value.
     *  \return The value 1 is always returned.
     */
    size_t write( uint8_t msg );

    /// Used to send a character to the USB shield.
    /** If you want to send commands to the USB shield directly from the serial monitor you can use
     *   this function.  Many of the commands in the <A HREF="http://www.ftdichip.com/Firmware/Precompiled/UM_VinculumFirmware_V205.pdf" target="_blank">FTDI Vinculum Firmware User Manual</A> are implemented in the
     *   existing firmware.  Remember commands have to be terminated with a carriage return ('\\r').
     *   \param msg The character to send.
     */
    void sendRawChar( char msg );

    /// Used to send commands to the USB shield.
    /** Other member methods use this method to simplify the task of sending commands to the USB shield.
     *  \param c The command to send.
     */
    void command( char* c );

    /// Used to wait for a response from the USB shield.
    /** Other member methods use this method to simplify the task of waiting for responses from the USB shield.
     *  \param show If true then the response will be sent to the serial monitor.
     *  \param limit How long to wait for a repsonse before timing out.  This is a number of iterations to sit in a spin loop.
     *  \return True if a response was obtained, false otherwise.
     */
    boolean waitResponse( boolean show = SHOW_RESPONSES, long limit = 32000 );

    /// Used to send a command and wait for a response from the USB shield.
    /** Other member methods use this method to simplify the task of sending commands and waiting for responses
     *  from the USB shield.
     *  \param c The command to send the shield.
     *  \param show If true then the response will be sent to the serial monitor.
     *  \param limit How long to wait for a response before timing out.  This is a number of iterations to sit in a spin loop.
     */
    boolean commandResponse( char* c, boolean show = SHOW_RESPONSES, long limit = 32000 );

    /// Used to send a command that takes a file name or folder name as an parameter.
    /** Other member methods use this method to simplify the task of sending commands that take filename 
     *  parameters to the USB shield.
     *  \param cmd The command to send the shield.
     *  \param filename The file name or folder name to send as an argument.
     */
    void fileCommand( char* cmd, char* filename );

    /// Used to create a timestamp that the USB shield can use.
    /** Other methods use this method to create a 32-bit timestamp in the format used by the shield.
     *  \param year The year to give the timestamp.
     *  \param month The month to give the timestamp.
     *  \param day The day of the month to give the timestamp.
     *  \param hour The hour of the day to give the timestamp.
     *  \param minute The minute of the hour to give the timestamp.
     *  \param second The second to give the timestamp.
     *  \return The 32-bit value representing the timestamp in the format the shield wants it in.
     */
    unsigned long getTimestamp( int year, int month, int day, int hour, int minute, int second );
    
    /// Used to set the baud rate of the communication with the shield.
    /** If the baud rate given does not match an acceptable value, the baud rate will be set to 19200.
     *  \param newBaud The buad rate to set the communications to.
     *  \return The baud rate that the communications was set to.
     */
    unsigned long setBaud( unsigned long newBaud );

    /// Prints the version information for the shield and its firmware to the serial monitor.
    /** Prints the hardware and firmware version of the shield to the serial monitor.
     */
    void ver();

    /// Prints all files and folders in the current directory to the serial monitor.
    /** Lists all of the files and folders in the active folder to the serial monitor.
     */
    void dir();

    /// Prints the file given along with its size in bytes to the serial monitor.
    /** The file size is reported in hex with the least significant byte first.
     *  For example, when "$F2 $85 $03 $00" is returned it represents 0x000385F2 = 230898 bytes.
     *  \param filename The filename to conduct the search on.
     */
    void dir( char* filename );

    /// Changes the current directory.
    /** Change the current directory to another in the current directory.
     *  \param dirname The directory to change into.  Use ".." to go up to the parent directory.
     */
    void cd( char* dirname );

    /// Prints the contents of a file to the serial monitor.
    /** The contents of the given file are writen to the serial monitor.
     *  \param filename The file to print to the serial monitor.
     */
    void dump( char* filename );

    /// Create a new folder in the current directory.
    /** Make a folder with the name given in the currently active folder.
     *  \param dirname The name of the new folder the be created.
     */
    void mkdir( char* dirname );

    /// Create a new folder in the current directory and set the folder's creation timestamp.
    /** Make a folder with the name given in the currently active folder.
     *  \param dirname The name of the new folder the be created.
     *  \param year The year to use for the creation of the new folder.
     *  \param month The month to use for the creation of the new folder.
     *  \param day The day to use for the creation of the new folder.
     *  \param hour The hour to use for the creation of the new folder.
     *  \param minute The minute to use for the creation of the new folder.
     *  \param second The second to use for the creation of the new folder.
     */
    void mkdir( char* dirname, int year, int month, int day, int hour, int minute, int second );

    /// Delete a folder.
    /** Remove a folder from the currently active directory.
     *  \param dirname The folder to delete.
     */
    void rmdir( char* dirname );

    /// Delete a file.
    /** Remove a file from the currently active directory.
     *  \param filename The file to delete.
     */
    void del( char* filename );

    /// Open a file for writing.  
    /** Only one file can be open at a time.  You must open it, write to it, and then close it.  Writing 
     *  will start at the end of the file.  If the file does not exist it will be created.
     *  \param filename The file to write to.
     */
    void openToAppend( char* filename );

    /// Open a file for writing and set its creation timestamp.
    /** Only one file can be open at a time.  You must open it (openToAppend), write to it 
     *  (append or appendln), and then close it (close).  Writing 
     *  will start at the end of the file.  If the file does not exist it will be created.
     *  \param filename The file to write to.
     *  \param year The year to use as the file's creation timestamp.
     *  \param month The month to use as the file's creation timestamp.
     *  \param day The day to use as the file's creation timestamp.
     *  \param hour The hour to use as the file's creation timestamp.
     *  \param minute The minute to use as the file's creation timestamp.
     *  \param second The second to use as the file's creation timestamp.
     */
    void openToAppend( char* filename, int year, int month, int day, int hour, int minute, int second );

    /// Write to a previously opened file.
    /** This function works just like the Arduino's Serial.print() method except it writes to a file
     *  on the USB flash drive and not the serial monitor.  You must use openToAppend() before calling 
     *  this method.
     *  \param d The value to write to the open file.
     *  \param p If parameter d is a floating-point value, then p should be how many digits to write after
     *    the decimal point.  If parameter d is an integer, the p should be the base of the number.
     */
    template<typename T> void append( const T& d, int p ) {
      unsigned chars = print( d, p );
      com.print( "WRF " );
      com.print( chars );
      com.print( "\r" );
      com.print( d, p );
      waitResponse();
    }

    /// Write to a previously opened file.
    /** This function works just like the Arduino's Serial.print() method except it writes to a file
     *  on the USB flash drive and not the serial monitor.  You must use openToAppend() before calling 
     *  this method.
     *  \param d The value to write to the open file.
     */
    template<typename T> void append( const T& d ) {
      unsigned chars = print( d );
      com.print( "WRF " );
      com.print( chars );
      com.print( "\r" );
      com.print( d );
      waitResponse();
    }
    
    /// Write to a previously opened file and insert a new line.
    /** This function works just like the Arduino's Serial.println() method except it writes to a file
     *  on the USB flash drive and not the serial monitor.  You must use openToAppend() before calling 
     *  this method.
     *  \param d The value to write to the open file.
     *  \param p If parameter d is a floating-point value, then p should be how many digits to write after
     *    the decimal point.  If parameter d is an integer, the p should be the base of the number.
     */
    template<typename T> void appendln( const T& d, int p ) {
      unsigned chars = println( d, p );
      com.print( "WRF " );
      com.print( chars );
      com.print( "\r" );
      com.println( d, p );
      waitResponse();
    }
    
    /// Write to a previously opened file and insert a new line.
    /** This function works just like the Arduino's Serial.println() method except it writes to a file
     *  on the USB flash drive and not the serial monitor.  You must use openToAppend() before calling 
     *  this method.
     *  \param d The value to write to the open file.
     */
    template<typename T> void appendln( const T& d ) {
      unsigned chars = println( d );
      com.print( "WRF " );
      com.print( chars );
      com.print( "\r" );
      com.println( d );
      waitResponse();
    }
    
    /// Write a new line to a previously opened file.
    /** This function works just like the Arduino's Serial.println() method except it writes to a file
     *  on the USB flash drive and not the serial monitor.  You must use openToAppend() before calling 
     *  this method.
     */    
    void appendln();

    /// Open a file for reading.  
    /** Only one file can be open at a time.  
     *  \param filename The file to read from.
     */    
    void openToRead( char* filename );
    
    /// Open a file for reading and set its accessed timestamp.  
    /** Only one file can be open at a time.  
     *  \param filename The file to read from.
     *  \param year The year to use as the file's accessed timestamp.
     *  \param month The month to use as the file's accessed timestamp.
     *  \param day The day to use as the file's accessed timestamp.
     *  \param hour The hour to use as the file's accessed timestamp.
     *  \param minute The minute to use as the file's accessed timestamp.
     *  \param second The second to use as the file's accessed timestamp.
     */    
    void openToRead( char* filename, int year, int month, int day, int hour, int minute, int second );

    /// Print data from an open file to the serial monitor.
    /** Reads a given number of bytes from a previously opened file ( openToRead() ) and prints those bytes 
     *  to the serial monitor.
     *  \param bytes The number of bytes to print to the serial monitor.
     */
    void readBytes( unsigned long bytes );

    /// Close a file that was previously opened.
    void close();

    /// Rename a file or folder in the current folder.
    /** Changes the name of a file or folder in the currently active directory.
     *  \param oldname The current name of the file or directory.
     *  \param newname The desired name of the file or directory.
     */
    void rename( char* oldname, char* newname );

    /// Move the file cursor to a given byte position.
    /** Move to a given byte position in the currently active file.
     *  \param byteposition The byte position to move to in the file.
     */
    void seek( unsigned long byteposition );

    /// Prints the free space available on the disk to the serial monitor
    void freeSpace();

    /// Prints information about the disk to the serial monitor.
    void diskInfo();

    /// Prints the serial number of the disk to the serial monitor.
    void diskSerial();

    /// Prints the logical volume label of the disk to the serial monitor.
    void diskLabel();

    /// Prints the dates associated with the file to the serial monitor.
    /** Prints the creation time, modified time, and accessed time of the file to the serial monitor.
     *  \param filename The file to report the informaiton on.
     */
    void fileDates( char* filename );

  private:
    byte rx;             ///< The Arduino's receive pin for communicating with the USB shield.
    byte tx;             ///< The Arduino's transmit pin for communicating with the USB shield.
    unsigned long baud;  ///< The baud rate to use for communicating with the USB shield.

    SoftwareSerial com;  ///< The communication object used to talk to the USB shield.

    char openfile[13];   ///< The name of the currently open file.
};

#endif
