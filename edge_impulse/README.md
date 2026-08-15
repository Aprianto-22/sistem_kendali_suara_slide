## Data
Folder ini mendokumentasikan manajemen dataset yang digunakan untuk melatih model. Dataset telah dibagi (data splitting) dengan rasio 90% untuk Data Latih (Training) dan 10% untuk Data Uji (Testing). Proses pembagian data ini dilakukan di awal, tepat sebelum dataset masuk ke tahap pra-pemrosesan (preprocessing). Di dalam folder ini juga dilampirkan bukti tangkapan layar (screenshot) dari platform Edge Impulse yang memvisualisasikan sampel data mentah serta distribusi hasil pembagian datanya.

## Model
Folder ini memuat hasil akhir dari model machine learning yang telah dikembangkan di Edge Impulse. Di dalamnya mencakup file model (weights), kode awal, serta parameter pra-pemrosesan (preprocessing parameters). Seluruh komponen arsitektur AI tersebut telah di-export dan dikemas (di- build) menjadi sebuah library C/C++ mandiri. Library ini sudah siap pakai dan dapat langsung diintegrasikan menggunakan Arduino IDE untuk di-flash ke dalam mikrokontroler.

## Training
Folder ini berisi rekam jejak dan detail teknis dari proses pelatihan model (training). Dokumentasi di dalam folder ini terbagi menjadi konfigurasi arsitektur dan hasil evaluasi, yang meliputi: Konfigurasi Impulse: Desain alur pemrosesan dari awal hingga inferensi yang digunakan pada sistem, Parameter MFCC: screenshot yang menunjukkan detail parameter Mel-frequency cepstral coefficients (MFCC) untuk tahap ekstraksi fitur suara, Arsitektur CNN: kode 1D CNN (Convolutional Neural Network) yang digunakan sebagai pengklasifikasi.



Confusion Matrix: Hasil evaluasi klasifikasi model terhadap data uji, disimpan dalam format .json untuk memudahkan analisis data.

Grafik Performa: Visualisasi kurva pelatihan model yang mencakup grafik akurasi (GAC) dan grafik loss (GLS) selama proses training berlangsung.
