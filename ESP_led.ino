#include <WiFi.h>

// guarda o nome e a senha da rede nas variáveis
const char* ssid = "SEU WIFI";
const char* senha = "SENHA";

WiFiServer server(80); // cria um servidor web dentro da esp32, a porta 80 é a padrão da web

int led = 2; // led conectado ao pino 2 da esp32

String pagina = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta name="viewport" content="width=device-width, initial-scale=1">
<style>
body{
  background-color:#FAF5F5;
  color:black;
  text-align:center;
  font-family:Arial;
}
button{
  padding:15px;
  font-size:20px;
  margin:10px;
  border: none;
  border-radius: 10px;
  cursor: pointer;
}
button:hover{
  padding: 13px;
}
.on{
  background-color: #4DD15B ;
}
.off{
  background-color: #A62424;
}
</style>
</head>

<body>

<h1>Controle LED ESP32</h1>

<a href="/LEDON"><button class="on">LED ON</button></a>
<a href="/LEDOFF"><button class="off">LED OFF</button></a>

</body>
</html>
)rawliteral";

void setup() {

  Serial.begin(115200); // inicia comunicação com o monitor serial
  pinMode(led, OUTPUT); // define o pino do led como saída

  Serial.println("Conectando ao wifi..."); // imprime mensagem no serial
  WiFi.begin(ssid, senha); // conecta ao wifi usando o nome e a senha da rede

  while (WiFi.status() != WL_CONNECTED) { // espera conectar ao wifi e continua tentando
    delay(500);
    Serial.print("."); // mostra pontos enquanto conecta
  }

  Serial.println(".");
  Serial.println("Wifi conectado"); // avisa que conectou
  Serial.print("IP da esp32: ");
  Serial.println(WiFi.localIP()); // mostra o ip que a esp32 recebeu do roteador

  server.begin(); // inicia o servidor web
}

void loop() {

  WiFiClient client = server.available(); // verifica se tem alguém acessando o site da esp32

  if (client) { // se tiver um cliente conectado

    String request = client.readStringUntil('\r'); // lê o pedido que veio do navegador
    Serial.println(request); // mostra o pedido no monitor serial

    if (request.indexOf("/LEDON") != -1) { // verifica se o botão de ligar foi clicado
      digitalWrite(led, HIGH); // liga o led
    }

    if (request.indexOf("/LEDOFF") != -1) { // verifica se o botão de desligar foi clicado
      digitalWrite(led, LOW); // desliga o led
    }

    client.println("HTTP/1.1 200 OK"); // envia resposta para o navegador
    client.println("Content-Type: text/html"); // informa que é uma página web
    client.println(""); // linha obrigatória do protocolo http

    client.println(pagina); //carrega a pagina
    client.stop(); // encerra a conexão com o navegador
  }

}
