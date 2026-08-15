# Sistem Kendali Suara Navigasi Slide 🎙️
**Implementasi Edge AI pada Mikrokontroler ESP32-S3**

Proyek ini adalah sistem kendali cerdas berbasis *machine learning* (TinyML) yang memungkinkan pengguna untuk menavigasi slide presentasi menggunakan perintah suara. Sistem ini berjalan sepenuhnya secara lokal (*offline/on-device*) pada mikrokontroler ESP32-S3 dan bertindak sebagai *Bluetooth Keyboard* nirkabel.



## 🛠️ Perangkat Keras (Hardware)
Sistem ini menggunakan beberapa komponen utama yang dikemas dalam *casing* cetak 3D kustom (HelmiCase):
* **ESP32-S3**: Mikrokontroler utama untuk pemrosesan AI dan komunikasi Bluetooth.
* **INMP441 (Microphone I2S)**: Sensor penangkap sinyal suara digital.
* **Buzzer**: Aktuator sebagai indikator audio (*feedback*) saat perintah dikenali.



---

## 📂 Struktur Repository

Repository ini dibagi menjadi beberapa direktori utama yang merangkum seluruh alur kerja proyek, mulai dari pengolahan data hingga implementasi akhir pada mikrokontroler.

### 1. Data
Folder ini mendokumentasikan manajemen dataset yang digunakan untuk melatih model. Dataset telah dibagi (*data splitting*) dengan rasio **90% untuk Data Latih (Training)** dan **10% untuk Data Uji (Testing)**. Proses pembagian data ini dilakukan di awal, tepat sebelum dataset masuk ke tahap pra-pemrosesan (*preprocessing*). Di dalam folder ini juga dilampirkan bukti tangkapan layar (*screenshot*) dari platform Edge Impulse yang memvisualisasikan sampel data mentah serta distribusi hasil pembagian datanya.

### 2. Training (Pelatihan Model)
Folder ini berisi rekam jejak dan detail teknis dari proses pelatihan model (*training*). Dokumentasi di dalam folder ini terbagi menjadi konfigurasi arsitektur dan hasil evaluasi, yang meliputi:
* **Konfigurasi Impulse**: Desain alur pemrosesan dari awal hingga inferensi yang digunakan pada sistem.
* **Parameter MFCC**: Tangkapan layar yang menunjukkan detail parameter *Mel-frequency cepstral coefficients* (MFCC) untuk tahap ekstraksi fitur suara.
* **Arsitektur CNN**: Blok kode jaringan saraf tiruan 1D CNN (*Convolutional Neural Network*) yang digunakan sebagai pengklasifikasi.
* **Confusion Matrix**: Hasil evaluasi klasifikasi model terhadap data uji, disimpan dalam format `.json` untuk memudahkan analisis data.
* **Grafik Performa**: Visualisasi kurva pelatihan model yang mencakup grafik akurasi (GAC) dan grafik *loss* (GLS) selama proses *training* berlangsung.

### 3. Model
Folder ini memuat hasil akhir dari model *machine learning* yang telah dikembangkan di Edge Impulse. Di dalamnya mencakup file model (*weights*), kode sumber awal jaringan saraf, serta parameter pra-pemrosesan (*preprocessing parameters*). Seluruh komponen arsitektur AI tersebut telah di-*export* dan dikemas (di- *build*) menjadi sebuah *library* C/C++ mandiri. *Library* ini sudah siap pakai dan dapat langsung diintegrasikan menggunakan **Arduino IDE** untuk di-*flash* ke dalam mikrokontroler.

### 4. Eksperimen Random Forest (Riwayat Riset)
Bagian ini berisi dua file skrip pelatihan (*training*) yang digunakan saat tahap awal eksperimen menggunakan algoritma Random Forest. Kedua kode ini pada dasarnya memiliki konfigurasi ekstraksi fitur yang identik, namun membedakan bagaimana data matriks fitur tersebut diumpankan ke dalam model.

**Informasi Dataset:**
* **Sumber Data**: Google Speech Commands v0.02 (diakses melalui *mount* Google Drive).
* **Jumlah Kelas**: Menggunakan 11 kelas kata yang telah diseleksi.
* **Distribusi Data**: Data dibuat seimbang (*balanced*) dengan mengambil tepat 1.400 file audio untuk masing-masing kelas.

**Parameter Ekstraksi Fitur (MFCC):**
* `N_MFCC` = 13, `FRAME_LENGTH` = 0.025, `FRAME_STRIDE` = 0.01, `PRE_EMPHASIS` = 0.98, `DURATION` = 1.0, `NFFT` = 256, `M` = 32.

**Perbedaan Pendekatan Model:**
1. **Pendekatan Rata-rata (Average):** Matriks ekstraksi MFCC dipadatkan dengan menghitung nilai rata-ratanya (*mean*), menghasilkan **13 fitur** utama per file audio.
2. **Pendekatan Data Utuh (Flatten):** Seluruh matriks digunakan secara utuh tanpa dirata-rata, menghasilkan total **1.274 fitur** per file audio.

Folder eksperimen ini juga menyajikan hasil **Evaluasi Random Forest** yang mencakup:
* **Confusion Matrix** & **Classification Report** (Precision, Recall, F1-Score).
* Hasil pengujian konsistensi menggunakan **5-Fold Cross-Validation**.
* **Visualisasi PCA 2D** untuk mengamati separabilitas fitur audio antar kelas.

### 5. Firmware (Kode Utama)
Bagian ini berisi keseluruhan kode (*source code*) utama yang telah dimodifikasi dan diintegrasikan untuk dijalankan pada mikrokontroler ESP32-S3. File ini merupakan program inti yang menggabungkan kemampuan inferensi *machine learning* (Edge AI) dengan kontrol perangkat keras secara *real-time*.

Beberapa fungsionalitas utama yang telah diimplementasikan:
* **Konektivitas Bluetooth**: Mikrokontroler dikonfigurasi agar dapat terdeteksi dan terhubung dengan PC/Laptop sebagai perangkat *Bluetooth Keyboard* nirkabel.
* **Kontrol Navigasi Slide**: Sistem secara otomatis menerjemahkan hasil klasifikasi perintah suara menjadi aksi penekanan tombol (*keystroke*). Fitur ini dioptimalkan untuk menggerakkan atau memandu jalannya presentasi (seperti berpindah ke *slide* berikutnya/sebelumnya) ketika berada dalam mode *Slide Show*.
* **Integrasi Hardware Menyeluruh**: Kode ini mengatur manajemen memori, pengambilan sampel audio melalui mikrofon I2S, pemrosesan ekstraksi fitur, serta pemicu aktuator (*buzzer*) sebagai indikator audio ketika perintah dikenali.

---

## 🚀 Cara Penggunaan

1. Buka folder `Model` dan masukkan *library* tersebut ke dalam Arduino IDE (Sketch > Include Library > Add .ZIP Library).
2. Buka folder `Firmware` dan jalankan file ekstensi `.ino` menggunakan Arduino IDE.
3. Pastikan pengaturan *board* di Arduino IDE sudah disetel untuk **ESP32-S3 Dev Module**.
4. Lakukan proses *Upload* ke perangkat.
5. Nyalakan perangkat, buka pengaturan Bluetooth di PC/Laptop Anda, dan hubungkan dengan perangkat.
6. Buka perangkat lunak presentasi (misalnya PowerPoint), masuk ke mode *Slide Show*, dan ucapkan perintah suara ke arah mikrofon.
