# 433 MHz LDR Tetikli Lazer Alarm

Basit ve güvenilir bir lazer/beam alarm projesi. LDR ile ışık kesilmelerini algılar, 433 MHz alıcı ile iki uzaktan komut (MUTE / RESET) dinler ve buzzer + LED ile uyarı verir. Alarm bir kez tetiklendiğinde (latch) LDR geri normale dönse bile kapanmaz — yalnızca **RESET** komutu ile sıfırlanır. **MUTE** komutu ise sistemi tamamen susturur ve yeni tetiklemeleri engeller.

---

## Özellikler
- LDR (A0) ile ışık kesilmesini algılama ve latch (kilitleme) mantığı  
- 433 MHz uzaktan kumanda ile iki komut:
  - **MUTE** → RF kodu: `4544259` — sistemi susturur, yeni tetiklemeleri engeller  
  - **RESET** → RF kodu: `4544268` — mute kaldırılır, alarm sıfırlanır  
- Buzzer ve LED ile sesli/görsel alarm  
- Non-blocking (Arduino donmaz) alarm döngüsü  
- Kolay genişletilebilir yapı (EEPROM, LCD, GSM bildirim vb. eklenebilir)

---

## Donanım
- Arduino Pro Mini / Nano / Uno vb.  
- 433 MHz alıcı modülü (DATA pini: **D2**)  
- Buzzer (pasif önerilir) — **D4**  
- LED (ser direnç ile) — **D5**  
- LDR (foto direnç) — **A0** (voltage divider gerektirir)  
- 10kΩ sabit direnç (LDR ile voltage divider için)  
- Anten (10–17 cm tel) — RF alma performansını arttırır

---

## Pinler (varsayılan)
- `D2` — 433 MHz RX DATA (interrupt destekli pin)  
- `D4` — Buzzer  
- `D5` — LED  
- `A0` — LDR analog giriş

Bu pinler istenirse projede kolayca değiştirilebilir; README’de ve projede kullanılan sabitlere dikkat et.

---

## Devre (özet)
- 433 MHz RX: VCC — 5V, GND — GND, DATA — D2  
- Buzzer: + → D4, − → GND (aktif/pasif buzzer farkına dikkat)  
- LED: D5 → 220Ω → LED → GND  
- LDR: 5V — LDR — A0 — Rfixed — GND (Rfixed ~10kΩ başlangıç için)

---

## Davranış / Kullanım
- Başlangıç: sistem **ARMED** durumunda (mute kapalı, alarm sıfır).  
- Eğer LDR değeri `650` eşik değerinin **altına** düşerse, alarm **kilitlenir** (latch) ve LED + buzzer çalışmaya başlar. LDR normale dönse bile alarm kapanmaz.  
- Uzaktan kumanda ile:
  - `4544259` gönderilirse → **MUTE**: sistem susturulur, alarm durdurulur ve yeni tetiklemeler engellenir.  
  - `4544268` gönderilirse → **RESET**: mute kaldırılır, alarm sıfırlanır, sistem başlangıç durumuna döner.  
- Sistem, alarm latched olduğu sürece sadece **RESET** ile sıfırlanır; MUTE alarmı durdurur ve yeni tetiklemeyi engeller.

---

## Ayarlar / Test
- **LDR eşiği**: `650` başlangıç değeri olarak önerilmiştir. Ortam ışığına göre test edip ayarlayın (ör. 600–800 arası deneyin).  
- **RF kod doğrulama**: Kumandanızın gerçek gönderdiği değeri seri monitörden kontrol edin; farklı kumandalar farklı kod/protokol gönderebilir. Gerekirse RF kodlarını güncelleyin.  
- **Buzzer tipi**: Pasif buzzer için `tone()` uygundur; aktif buzzer kullanıyorsanız `HIGH/LOW` ile sürün.

---

## Hata Giderme (Troubleshooting)
- **RF kodları farklı çıkıyor**: Seri monitör ile gelen RF değerini okuyun; kumandanız farklı kod gönderiyorsa verilen MUTE/RESET kodlarını güncelleyin.  
- **LDR tetikleme hassasiyeti**: Eşik değeri ortam ışığına göre ayarlanmalı. Voltage divider (LDR + sabit direnç) doğru bağlandığından emin olun.  
- **Alıcı çalışmıyor**: Anten bağlı mı? VCC/GND doğru mu? DATA pini doğru yerde mi (interrupt destekli pin)?  
- **Buzzer çalışmıyor**: Aktif vs pasif buzzer farkı ve sürme yöntemi kontrol edilmeli.  
- **LED çok sönük**: Seri direnç ve bağlantı kontrolü yapılmalı.

---

## Geliştirme fikirleri
- Durum göstergesi için küçük bir OLED/LCD: `ARMED / MUTE / ALARM` gösterimi  
- EEPROM ile `muted` ve `alarmLatched` durumlarının güç kesintisinde korunması  
- Birden fazla LDR bölgesi ile bölge tabanlı alarm (bölgesel alarm tanımlama)  
- SMS/Push/GSM/IoT bildirim entegrasyonu (alarm tetiklenince bildirim gönderme)  
- Zamanlayıcı (gece modu / gündüz modu) ekleme

---

## Güvenlik ve Etik
- Bu cihaz deneyseldir; kritik güvenlik amaçlı kullanılmadan önce kapsamlı test yapın.  
- Başkalarının cihazlarını izinsiz dinlemek veya kumanda sinyallerini yeniden göndermek etik ve/veya yasal sorunlar çıkarabilir — bulunduğunuz yerin yasalarına uyun.  
- Lazer kullanımı veya doğrudan göze yönelik ışıklar tehlikelidir; göz güvenliğine dikkat edin.

---

## Lisans
MIT License — Bu projeyi kopyalayabilir, değiştirebilir ve dağıtabilirsiniz.


