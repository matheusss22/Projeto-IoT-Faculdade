# 📡 SPI Loopback Debug com ESP32

Este projeto tem como objetivo testar o funcionamento da comunicação SPI no ESP32 utilizando o modo **loopback**, onde o pino **MOSI** é conectado diretamente ao pino **MISO**. Esse teste verifica se o canal SPI está funcionando corretamente, seja via software ou hardware.

---

## 🧰 Requisitos

- ESP32 (qualquer modelo com suporte a SPI)
- Jumpers para conexão
- Conexão física entre os pinos **MOSI (GPIO 23)** e **MISO (GPIO 19)**

---

## 🔌 Conexões

| Função | Pino ESP32 | Descrição                   |
|--------|------------|-----------------------------|
| MOSI   | GPIO 23    | Master Out Slave In         |
| MISO   | GPIO 19    | Master In Slave Out         |
| SCK    | GPIO 18    | Clock SPI                   |
| SS     | GPIO 5     | Slave Select (apenas para `SPI.begin()`) |

> ⚠️ **Importante**: Conecte fisicamente o pino **MOSI (23)** ao **MISO (19)** para que o teste funcione (loopback físico).

---

## 🧪 Funcionamento

O código envia o byte `0x12` via SPI e espera receber exatamente o mesmo valor. Isso só ocorrerá se:

- A comunicação SPI estiver funcional.
- Os pinos **MOSI** e **MISO** estiverem conectados corretamente (loopback).

Caso receba corretamente, o monitor serial mostrará:

```arduino
✅ SPI funcionando (loopback ok)
```

Caso contrário, exibirá:

```arduino
❌ SPI falhou - problema no ESP32 ou fiação
```

---

## 🛠️ Dicas de Debug

- Verifique se os pinos estão corretamente conectados.
- Certifique-se de que nenhum outro dispositivo está utilizando o barramento SPI.
- Use uma protoboard ou jumper de boa qualidade.
- Teste diferentes pinos SPI se estiver usando um módulo com múltiplos canais SPI.

---