#include "delay.h"

void atraso_ms(unsigned long int t) {
    volatile unsigned char j, k;
    volatile unsigned int i;
    
    for (i = 0; i < (t); i++) {        
        for (j = 0; j < 41; j++) {
            for (k = 0; k < 3; k++);
        }
    }
}
