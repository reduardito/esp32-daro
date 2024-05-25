# esp32-daro

Sensor de humedad Servidor/Cliente
Multiples clientes
Mono server centralizado


https://drive.google.com/file/d/1R1pvMdCNNM5A3M-Cr81p2wkkZPHAgzsc/view?usp=drive_link


Ultima version 25.05.2024
se cambia el check ntp continuamente mediante internet a una consulta para configurar el rtc (reloj local del esp32), ahora para cada registro de datos se usa este rtc local evitando consultar por cada scand
se agrega estado de microsd 
