# ❤️ ESP32 Wi-Fi Controlled Smart LED Heart
<img width="800" height="450" alt="ezgif-5c6f59d9c00da0dd" src="https://github.com/user-attachments/assets/1b8e3f58-e8a4-4eb7-a0f4-a16e9fdfc395" />

## 📌 Proje Hakkında (About the Project)
Bu proje, ESP32 mikrodenetleyicisi kullanılarak tasarlanmış, taşınabilir ve Wi-Fi üzerinden mobil web arayüzü ile kontrol edilebilen akıllı bir LED kalp ekranıdır. Cihaz kendi Access Point (AP) ağını yaydığı için herhangi bir dış internet bağlantısına (modem/router) ihtiyaç duymadan her yerde akıllı telefonlardan kontrol edilebilir.

## ✨ Özellikler (Features)
* **Bağımsız Wi-Fi Ağı:** ESP32 kendi ağını oluşturur (Kişiselleştirilmiş SSID ve Şifre).
* **Captive Portal:** Ağa bağlanıldığında, kontrol paneli telefonda otomatik olarak açılır (Otel/Kafe Wi-Fi ağları gibi).
* **Asenkron Web Sunucusu (Smart Delay):** Animasyonlar döngüde çalışırken bekleme (delay) tıkanmaları yaşanmaz, sistem anında komut alır.
* **7 Farklı Efekt:** Flaşör, kalp atışı, yıldız kayması, kuyruklu dönüş, kavuşma vb.
* **Otomatik Güç Tasarrufu:** Sistem açıldıktan 5 dakika sonra Wi-Fi antenini otomatik kapatarak 18650 lityum pilin ömrünü maksimize eder.

## 🛠️ Kullanılan Donanımlar (Hardware)
* ESP-32U (38-Pin)
* 16x Kırmızı LED (5mm/3mm) & 16x 220Ω Direnç
* 18650 Li-ion Pil ve Yuvası
* TP4056 Lityum Pil Şarj Modülü
* MT3608 Step-Up (Boost) Voltaj Yükseltici Modül
* Slide Switch (Aç/Kapa Anahtarı)
* 20x30 cm Bakırlı Delikli Plaket



## 🔌 Mühendislik Notları ve Donanım Çözümleri (Engineering Notes)
* **Güç Yönetimi:** 18650 pilden alınan nominal 3.7V, MT3608 voltaj yükseltici ile stabil 5V'a çıkarılarak ESP32'nin `VIN` hattına beslendi. TP4056 ile sistem Type-C/Micro-USB üzerinden şarj edilebilir hale getirildi.
* **Ortak Şase (Common Ground) Mimarisi:** Bağımsız güç kaynaklarının sinyal çakışmasını önlemek için; MT3608 eksi çıkışı, ESP32 GND pini ve 16 LED'in eksi hattı donanımsal olarak tek bir noktada (Star Grounding) birleştirildi.
* **Kritik ESP-32U Çözümü:** 38-pin ESP-32U kartlarındaki donanımsal yerleşim farkı (5V pininin yanındaki pinin GND değil, flaş hafıza iletişim pini olan `CMD` olması) tespit edilerek, şase hattı gerçek GND pinine çekildi. Bu sayede `Serial noise/corruption` ve asılı kalan voltaj hataları (floating pins) engellendi.

## 🚀 Kurulum ve Kullanım (How to Use)
1. Repodaki `.ino` dosyasını Arduino IDE kullanarak ESP32'ye yükleyin. (Gerekli kütüphaneler: `WiFi.h`, `WebServer.h`, `DNSServer.h`)
2. Devreye güç verin.
3. Telefonunuzun Wi-Fi listesinden cihazın yaydığı ağa bağlanın.
4. Ekrana otomatik düşen arayüzden animasyonları kontrol edin. (Sayfa otomatik açılmazsa tarayıcıdan `192.168.4.1` adresine gidin).
## 🤝 Geliştirici Notu & Teşekkür (Developer Note & Credits)

Bu projenin donanım tasarımı, fiziksel montajı, güç yönetimi (MT3608/TP4056 entegrasyonu) ve ortak şase probleminin çözümü tamamen tarafımca yapılmıştır. 

C programlama ve gömülü sistem yazılımları üzerindeki öğrenme sürecim aktif olarak devam etmektedir. Bu nedenle, projenin ilk versiyonundaki Wi-Fi Access Point kurulumu, Captive Portal yönlendirmesi ve asenkron web sunucusu kodlarının iskeleti **Google Gemini (AI)** rehberliği ve asistanlığı ile oluşturulmuştur. Yazılım alanında yetkinliğimi artırdıkça, ilerleyen süreçte bu depo (repository) tamamen kendi yazacağım C tabanlı algoritmalarla güncellenecektir.
