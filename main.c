#include "gpio.h"
#include "timer.h"

#include <stdint.h>

// ---------- Fonction main ----------
int main(void) {
	gpio_init('I'); 			// INITIALIZE PORT I

	gpio_mode('I',12,0); 		// SET THE BIT 0 OF PORT I TO INPUT MODE
	gpio_set('I',12,0); 		// SET THE VALUE OF THE BIT 0 OF PORT I TO 0

	timer_init(2); 				// INITIALIZE TIM2
	timer_config(2, 1000000);

	for(;;) {
//		for(int i=0;i<10000000;i++);
//		gpio_toggle('I', 12);
		timer_wait(2);
		gpio_toggle('I', 12);
	}

    return 0;
}
// ---------- Implémentation ----------

