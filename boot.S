/*
 * boot.S
 *
 *  Created on: 16 mars 2026
 *      Author: mi11p002
 */

.title "Programme de démarrage"

.syntax unified     @ Utiliser la syntaxe ARM unifiée

/* Contenu de la section data */
.data
/*
myword:     .word   0x42424244  @ donnée 32 bits
mybytes:    .byte   0x42, 0x42  @ des octets
mybytes2:   .byte   0b01000010  @ des octets en binaire
mystr:      .asciz  "Hello"     @ Une chaîne terminée par des zéros
*/

.section vectors, "ax", %progbits

.long 0x2001FFFC
.long _start
.long _default_handler
.long _default_handler
.long _default_handler
.long _default_handler
.long _default_handler

.thumb_func
_default_handler:
  b .

/* Contenu de la section boot */
.section	boot, "awx", %progbits
.align 4    @ Alignement de la section

.global     _start  @ _start est un symbole global pour l'éditeur de liens
.thumb_func
_start:     @ Une étiquette qui s'appelle _start
			ldr		sp, =0x2001FFFC		/* init stack pointer sur haut de mémoire multiple 4 bits */

			ldr		r0, =bss_start		/* on sait que bss_start + bss_size = bss_end */
			ldr		r1, =bss_end
			mov		r2, #0
boucle:
			str		r2, [r0], #4
			cmp		r0, r1
			blo		boucle				/* tant qu'on a pas écrit toute la section bss à 0, on boucle */
			bl		main				/* appelle main mais stocke l'addr de retour sur la pile avant */
			b 		.					/* boucle infinie après retour de main */
.end
