

# ESP8266 Communication With Server and ESP8266
[![enter image description here][7]][7]



This is a version of [ESP8266 Communication With Server and ESP8266Hubot](https://www.instructables.com/id/ESP8266-Communication-With-Server-and-ESP8266/), which is designed to be deployed on [Heroku](http://www.heroku.com) with the The ESP8266 in Arduino IDE.

## Instructions:

#### Step 1: Creating Web Server

The first step is to create a WEB SERVER and we will  **[ Sing Up in Heroku](https://signup.heroku.com/)** for this. Go to [Heroku][1] and create an account. You will need to follow the instructions and create your website. The website provides you an easy interface where you can choose, drag and drop things to create your first website. Most importantly, create your domain name and remember it. You will get Password for your domain name, do remember that also.

Next step is to create a model on our desk which we will follow while communication to Web Server.

MODEL

There will be a text file with name “datastorage.txt”. We will communicate with this file only. I have two ESP8266 and both are needed to communicate with each other. Therefore, instead of communicating directly, Server will act as an intermediate and both ESPs will communicate with Server. ESP8266 can read the data from the file as well write the data into the file. Each ESP connected to the server will add its key while sending data to file. In addition, this key will be used to recognize the ESP, which wants to communicate.

We need PHP scripts to write data to the file. We do not need PHP script to read data from a file.

Read and Write

I thought it would be very easy for anyone if we can make a custom URL that can be used to write data in the file as well read from the file.

URLs:

Let say:

Your domain name is **[http://gsh123.herokuapp.com/][2]**

Filename is datastorage.txt

URL to access file contents [**http://gsh123.herokuapp.com/datastorage.txt**][3]

Custom URL to write data in the file will be http://gsh123.herokuapp.com/writefile.php?data=n...

[Here “name” in above URL is data, which will be added to file]


#### Step 2: PHP Script

To deploy the below php coddes to your = own **[Heroku Account][4]**, just click the button below:

[![Deploy](https://www.herokucdn.com/deploy/button.svg)](https://heroku.com/deploy?template=https://github.com/GSH-Open-source-projects/ESP8266_IOT_Free_host)


Now PHP script to append data to file with name "writefile.php"

That button  save this codes as name writefile.php ins to your heroku host:



 ```
<?php

$val = $_GET["data"];

$fileContent=$val."\n";

$fileStatus=file_put_contents("datastorage.txt",$fileContent,FILE_APPEND);

if($fileStatus != false)

{

                echo  "SUCCESS. Data written in file.";

}

else

{

                echo  "FAIL. Could not connect to file.";

}

?>
```
***Code Explanation:***

Code started with initialization (Basic PHP code syntax). Then GET request is added to get the text after the ‘=’ sign in the URL and that text will be saved in the variable named ‘val’. PHP variables start with ‘$’ character. Must not forget to put **‘;’** semicolon at end of every instruction. **$fileContent** is the variable, which appends ‘\n’ (next line) character at end of the text. Now file_put_contents () is a function used to push the contents in the file. Filename, data and flag (FILE_APPEND) are arguments of this function. This function returns a status flag i.e. True or False. IF flag is true, means Data is stored successfully. In case of False, there might be some error.

That button make these two file  **[datastrorage.txt and writefile.php]** in  your Server.

#### Step 3: Testing the Server

Now open another tab and type custom URL (for my heroku app is:[**http://gsh123.herokuapp.com/writefile.php?data=n**][5]!)  and replace “name” (written after ‘=’ character) with any other text (maybe your name) and press Enter. You will see message “SUCCESS. Data are written in the file." On the webpage. In case you do not get any message, make sure your PHP file does not have any error. Now open File URL in another tab and you will be able to see the contents of the file. You will be able to see your text, which you had written previously in the file. This is all.

[![enter image description here][8]][8]

#### Step 4: NODEMCU ESP8266 12e Board 


Now it is time to program ESP such that it can send and receive messages from the server.

We will be using ESP8266HTTPClient library for our task. You can go through the documentation of this library. This library provides function to begin our HTTP connection with any website and send GET/POST requests. With this library, we will also use ESP8266WiFi library for connecting our Wi-Fi module with Internet. Setup function will be the trivial one containing the code to connect with Internet and display ‘Connected’. I had added one more thing to setup. It will send a message to server to save something on server file. One important thing, which I want to add, is that you should not add spaces in text while sending. Because spaces are replaced at server end with ‘%20’ and I had not added any such code to replace spaces with ‘%20’ in text before sending. You can add it and then your spaces, but for now, I had not used that, so do not do that, it will not work. After that, we will poll in Loop function by reading the received message in a continuous manner and will handle messages if necessary.

Because I was communicating with another ESP, I added a message validation method, which checks the received message, and notifies ESP whether the message needs to be replied or not. If not, it will continue to read messages and if yes, it will stop receiving messages and will send reply to server and again resumes the loop for receiving messages.

Methods:

 1. Receive Message: will receive contents of file, extract the last
    line    of the file, and return it.

 2. Send Message: Will send message to server using custom URL and read the response. According to response, will send its own response whether message was sent successfully or not.

 3. Message Validation: This method will read the received message and check whether the received message is needed to be replied or not. It returns its own response as 1/0.

 4. Handle messages: This method send reply to server in case message is validated and needed to be replied. It will compare the message with pre-defined messages and reply them accordingly.

Now explaining the code to send and receive data from server using ESP8266

***RECEIVE DATA:***
```
String receivelastmessage()

{

  String lastmessage="";

 if(WiFi.status()==WL_CONNECTED)

  {

    HTTPClient http;

    String url="http://gsh123.herokuapp.com/datastorage.txt";

    http.begin(url);

    http.addHeader("Content-Type","text/plain");

    int httpCode=http.GET();

    String data=http.getString();

    lastmessage=getlastline(data);

    http.end();
```
**Explanation:**

‘**http**’ object is created of class ‘**HTTPClient**’. The connection I started with the URL. HTTP headers are added. Now simple GET request is sent to the server. This will return the HTTP code and at the end, collect the received message from **http.getString ()** method of HTTPClient class. This will send you the contents of the File. Using **getlastline ()** method to extract the last line of the data.


***SEND DATA:***

```
int sendmessage(String d)

{

  int sres;

  int net;

  if(WiFi.status()==WL_CONNECTED)

  {

    HTTPClient http;

    String url="http://gsh123.herokuapp.com/writefile.php?data="+d;

    http.begin(url);

    http.addHeader("Content-Type","text/plain");

    int httpCode=http.GET();

    String payload=http.getString();

   Serial.println("While sending I received this from server :"+payload);

    if(payload=="SUCCESS. Data written in file.")

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

  return (net&& sres);

}
```

***Explanation:***

 This code is similar to receive data code. In
this code, we added the custom URL to HTTP begin function and then read the response of this URL using GET function. We compared it with the text, which we had added in our PHP File ‘SUCCESS. Data written in the file.’ At last, the response is sent whether the message is sent successfully or not.

Other methods are self-explanatory and not have anything to discuss here. You can download the code from [***here***][6], which contains all the methods.



## License

[MIT](./LICENSE), just like the upstream [Hubot license](https://github.com/github/hubot/blob/master/LICENSE), so go :nut_and_bolt:s.


## Deploy to Heroku

[![Deploy](https://www.herokucdn.com/deploy/button.svg)](https://heroku.com/deploy)


  [1]: https://signup.heroku.com/
  [2]: http://gsh123.herokuapp.com/
  [3]: http://gsh123.herokuapp.com/datastorage.txt
  [4]: https://id.heroku.com/
  [5]: http://gsh123.herokuapp.com/writefile.php?data=n
  [6]: https://github.com/GSH-Open-source-projects/ESP8266_IOT_Free_host/blob/master/ESP8266_IOT_Free_host%20.ino  
  [8]: https://i.stack.imgur.com/yHpwS.png
  [7]: https://i.stack.imgur.com/e3QVs.png
