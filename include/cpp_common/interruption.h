//
// Created by mohamed on 04.06.20.
//

#ifndef PGROUTING_INTERRUPTION_H
#define PGROUTING_INTERRUPTION_H
/*
 * Suppress the -Wpedantic warning temporarily about the postgres file
 */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
extern "C" {
#include "postgres.h"
#include "miscadmin.h"
}
#pragma GCC diagnostic pop
#endif //PGROUTING_INTERRUPTION_H
