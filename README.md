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

Cette contrainte n'est pas artificielle. Dans des contextes industriels critiques — avionique, spatial, automobile — la maîtrise du bare-metal est indispensable pour garantir un comportement déterministe et certifiable, minimiser l'empreinte mémoire et la latence, et comprendre précisément ce que le code fait au niveau silicium.

L'objectif pédagogique est donc double : acquérir une maîtrise technique des périphériques Cortex-M7, et développer la rigueur de lecture de documentation technique.

---

## Environnement de développement

| Composant | Détail |
|---|---|
| Cible | STM32H743ZI (Nucleo-H743ZI2) |
| Architecture | ARM Cortex-M7, 480 MHz |
| Compilateur | `arm-none-eabi-gcc` |
| Build system | `Makefile` custom |
| Débogage | GDB + OpenOCD via ST-Link V3 |
| Émulation | Renode — simulation sans cible physique |

---

## Fonctionnalités implémentées

**GPIO** — Configuration et pilotage direct des broches en entrée/sortie, sans passer par la HAL. Chaque registre de contrôle (mode, type, vitesse, pull) est configuré manuellement après activation de l'horloge périphérique.

**Timer / PWM** — Génération d'un signal PWM à fréquence et rapport cyclique configurables, par initialisation directe du prescaler et des registres de comparaison du timer.

**Interruptions (NVIC / EXTI)** — Câblage complet d'une interruption externe sur front de signal : configuration de la ligne EXTI, activation dans le NVIC et écriture du handler en C avec acquittement de l'IRQ.

---

## Simulation avec Renode

Renode permet de simuler le STM32H743 et ses périphériques sans accès à la carte physique. Cette étape a servi à valider la logique des drivers en amont du test sur matériel réel, en instanciant les périphériques GPIO et Timer dans un environnement virtuel observable.

La démarche *émulation → validation sur cible* a permis de réduire les cycles de debug et de mieux distinguer les erreurs logiques des problèmes de configuration matérielle.

---

## Ce que ce TP nous a appris

**Sur le plan technique**, ce TP a exigé de naviguer dans un Reference Manual de plusieurs milliers de pages pour localiser les registres pertinents et comprendre les dépendances entre périphériques. Écrire le startup code et le linker script à la main a rendu concret ce qui se passe avant `main()` — initialisation de la pile, copie des données initialisées, mise à zéro du BSS. Le débogage avec GDB directement sur les registres physiques a aussi affiné notre capacité à distinguer un bug logiciel d'un problème de configuration d'horloge.

**Sur le plan méthodologique**, travailler en binôme sur du code bas niveau a imposé une rigueur de documentation : chaque registre modifié est commenté avec sa référence dans le RM. Ce TP installe durablement une compréhension de la chaîne de compilation croisée et de ce qu'un compilateur, un linker et un flasheur font réellement.

---

## Références

- [STM32H743 Reference Manual (RM0433)](https://www.st.com/resource/en/reference_manual/rm0433-stm32h742-stm32h743753-and-stm32h750-value-line-advanced-armbased-32bit-mcus-stmicroelectronics.pdf)
- [ARMv7-M Architecture Reference Manual](https://developer.arm.com/documentation/ddi0403/latest/)
- [Renode documentation](https://renode.readthedocs.io/)
- [GNU ARM Embedded Toolchain](https://developer.arm.com/Tools%20and%20Software/GNU%20Toolchain)
