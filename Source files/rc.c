//comandos únicos utilizados pela main.c
//Pedro Vieira Bertoni 2020017279
#include "rc.h"
#include "lcd.h"
#include "pic18f4520.h"
#include "config.h"
#include "delay.h"
#include "bits.h"


static const char valores[]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};
long int unsigned count=0;
unsigned char i,j;
static int cont=0;


void start(){ 
    ADCON1 = 0x06;
    TRISB = 0x01;
    TRISD = 0x00;
    TRISE = 0x00;
    TRISB = 0xF8;
    TRISA=0x00;
    TRISC=0x00;
    BitSet(PORTC,1); 
    lcd_init();
}
void shift(int t,int op){
    for (i = 0; i < 15; i++) {
        atraso_ms(t);
        lcd_cmd(0x18);
    }
    for (i = 0; i < 25; i++) {
        atraso_ms(t);
        lcd_cmd(0x1C); //lcd desliza pra direita
    }
    if(op==1) return;
    for (i = 0; i < 10; i++) {
        atraso_ms(t);
        lcd_cmd(0x18); //lcd desliza pra esquerda
    }
    atraso_ms(1000);
}
void functionselect(){
    lcd_cmd(L_CLR);
    for(i=0;i<5;i++){
        lcd_cmd(L_L1);
        lcd_str("Escolha o tipo de Arroz");
        lcd_cmd(L_L2);
        lcd_str("1-Comum 2-Integral 3-Arboreo");
        shift(200,0);
    }
}
void intro(){ //Rice Cooker Msgs
    lcd_cmd(L_CLR);
    lcd_cmd(L_L1);
    lcd_str("  Peter's");
    lcd_cmd(L_L2);
    lcd_str("Rice Cooker");
    shift(50,0);

    lcd_cmd(L_CLR);
    lcd_cmd(L_L1);
    lcd_str("       _§_§_§_");
    lcd_cmd(L_L2);
    lcd_str("       |______|");
    shift(50,1);
}
unsigned char leituraTeclado(){ //Leitura de tecla pressionada
    unsigned char tmp;
    TRISD = 0x0F; 
    tmp = tc_tecla(0) + 0x30; //lê a tecla e armazena em tmp         
    TRISD = 0x00;
    return tmp;
}
void timer(int inicio, int fim){ //timer em segundos das funções
    PORTD=0x00;
    count=inicio*60; //começa a contar do parâmetro enviado, em segundos
    while(1){ //contagem em segundos e milissegundos nos displays de 7 segmentos
        for(i=2;i<6;i++){
            if(i==2) PORTD=valores[(count/60)/10]; 
            if(i==3) PORTD=valores[(count/60)%10];
            if(i==4) PORTD=valores[(count%60)/10];
            if(i==5) PORTD=valores[(count%60)%10];
            PORTA=0x00;
            BitSet(PORTA,i); //liga um display de cada vez
            atraso_ms(1);
        }
        count--;
        if(count==fim*60){ //termina a contagem no parâmetro enviado, em segundos
            PORTA=0x00;
            break;
        }
    }
}
void contagem(int cont){ //Mostra nos leds do PORTB o número de refeições
    TRISB=0x00;
    PORTB=0x00;
    for(i=1;i<=cont;i++) BitSet(PORTB,i); //liga os leds do PORTB até o parâmetro recebido
    atraso_ms(600);
}
void msg(char tipo[10]){  //Etapas de cocção
    lcd_cmd(L_CLR);
    lcd_cmd(L_L1);
    lcd_str("Modo Selecionado");
    lcd_cmd(L_L2);
    lcd_str("  ");
    lcd_str(tipo);
    atraso_ms(1000);
    lcd_cmd(L_CLR);
    lcd_str("Inicializando");
    timer(5,0);
    lcd_cmd(L_CLR);
    lcd_str("Medindo");
    lcd_cmd(L_L2);
    lcd_str(tipo);
    lcd_str(" ");
    atraso_ms(50);
    timer(10,0);
    lcd_cmd(L_CLR);
    lcd_cmd(L_L1);
    lcd_str("Preenchendo");
    lcd_cmd(L_L2);
    lcd_str("com Agua");
    lcd_str(" ");
    atraso_ms(50);
    timer(10,0);
    lcd_cmd(L_CLR);
    lcd_cmd(L_L1);
    lcd_str("Cozinhando");
    lcd_cmd(L_L2);
    lcd_str(tipo);
    lcd_str("...");
    BitSet(PORTC,2); //liga o cooler
    atraso_ms(50);
    timer(30,0);
    lcd_cmd(L_CLR);
    lcd_cmd(L_L1);
    BitClr(PORTC,2); //desliga o cooler
    lcd_cmd(L_CLR);
}
int msgfunction(unsigned char tmp){ //Designação do tipo de Arroz
    PORTC=0x00;
    BitSet(PORTC,1);
    if(tmp=='1'){
        msg("arroz comum");
    }
    if(tmp=='2'){
        msg("integral");
    }
    if(tmp=='3'){
        msg("arboreo");
    }
    cont++; //acrescenta 1 ao número de refeições
    
    BitClr(PORTC,1); //ativa o buzzer para avisar o fim do processo de cocção
    lcd_str("Finalizado");
    lcd_cmd(L_L2);
    lcd_str("Arroz pronto");
    atraso_ms(50);
    BitSet(PORTC,1); //desativa o buzzer
    lcd_cmd(L_CLR);
    do{ //garante que a tecla certa seja digitada
        lcd_str("Pressione 1 para");
        lcd_cmd(L_L2);
        lcd_str("finalizar");
        i=leituraTeclado();
        if(i!='1'){
            lcd_cmd(L_CLR);
            lcd_str("Tecla invalida");
            atraso_ms(5000);
        }
        lcd_cmd(L_CLR);
    }while(i!='1');
    
    return cont; //retorna o número de refeições
} 



