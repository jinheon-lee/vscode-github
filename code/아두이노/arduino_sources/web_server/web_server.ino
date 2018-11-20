#include <SPI.h>
#include <Ethernet.h>
 
// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192,168,219,161);
 
// Initialize the Ethernet server library
// with the IP address and port you want to use 
// (port 80 is default for HTTP):
EthernetServer server(80);

// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
int led = 3;
String get_method_string = String(20);
 
void setup() 
{
   // initialize the digital pin as an output.
   pinMode(led, OUTPUT);  
   digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  
 
   // Open serial communications and wait for port to open:
   Serial.begin(9600);
   
   while (!Serial) {
     ; // wait for serial port to connect. Needed for Leonardo only
    }
 

   // start the Ethernet connection and the server:
   Ethernet.begin(mac, ip);
   server.begin();
   
   Serial.print("server is at ");
   Serial.println(Ethernet.localIP());
 
 } 

void loop() {
   // listen for incoming clients
   EthernetClient client = server.available();
   
   if (client) 
   {
     Serial.println("new client");
     // an http request ends with a blank line
     boolean currentLineIsBlank = true;
     while (client.connected()) 
     {
       if (client.available()) 
       {
         char c = client.read();
         
         if( get_method_string.length() < 30 )
             get_method_string.concat(c);
             
         Serial.write(c);
         // if you've gotten to the end of the line (received a newline
         // character) and the line is blank, the http request has ended,
         // so you can send a reply
         if (c == '\n' && currentLineIsBlank) {
           // send a standard http response header
           client.println("HTTP/1.1 200 OK");
           client.println("Content-Type: text/html");
           client.println("Connection: close");  // the connection will be closed after completion of the response       
           client.println();
           
           client.println("<!DOCTYPE HTML>");
           client.println("<html>");
           client.println("<head><title>Arduino LED ON/OFF Test</title></head>");
           client.println("<body>");
           
           client.println("<br />Arduino LED(Pin3) ON/OFF Test<br /><br /><br />");
           client.println("<form method=\"get\" name=\"led\">");
           client.println("<input type=\"submit\" name=\"button1\" value=\"LED ON\" />");
           client.println("<input type=\"submit\" name=\"button2\" value=\"LED OFF\" />");
           client.println("</form>");
           
           client.println("</body>");
           client.println("</html>");
           
           if(get_method_string.indexOf("button1=LED+ON")!=-1)
           {
               digitalWrite(led, HIGH);  // LED ON
               Serial.println("=== LED ON");
           }
           else
           {
               digitalWrite(led, LOW);  // LED OFF
               Serial.println("=== LED OFF");
           }           
           
           Serial.println("get_method_string = " + get_method_string);
           get_method_string = "";
           
           break;
         }
         if (c == '\n') 
         {
           // you're starting a new line
           currentLineIsBlank = true;
         } 
        else if (c != '\r') 
        {
           // you've gotten a character on the current line
           currentLineIsBlank = false;
         }
       }
     }
     // give the web browser time to receive the data
     delay(1);
     // close the connection:
     client.stop();
     Serial.println("client disconnected");        
   }
 }

