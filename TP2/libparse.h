#ifndef _LIBPARSE_H
#define _LIBPARSE_H 1

#define PARSECMD_FAIL   (-1)            // Returned by parseCmdLine
#define PARAMS_VALID    (1)             // Returned by callback
#define PARAMS_INVALID  (!PARAMS_VALID) // Returned by callback

typedef int (*pCallback) (char *, char*, void *);

int parseCmdLine (int argc, char *argv[], pCallback p, void *userData);

#endif /* _LIBPARSE_H */
