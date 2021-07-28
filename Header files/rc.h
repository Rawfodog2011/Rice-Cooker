#ifndef RC_H
#define	RC_H
#include "config.h"
#include "pic18f4520.h"
#include "delay.h"
#include "lcd.h"
#include "teclado.h"

void start();                       //inicializa todos os ports e o lcd
void shift(int,int);                    //desliza a mensagem do lcd
void functionselect();                //menu para escolha da função de cocção
void intro();                         //breve introdução do projeto
unsigned char leituraTeclado();      //lê e retorna uma tecla 
void msg(char[10]);             //mostra as mensagens no lcd
int msgfunction(unsigned char); //funciona em conjunto com a função msg
void contagem(int);
void timer();                    //conta o tempo de cocção nos displays de 7 segmentos

#endif	

