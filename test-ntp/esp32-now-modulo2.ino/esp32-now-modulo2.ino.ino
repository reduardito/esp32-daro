#include <esp_now.h>
#include <WiFi.h>

// REPLACE WITH YOUR RECEIVER MAC Address
uint8_t broadcastAddress[] = {0x0C, 0xB8, 0x15, 0xD8, 0xA7, 0x38};

// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
    //int a;
    String a;
} struct_message;

// Create a struct_message called myData
struct_message myDataRec;
struct_message myDataSen;

esp_now_peer_info_t peerInfo;

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myDataRec, incomingData, sizeof(myDataRec));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("Int: ");
  Serial.println(myDataRec.a);
  Serial.println();
}
 
void setup() {
  // Initialize Serial Monitor
  Serial.begin(9600);
  
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(OnDataRecv);

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("No se encuentra ESP32-");
    Serial.println(WiFi.macAddress());
    return;
  }
}
 
void loop() {
  // Set values to send
  myDataSen.a = "Esclavo";

  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myDataSen, sizeof(myDataSen));
   
  if (result == ESP_OK) {
    Serial.println("Mensaje enviado");
  }
  else {
    Serial.println("Error al enviar mensaje");
  }
  delay(2000);
}
