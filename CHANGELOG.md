# Changelog — MI11 Bare-Metal STM32H7

Tous les changements notables du projet sont documentés ici.

---

## [0.2.0] — 2026-03-30

### Ajouté
- **`timer.c` / `timer.h`** — Nouveau driver Timer bare-metal
  - `timer_init(n)` : activation de l'horloge TIM2–TIM7 et TIM12–TIM14 via `RCC_APB1LENR`, calcul dynamique du bit selon le numéro de timer
  - `timer_config(n, d)` : configuration de TIM2 avec prescaler à 63 (64 MHz → 1 MHz), ARR à 1 000 000 (période d'1 seconde)
  - `timer_wait(n)` : attente bloquante sur le flag UIF (`TIM2_SR`), puis acquittement

- **`main.c`** — Refonte du programme principal
  - Remplacement de la boucle de délai logiciel (`for` vide) par `timer_wait(2)` pour un timing précis basé hardware
  - Intégration de `timer_init(2)` et `timer_config(2, 1000000)` au démarrage
  - Toggle de la LED sur PI12 à intervalle régulier d'1 seconde

- **`gpio.c`** — Ajout de `gpio_toggle()`
  - Inversion d'une broche de sortie par opération XOR sur `ODR`, sans lecture-modification-écriture explicite

### Modifié
- **`gpio.h`** — Ajout de la déclaration de `gpio_toggle(char port, int bit)`

---

## [0.1.0] — 2026-03-27

### Ajouté
- **`test1.c`** — Premier programme de test monofichier
  - Implémentation inline de toutes les fonctions GPIO (`gpio_init`, `gpio_mode`, `gpio_set`, `gpio_get`, `gpio_toggle`)
  - Clignotement de PI0 pendant 10 cycles avec délai logiciel (`for` vide sur 10 000 000 itérations)

- **`gpio.c` / `gpio.h`** — Extraction des fonctions GPIO dans un driver dédié
  - `gpio_init(port)` : activation horloge via `RCC_AHB4ENR` (bit 8 pour GPIOI, bit 9 pour GPIOJ)
  - `gpio_mode(port, bit, dir)` : configuration `MODER` en entrée (`00`) ou sortie (`01`) par manipulation bit à bit
  - `gpio_set(port, n, value)` : écriture sur `ODR` avec masque
  - `gpio_get(port, bit)` : lecture sur `IDR` avec masque

- **`boot.S`** — Code de démarrage assembleur (vecteur de reset, init pile)
- **`stm32h7.x`** — Linker script custom (sections `.text`, `.data`, `.bss`, stack)
- **`openocd.cfg`** — Configuration OpenOCD pour ST-Link DAP sur STM32H747
- **`stm32h7x.cfg`** — Configuration cible OpenOCD STM32H7x

---

## Notes

- `timer_config()` ignore pour l'instant le paramètre `n` (TIM2 câblé en dur) — généralisation à prévoir
- `test2.c` vide — fichier réservé pour les prochains tests
