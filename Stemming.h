#ifndef _WORD_STEMMING_
#define _WORD_STEMMING_
#include <string>
#include <stdlib.h>  /* for malloc, free */
#include <string.h>  /* for memcmp, memmove */

using namespace std;

#define TRUE 1
#define FALSE 0
#define INC 50           /* size units in which s is increased */

#define LETTER(ch) (isupper(ch) || islower(ch))
extern int i_max;

struct stemmer {
   char * b;       /* buffer for word to be stemmed */
   int k;          /* offset to the end of the string */
   int j;          /* a general offset into the string */
};

struct stemmer * create_stemmer(void);extern void free_stemmer(struct stemmer * z);
static int cons(struct stemmer * z, int i);
static int m(struct stemmer * z);
static int vowelinstem(struct stemmer * z);
static int doublec(struct stemmer * z, int j);
static int cvc(struct stemmer * z, int i);
static int ends(struct stemmer * z, char * s);
static void setto(struct stemmer * z, char * s);
static void r(struct stemmer * z, char * s);
static void step1ab(struct stemmer * z);
static void step1c(struct stemmer * z);
static void step3(struct stemmer * z);
static void step4(struct stemmer * z);
static void step5(struct stemmer * z);
int stem(struct stemmer * z, char * b, int k);
string stemfile(struct stemmer * z, char * s, string term);

#endif
