#pragma once

#ifndef MAIN_H
#define MAIN_H

// #define DEBUG
// #define SERIAL_DEBUG
// #define WHITE_BOT
// #define BLACK_BOT

#include <common.h>

#define S0 D2
#define S1 D3
#define S2 D4
#define S3 D5

#define MUX1 D0
#define MUX2 D1
#define LIGHTGATE D9

#define KICKER D10

// global variables

const int ball_threshold = 3500;
extern Layer1TxDataUnion tx_data;
extern Layer1RxDataUnion rx_data;

#endif