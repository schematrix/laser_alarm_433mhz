#include <RCSwitch.h>

// ----- Pin tanımları -----
const uint8_t PIN_RX     = 2;   // 433MHz alıcı data (interrupt destekli pin)
const uint8_t BUZZER_PIN = 4;   // Buzzer (tone/noTone ile kullanacağız)
const uint8_t LED_PIN    = 5;   // Alarm LED
const uint8_t LDR_PIN    = A0;  // LDR analog giriş

// ----- RF kodları (sana verdiğin değerler) -----
const unsigned long RF_MUTE_CODE  = 4544259UL;  // 1. tuş -> Mute (sistem susacak)
const unsigned long RF_RESET_CODE = 4544268UL;  // 2. tuş -> Reset (sistemi başa döndür)

// ----- LDR eşik -----
const int LDR_THRESHOLD = 650; // Analog okuma < 650 ise alarm setlenecek

// ----- Alarm durumları -----
bool muted = false;          // true ise sistem susturulmuş (LDR tetiklemeyecek)
bool alarmLatched = false;   // true ise alarm setlenmiş (latch), sadece reset ile kapanır

// ----- RCSwitch -----
RCSwitch rc;

// ----- Debounce / zamanlama -----
unsigned long lastRfMillis = 0;
const unsigned long RF_DEBOUNCE_MS = 300; // aynı kodu kısa sürede tekrar ignore et

// ----- Alarm ses/led zamanlaması (non-blocking) -----
unsigned long lastAlarmToggle = 0;
const unsigned long ALARM_ON_MS  = 250;
const unsigned long ALARM_OFF_MS = 250;
bool alarmStateOn = false; // şu an LED/buzzer açık mı?

void setup() {
  Serial.begin(115200);

  // Pinler
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(LED_PIN, LOW);

  // RF receiver
  rc.enableReceive(digitalPinToInterrupt(PIN_RX)); // D2 için interrupt
  // Eğer transmit kullanılmayacaksa enableTransmit çağırmaya gerek yok.

  Serial.println(F("Başladı: LDR alarmı, RF mute/reset kontrolü"));
  Serial.print(F("LDR threshold = ")); Serial.println(LDR_THRESHOLD);
}

void loop() {
  // 1) RF gelen paketleri kontrol et
  if (rc.available()) {
    unsigned long v = rc.getReceivedValue();
    int bits = rc.getReceivedBitlength();
    int proto = rc.getReceivedProtocol();

    unsigned long now = millis();
    // Basit debounce: çok hızlı tekrarlanan RF paketleri yoksay
    if (now - lastRfMillis > RF_DEBOUNCE_MS) {
      lastRfMillis = now;
      Serial.print(F("RF alindi: val=")); Serial.print(v);
      Serial.print(F(" bits=")); Serial.print(bits);
      Serial.print(F(" proto=")); Serial.println(proto);

      if (v == RF_MUTE_CODE) {
        // Mute: sistemi tamamen sustur
        muted = true;
        alarmLatched = false; // varsa aktif alarmı durdur
        stopAlarmImmediate();
        Serial.println(F("RF: MUTE etkin. Sistem susturuldu."));
      } else if (v == RF_RESET_CODE) {
        // Reset: sisteme başa dön
        muted = false;
        alarmLatched = false;
        stopAlarmImmediate();
        Serial.println(F("RF: RESET. Sistem başa döndü (mute kaldırıldı, alarm sıfırlandı)."));
      } else {
        Serial.println(F("RF: tanımlı değil, yok sayildi."));
      }
    } // debounce sonu

    rc.resetAvailable();
  }

  // 2) LDR oku ve alarm tetikle
  if (!muted && !alarmLatched) {
    int ldr = analogRead(LDR_PIN);
    // ldr değeri 0..1023. Koşul: eğer < 650 ise alarm set (latch)
    if (ldr < LDR_THRESHOLD) {
      alarmLatched = true;
      Serial.print(F("LDR trigger! deger=")); Serial.println(ldr);
      // alarm hemen başlayacak; alarmLatched true olduğu için alarm döngüsü yürütecek
      lastAlarmToggle = millis(); // alarm zamanlayıcısını sıfırla
      alarmStateOn = false; // toggle ile hemen açılacak
    }
  }

  // 3) Eğer alarm latched ise alarm efektini çalıştır (non-blocking)
  if (alarmLatched && !muted) {
    runAlarmCycle();
  } else {
    // Eğer alarm latched değil veya sistem muted ise alarm kapalı tutulmalı
    if (alarmStateOn) {
      stopAlarmImmediate();
    }
  }

  // küçük gecikme (çok sık loop etmeyi engellemek için) - değilse sorun yok
  delay(5);
}

// ----- Alarm efektini non-blocking çalıştır -----
void runAlarmCycle() {
  unsigned long now = millis();
  if (!alarmStateOn) {
    // şu an kapalı, açma zamanı mı?
    if (now - lastAlarmToggle >= ALARM_OFF_MS) {
      // aç
      digitalWrite(LED_PIN, HIGH);
      tone(BUZZER_PIN, 2000); // 2kHz bip; pasif buzzer için tone kullan; aktif buzzer ise HIGH/LOW ile değiştir
      alarmStateOn = true;
      lastAlarmToggle = now;
    }
  } else {
    // şu an açık, kapama zamanı mı?
    if (now - lastAlarmToggle >= ALARM_ON_MS) {
      // kapa
      digitalWrite(LED_PIN, LOW);
      noTone(BUZZER_PIN);
      alarmStateOn = false;
      lastAlarmToggle = now;
    }
  }
}

// ----- Alarmı hemen durdur ve tüm çıktıları kapat -----
void stopAlarmImmediate() {
  noTone(BUZZER_PIN);
  digitalWrite(LED_PIN, LOW);
  alarmStateOn = false;
  lastAlarmToggle = millis();
}
