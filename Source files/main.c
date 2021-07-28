#include"rc.h"
#include "pic18f4520.h"
#include "config.h"
#include "delay.h"
#include "lcd.h"

void main(){
    unsigned char tmp;
    int cont,i;
    start();
    intro();
    while(1){  
        BitSet(PORTC,1); 
        lcd_cmd(L_CLR);
        if(cont>0){ 
            do{ 
                lcd_str("1-Cozinhar novamente");
                lcd_cmd(L_L2);
                lcd_str("2-Desligar a Panela");
                shift(200,0);
                tmp=leituraTeclado();
                if(tmp!='1' && tmp!='2'){
                    lcd_cmd(L_CLR);
                    lcd_str("Tecla invalida");
                    atraso_ms(5000);
                    lcd_cmd(L_CLR);
                }
                if(tmp=='2') goto sair;   
            }while(tmp!='1' && tmp!='2');
        }
        do{ 
            functionselect();
            tmp=leituraTeclado();
            if(tmp!='1' && tmp!='2' && tmp!='3'){
                lcd_cmd(L_CLR);
                lcd_str("Tecla invalida");
                atraso_ms(5000);
                lcd_cmd(L_CLR);
            }
        }while(tmp!='1' && tmp!='2' && tmp!='3');
        if(cont==0) { 
            TRISB=0x00;
            PORTB=0x00;
        }
        cont=msgfunction(tmp);
        contagem(cont);
        if(cont>=4){ //Após 4 refeições ininterruptas, desliga a panela a fim de evitar o sobreaquecimento da panela 
            break;
        }
        BitSet(PORTC,1); 
    }
    sair:
    lcd_cmd(L_CLR);
    lcd_cmd(L_L1);
    lcd_str("Boa refeicao");
    shift(120,0);
    atraso_ms(1000);
    lcd_cmd(L_CLR);
    while(1);
}