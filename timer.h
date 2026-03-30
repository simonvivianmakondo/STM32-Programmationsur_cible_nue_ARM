/*
 * timer.h
 *
 *  Created on: 30 mars 2026
 *      Author: mi11p002
 */

/**
 * Active le timer n
 * n represente le numero du timer
 * que l'on souhaite activer
 * ref. p496, 141
 */
void timer_init(int n);

/**
 * Configure le timer n
 * Fait en sorte que l'évènement update se produise à intervalle d
 * ref.
 * @param n numéro du timer
 * @param d intervalle en MICROSECONDE
 */
void timer_config(int n, int d);

/**
 * Attend
 */
void timer_wait(int n);
