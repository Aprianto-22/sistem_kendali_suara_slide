
## Firmware (Kode Utama)

Bagian ini berisi keseluruhan kode (*source code*) utama yang telah dimodifikasi dan diintegrasikan untuk dijalankan pada mikrokontroler ESP32-S3. File ini merupakan program inti yang menggabungkan kemampuan inferensi *machine learning* (Edge AI) dengan kontrol perangkat keras secara *real-time*.

Beberapa fungsionalitas utama yang telah diimplementasikan dalam kode ini meliputi:

* **Konektivitas Bluetooth**: Mikrokontroler dikonfigurasi agar dapat terdeteksi dan terhubung dengan PC/Laptop sebagai perangkat *Bluetooth Keyboard* nirkabel, sehingga tidak memerlukan instalasi perangkat lunak tambahan di sisi komputer.
* **Kontrol Navigasi Slide**: Sistem secara otomatis menerjemahkan hasil klasifikasi perintah suara menjadi aksi penekanan tombol (*keystroke*). Fitur ini secara khusus dioptimalkan untuk menggerakkan atau memandu jalannya presentasi (seperti berpindah ke *slide* berikutnya atau sebelumnya) ketika berada dalam mode *Slide Show*.
* **Integrasi Hardware Menyeluruh**: Selain komunikasi nirkabel, kode ini juga mengatur manajemen memori dan alur kerja perangkat keras lainnya, termasuk pengambilan sampel audio terus-menerus melalui mikrofon, pemrosesan ekstraksi fitur, serta pemicu aktuator (*buzzer*) sebagai indikator audio ketika perintah suara berhasil dikenali.
