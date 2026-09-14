# arduino-smart-study-timer
# 📚 Sessiz Alarmlı Çalışma Süresi Takipçisi

## 📌 Proje Hakkında

Bu proje, öğrencilerin çalışma ve mola sürelerini daha verimli yönetmelerine yardımcı olmak amacıyla geliştirilmiş **Arduino tabanlı akıllı bir çalışma takip sistemidir**.

Sistem, Pomodoro mantığıyla çalışırken kullanıcının nabız verisini **MAX30100 Pulse Oximeter sensörü** aracılığıyla ölçer ve elde edilen verilere göre kullanıcının stres durumunu değerlendirerek **dinamik mola süresi** belirler.

Proje özellikle kütüphane gibi sessiz çalışma ortamları düşünülerek tasarlanmıştır. Kullanıcıya LED'ler, LCD ekran ve düşük sesli buzzer aracılığıyla bilgi verilir.

---

## ✨ Özellikler

* ⏱️ Pomodoro tabanlı çalışma ve mola sistemi
* 🔘 Tek buton ile kontrol
* 📚 10, 15 ve 20 saniyelik örnek çalışma süresi seçenekleri
* ❤️ MAX30100 ile nabız ölçümü
* 🧠 Nabız değerine göre stres durumunun değerlendirilmesi
* ☕ Dinamik mola süresi belirleme
* 🖥️ 16x2 I2C LCD ekran üzerinden bilgi gösterimi
* 🔴🟢🔵 LED'ler ile sistem durumunun gösterilmesi
* 🔔 Düşük sesli buzzer ile uyarı sistemi
* ⚡ Timer interrupt kullanılarak zaman takibi
* 🔄 Button interrupt ve debounce mekanizması
* ⏸️ Çalışmayı duraklatma ve devam ettirme

---

## 🛠️ Kullanılan Teknolojiler

### Yazılım

* C/C++
* Arduino IDE
* AVR-GCC

### Kütüphaneler

* `Wire.h`
* `LiquidCrystal_I2C.h`
* `TimerOne.h`
* `MAX30100_PulseOximeter.h`

---

## 🔌 Kullanılan Donanımlar

* Arduino Uno / Nano
* MAX30100 Pulse Oximeter Sensörü
* 16x2 I2C LCD Ekran
* Pasif Buzzer
* Kırmızı LED
* Yeşil LED
* Mavi LED
* Push Button
* Dirençler
* Jumper Kablolar
* Breadboard

---

## ⚙️ Sistem Nasıl Çalışır?

1. Kullanıcı çalışma süresini seçer.
2. Çalışma süreci başlar.
3. Sistem çalışma süresini Timer interrupt kullanarak takip eder.
4. Çalışma süresi tamamlandığında MAX30100 sensörü ile nabız ölçümü yapılır.
5. Ölçülen nabız değerine göre stres durumu değerlendirilir.
6. Stres seviyesine göre mola süresi otomatik olarak belirlenir.
7. Mola tamamlandıktan sonra sistem tekrar başlangıç durumuna döner.

Bu süreç sürekli olarak döngü halinde devam eder.

---

## 🔴🟢🔵 LED Durumları

| LED            | Durum          |
| -------------- | -------------- |
| 🔴 Kırmızı LED | Çalışma zamanı |
| 🟢 Yeşil LED   | Mola zamanı    |
| 🔵 Mavi LED    | Nabız ölçümü   |

---

## 🔧 Pin Bağlantıları

| Bileşen     | Pin        |
| ----------- | ---------- |
| Buzzer      | Pin 9      |
| Button      | Pin 3      |
| Kırmızı LED | Pin 8      |
| Yeşil LED   | Pin 7      |
| Mavi LED    | Pin 4      |
| LCD         | I2C (0x27) |
| MAX30100    | I2C        |

---

## 🧠 Kullanılan Yapılar

Projede aşağıdaki gömülü sistem yapıları kullanılmıştır:

* **Timer Interrupt**
* **Digital Pin Interrupt**
* **Debounce Mekanizması**
* **State Machine (Durum Makinesi)**
* **I2C Haberleşmesi**
* **Sensör Verisi İşleme**

---

## 🚀 Gelecekte Yapılabilecek Geliştirmeler

* 📱 Mobil uygulama entegrasyonu
* 📊 Çalışma ve mola sürelerinin grafiksel gösterimi
* ☁️ Nabız verilerinin bulut ortamında saklanması
* 📶 Bluetooth veya Wi-Fi bağlantısı
* 👤 Çoklu kullanıcı desteği
* 🧬 Daha gelişmiş biyometrik sensörlerin eklenmesi
* ⚙️ Kullanıcının mola süresini manuel olarak özelleştirebilmesi

---

## 👩‍💻 Proje Ekibi

**Azra Nur Akbaba**

**Dila Kemer**

---

## 📄 Proje

Bu proje,  Gömülü Sistem Dersi Laboratuvar Projesi** kapsamında geliştirilmiştir.
