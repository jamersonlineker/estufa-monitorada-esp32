# DESCRIÇÃO

# 🌱 Estufa Monitorada e Controlada com ESP32

Sistema embarcado desenvolvido com ESP32 para monitoramento e controle de condições ambientais de uma estufa, utilizando sensores de temperatura, umidade e luminosidade, com interface em display OLED, LEDs indicadores e armazenamento de logs em SPIFFS.

---

# Funcionalidades

* Leitura de temperatura e umidade com sensor DHT11
* Leitura de luminosidade com LDR
* Interface gráfica em display OLED SSD1306
* Menu de navegação com botões
* Controle visual por LEDs:

  * 🔴 LED Vermelho → Temperatura
  * 🟢 LED Verde → Umidade
  * 🔵 LED Azul → Luminosidade
* Sistema de logs em memória SPIFFS
* Comunicação serial UART

## Comandos via terminal

```bash
readlog
clearlog
```

---

# Hardware Utilizado

## Microcontrolador

* ESP32 DevKit V1

## Sensores

* DHT11
* LDR

## Display

* OLED SSD1306 128x64 (I2C)

---

# Diagrama de Blocos

O diagrama apresenta a comunicação entre sensores, display OLED, botões e saídas de controle do sistema embarcado.

![alt text](<Diagrama de bloco.png>)

---
# Estrutura do Projeto

```text
main/
│
├── main.c
├── display.c
├── display.h
├── menu.c
├── menu.h
├── sensors.c
├── sensors.h
├── storage.c
├── storage.h
├── control.c
├── control.h
```

---

# Instalação e Configuração

## 1. Clonar o projeto

```bash
git clone https://github.com/jamersonlineker/estufa-monitorada-esp32.git
```

## 2. Entrar na pasta

```bash
cd Estufa_controlada
```

## 3. Configurar

```bash
idf.py menuconfig
```

## 4. Compilar

```bash
idf.py build
```

## 5. Gravar no ESP32

```bash
idf.py flash monitor
```

---

# Sistema de LEDs

| LED         | Função                        |
| ----------- | ----------------------------- |
| 🔴 Vermelho | Temperatura acima do limite   |
| 🟢 Verde    | Umidade abaixo do limite      |
| 🔵 Azul     | Luminosidade abaixo do limite |

---

# Tecnologias Utilizadas

* ESP-IDF
* Linguagem C
* FreeRTOS
* SPIFFS
* I2C
* UART
* ADC

---

# Autor

Jamerson Lineker

Projeto desenvolvido para disciplina de Sistemas Embarcados.
