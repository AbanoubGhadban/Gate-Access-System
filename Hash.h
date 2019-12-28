#ifndef HASH_H
#define HASH_H

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define F(X,Y,Z) (((X)&(Y))|((~(X))&(Z)))
#define G(X,Y,Z) (((X)&(Y))|((X)&(Z))|((Y)&(Z)))
#define H(X,Y,Z) ((X)^(Y)^(Z))

#define LEFTROTATE(A,N) ((A)<<(N))|((A)>>(32-(N)))

#define MD4ROUND1(a,b,c,d,x,s) a += F(b,c,d) + x; a = LEFTROTATE(a, s);
#define MD4ROUND2(a,b,c,d,x,s) a += G(b,c,d) + x + (uint32_t)0x5A827999; a = LEFTROTATE(a, s);
#define MD4ROUND3(a,b,c,d,x,s) a += H(b,c,d) + x + (uint32_t)0x6ED9EBA1; a = LEFTROTATE(a, s);

char *MD4(char *str, int len); //this is the prototype you want to call. Everything else is internal.

typedef struct string{
        char *c;
        int len;
        char sign;
}string;

static uint32_t *MD4Digest(uint32_t *w, int len);
static void setMD4Registers(uint32_t AA, uint32_t BB, uint32_t CC, uint32_t DD);
static uint32_t changeEndianness(uint32_t x);
static void resetMD4Registers(void);
static string stringCat(string first, string second);
static string uint32ToString(uint32_t l);
static uint32_t stringToUint32(string s);

static const char *BASE16 = "0123456789abcdef=";

static uint32_t A = 0x67452301;
static uint32_t B = 0xefcdab89;
static uint32_t C = 0x98badcfe;
static uint32_t D = 0x10325476;

string newString(char * c, int t); 
string stringCat(string first, string second);
string base16Encode(string in) ; 
string uint32ToString(uint32_t l);
uint32_t stringToUint32(string s); 
uint32_t *MD4Digest(uint32_t *w, int len);
uint32_t changeEndianness(uint32_t x) ;
void setMD4Registers(uint32_t AA, uint32_t BB, uint32_t CC, uint32_t DD);
void resetMD4Registers(void);


#endif
