#ifndef FR_H
#define FR_H
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "frame3dd.h"
#include "frame3dd_io.h"
#include "eig.h"
#include "HPGmatrix.h"
#include "HPGutil.h"
#include "NRutil.h"
double length(nodeData n1,nodeData n2);
int calculateMatrixMethod(int noOfNodes,nodeData *nodes,int noOfElements,elementData *elements);
#endif