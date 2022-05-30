#ifndef RANDOM_H
#define RANDOM_H

#define randnum(min, max) \
        ((rand() % (int)(((max) + 1) - (min))) + (min))

#endif /* RANDOM_H */