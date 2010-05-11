#include "structure.h"
#include "vectormath.h"
void diffusetrace(color* final, const vector &cam, vector &ray, int it, int depth, int objind);
bool lighttrace(const vector &point, vector &ray, int itri, int &closesti, int objind);