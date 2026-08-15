
## Hasil Evaluasi Random Forest

Bagian ini menyajikan dokumentasi hasil pengujian dan evaluasi performa komparatif antara kedua pendekatan model Random Forest (model dengan 13 fitur rata-rata dan model dengan 1.274 fitur utuh). Data evaluasi ini digunakan untuk membandingkan efektivitas masing-masing pendekatan melalui beberapa parameter berikut:

* **Confusion Matrix**: Visualisasi matriks performa klasifikasi untuk kedua model. Grafik ini menunjukkan dengan jelas kemampuan model dalam menebak masing-masing kelas audio (akurasi prediksi terhadap kelas aktual) dan melihat pada kelas mana model sering mengalami kesalahan (*misclassification*).
* **Classification Report**: Laporan metrik evaluasi menyeluruh yang mencakup nilai *Precision*, *Recall*, dan *F1-Score* untuk ke-11 kelas. Laporan ini memberikan gambaran detail mengenai performa pengenalan suara pada setiap kategori.
* **5-Fold Cross-Validation**: Hasil pengujian konsistensi dan stabilitas model menggunakan teknik validasi silang (*cross-validation*) sebanyak 5 lipatan (*fold*). Pengujian ini memastikan bahwa keandalan model bersifat konsisten pada berbagai partisi dataset dan tidak terjadi *overfitting*.
* **Visualisasi PCA 2D**: Grafik persebaran data ekstraksi MFCC yang divisualisasikan dalam bentuk 2 Dimensi. Reduksi dimensi ini menggunakan metode *Principal Component Analysis* (PCA) untuk mengamati seberapa baik fitur audio terpisah (*separability*) antar kelas sebelum diumpankan ke dalam algoritma Random Forest.
