//  阿正原创，个人博客：www.wenzheng.club
// 原理为利用ios12 支持的捷径软件和esp8266的http服务器
// 捷径进行http访问指定ip，esp8266检测到特定ip进行相应的io操作
// 2018年10月26日   农历9月19日  生日  更新2.0  支持微信配网和安卓配网（可显示IP地址便于siri捷径添加ip）  安卓软件下载地址： https://fir.im/sc1t
#include <ESP8266WiFi.h>
#define LED 0
// 以上烧写程序配网方式可以换成微信扫码配网，等等，具体操作可查看博客相关例子
WiFiServer server(80);  // 服务器端口号
void smartConfig()
{
  WiFi.mode(WIFI_STA);
  Serial.println("\r\nWait for Smartconfig");
  WiFi.beginSmartConfig();
  while (1)
  {
    Serial.print(".");
    digitalWrite(LED, 0);
    delay(500);
    digitalWrite(LED, 1);
    delay(500);
    if (WiFi.smartConfigDone())
    {
      Serial.println("SmartConfig Success");
      Serial.printf("SSID:%s\r\n", WiFi.SSID().c_str());
      Serial.printf("PSW:%s\r\n", WiFi.psk().c_str());
      break;
    }
  }
}
void setup() {
  Serial.begin(115200);
  delay(10);
  pinMode(2, OUTPUT);
  digitalWrite(2, 1);
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  smartConfig();  //微信智能配网
  delay(500);
  Serial.println("");
  Serial.println("WiFi connected");
  // Start the server
  server.begin();
  Serial.println("Server started @ ");
  // Print the IP address & instructions
  Serial.println(WiFi.localIP());
  //打印出IP地址，后期可以制作显示器来外部硬件显示ip
  Serial.println("To control GPIO, open your web browser.");
  Serial.println("To set GPIO 0 high, type:");
  Serial.print(WiFi.localIP());
  Serial.println("/gpio/1");
  Serial.println("To set GPIO 0 low, type:");
  Serial.print(WiFi.localIP());
  Serial.println("/gpio/0");
  Serial.println("To toggle GPIO 0, type:");
  Serial.print(WiFi.localIP());
  Serial.println("/gpio/4");
}
void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    delay(100);
    return;
  }
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();
  // Match the request
  int val;          
  //      下面开始判断ip进行动作指令
  if (req.indexOf("/gpio/10") != -1)
    digitalWrite(2, 1);
  else if (req.indexOf("/gpio/11") != -1)
    digitalWrite(2, 0);
  else {
    Serial.println("invalid request");
    client.print("HTTP/1.1 404\r\n");
    client.stop();
    return;
  }
  client.flush();
  // Prepare the response
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\nWelcome wenzheng space! ";
  s += "</html>\n";
  // Send the response to the client
  client.print(s);
  delay(1);
  Serial.println("Client disonnected");
}
