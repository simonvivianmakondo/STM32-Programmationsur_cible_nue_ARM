# MI11 — Développement sur cible nue (Bare-Metal STM32H7)

> Travaux pratiques de développement bare-metal sur microcontrôleur **STM32H743** (Cortex-M7),
> réalisés dans le cadre de l'UE MI11 à l'**UTC Compiègne**.  
> Aucune couche d'abstraction matérielle (HAL/BSP) n'est utilisée : chaque registre est adressé directement.

**Auteurs :** Simon · Éric Bjärstål  
**Formation :** Ingénierie des Systèmes Embarqués et Autonomes (INES) — UTC Compiègne  
**Année :** 2025–2026

---

## Contexte et enjeux

La majorité des formations en systèmes embarqués s'appuient sur des bibliothèques d'abstraction (ST HAL, Arduino, Zephyr…) qui masquent la complexité du matériel. Ce TP adopte volontairement l'approche inverse : **travailler directement au niveau des registres**, en s'appuyant uniquement sur le *Reference Manual* du STM32H743 et la spécification ARMv7-M.

Cette contrainte n'est pas artificielle. Dans des contextes industriels critiques — avionique, spatial, automobile — la maîtrise du bare-metal est indispensable pour :

- garantir un comportement déterministe et certifiable,
- minimiser l'empreinte mémoire et la latence,
- comprendre précisément ce que le code fait au niveau silicium.

L'objectif pédagogique est donc double : acquérir une maîtrise technique des périphériques Cortex-M7, et développer la rigueur de lecture de documentation technique (RM0433).

---

## Environnement de développement

| Composant | Détail |
|---|---|
| Cible | STM32H743ZI (Nucleo-H743ZI2) |
| Architecture | ARM Cortex-M7, 480 MHz |
| Compilateur | `arm-none-eabi-gcc` (GCC 12) |
| Build system | `Makefile` custom (pas de CMake, pas de CubeIDE) |
| Linker script | Écrit à la main — définition des sections `.text`, `.data`, `.bss`, stack |
| Débogage physique | GDB + OpenOCD via ST-Link V3 |
| Émulation | **Renode** — simulation du STM32H743 sans cible physique |
| Éditeur | VS Code + extension Cortex-Debug |

---

## Structure du projet

```
mi11-tp2/
├── Makefile
├── linker.ld               # Linker script custom
├── startup.c               # Vecteur de reset, init .data/.bss
├── src/
│   ├── main.c
│   ├── gpio.c              # Driver GPIO — configuration, lecture, écriture
│   ├── rcc.c               # Configuration des horloges système
│   ├── timer.c             # Driver TIM — PWM, comptage
│   └── nvic.c              # Gestion des interruptions (NVIC + EXTI)
├── include/
│   ├── gpio.h
│   ├── rcc.h
│   ├── timer.h
│   └── nvic.h
└── renode/
    └── stm32h743.resc      # Script de simulation Renode
```

---

## Fonctionnalités implémentées

### GPIO — Pilotage direct des broches

Configuration complète des registres `MODER`, `OTYPER`, `OSPEEDR`, `PUPDR`, `ODR`, `IDR` sans passer par la HAL.  
Activation de l'horloge périphérique via `RCC->AHB4ENR` avant tout accès.

```c
// Activation horloge GPIOB
RCC->AHB4ENR |= (1 << 1);

// PB0 en sortie push-pull
GPIOB->MODER  &= ~(3 << (0 * 2));
GPIOB->MODER  |=  (1 << (0 * 2));
GPIOB->OTYPER &= ~(1 << 0);

// Toggle LED
GPIOB->ODR ^= (1 << 0);
```

### Timer / PWM — TIMx en mode PWM

Initialisation du prescaler, de l'ARR (Auto-Reload Register) et du CCR (Capture/Compare Register) pour générer un signal PWM à fréquence et rapport cyclique configurables.

```c
// TIM3 — PWM sur CH1, fréquence 1 kHz, duty 50%
RCC->APB1LENR |= (1 << 1);   // Horloge TIM3
TIM3->PSC  = 239;             // Prescaler : 240 MHz / 240 = 1 MHz
TIM3->ARR  = 999;             // Période : 1 MHz / 1000 = 1 kHz
TIM3->CCR1 = 499;             // Duty cycle 50%
TIM3->CCMR1 |= (6 << 4);     // Mode PWM 1
TIM3->CCER  |= (1 << 0);     // Activation sortie CH1
TIM3->CR1   |= (1 << 0);     // Démarrage
```

### Interruptions — NVIC et EXTI

Câblage complet d'une interruption externe sur front descendant (bouton utilisateur) : configuration EXTI, activation dans le NVIC, écriture du handler en C.

```c
// EXTI13 — bouton PC13, front descendant
SYSCFG->EXTICR[3] |= (2 << 4);   // PC13 → EXTI13
EXTI->FTSR1        |= (1 << 13);  // Déclenchement sur front descendant
EXTI->IMR1         |= (1 << 13);  // Démasquage
NVIC_EnableIRQ(EXTI15_10_IRQn);

void EXTI15_10_IRQHandler(void) {
    if (EXTI->PR1 & (1 << 13)) {
        EXTI->PR1 |= (1 << 13);   // Acquittement
        toggle_led();
    }
}
```

---

## Simulation avec Renode

Renode permet de simuler le STM32H743 et ses périphériques sans accès à la carte physique, ce qui s'est avéré utile pour valider la logique des drivers en amont du test matériel.

```bash
# Lancer la simulation
renode renode/stm32h743.resc

# Dans la console Renode :
(monitor) start
(monitor) sysbus.gpioPortB.led Toggle
```

Le script `.resc` charge le binaire ELF, instancie les périphériques GPIO/TIM et branche les sorties sur des LEDs virtuelles observables dans l'interface graphique.

---

## Compilation et flash

```bash
# Compiler
make all

# Flasher via OpenOCD (ST-Link V3)
make flash

# Débogage GDB
make debug
# Puis dans GDB :
# (gdb) target remote :3333
# (gdb) load
# (gdb) break main
# (gdb) continue
```

---

## Ce que ce TP nous a appris

### Côté technique

- **Lecture de documentation** : naviguer dans un Reference Manual de 3 000 pages pour localiser les registres pertinents, comprendre les dépendances entre périphériques (ex. : activer l'horloge RCC avant tout accès GPIO).
- **Startup code** : comprendre ce qui se passe *avant* `main()` — initialisation de la pile, copie de `.data` depuis la Flash vers la RAM, mise à zéro de `.bss`.
- **Linker script** : définir manuellement les régions mémoire, les sections et les symboles exportés vers le code C.
- **Débogage bas niveau** : utiliser GDB pour inspecter les registres (`info registers`, `x/1xw 0x58020400`), poser des watchpoints sur des adresses mémoire, et distinguer un bug logiciel d'un problème de configuration d'horloge.
- **Émulation vs cible réelle** : identifier les divergences de comportement entre Renode et le silicium (notamment sur le timing des périphériques).

### Côté méthodologie

- Travailler en binôme sur du code bas niveau impose une rigueur de documentation interne : chaque registre modifié est commenté avec sa référence dans le RM.
- La démarche *test sur émulateur → validation sur cible* réduit les cycles de debug et force à séparer la logique applicative des dépendances matérielles.
- Écrire son propre Makefile et linker script, même une seule fois, donne une compréhension durable de la chaîne de compilation croisée.

---

## Références

- [STM32H743 Reference Manual (RM0433)](https://www.st.com/resource/en/reference_manual/rm0433-stm32h742-stm32h743753-and-stm32h750-value-line-advanced-armbased-32bit-mcus-stmicroelectronics.pdf)
- [ARMv7-M Architecture Reference Manual](https://developer.arm.com/documentation/ddi0403/latest/)
- [Renode documentation](https://renode.readthedocs.io/)
- [GNU ARM Embedded Toolchain](https://developer.arm.com/Tools%20and%20Software/GNU%20Toolchain)
