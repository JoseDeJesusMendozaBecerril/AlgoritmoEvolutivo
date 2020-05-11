/*
 * rand.h
 *
 *  Created on: 24/01/2011
 *      Author: Saúl Zapotecas
 */
#ifndef RAND_H_
#define RAND_H_

void randomize();

void warmup_random(double seed);

void advance_random();

double randomperc();

int rndint(int low, int high);

double rndreal(double low, double high);

double box_muller(double m, double s);

double Gauss(double sigma);

double N(double m, double sigma);

void shuffle_int_vector(int *a, int n);

void myshuffle(int *array, int n);

#endif /* RAND_H_ */
