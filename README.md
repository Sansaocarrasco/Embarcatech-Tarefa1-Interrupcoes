# Tarefa 1 - Interrupções no RP2040

## 📌 Introdução
Esta tarefa tem como objetivo consolidar a compreensão dos conceitos relacionados ao uso de **interrupções** no microcontrolador **RP2040**, utilizando a placa de desenvolvimento **BitDogLab**. O desafio envolve o controle de **LEDs comuns e endereçáveis**, além da manipulação de **botões com tratamento de bouncing**.

## 🎯 Objetivos
- Compreender o funcionamento e a aplicação de **interrupções** em microcontroladores.
- Implementar **debouncing via software** para evitar efeitos indesejados nos botões.
- Manipular e controlar **LEDs comuns** e **LEDs WS2812 endereçáveis**.
- Aplicar **resistores de pull-up internos** nos botões.
- Desenvolver um projeto funcional combinando **hardware e software**.

## 🛠 Componentes Utilizados
A tarefa requer os seguintes componentes conectados à placa **BitDogLab**:

| Componente                  | Conexão à GPIO |
|-----------------------------|---------------|
| Matriz 5x5 de LEDs WS2812   | GPIO 7        |
| LED RGB                     | GPIOs 11, 12, 13 |
| Botão A                     | GPIO 5        |
| Botão B                     | GPIO 6        |

## 🔧 Funcionalidades do Projeto

1. **Piscar LED RGB:** O LED vermelho do LED RGB deve piscar **5 vezes por segundo**.
2. **Incremento de número:** O botão **A** deve **incrementar** o número exibido na matriz de LEDs a cada pressionamento.
3. **Decremento de número:** O botão **B** deve **decrementar** o número exibido na matriz de LEDs a cada pressionamento.
4. **Exibição de números:** Os LEDs WS2812 devem exibir números de **0 a 9** em um formato fixo (estilo digital) ou criativo, desde que sejam identificáveis.

## 📌 Requisitos do Projeto

- **Uso de interrupções:** Os botões devem ser gerenciados exclusivamente por **rotinas de interrupção (IRQ)**.
- **Implementação de debouncing:** O código deve tratar **bouncing** dos botões via **software**.
- **Controle de LEDs:** O projeto deve demonstrar o uso de **LEDs comuns** e **LEDs WS2812**.
- **Código bem estruturado:** O código deve ser **organizado e comentado** para facilitar a compreensão.

## 🎥 Demonstração em Vídeo

https://youtu.be/5is3cqVzgNk

## 📜 Licença
Este projeto está licenciado sob a **Licença MIT**. Consulte o arquivo [LICENSE](LICENSE) para mais informações.
