

## Eksperimen Random Forest

Bagian ini berisi dua file skrip pelatihan (*training*) yang digunakan saat tahap awal eksperimen menggunakan algoritma Random Forest. Kedua kode ini pada dasarnya memiliki konfigurasi ekstraksi fitur yang identik, namun membedakan bagaimana data matriks fitur tersebut diumpankan ke dalam model.

**Informasi Dataset:**

* **Sumber Data**: Google Speech Commands v0.02 (diakses melalui *mount* Google Drive).
* **Jumlah Kelas**: Menggunakan 11 kelas kata yang telah diseleksi.
* **Distribusi Data**: Data dibuat seimbang (*balanced*) dengan mengambil tepat 1.400 file audio untuk masing-masing kelas.

**Parameter Ekstraksi Fitur (MFCC):**
Proses pengolahan sinyal audio (*preprocessing*) dilakukan menggunakan parameter *Mel-frequency cepstral coefficients* (MFCC) berikut:

* `N_MFCC` = 13 (Jumlah koefisien yang diambil)
* `FRAME_LENGTH` = 0.025 (Panjang jendela 25 ms)
* `FRAME_STRIDE` = 0.01 (Langkah pergeseran 10 ms)
* `PRE_EMPHASIS` = 0.98 (Untuk memperkuat frekuensi tinggi)
* `DURATION` = 1.0 (Durasi audio 1 detik)
* `NFFT` = 256 (Ukuran *Fast Fourier Transform*)
* `M` = 32 (Jumlah *filterbank*)

**Perbedaan Implementasi pada Kedua File:**
Meskipun parameter ekstraksinya sama, terdapat perbedaan pada bentuk masukan data (*input shape*) yang dilatih pada Random Forest:

1. **Kode 1 (Pendekatan Rata-rata / Average):** Pada skrip pertama, matriks hasil ekstraksi MFCC dipadatkan dengan menghitung nilai rata-ratanya (*mean*). Proses ini mereduksi dimensi data secara drastis sehingga setiap file audio hanya diwakili oleh **13 fitur** utama. Pendekatan ini menghasilkan model yang jauh lebih ringan dan komputasi yang lebih ringkas.
2. **Kode 2 (Pendekatan Data Utuh / Flatten):** Pada skrip kedua, seluruh matriks hasil ekstraksi MFCC digunakan secara utuh tanpa dirata-rata. Dimensi matriks diratakan (*flatten*) sehingga satu file audio menghasilkan total **1.274 fitur**. Pendekatan ini memberikan informasi data yang lebih detail dan komprehensif kepada model.
