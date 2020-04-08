/*
 * Testing 
 */

// Libraries 
#include <ESP8266HTTPClient.h>
#include<ESP8266WiFi.h>
const char* ssid = "TP-LINK_4AD2"; 
const char* password = "9155253009";
#define WIFI_TX_POWER 20.5 //Limiting output power 0 - 20.5 dBm (Router setting = 20db)

void setup()
{
  Serial.begin(115200);
  
  
  int n = WiFi.scanNetworks();
  if (n == 0){
    Serial.println("No networks found");}
  else
  {
    Serial.print(n);
    Serial.println(" networks found");
    Serial.println("-------------------------------");
    for (int i = 0; i < n; ++i)
    {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      if (WiFi.encryptionType(i) == 2) Serial.print("WPA ");
      if (WiFi.encryptionType(i) == 4) Serial.print("WPA2");
      if (WiFi.encryptionType(i) == 5) Serial.print("WEP ");
      if (WiFi.encryptionType(i) == 8) Serial.print("AUTO");
      if (WiFi.encryptionType(i) == 7) Serial.print("OPEN");
      Serial.print(" | ");
      Serial.print(WiFi.SSID(i)); 
      Serial.print(" | ");
      Serial.print(WiFi.RSSI(i));
      Serial.println("dBm");

      delay(20);     
     }
    Serial.println("-------------------------------");
    Serial.println("");
    }
  //end scan for all Networks

  //SSID connection param
  WiFi.setOutputPower(WIFI_TX_POWER);
  WiFi.mode(WIFI_STA); //set mode to WIFI_AP, WIFI_STA, WIFI_AP_STA or WIFI_OFF.  
  WiFi.hostname("ESP12-roomThermostat");
  WiFi.begin(ssid, password);

  

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("Connected...");
  delay(1000);
  if (WiFi.status() ==WL_CONNECTED)
  {
    Serial.println("Wi-Fi Connected!");
  }
  delay(2000);
  Serial.println("Sending message to server espcomm");
  delay(5000);
  int res=sendmessage("Hi,Server");
  delay(1000);
  if (res==1)
  {
    Serial.println("Send Successfully");
  }
  else
  {
    Serial.println("Error on Server side or client side.");
  }
  
}

void loop()
{
  String data=receivelastmessage();
  int response=validatemessage(data);
  if (response==1)
  {
    Serial.print("Message Received from client : ");
    Serial.println(data);
    handleresponse(data);
  }
}

String receivelastmessage()
{
  String lastmessage="";
  if(WiFi.status()==WL_CONNECTED)
  {
    HTTPClient http;
    String url="https://gsh123.herokuapp.com/datastorage.txt";
    http.begin(url);
    http.addHeader("Content-Type","text/plain");
    int httpCode=http.GET();
    String data=http.getString();
    lastmessage=getlastline(data);
    http.end();
  }
  else
  {
    lastmessage="";
  }
  return lastmessage;
}

String getlastline(String str)
{
  String s="";
  int len=str.length();
  for (int i=len-2;i>=0;i--)
  {
    if (str[i]=='\n')
    {
      break;
    }
    else
    {
      s=s+str[i];
    }
  }
  String rs="";
  for (int i=s.length()-1;i>=0;i--)
  {
    rs=rs+s[i];
  }
  return rs;
}

int validatemessage(String message)
{
  String sender="";
  for (int i=0;i<message.length();i++)
  {
    if (message[i]==':')
    {
      break;
    }
    else
    {
      sender=sender+message[i];
    }
  }
  if (sender=="client")
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

void handleresponse(String message)
{
  Serial.println("Sending reply to client.");
  String mes="";
  for (int i=7;i<message.length();i++)
  {
    mes=mes+message[i];
  }
  if (mes=="hello,server")
  {
    int response=sendmessage("server:hello,client");
    if (response==1)
    {
      Serial.println("Replied Successfully.");
    }
    else
    {
      Serial.println("Error! Check Internet Connection or Server Error!");
    }
  }
}

int sendmessage(String d)
{
  int sres;
  int net;
  if (WiFi.status()==WL_CONNECTED)
  {
    HTTPClient http;
    String url="https://gsh123.herokuapp.com/writefile.php?data="+d;
    http.begin(url);
    http.addHeader("Content-Type","text/plain");
    int httpCode=http.GET();
    String payload=http.getString();
    Serial.println("While sending I received this from server : "+payload);
    if (payload=="SUCCESS. Data written in file.")
    {
      sres=1;
    }
    else
    {
      sres=0;
    }
    http.end();
    net=1;
  }
  else
  {
    Serial.println("Internet Problem!");
    net=0;
  }
  return (net && sres);
}
