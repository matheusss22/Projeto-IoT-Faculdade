# 🛰️ Projeto: Leitor de Cartões RFID com ESP32 + RC522

Este projeto tem como objetivo realizar a leitura do **UID de cartões RFID** utilizando o módulo **RC522** com um **ESP32**, usando a biblioteca [MFRC522](https://github.com/miguelbalboa/rfid) e o **PlatformIO** como ambiente de desenvolvimento.

---

## 📦 Componentes Utilizados

| Componente        | Modelo / Especificação      |
|-------------------|-----------------------------|
| Microcontrolador  | ESP32 DevKit V1             |
| Leitor RFID       | RC522                       |
| Tags RFID         | 13.56 MHz                   |
| IDE               | PlatformIO (VS Code)        |
| Biblioteca        | `miguelbalboa/MFRC522@^1.4.12` |

---

## 🔌 Esquema de Ligações (Wiring)

| Pino RC522 | Pino ESP32 | Função             |
|------------|------------|--------------------|
| SDA        | GPIO 4     | `SS` - Chip Select |
| SCK        | GPIO 18    | `SCK` - Clock      |
| MOSI       | GPIO 23    | `MOSI` - Dados     |
| MISO       | GPIO 19    | `MISO` - Dados     |
| RST        | GPIO 2     | Reset              |
| GND        | GND        | Terra              |
| 3.3V       | 3.3V       | Alimentação        |

⚠️ **Importante:** Nunca ligue o RC522 ao pino de 5V no ESP32! Use sempre **3.3V**, pois o ESP32 não é tolerante a 5V.

🔍 [Saiba mais sobre SPI](https://embarcados.com.br/spi-parte-1/)