
# DESCRIÇÃO

# Estufa Monitorada e Controlada com ESP32

Sistema embarcado desenvolvido com ESP32 para monitoramento e controle de condições ambientais de uma estufa, utilizando sensores de temperatura, umidade e luminosidade, com interface em display OLED, LEDs indicadores e armazenamento de logs em SPIFFS.

---

# Funcionalidades

- Leitura de temperatura e umidade com sensor DHT11
- Leitura de luminosidade com LDR
- Interface gráfica em display OLED SSD1306
- Menu de navegação com botões
- Controle visual por LEDs:
  - LED Vermelho → Temperatura
  - LED Verde → Umidade
  - LED Azul → Luminosidade
- Sistema de logs em memória SPIFFS
- Comunicação serial UART
- Comandos via terminal:
  - `readlog`
  - `clearlog`

---

# Hardware Utilizado

## Microcontrolador
- ESP32 DevKit V1

## Sensores
- DHT11
- LDR

## Display
- OLED SSD1306 128x64 (I2C)

## Atuadores
- LED Vermelho
- LED Verde
- LED Azul

## Entradas
- 3 Botões de navegação

---

# Mapeamento de Pinos

| Função | GPIO |
|---|---|
| DHT11 | GPIO 33 |
| LDR | GPIO 32 |
| Botão UP | GPIO 18 |
| Botão DOWN | GPIO 19 |
| Botão SELECT | GPIO 23 |
| OLED SDA | GPIO 21 |
| OLED SCL | GPIO 22 |
| LED Vermelho | GPIO 16 |
| LED Verde | GPIO 17 |
| LED Azul | GPIO 4 |

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



# Instalação e Configuração

1. Instalar ESP-IDF

Instale o ESP-IDF conforme documentação oficial da Espressif.

2. Clonar o projeto
git clone https://github.com/jamersonlineker/estufa-monitorada-esp32.git

3. Abrir projeto
cd Estufa_controlada

4. Configurar projeto
idf.py menuconfig


## Configurar:

GPIO do DHT11
GPIO do LDR
Valores padrão:
Temperatura máxima
Umidade mínima
Luminosidade mínima

5. Compilar
idf.py build


6. Gravar no ESP32
idf.py flash monitor


# Funcionamento do Sistema
`Tela Principal`

Exibe:

Temperatura
Umidade
Luminosidade

Também permite acessar o menu através do botão SELECT.

## Sistema de Menu
`Edit Sensor`
Permite alterar:

Temperatura máxima
Umidade mínima
Luminosidade mínima

`Config`
Permite:

Ativar/desativar sistema
Ativar/desativar logs


Sistema de LEDs
LED	Função
Vermelho	Temperatura acima do limite
Verde	Umidade abaixo do limite
Azul	Luminosidade abaixo do limite


Sistema de Logs
Os dados são armazenados no arquivo:

/spiffs/log.txt
Comandos UART
Ler logs
readlog
Apagar logs
clearlog


# Exemplos de Funcionamento
Exemplo 1 — Temperatura Alta

Se:

Temperatura >= temp_max
Então:
LED Vermelho ON


Exemplo 2 — Baixa Luminosidade

Se:
Luminosidade <= luz_min
Então:
LED Azul ON

Tecnologias Utilizadas

ESP-IDF
Linguagem C
FreeRTOS
SPIFFS
I2C
UART
ADC


Autor
Jamerson Lineker
Projeto desenvolvido para Sistemas Embarcados.