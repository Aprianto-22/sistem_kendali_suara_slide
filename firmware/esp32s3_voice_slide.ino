/*
 * PROJECT: Voice Controlled Slide Navigator
 * HARDWARE: ESP32-S3 + INMP441
 */

#define USE_NIMBLE 

#include <Helmi_234-project-1_inferencing.h> 
#include <NimBLEDevice.h>
#include <BleKeyboard.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2s.h"

// ================= PIN I2S ESP32-S3 =================
#define I2S_SD_PIN  15
#define I2S_WS_PIN  16
#define I2S_SCK_PIN 17
#define I2S_PORT    I2S_NUM_0  

#define CONFIDENCE_THRESHOLD 0.9 
#define NUMBER_THRESHOLD     0.9
#define ACTION_DELAY         500 

BleKeyboard bleKeyboard("Voice Remote", "ESP32", 100);

typedef struct {
    int16_t *buffer;
    uint8_t buf_ready;
    uint32_t buf_count;
    uint32_t n_samples;
} inference_t;

static inference_t inference;
static const uint32_t sample_buffer_size = 2048;
// Buffer penampung mentah dari I2S harus 32-bit karena hardware INMP441
static int32_t rawSampleBuffer[sample_buffer_size]; 
static bool record_status = true;

// Prototipe Fungsi
static bool microphone_inference_start(uint32_t n_samples);
static bool microphone_inference_record(void);
static void microphone_inference_end(void);
static int microphone_audio_signal_get_data(size_t offset, size_t length, float *out_ptr);
static int i2s_init(uint32_t sampling_rate);
static int i2s_deinit(void);

void setup() {
    Serial.begin(115200);
    bleKeyboard.begin();
    pinMode(2, OUTPUT);
    
    if (microphone_inference_start(EI_CLASSIFIER_RAW_SAMPLE_COUNT) == false) {
        ei_printf("ERR: Gagal alokasi buffer audio\n");
        return;
    }
    ei_printf("\nSISTEM SIAP. SILAKAN HUBUNGKAN BLUETOOTH...\n");
}

void loop() {
    if (!microphone_inference_record()) return;

    signal_t signal;
    signal.total_length = EI_CLASSIFIER_RAW_SAMPLE_COUNT;
    signal.get_data = &microphone_audio_signal_get_data;
    ei_impulse_result_t result = { 0 };

    if (run_classifier(&signal, &result, false) != EI_IMPULSE_OK) return;

    // Menghitung skor tertinggi
    float max_score = 0.0;
    int max_index = -1;
    for (size_t ix = 0; ix < EI_CLASSIFIER_LABEL_COUNT; ix++) {
        if (result.classification[ix].value > max_score) {
            max_score = result.classification[ix].value;
            max_index = ix;
        }
    }

    const char* label = result.classification[max_index].label;
    ei_printf("Top Prediksi: %s (%.2f)\n", label, max_score);

  // BLUETOOTH
    
    if (bleKeyboard.isConnected()) {
        digitalWrite(2, HIGH); // LED Menyala jika terhubung bluetooth
        bool actionTaken = false;
        float current_threshold = CONFIDENCE_THRESHOLD;

        if (strcmp(label, "forward") != 0 && strcmp(label, "backward") != 0 && 
            strcmp(label, "unknown") != 0 && strcmp(label, "noise") != 0) {
            current_threshold = NUMBER_THRESHOLD;
        }

        if (max_score >= current_threshold) {
            
            // --- LOGIKA NAVIGASI ---
            if (strcmp(label, "forward") == 0) {
                bleKeyboard.write(KEY_RIGHT_ARROW);
                actionTaken = true;
            }
            else if (strcmp(label, "backward") == 0) {
                bleKeyboard.write(KEY_LEFT_ARROW);
                actionTaken = true;
            }
            // --- LOGIKA JUMP SLIDE ---
            else if (strcmp(label, "one") == 0) { bleKeyboard.print("1"); delay(50); bleKeyboard.write(KEY_RETURN); actionTaken = true; }
            else if (strcmp(label, "two") == 0) { bleKeyboard.print("2"); delay(50); bleKeyboard.write(KEY_RETURN); actionTaken = true; }
            else if (strcmp(label, "three") == 0) { bleKeyboard.print("3"); delay(50); bleKeyboard.write(KEY_RETURN); actionTaken = true; }
            else if (strcmp(label, "four") == 0) { bleKeyboard.print("4"); delay(50); bleKeyboard.write(KEY_RETURN); actionTaken = true; }
            else if (strcmp(label, "five") == 0) { bleKeyboard.print("5"); delay(50); bleKeyboard.write(KEY_RETURN); actionTaken = true; }
            else if (strcmp(label, "six") == 0) { bleKeyboard.print("6"); delay(50); bleKeyboard.write(KEY_RETURN); actionTaken = true; }
            else if (strcmp(label, "seven") == 0) { bleKeyboard.print("7"); delay(50); bleKeyboard.write(KEY_RETURN); actionTaken = true; }
            else if (strcmp(label, "eight") == 0) { bleKeyboard.print("8"); delay(50); bleKeyboard.write(KEY_RETURN); actionTaken = true; }
            else if (strcmp(label, "nine") == 0) { bleKeyboard.print("9"); delay(50); bleKeyboard.write(KEY_RETURN); actionTaken = true; }

            if (actionTaken) {
                ei_printf(">>> AKSI DIEKSEKUSI BLE: %s <<<\n", label);
                delay(ACTION_DELAY);
                // Reset buffer setelah aksi sukses agar tidak berulang
                inference.buf_ready = 0; 
                inference.buf_count = 0;
            }
        }
    } else {
        digitalWrite(2, LOW);
    }
}

// ================= I2S & AUDIO PROCESSING =================

static void capture_samples(void* arg) {
    const int32_t i2s_bytes_to_read = sample_buffer_size * 4; // Karena membaca 32-bit (4 bytes per sample)
    size_t bytes_read;
    
    while (record_status) {
        // Baca data mentah 32-bit dari port 0
        i2s_read(I2S_PORT, (void*)rawSampleBuffer, i2s_bytes_to_read, &bytes_read, portMAX_DELAY);
        
        int samples_actual = bytes_read / 4;
        
        if (samples_actual > 0) {
            for (int i = 0; i < samples_actual; i++) {
                // Konversi data 32-bit INMP441 ke 16-bit dengan pergeseran bit (bit-shift)
                int32_t sample = rawSampleBuffer[i] >> 14; // Menggeser presisi ke jangkauan 16-bit
                
                // Berikan penguatan volume tambahan (gain)
                sample = sample * 4; 

                // Batasi nilai agar tidak berlebih (clipping)
                if (sample > 32767) sample = 32767;
                if (sample < -32768) sample = -32768;

                inference.buffer[inference.buf_count++] = (int16_t)sample;
                
                if (inference.buf_count >= inference.n_samples) {
                    inference.buf_count = 0;
                    inference.buf_ready = 1;
                }
            }
        }
    }
    vTaskDelete(NULL);
}

static bool microphone_inference_start(uint32_t n_samples) {
    inference.buffer = (int16_t *)malloc(n_samples * sizeof(int16_t));
    if(inference.buffer == NULL) return false;
    inference.buf_count = 0;
    inference.n_samples = n_samples;
    inference.buf_ready = 0;
    
    if (i2s_init(EI_CLASSIFIER_FREQUENCY) != 0) return false;
    record_status = true;
    
    // Alokasikan task pada core 1 (agar Core 0 fokus menangani Bluetooth BLE)
    xTaskCreatePinnedToCore(capture_samples, "CaptureSamples", 1024 * 8, NULL, 5, NULL, 1);
    return true;
}

static bool microphone_inference_record(void) {
    while (inference.buf_ready == 0) {
        vTaskDelay(pdMS_TO_TICKS(10));
    }
    inference.buf_ready = 0;
    return true;
}

static int microphone_audio_signal_get_data(size_t offset, size_t length, float *out_ptr) {
    numpy::int16_to_float(&inference.buffer[offset], out_ptr, length);
    return 0;
}

static int i2s_init(uint32_t sampling_rate) {
    i2s_config_t i2s_config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
        .sample_rate = sampling_rate,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT, // WAJIB 32BIT untuk INMP441
        .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT, 
        .communication_format = i2s_comm_format_t(I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB),
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
        .dma_buf_count = 8,
        .dma_buf_len = 256
    };
    i2s_pin_config_t pin_config = {
        .bck_io_num = I2S_SCK_PIN, 
        .ws_io_num = I2S_WS_PIN, 
        .data_out_num = -1, 
        .data_in_num = I2S_SD_PIN
    };
    
    esp_err_t ret = i2s_driver_install(I2S_PORT, &i2s_config, 0, NULL);
    if (ret != ESP_OK) return (int)ret;
    return i2s_set_pin(I2S_PORT, &pin_config);
}

static int i2s_deinit(void) {
    i2s_driver_uninstall(I2S_PORT);
    return 0;
}
