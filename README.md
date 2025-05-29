# 🔐 Fechadura Eletrônica com Cartão RFID e ESP32 (PlatformIO)

Projeto desenvolvido com PlatformIO e ESP32 para controle de acesso com cartões RFID usando o módulo MFRC522.

## 📸 Funcionalidades:

- ✅ Acesso via cartões RFID autorizados
- ✅ Controle de uma fechadura eletrônica
- ✅ LEDs indicativos: verde (autorizado), vermelho (negado)
- ✅ Debug serial ativado
- ✅ Fácil cadastro de novos cartões

## 🔌 Esquemático de Ligações:

(ESP32)GPIO05  <----------- (MFRC522) SS - SDA</br>
(ESP32)GPIO02  <----------- (MFRC522) RST</br>
(ESP32)GPIO18 <----------- (MFRC522) SCK</br>
(ESP32)GPIO23 <----------- (MFRC522) MOSI</br>
(ESP32)GPIO19 <----------- (MFRC522) MISO</br>
</br>
(ESP32)GPIO15 ------------> (MFRC522) FECHADURA ELETRÔNICA</br>
(ESP32)GPIO32 ------------> (MFRC522) LED VERDE</br>
(ESP32)GPIO33 ------------> (MFRC522) LED VERMELHO</br>


 Observações:
 - Use conversores de nível lógico se necessário.
 - A fechadura pode ser um relé, solenóide ou trava magnética.

## 🚀 Instalação com PlatformIO

### 1. Requisitos

- [VS Code](https://code.visualstudio.com/)
- [PlatformIO IDE](https://platformio.org/install/ide?install=vscode)

### 2. Clonar o repositório

```bash
git clone https://github.com/seu-usuario/rfid-door-lock-esp32.git
cd rfid-door-lock-esp32
```

### 3. Abrir o projeto no VS Code
- Vá em File > Open Folder e selecione o diretório clonado.

### 4. Verifique o platformio.ini

Exemplo básico:

```ini
[env:esp32dev]
platform = espressif32
board = esp32dev
framework = arduino
monitor_speed = 115200
```

### 5. Compilar e enviar para o ESP32
- Clique em PlatformIO: Upload ou use o atalho Ctrl + Alt + U.

### 6. Abrir o monitor serial
- Use o ícone do plug serial ou Ctrl + Alt + M.

### 7. Como Cadastrar Novos Cartões

1. Abra o Monitor Serial após enviar o código.

2. Aproxime um novo cartão RFID.

3. Você verá algo como:

```
Card UID: 53 B2 61 0B
Card SAK: 08
PICC type: MIFARE 1KB
Reading success: Success.       
Unauthorized card.
```
4. Copie o UID exibido e adicione ao array authorizedUIDs:

```cpp
const char *authorizedUIDs[] = {
    "8C:7B:C2:6E",
    "AA:BB:CC:DD", // Novo cartão aqui
};
```
5. Reenvie o código (Ctrl + Alt + U).


### 📁 Estrutura do Projeto

rfid-door-lock-esp32/
├── doc/img
├── include/
├── lib/
├── src/main.cpp
├── test
├── .gitignore
├── platformio.ini
└── README.md
