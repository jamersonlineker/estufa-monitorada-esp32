#include <stdio.h> // Biblioteca padrão de entrada e saída

#include "storage.h" // Biblioteca com funções de armazenamento
#include "sensors.h" // Biblioteca com variáveis dos sensores

#include "esp_spiffs.h" // Biblioteca SPIFFS do ESP-IDF
#include "esp_vfs.h"    // Biblioteca Virtual File System

// ======================================================
// FUNÇÃO: init_spiffs
// OBJETIVO:
// Inicializar o sistema de arquivos SPIFFS
// ======================================================

void init_spiffs() {

  // Estrutura de configuração do SPIFFS
  esp_vfs_spiffs_conf_t conf = {

      // Caminho base onde o SPIFFS será montado
      .base_path = "/spiffs",

      // Usa partição padrão
      .partition_label = NULL,

      // Número máximo de arquivos abertos simultaneamente
      .max_files = 5,

      // Formata automaticamente se houver falha na montagem
      .format_if_mount_failed = true
  };

  // Registra e monta o SPIFFS
  esp_err_t ret = esp_vfs_spiffs_register(&conf);

  // Verifica se houve erro
  if (ret != ESP_OK) {

    printf("Erro SPIFFS\n");

  } else {

    printf("SPIFFS OK\n");
  }
}

// ======================================================
// FUNÇÃO: save_log
// OBJETIVO:
// Salvar dados dos sensores em arquivo de log
// ======================================================

void save_log() {

  // Abre o arquivo em modo append ("a")
  // Novos dados serão adicionados ao final
  FILE *f = fopen("/spiffs/log.txt", "a");

  // Verifica se houve erro ao abrir o arquivo
  if (f == NULL) {

    printf("Erro ao abrir log\n");
    return;
  }

  // Escreve os dados dos sensores no arquivo
  fprintf(
      f,
      "Temp: %.1f | Umid: %.1f | Luz: %d\n",
      temperatura,
      umidade,
      luminosidade
  );

  // Fecha o arquivo
  fclose(f);
}

// ======================================================
// FUNÇÃO: read_log_file
// OBJETIVO:
// Ler e exibir conteúdo do arquivo de log
// ======================================================

void read_log_file() {

  // Abre o arquivo em modo leitura
  FILE *f = fopen("/spiffs/log.txt", "r");

  // Verifica se houve erro
  if (f == NULL) {

    printf("Erro ao abrir log.txt\n");
    return;
  }

  // Buffer para armazenar cada linha lida
  char line[128];

  printf("\n===== LOG =====\n");

  // Lê linha por linha até o final do arquivo
  while (fgets(line, sizeof(line), f) != NULL) {

    // Exibe linha no terminal
    printf("%s", line);
  }

  printf("\n===== FIM =====\n");

  // Fecha o arquivo
  fclose(f);
}

// ======================================================
// FUNÇÃO: clear_log_file
// OBJETIVO:
// Apagar arquivo de log do SPIFFS
// ======================================================

void clear_log_file() {

    // Remove o arquivo log.txt
    remove("/spiffs/log.txt");

    // Mensagem de confirmação
    printf("Log apagado\n");
}