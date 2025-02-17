# Quiz Game - Raspberry Pi Pico <img src="https://skillicons.dev/icons?i=raspberrypi,c,git" alt="Raspberry Pi, C & Git Icons" style="vertical-align: middle; height: 35px;"/>

**1. Visão Geral** 🎮
O projeto **Quiz Game** é uma aplicação educacional desenvolvida para o Raspberry Pi Pico que implementa um jogo de perguntas e respostas interativo sobre tecnologia e programação. O sistema utiliza um display OLED, LEDs RGB, botões e um buzzer para criar uma experiência de aprendizado envolvente e interativa.

**2. Funcionalidades** ⚡
Abaixo estão listadas as principais funcionalidades do projeto:
| Funcionalidade | Descrição |
| ---------------------------- | --------------------------------------------------------------------------------------- |
| **Interface OLED** | Display de texto claro e organizado para perguntas e feedback |
| **Feedback Multimodal** | Combinação de LEDs RGB e sons para feedback imediato das respostas |
| **Sistema de Pontuação** | Acompanhamento da pontuação atual e recorde |
| **Controle por Botões** | Interface intuitiva com dois botões para seleção de respostas |
| **Explicações Educativas** | Feedback explicativo após cada resposta para reforçar o aprendizado |
| **10 Questões Tecnológicas** | Conjunto diversificado de perguntas sobre programação e tecnologia |

**3. Hardware Necessário** 🔧
* **Microcontrolador**: Raspberry Pi Pico
* **Display**: OLED SSD1306 (I2C)
* **LEDs**: 3 LEDs (Vermelho, Verde)
* **Botões**: 2 botões de pressão
* **Buzzer**: Buzzer piezoelétrico
* **Componentes Adicionais**:
   * Resistores pull-up para botões
   * Resistores limitadores para LEDs
   * Jumpers/fios para conexões

**4. Pinout** 📌
| Componente | Pino Pico | Descrição |
|--------------|-----------|------------------------------|
| OLED SDA | GPIO 14 | Dados I2C |
| OLED SCL | GPIO 15 | Clock I2C |
| LED Vermelho | GPIO 13 | Indicador de erro |
| LED Verde | GPIO 11 | Indicador de acerto |
| LED Azul | GPIO 12 | Indicador auxiliar |
| Botão A | GPIO 5 | Seleção primeira opção |
| Botão B | GPIO 6 | Seleção segunda opção |
| Buzzer | GPIO 21 | Feedback sonoro |

**5. Configuração e Execução** 🚀
**5.1. Pré-requisitos**
* Raspberry Pi Pico SDK
* CMake ≥ 3.13
* GCC ARM toolchain
* Visual Studio Code (recomendado)
* Git instalado

**5.2. Clonar e Compilar**
```bash
git clone git@github.com:FelipeTiLustosa/quizGame-RaspberryPiPico.git
cd project1
mkdir build
cd build
cmake ..
make
```

**5.3. Carregar no Pico**
1. Pressione e segure o botão BOOTSEL do Pico
2. Conecte o Pico ao computador via USB
3. Solte o botão BOOTSEL
4. Copie o arquivo `build/project1.uf2` para o drive RPI-RP2

**6. Estrutura do Código** 📁
```bash
project1/
├── CMakeLists.txt # Configuração do CMake
├── inc/ # Headers
│   ├── ssd1306.h # Driver do display OLED
│   └── ssd1306_i2c.h # Configurações I2C
├── project1.c # Código principal
└── README.md # Documentação
```

**7. Funcionamento** 🔄
1. **Inicialização**:
   * Configuração do hardware (I2C, GPIOs, PWM)
   * Exibição da tela de boas-vindas
2. **Loop do Jogo**:
   * Apresentação das perguntas
   * Leitura das respostas via botões
   * Feedback visual (LEDs) e sonoro (buzzer)
   * Exibição de explicações
   * Atualização da pontuação
3. **Finalização**:
   * Exibição da pontuação final
   * Comparação com recorde
   * Opção de reinício

**8. Contribuição** 🤝
Para contribuir com o projeto:
1. Faça um fork do repositório
2. Crie uma branch para sua feature
3. Implemente e teste suas mudanças
4. Envie um pull request

**9. Link do vídeo explicativo sobre o projeto**

[Link do vídeo demonstrativo](https://youtu.be/a6SSw6kvukw?si=uR3zhW79hlxwzJBE)

**Autor** ✍️
Felipe Lustosa Carvalho

**Licença** 📄
Este projeto está sob a licença **MIT**.
