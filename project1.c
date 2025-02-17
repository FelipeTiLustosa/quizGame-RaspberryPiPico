#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/i2c.h"
#include "hardware/clocks.h"
#include "inc/ssd1306.h"

// Definições dos pinos
#define BUZZER 21
#define LED_VERMELHO 13
#define LED_VERDE 11
#define LED_AZUL 12   
#define BOTAO_A 5     
#define BOTAO_B 6     

#define TOTAL_PERGUNTAS 10

// Estrutura para armazenar as perguntas
typedef struct {
    char pergunta[200];
    char opcaoA[50];
    char opcaoB[50];
    int resposta_correta; // 0 para A, 1 para B
    char explicacao[100];
} Pergunta;

// Variáveis globais para o display OLED
struct render_area frame_area;
uint8_t ssd[1024];

const uint I2C_SDA = 14;
const uint I2C_SCL = 15;
int pontuacao = 0;
int melhor_pontuacao = 0;

// Array de perguntas com explicações
Pergunta perguntas[TOTAL_PERGUNTAS] = {
    {
        "O que e CPU?\n\nA)Processador\nB)Placa de video",
        "A", "B", 0,
        "CPU processa\ntodas operacoes"
    },
    {
        "Python e:\n\nA)Compilada\nB)Interpretada",
        "A", "B", 1,
        "Python executa\nlinha a linha"
    },
    {
        "RAM e:\n\nA)Temporaria\nB)Permanente",
        "A", "B", 0,
        "RAM perde dados\nao desligar PC"
    },
    {
        "AND em C e:\n\nA) ||\nB) &&",
        "A", "B", 1,
        "&& e o AND\nlogico em C"
    },
    {
        "Bug e:\n\nA)Novo recurso\nB)Erro codigo",
        "A", "B", 1,
        "Bug e um erro\nno programa"
    },
    {
        "HTML e:\n\nA)Programacao\nB)Marcacao",
        "A", "B", 1,
        "HTML estrutura\npaginas web"
    },
    {
        "Git e:\n\nA)Versao codigo\nB)Linguagem",
        "A", "B", 0,
        "Git controla\nversoes codigo"
    },
    {
        "Guarda texto:\n\nA)int\nB)string",
        "A", "B", 1,
        "string/char[]\nguarda textos"
    },
    {
        "1 byte tem:\n\nA)8 bits\nB)4 bits",
        "A", "B", 0,
        "1 byte sempre\ntem 8 bits"
    },
    {
        "SQL e:\n\nA)Prog geral\nB)Banco dados",
        "A", "B", 1,
        "SQL e para\nbancos dados"
    }
};

// Inicializa PWM para o buzzer
void pwm_init_buzzer(uint pino) {
    gpio_set_function(pino, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(pino);
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, 4.0f);
    pwm_init(slice_num, &config, true);
    pwm_set_gpio_level(pino, 0);
}

// Toca uma nota musical com frequencia e duracao especificas
void tocar_nota(int frequencia, int duracao) {
    if (frequencia == 0) {
        pwm_set_gpio_level(BUZZER, 0);
        return;
    }
    uint slice_num = pwm_gpio_to_slice_num(BUZZER);
    uint32_t clock_freq = clock_get_hz(clk_sys);
    float div = 4.0f;
    uint32_t top = (clock_freq / (div * frequencia)) - 1;
    pwm_set_wrap(slice_num, top);
    pwm_set_clkdiv(slice_num, div);
    pwm_set_gpio_level(BUZZER, top / 2);
    sleep_ms(duracao);
    pwm_set_gpio_level(BUZZER, 0);
}

// Toca melodia para resposta correta
void tocar_melodia_acerto() {
    tocar_nota(523, 150); 
    sleep_ms(50);
    tocar_nota(659, 150); 
}

// Toca melodia para resposta errada
void tocar_melodia_erro() {
    tocar_nota(300, 150);
    sleep_ms(50);
    tocar_nota(300, 150);
}

// Exibe texto no display OLED com suporte a quebra de linha
void exibir_mensagem(const char *mensagem) {
    memset(ssd, 0, ssd1306_buffer_length);
    int line_height = 8;
    int y = 0;
    int start = 0;
    int i;
    char linha[128];

    for (i = 0; mensagem[i] != '\0'; i++) {
        if (mensagem[i] == '\n') {
            int len = i - start;
            if (len >= sizeof(linha))
                len = sizeof(linha) - 1;
            strncpy(linha, mensagem + start, len);
            linha[len] = '\0';
            ssd1306_draw_string(ssd, 0, y, linha);
            y += line_height;
            start = i + 1;
        }
    }
    if (i > start) {
        int len = i - start;
        if (len >= sizeof(linha))
            len = sizeof(linha) - 1;
        strncpy(linha, mensagem + start, len);
        linha[len] = '\0';
        ssd1306_draw_string(ssd, 0, y, linha);
    }
    render_on_display(ssd, &frame_area);
}

// Define cor do LED RGB
void definir_cor_led(uint8_t r, uint8_t g, uint8_t b) {
    gpio_put(LED_VERMELHO, r);
    gpio_put(LED_VERDE, g);
    gpio_put(LED_AZUL, b);
}

// Le resposta dos botoes (retorna 0 para A, 1 para B)
int ler_resposta() {
    while (1) {
        if (!gpio_get(BOTAO_A)) {
            sleep_ms(50);
            return 0;
        }
        if (!gpio_get(BOTAO_B)) {
            sleep_ms(50);
            return 1;
        }
        sleep_ms(10);
    }
}

// Mostra feedback visual e sonoro para resposta
void mostrar_feedback(int pergunta_atual, int resposta_correta) {
    if (resposta_correta) {
        definir_cor_led(0, 1, 0);
        tocar_melodia_acerto();
        exibir_mensagem("Correto!\n\n");
        sleep_ms(500);
        exibir_mensagem(perguntas[pergunta_atual].explicacao);
        sleep_ms(2000);
        definir_cor_led(0, 0, 0);
        pontuacao++;
    } else {
        definir_cor_led(1, 0, 0);
        tocar_melodia_erro();
        exibir_mensagem("Incorreto!\n\n");
        sleep_ms(500);
        exibir_mensagem(perguntas[pergunta_atual].explicacao);
        sleep_ms(2000);
        definir_cor_led(0, 0, 0);
    }
}

// Exibe pontuacao final e melhor pontuacao
void mostrar_pontuacao_final() {
    char mensagem[100];
    if (pontuacao > melhor_pontuacao) {
        melhor_pontuacao = pontuacao;
        sprintf(mensagem, "Novo recorde!\n\nPontos: %d\nMelhor: %d", pontuacao, melhor_pontuacao);
    } else {
        sprintf(mensagem, "Fim de jogo!\n\nPontos: %d\nMelhor: %d", pontuacao, melhor_pontuacao);
    }
    exibir_mensagem(mensagem);
    sleep_ms(3000);
}

// Executa uma rodada completa do quiz
void jogar_quiz() {
    pontuacao = 0;
    
    for (int i = 0; i < TOTAL_PERGUNTAS; i++) {
        exibir_mensagem(perguntas[i].pergunta);
        
        int resposta = ler_resposta();
        mostrar_feedback(i, resposta == perguntas[i].resposta_correta);
        sleep_ms(500);
    }
    
    mostrar_pontuacao_final();
}

// Funcao principal: inicializa hardware e loop do jogo
int main() {
    stdio_init_all();
    
    i2c_init(i2c1, 400000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    
    sleep_ms(100);
    
    ssd1306_init();
    
    frame_area.start_column = 0;
    frame_area.end_column = ssd1306_width - 1;
    frame_area.start_page = 0;
    frame_area.end_page = ssd1306_n_pages - 1;
    calculate_render_area_buffer_length(&frame_area);
    
    memset(ssd, 0, ssd1306_buffer_length);
    render_on_display(ssd, &frame_area);
    
    gpio_init(BOTAO_A);
    gpio_init(BOTAO_B);
    gpio_set_dir(BOTAO_A, GPIO_IN);
    gpio_set_dir(BOTAO_B, GPIO_IN);
    gpio_pull_up(BOTAO_A);
    gpio_pull_up(BOTAO_B);
    
    gpio_init(LED_VERMELHO);
    gpio_init(LED_VERDE);
    gpio_init(LED_AZUL);
    gpio_set_dir(LED_VERMELHO, GPIO_OUT);
    gpio_set_dir(LED_VERDE, GPIO_OUT);
    gpio_set_dir(LED_AZUL, GPIO_OUT);
    
    pwm_init_buzzer(BUZZER);
    
    while (true) {
        exibir_mensagem("Quiz de\nTecnologia!\n\nPressione A\npara iniciar");
        
        while (gpio_get(BOTAO_A)) {
            sleep_ms(10);
        }
        sleep_ms(200);
        
        exibir_mensagem("Iniciando...\n\nA) Opcao 1\nB) Opcao 2");
        sleep_ms(2000);
        
        jogar_quiz();
    }
    
    return 0;
}
