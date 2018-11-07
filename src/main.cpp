#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

// Initialize Telegram BOT
#define BOTtoken "616208045:AAGtUjScjHNxBVXnNYez1Vp74ZJjzsoKzbI"  // your Bot Token (Get from Botfather)

//Credentials for Google GeoLocation API...
const char* Host = "www.googleapis.com";
String thisPage = "/geolocation/v1/geolocate?key=";
String key = "AIzaSyCViu_q6gVye4gVNZB1IYqos62fuN10CcA";

int status = WL_IDLE_STATUS;
String jsonString = "{\n";

double latitude    = 0.0;
double longitude   = 0.0;

const char* ssid = "WORKSHOP ESP8266";
const char* password = "WORKSHOPESP8266";

String latitude_atual;
String longitude_atual;

String latitude_inicial;
String longitude_inicial;

String latitude_final; //= "-15.610161";
String longitude_final; //= "-56.066349";
float distancia;

int uma_vez = 0;
IPAddress IP;

//int Bot_mtbs = 1000; //mean time between scan messages
//long Bot_lasttime;   //last time messages' scan has been done

String id, text;//Váriaveis para armazenamento do ID e TEXTO gerado pelo Usuario
unsigned long tempo;

ESP8266WebServer server(80);

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

void calculardistancia(float lat1, float lon1, float lat2, float lon2)
{
  const double R = 6371000.0;
  const double lat1Rad = lat1 * 0.01745329251994329576923690768489;
  const double lat2Rad = lat2 * 0.01745329251994329576923690768489;
  const double deltaLatRad = lat2Rad - lat1Rad;
  const double deltaLonRad = (lon2 - lon1) * 0.01745329251994329576923690768489;
  const double sinDeltaLatRad = sin(deltaLatRad * 0.5);
  const double sinDeltaLonRad = sin(deltaLonRad * 0.5);
  const double a = (sinDeltaLatRad * sinDeltaLatRad) + (cos(lat1Rad) * cos(lat2Rad) * sinDeltaLonRad * sinDeltaLonRad);
  distancia = R * 2.0 * atan2(sqrt(a), sqrt(1.0 - a));
}

void handleRoot() 
{
  // HTML da pagina principal
  String html = "<!DOCTYPE html>";
  html += "<html>";
  html += "  <head>";
  html += "    <meta name=\"description\" content="">";
  html += "    <meta name=\"author\" content="">";
  html += "    <meta name=\"viewport\" content=\"initial-scale=1.0, user-scalable=no\">";
  html += "    <meta charset=\"utf-8\">";
  html += "    <title>MASCOTE DRONE</title>";
  html += "    <!-- Bootstrap core CSS -->";
  html += "    <link href=\"vendor/bootstrap/css/bootstrap.min.css\" rel=\"stylesheet\">";

  html += "     <!-- Custom styles for this template -->";
  html += "     <link href=\"css/simple-sidebar.css\" rel=\"stylesheet\">";
  html += "    <style>";

  html += "      #map {";
  html += "        height: 100%;";
  html += "      }";
  html += "      html, body {";
  html += "        height: 100%;";
  html += "        margin: 0;";
  html += "        padding: 0;";
  html += "    </style>";
  html += "  </head>";
  html += "  <body>";
  //html += "     <div class=\"container-fluid\">";
  html += "         <div id=\"map\">";
  //html += "             <a href=\"#menu-toggle\" class=\"btn btn-secondary\" id=\"menu-toggle\">Toggle Menu</a>";
  html += "         </div>";
  //html += "     </div>";

  html += "    <script src=\"http://maps.google.com/maps/api/js?sensor=false&libraries=geometry\" type=\"text/javascript\"></script> ";
  html += "    <script type=\"text/javascript\">";
  
  html += "      function initMap() {";
  html += "        var map = new google.maps.Map(document.getElementById('map'), {";
  html += "          zoom: 15,";
  html += "          center: {lat: " + latitude_atual + ", lng: " + longitude_atual + "},";
  html += "          mapTypeId: 'terrain',";
  html += "          styles: [";
  html += "           {elementType: 'geometry', stylers: [{color: '#242f3e'}]},";
  html += "                     {elementType: 'labels.text.stroke', stylers: [{color: '#242f3e'}]},";
  html += "                     {elementType: 'labels.text.fill', stylers: [{color: '#746855'}]},";
  html += "                     {";
  html += "                       featureType: 'administrative.locality',";
  html += "                       elementType: 'labels.text.fill',";
  html += "                       stylers: [{color: '#d59563'}]";
  html += "                     },";
  html += "                     {";
  html += "                       featureType: 'poi',";
  html += "                       elementType: 'labels.text.fill',";
  html += "                       stylers: [{color: '#d59563'}]";
  html += "                     },";
  html += "                     {";
  html += "                       featureType: 'poi.park',";
  html += "                       elementType: 'geometry',";
  html += "                       stylers: [{color: '#263c3f'}]";
  html += "                     },";
  html += "                     {";
  html += "                       featureType: 'poi.park',";
  html += "                       elementType: 'labels.text.fill',";
  html += "                       stylers: [{color: '#6b9a76'}]";
  html += "                     },";
  html += "                     {";
  html += "                       featureType: 'road',";
  html += "                       elementType: 'geometry',";
  html += "                       stylers: [{color: '#38414e'}]";
  html += "                     },";
  html += "                     {";
  html += "                       featureType: 'road',";
  html += "                       elementType: 'geometry.stroke',";
  html += "                       stylers: [{color: '#212a37'}]";
  html += "                     },";
  html += "                     {";
  html += "                       featureType: 'road',";
  html += "                       elementType: 'labels.text.fill',";
  html += "                       stylers: [{color: '#9ca5b3'}]";
  html += "                     },";
  html += "                     {";
  html += "                       featureType: 'road.highway',";
  html += "                       elementType: 'geometry',";
  html += "                       stylers: [{color: '#746855'}]";
  html += "                     },";
  html += "                     {";
  html += "                       featureType: 'road.highway',";
  html += "                       elementType: 'geometry.stroke',";
  html += "                       stylers: [{color: '#1f2835'}]";
  html += "                     },";
  html += "                     {";
  html += "                       featureType: 'road.highway',";
  html += "                       elementType: 'labels.text.fill',";
  html += "                       stylers: [{color: '#f3d19c'}]";
  html += "                     },";
  html += "                     {";
  html += "                       featureType: 'transit',";
  html += "                       elementType: 'geometry',";
  html += "                       stylers: [{color: '#2f3948'}]";
  html += "                     },";
  html += "                     {";
  html += "                       featureType: 'transit.station',";
  html += "                       elementType: 'labels.text.fill',";
  html += "                       stylers: [{color: '#d59563'}]";
  html += "                     },";
  html += "                     {";
  html += "                       featureType: 'water',";
  html += "                       elementType: 'geometry',";
  html += "                       stylers: [{color: '#17263c'}]";
  html += "                     },";
  html += "                     {";
  html += "                       featureType: 'water',";
  html += "                       elementType: 'labels.text.fill',";
  html += "                       stylers: [{color: '#515c6d'}]";
  html += "                     },";
  html += "                     {";
  html += "                       featureType: 'water',";
  html += "                       elementType: 'labels.text.stroke',";
  html += "                       stylers: [{color: '#17263c'}]";
  html += "                     }";
  html += "                   ]";
  html += "                 });";

  html += "         function calcDistance (fromLat, fromLng, toLat, toLng) {";
  html += "            return google.maps.geometry.spherical.computeDistanceBetween(";
  html += "            new google.maps.LatLng(fromLat, fromLng), new google.maps.LatLng(toLat, toLng));";
  html += "         }";
  
  html += "        var flightPlanCoordinates1 = ["     ;     // FIM - VERMELHO
  html += "          {lat: " + latitude_final + ", lng: " + longitude_final + "},"  ; // Ponto Final - Casa
  
  html += "          {lat: " + latitude_atual + ", lng: " + longitude_atual + "}"  ;  //Ponto onde está o drone
  html += "        ];";
  html += "        var flightPath1 = new google.maps.Polyline({";
  html += "          path: flightPlanCoordinates1,";
  html += "          geodesic: true,";
  html += "          strokeColor: '#FF0000',";
  html += "          strokeOpacity: 1.0,";
  html += "          strokeWeight: 2";
  html += "        });";
  
  html += "        flightPath1.setMap(map);";
  
  html += "        var flightPlanCoordinates2 = ["       ;   // INICÍO - AZUL
  html += "          {lat: " + latitude_inicial + ", lng: " + longitude_inicial + "}," ;  //Ponto Inicial - UFMT
  html += "          {lat: " + latitude_atual + ", lng: " + longitude_atual + "}" ;   // Ponto onde está o drone
  html += "        ];";
  html += "        var flightPath2 = new google.maps.Polyline({";
  html += "          path: flightPlanCoordinates2,";
  html += "          geodesic: true,";
  html += "          strokeColor: '#000080',";
  html += "          strokeOpacity: 1.0,";
  html += "          strokeWeight: 2";
  html += "        });";
  
  html += "        flightPath2.setMap(map);";

  if(latitude_final != latitude_inicial && longitude_final != longitude_inicial)
  {
    calculardistancia(latitude_atual.toFloat(), longitude_atual.toFloat(), latitude_final.toFloat(), longitude_final.toFloat());
    html += "        var myLatlng_destino = new google.maps.LatLng(" + latitude_final + "," + longitude_final + ");";
    html += "        var marker_destino = new google.maps.Marker({";
    html += "           map: map,";
    html += "           draggable: true,";
    html += "           position: myLatlng_destino,";
    html += "           title:\"Destino\"";
    html += "        });";
    html += "        marker_destino.setMap(map);";
  
    html += "      var contentString = '<div id=\"content\">'+";
    html += "                '<div id=\"siteNotice\">'+";
    html += "                '</div>'+";
    html += "                '<h4>Mascote Drone</h4>'+";
    html += "                '<div><p>Distância: " + String(distancia) + " metros</p>'+";
    html += "                '</div>'+";
    html += "                '</div>';";

    html += "      var infowindow = new google.maps.InfoWindow({";
    html += "              content: contentString";
    html += "      });";
  }

  html += "        var myLatlng_drone = new google.maps.LatLng(" + latitude_atual + "," + longitude_atual + ");";
  html += "        var marker_drone = new google.maps.Marker({";
  html += "           map: map,";
  html += "           draggable: true,";
  html += "           position: myLatlng_drone,";
  html += "           icon: {";
  html += "             url: \"http://bluesuedemedia.com/wp-content/uploads/2015/10/Drone-Icon-resized-white.png\","; //https://d30y9cdsu7xlg0.cloudfront.net/png/375033-200.png
  html += "             scaledSize: new google.maps.Size(50, 50)";
  html += "           },";
  html += "           title:\"Drone\"";
  html += "        });";
  html += "        marker_drone.setMap(map);";

  html += "        marker_drone.addListener('click', function() {";
  html += "                infowindow.open(map, marker_drone);";
  html += "        });";

  html += "        window.setInterval(initMap, 30000);";

  html += "      }";
  html += "    </script>";
  html += "    <script async defer";
  html += "    src=\"https://maps.googleapis.com/maps/api/js?key=AIzaSyCViu_q6gVye4gVNZB1IYqos62fuN10CcA&callback=initMap\">";
  html += "    </script>";
  html += "  </body>";
  html += "</html>";

  // Enviando HTML para o servidor
  server.send(200, "text/html", html);
}

void readTel()//Funçao que faz a leitura do Telegram.
{
   int newmsg = bot.getUpdates(bot.last_message_received + 1);
 
   for (int i = 0; i < newmsg; i++)//Caso haja X mensagens novas, fara este loop X Vezes.
   {
      id = bot.messages[i].chat_id;//Armazenara o ID do Usuario à Váriavel.
      text = bot.messages[i].text;//Armazenara o TEXTO do Usuario à Váriavel.
      String nome = bot.messages[i].from_name;
      if (nome == "") nome = "Convidado";
      
      int val = text.toInt();
      if(val != 0)
      {
        bot.sendMessage(id, "É um número", "");
        Serial.println(val);
      }
      else if (bot.messages[i].longitude != 0 || bot.messages[i].latitude != 0) 
      {
        //Serial.print("Long: ");
        //Serial.println(String(bot.messages[i].longitude, 6));
        longitude_final = String(bot.messages[i].longitude, 6);
        //Serial.print("Lat: ");
        //Serial.println(String(bot.messages[i].latitude, 6));
        latitude_final = String(bot.messages[i].latitude, 6);
  
        //String message = "Long: " + String(bot.messages[i].longitude, 6) + "\n";
        //message += "Lat: " + String(bot.messages[i].latitude, 6) + "\n";
        //bot.sendMessage(id, message, "Markdown");
        String site = "Acesse em seu browser o ip: http://" + WiFi.localIP().toString() + " para visualização do trajeto.";
        bot.sendMessage(id, site, "");
      }
      else
      {
        text.toUpperCase();//Converte a STRING_TEXT inteiramente em Maiuscúla.
        
        if (text.indexOf("ON") > -1)//Caso o texto recebido contenha "ON"
        {
           digitalWrite(D4, 0);//Liga o LED
           bot.sendMessage(id, "LED ON", "");//Envia uma Mensagem para a pessoa que enviou o Comando.
        }
   
        else if (text.indexOf("OFF") > -1)//Caso o texto recebido contenha "OFF"
        {
           digitalWrite(D4, 1);//Desliga o LED
           bot.sendMessage(id, "LED OFF", "");//Envia uma Mensagem para a pessoa que enviou o Comando.
        }
   
        else if (text == "START") 
        {
           String welcome = "Olá," + nome + ". Seja Bem vindo ao chat de comunicação MASCOTE_BOT.\n";
           welcome += "Compartilhe sua localização, que lhe informarei suas coordenadas, distância e tempo estimado de viagem\n";

           bot.sendSimpleMessage(id, "ID: " + id, "");//Envia uma mensagem com seu ID.
           bot.sendMessage(id, welcome, "");
        }
 
        else//Caso o texto recebido nao for nenhum dos acima, Envia uma mensagem de erro.
        {
           bot.sendSimpleMessage(id, "Comando Invalido", "");
        }
      }
   }
 
}

void Geolocalizacao()
{
  //while(latitude_inicial == 0 || longitude_inicial == 0)
  //{
    DynamicJsonBuffer jsonBuffer;
    int n = WiFi.scanNetworks();
  
    // now build the jsonString...
    jsonString="{\n";
    jsonString +="\"homeMobileCountryCode\": 234,\n";  // this is a real UK MCC
    jsonString +="\"homeMobileNetworkCode\": 27,\n";   // and a real UK MNC
    jsonString +="\"radioType\": \"gsm\",\n";          // for gsm
    jsonString +="\"carrier\": \"Vodafone\",\n";       // associated with Vodafone 
    jsonString +="\"wifiAccessPoints\": [\n";
    for (int j = 0; j < n; ++j)
    {
      jsonString +="{\n";
      jsonString +="\"macAddress\" : \"";    
      jsonString +=(WiFi.BSSIDstr(j));
      jsonString +="\",\n";
      jsonString +="\"signalStrength\": ";     
      jsonString +=WiFi.RSSI(j);
      jsonString +="\n";
      if(j<n-1)
      {
      jsonString +="},\n";
      }
      else
      {
      jsonString +="}\n";  
      }
    }
    jsonString +=("]\n");
    jsonString +=("}\n"); 
  //--------------------------------------------------------------------  
  
    //Connect to the client and make the api call  
    if (client.connect(Host, 443)) {
      Serial.println("Connected");    
      client.println("POST " + thisPage + key + " HTTP/1.1");    
      client.println("Host: "+ (String)Host);
      client.println("Connection: close");
      client.println("Content-Type: application/json");
      client.println("User-Agent: Arduino/1.0");
      client.print("Content-Length: ");
      client.println(jsonString.length());    
      client.println();
      client.print(jsonString); 
      yield(); 
      delay(500);
    }
  
    //Read and parse all the lines of the reply from server          
    while (client.available()) {
      String line = client.readStringUntil('\r');
      JsonObject& root = jsonBuffer.parseObject(line);
      if(root.success()){
      latitude    = root["location"]["lat"];
      longitude   = root["location"]["lng"];
      }
      yield();
    }
  
    //Serial.println("closing connection");
    //Serial.println();
    client.stop();
  
    //Serial.print("Latitude = ");
    //Serial.println(latitude,6);
    //Serial.print("Longitude = ");
    //Serial.println(longitude,6);
  
    latitude_atual = String(latitude, DEC);
    longitude_atual = String(longitude, DEC);
  
    if(!uma_vez) //&& (latitude_inicial != 0 || longitude_inicial != 0))
    {
      latitude_inicial = latitude_atual;
      longitude_inicial = longitude_atual;
      uma_vez = 1;
    }
  //}
}

void setup() 
{
  // Iniciando Serial
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("");
  pinMode(D4, OUTPUT);//LED conectado à saida
  digitalWrite(D4, HIGH);

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  IP = WiFi.localIP();
  Serial.println(IP);
  
  // Atribuindo urls para funções
  server.on("/", handleRoot);
  
  // Iniciando servidor
  server.begin();
  
  // Apenas informando que servidor iniciou
  Serial.println("HTTP server started");
}

void loop() 
{
  WiFiClientSecure client;

  Geolocalizacao();

  if (millis() - tempo > 2000)//Faz a verificaçao das funçoes a cada 2 Segundos
  {
    readTel();//Funçao para ler o telegram
    tempo = millis();//Reseta o tempo
  }
  
  if(latitude_final.length() == 0 || longitude_final.length() == 0)
  {
    latitude_final = latitude_inicial;
    longitude_final = longitude_inicial;
  }
  
  server.handleClient();
}