
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <driver/i2s.h>

const char* ssid     = ""; // not public of course :) if someone know how to
const char* password = ""; // keep secret this, open an issue to help me.

#define I2S_WS 15
#define I2S_SD 13
#define I2S_SCK 2
#define I2S_PORT I2S_NUM_0

unsigned long previousMillis = 0;
const long interval = 5000;

WiFiUDP udp;
const IPAddress listener = { 192, 168, 0, 0 }; // Cardinal server listener here
const int port = 1720; // C Benoux

void setup() {
  Serial.begin(115200); // serial in case of debugging via usb
  delay(1000);

  WiFi.mode(WIFI_STA); // enable wifi client only
  i2s_rxtx_begin(true, false); // Enable I2S RX
  i2s_set_rate(11025);
  connectToWifi();

  i2s_install();
  i2s_setpin();
  i2s_start(I2S_PORT);
  Serial.println("ready to send");

  delay(500);
}

void loop() {
  /*unsigned long currentMillis = millis();
  
  if(currentMillis - previousMillis >= interval) {
     // Check WiFi connection status
    if (WiFi.status() != WL_CONNECTED) {
      connectToWifi();
    }

    static uint32_t cnt = 0;
    // Each loop will send 100 raw samples (400 bytes)
    // UDP needs to be < TCP_MSS which can be 500 bytes in LWIP2
    for (int i = 0; i < 100; i++) {
      i2s_read_sample(&buffer[i][0], &buffer[i][1], true);
    }
    udp.beginPacket(listener, port);
    udp.write((uint8_t*)buffer, sizeof(buffer));
    udp.endPacket();
    cnt++;
    if ((cnt % 100) == 0) {
      Serial.printf("%" PRIu32 "\n", cnt);
    }


    Serial.println("loop end");
    
    previousMillis = currentMillis;
  }*/

  int32_t sample = 0;
  int bytes = i2s_pop_sample(I2S_PORT, (char*)&sample, portMAX_DELAY);
  if(bytes > 0){
    Serial.println(sample);
  }

}

void i2s_install(){
  const i2s_config_t i2s_config = {
    .mode = i2s_mode_t(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = 44100,
    .bits_per_sample = i2s_bits_per_sample_t(16),
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = i2s_comm_format_t(I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB),
    .intr_alloc_flags = 0, // default interrupt priority
    .dma_buf_count = 8,
    .dma_buf_len = 64,
    .use_apll = false
  };

  i2s_driver_install(I2S_PORT, &i2s_config, 0, NULL);
}

void i2s_setpin(){
  const i2s_pin_config_t pin_config = {
    .bck_io_num = I2S_SCK,
    .ws_io_num = I2S_WS,
    .data_out_num = -1,
    .data_in_num = I2S_SD
  };

  i2s_set_pin(I2S_PORT, &pin_config);
}

void connectToWifi() {
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.hostname("CardinalESP");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(2000);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected. IP: ");
  Serial.println(WiFi.localIP());
}