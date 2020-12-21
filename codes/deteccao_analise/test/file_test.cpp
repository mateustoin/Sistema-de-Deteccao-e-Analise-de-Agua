#include <Arduino.h>
#include "FileIO.h"

void setup() {
    Serial.begin(115200);

    if(!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)){
        Serial.println("SPIFFS Mount Failed");
        return;
    }

    listDir(SPIFFS, "/", 0);
    //writeFile(SPIFFS, "/sac-dm.txt", "");
    //appendFile(SPIFFS, "/sac-dm.txt", "Mateus Antonio da Silva\r\n");
/*
    delay(1000);
    writeFile(SPIFFS, "/hello.txt", "Hello ");
    delay(1000);
    appendFile(SPIFFS, "/hello.txt", "World!\r\n");
    delay(1000);
    readFile(SPIFFS, "/hello.txt");
    delay(1000);
    renameFile(SPIFFS, "/hello.txt", "/foo.txt");
    delay(1000);
    readFile(SPIFFS, "/foo.txt");
    delay(1000);
    deleteFile(SPIFFS, "/foo.txt");
    delay(1000);
    testFileIO(SPIFFS, "/test.txt");
    delay(1000);
    deleteFile(SPIFFS, "/test.txt");
    delay(1000);
    Serial.println( "Test complete" );
*/
}

void loop() {
    
}