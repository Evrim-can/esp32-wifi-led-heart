#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h> // Otomatik sayfa açılışı için gerekli kütüphane

// --- Wİ-Fİ AYARLARI ---
const char* ssid = "Sinem ❤️ Evrim";     
const char* password = "senicokseviyorum"; 

// --- LED PINLERİ ---
const int ledPins[16] = {4, 16, 17, 5, 18, 19, 21, 22, 23, 13, 14, 27, 26, 25, 33, 32};
const int NUM_LEDS = 16;

// Sunucu ayarları
const byte DNS_PORT = 53;
DNSServer dnsServer;
WebServer server(80);

int aktifAnimasyon = 1; 
bool wifiAcik = true;
unsigned long acilisZamani = 0;
const unsigned long WIFI_KAPANMA_SURESI = 5 * 60 * 1000; // 5 Dakika (Milisaniye cinsinden)

// --- GÖRSEL WEB ARAYÜZÜ ---
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>Kalp Kontrol Paneli</title>
  <style>
    body { font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; text-align: center; background-color: #1a1a1a; color: white; margin: 0; padding: 15px; }
    h1 { color: #ff4d4d; letter-spacing: 2px; font-size: 24px;}
    .btn { display: inline-block; width: 90%; max-width: 300px; padding: 14px; margin: 8px; font-size: 16px; font-weight: bold; color: white; background-color: #2a2a2a; border: 2px solid #ff4d4d; border-radius: 12px; cursor: pointer; transition: 0.3s; box-shadow: 0 4px 10px rgba(255, 77, 77, 0.2); }
    .btn:active { background-color: #ff4d4d; transform: scale(0.95); }
    .icon { font-size: 22px; margin-right: 10px; vertical-align: middle; }
    .info { margin-top: 20px; font-size: 12px; color: #888; padding: 10px; }
  </style>
  <script>
    function setAnim(id) {
      fetch('/set?anim=' + id);
    }
  </script>
</head>
<body>
  <h1>❤️ KALP KONTROL</h1>
  <div class="btn" onclick="setAnim(1)"><span class="icon">💫</span>Kuyruklu Dönüş</div>
  <div class="btn" onclick="setAnim(2)"><span class="icon">💓</span>Kalp Atışı</div>
  <div class="btn" onclick="setAnim(3)"><span class="icon">✨</span>Sırayla Dolma</div>
  <div class="btn" onclick="setAnim(6)"><span class="icon">💞</span>Kavuşma</div>
  <div class="btn" onclick="setAnim(7)"><span class="icon">🌟</span>Yıldız Kayması</div>
  <div class="btn" onclick="setAnim(5)"><span class="icon">⚡</span>Flaşör Etkisi</div>
  <div class="btn" onclick="setAnim(4)"><span class="icon">🔥</span>Sabit Yanma</div>
  
  <div class="info">🔋 Pil tasarrufu için Wi-Fi modülü 5 dakika sonra kapanır. Yeniden bağlanmak için pili kapatıp açın.</div>
</body>
</html>
)rawliteral";

// Smart delay: Beklerken hem web isteklerini hem de DNS isteklerini işler
void smartDelay(unsigned long ms) {
  unsigned long baslangic = millis();
  while (millis() - baslangic < ms) {
    if (wifiAcik) {
      dnsServer.processNextRequest();
      server.handleClient(); 
    }
    delay(1);
  }
}

void setup() {
  Serial.begin(115200);
  
  for (int i = 0; i < NUM_LEDS; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW);
  }

  // Access Point (AP) kurulumu
  WiFi.softAP(ssid, password);
  Serial.println("Wi-Fi Agi Kuruldu!");
  
  // DNS Sunucusunu başlat (Tüm adresleri ESP'nin IP'sine yönlendirir)
  dnsServer.start(DNS_PORT, "*", WiFi.softAPIP());

  // Ana sayfa
  server.on("/", []() {
    server.send(200, "text/html", index_html);
  });

  // Animasyon seçici
  server.on("/set", []() {
    if (server.hasArg("anim")) {
      aktifAnimasyon = server.arg("anim").toInt();
      for (int i = 0; i < NUM_LEDS; i++) digitalWrite(ledPins[i], LOW); // Ekranı temizle
    }
    server.send(200, "text/plain", "OK");
  });

  // Otomatik sayfa açılışı için yakalayıcı (Captive Portal Redirect)
  server.onNotFound([]() {
    server.sendHeader("Location", String("http://") + WiFi.softAPIP().toString(), true);
    server.send(302, "text/plain", "");
  });

  server.begin();
  acilisZamani = millis(); 
}

void loop() {
  // 5 Dakika Dolduysa Wi-Fi Kapatma
  if (wifiAcik && (millis() - acilisZamani > WIFI_KAPANMA_SURESI)) {
    WiFi.mode(WIFI_OFF); 
    dnsServer.stop();
    wifiAcik = false;
  }

  // İstekleri Dinle
  if (wifiAcik) {
    dnsServer.processNextRequest();
    server.handleClient();
  }

  // Animasyon Yönlendirici
  switch (aktifAnimasyon) {
    case 1: animasyonKuyruk(); break;
    case 2: animasyonKalpAtisi(); break;
    case 3: animasyonDolma(); break;
    case 4: animasyonSabit(); break;
    case 5: animasyonFlasor(); break;
    case 6: animasyonCarpisma(); break;
    case 7: animasyonRastgele(); break;
  }
}

// ==========================================
//          ANİMASYON FONKSİYONLARI
// ==========================================

void animasyonKuyruk() {
  for (int i = 0; i < NUM_LEDS; i++) {
    if (aktifAnimasyon != 1) return; 
    digitalWrite(ledPins[i], HIGH);
    
    int sondur = (i - 2 + NUM_LEDS) % NUM_LEDS;
    digitalWrite(ledPins[sondur], LOW);
    smartDelay(35); 
  }
}

void animasyonKalpAtisi() {
  if (aktifAnimasyon != 2) return;
  tumLedler(HIGH);
  smartDelay(150);
  tumLedler(LOW);
  smartDelay(150);
  tumLedler(HIGH);
  smartDelay(150);
  tumLedler(LOW);
  smartDelay(800);
}

void animasyonDolma() {
  for (int i = 0; i < NUM_LEDS; i++) {
    if (aktifAnimasyon != 3) return;
    digitalWrite(ledPins[i], HIGH);
    smartDelay(60);
  }
  for (int i = NUM_LEDS - 1; i >= 0; i--) {
    if (aktifAnimasyon != 3) return;
    digitalWrite(ledPins[i], LOW);
    smartDelay(60);
  }
}

void animasyonSabit() {
  tumLedler(HIGH);
  smartDelay(100); 
}

void animasyonFlasor() {
  if (aktifAnimasyon != 5) return;
  tumLedler(HIGH);
  smartDelay(40);
  tumLedler(LOW);
  smartDelay(60);
  
  if (aktifAnimasyon != 5) return;
  tumLedler(HIGH);
  smartDelay(40);
  tumLedler(LOW);
  smartDelay(400);
}

void animasyonCarpisma() {
  for (int i = 0; i <= NUM_LEDS / 2; i++) {
    if (aktifAnimasyon != 6) return;
    digitalWrite(ledPins[i], HIGH);
    digitalWrite(ledPins[NUM_LEDS - 1 - i], HIGH); 
    smartDelay(70);
  }
  for (int i = 0; i <= NUM_LEDS / 2; i++) {
    if (aktifAnimasyon != 6) return;
    digitalWrite(ledPins[i], LOW);
    digitalWrite(ledPins[NUM_LEDS - 1 - i], LOW);
    smartDelay(70);
  }
}

void animasyonRastgele() {
  if (aktifAnimasyon != 7) return;
  int rastgeleLed = random(0, NUM_LEDS); 
  
  digitalWrite(ledPins[rastgeleLed], HIGH);
  smartDelay(40);
  digitalWrite(ledPins[rastgeleLed], LOW);
  smartDelay(40);
}

void tumLedler(int durum) {
  for (int i = 0; i < NUM_LEDS; i++) {
    digitalWrite(ledPins[i], durum);
  }
}