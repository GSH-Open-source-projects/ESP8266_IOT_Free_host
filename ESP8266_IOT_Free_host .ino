/*
 * Testing 
 */

// Libraries 
#include <ESP8266HTTPClient.h>
#include<ESP8266WiFi.h>

void setup()
{
  Serial.begin(115200);
  WiFi.begin(ssid,password);  // add ssid and password here

  while(WiFi.status() !=WL_CONNECTED)
  {
    delay(500);
    Serial.println("Waiting for connection");
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
