#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "matrizes.c"

// Biblioteca gerada pelo arquivo .pio durante compilação.
#include "ws2818b.pio.h"

// Definição do número de LEDs e pino.
#define LED_COUNT 25
#define LED_PIN 7
#define BUTTON_A 5 // Botão "A" na GPIO 5
#define BUTTON_B 6 // Botão "B" na GPIO 6

// Definição de pixel GRB
struct pixel_t
{
  uint8_t G, R, B; // Três valores de 8-bits compõem um pixel.
};
typedef struct pixel_t pixel_t;
typedef pixel_t npLED_t; // Mudança de nome de "struct pixel_t" para "npLED_t" por clareza.

// Declaração do buffer de pixels que formam a matriz.
npLED_t leds[LED_COUNT];

// Variáveis para uso da máquina PIO.
PIO np_pio;
uint sm;

// Variável global para armazenar o número atual
volatile int numero_atual = 0;

/**
 * Inicializa a máquina PIO para controle da matriz de LEDs.
 */
void npInit(uint pin)
{

  // Cria programa PIO.
  uint offset = pio_add_program(pio0, &ws2818b_program);
  np_pio = pio0;

  // Toma posse de uma máquina PIO.
  sm = pio_claim_unused_sm(np_pio, false);
  if (sm < 0)
  {
    np_pio = pio1;
    sm = pio_claim_unused_sm(np_pio, true); // Se nenhuma máquina estiver livre, panic!
  }

  // Inicia programa na máquina PIO obtida.
  ws2818b_program_init(np_pio, sm, offset, pin, 800000.f);

  // Limpa buffer de pixels.
  for (uint i = 0; i < LED_COUNT; ++i)
  {
    leds[i].R = 0;
    leds[i].G = 0;
    leds[i].B = 0;
  }
}

/**
 * Atribui uma cor RGB a um LED.
 */
void npSetLED(const uint index, const uint8_t r, const uint8_t g, const uint8_t b)
{
  leds[index].R = r;
  leds[index].G = g;
  leds[index].B = b;
}

/**
 * Limpa o buffer de pixels.
 */
void npClear()
{
  for (uint i = 0; i < LED_COUNT; ++i)
    npSetLED(i, 0, 0, 0);
}

/**
 * Escreve os dados do buffer nos LEDs.
 */
void npWrite()
{
  // Escreve cada dado de 8-bits dos pixels em sequência no buffer da máquina PIO.
  for (uint i = 0; i < LED_COUNT; ++i)
  {
    pio_sm_put_blocking(np_pio, sm, leds[i].G);
    pio_sm_put_blocking(np_pio, sm, leds[i].R);
    pio_sm_put_blocking(np_pio, sm, leds[i].B);
  }
  sleep_us(100); // Espera 100us, sinal de RESET do datasheet.
}

// Função para converter a posição do matriz para uma posição do vetor.
int getIndex(int x, int y)
{
  // Se a linha for par (0, 2, 4), percorremos da esquerda para a direita.
  // Se a linha for ímpar (1, 3), percorremos da direita para a esquerda.
  if (y % 2 == 0)
  {
    return 24 - (y * 5 + x); // Linha par (esquerda para direita).
  }
  else
  {
    return 24 - (y * 5 + (4 - x)); // Linha ímpar (direita para esquerda).
  }
}

// Função pra desenhar a matriz na matriz de LEDs.
void npDrawMatrix(int matriz[5][5][3]) {
    for (int linha = 0; linha < 5; linha++) {
        for (int coluna = 0; coluna < 5; coluna++) {
            int posicao = getIndex(linha, coluna);
            npSetLED(posicao, matriz[coluna][linha][0], matriz[coluna][linha][1], matriz[coluna][linha][2]);
        }
    }
    npWrite();
}

void button_callback(uint gpio, uint32_t events) {
    if (gpio == BUTTON_A) {
      if (numero_atual == 9){
        numero_atual = 0;
      }
      else numero_atual = numero_atual + 1; // Alterna entre 0 e 9
    }
    if (gpio == BUTTON_B) {
      if (numero_atual == 0){
        numero_atual = 9;
      }
      else numero_atual = numero_atual - 1; // Alterna entre 0 e 9
    }
}

int main()
{
  // Inicializa entradas e saídas.
  stdio_init_all();

  // Inicializa matriz de LEDs NeoPixel.
  npInit(LED_PIN);

  // Iniciando os botões
  gpio_init(BUTTON_A);
  gpio_set_dir(BUTTON_A, GPIO_IN);
  gpio_pull_up(BUTTON_A);

  gpio_init(BUTTON_B);
  gpio_set_dir(BUTTON_B, GPIO_IN);
  gpio_pull_up(BUTTON_B);

  // Configura interrupção na borda de descida (pressionamento)
  gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_FALL, true, &button_callback);

  // Configura interrupção na borda de descida (pressionamento)
  gpio_set_irq_enabled_with_callback(BUTTON_B, GPIO_IRQ_EDGE_FALL, true, &button_callback);

  // Dando um clear pra não ter problemas de lixo na tela
  npClear();

 // Loop Principal de execução
  while (true) {
    npDrawMatrix(matrizes[numero_atual]); // Usa o índice para acessar a matriz correta
    sleep_ms(100); // Pequeno delay para evitar atualização excessiva
  }
}
