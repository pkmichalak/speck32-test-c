/* 2020-05-05 */

#include <stdint.h>
#include <stdio.h>

// speck 16, 4
#define WORDSIZE 16
#define TEXTWORDS 2
#define KEYWORDS 4
#define ROUNDS 22
#define ALPHA 7
#define BETA 2

#define ROR(x, r) ((x >> r) | ((x << (WORDSIZE - r)) & 0xFFFF))
#define ROL(x, r) (((x << r) & 0xFFFF) | (x >> (WORDSIZE - r)))
#define ROUND(x, y, k) (x = ROR(x, ALPHA), x += y, x ^= k, y = ROL(y, BETA), y ^= x)
#define RROUND(x, y, k) (y ^= x, y = ROR(y, BETA), x ^= k, x -= y, x = ROL(x, ALPHA))

typedef uint16_t T;
typedef uint16_t TK[KEYWORDS];
typedef uint16_t TT[TEXTWORDS];

void
encrypt(uint16_t const pt[2],
				uint16_t ct[2],
				uint16_t const k[4])
{
	T rk[ROUNDS];
	T l[ROUNDS + KEYWORDS - 1];
	rk[0] = k[0];
	l[0] = k[1];
	l[1] = k[2];
	l[2] = k[3];
	for (int i = 0; i < ROUNDS - 1; ++i)
	{
		T a = l[i];
		T b = rk[i];
		ROUND(a, b, i);
		l[i + KEYWORDS - 1] = a;
		rk[i + 1] = b;
	}

	T x = pt[1];
	T y = pt[0];

	printf("n   x    y    k\n");
	for (int i = 0; i < ROUNDS; ++i)
	{
		printf("%2d: %04x %04x %04x\n", i, x, y, rk[i]);
		ROUND(x, y, rk[i]);
	}
	printf("    %04x %04x\n", x, y);

	ct[1] = x;
	ct[0] = y;
}

void
decrypt(uint16_t pt[2],
				uint16_t const ct[2],
				uint16_t const k[4])
{
	T rk[ROUNDS];
	T l[ROUNDS + KEYWORDS - 1];
	rk[0] = k[0];
	l[0] = k[1];
	l[1] = k[2];
	l[2] = k[3];
	for (int i = 0; i < ROUNDS - 1; ++i)
	{
		T a = l[i];
		T b = rk[i];
		ROUND(a, b, i);
		l[i + KEYWORDS - 1] = a;
		rk[i + 1] = b;
	}

	T x = ct[1];
	T y = ct[0];

	printf("n   x    y    k\n");
	for (int i = ROUNDS - 1; 0 <= i; --i)
	{
		printf("%2d: %04x %04x %04x\n", i, x, y, rk[i]);
		RROUND(x, y, rk[i]);
	}
	printf("    %04x %04x\n", x, y);

	pt[1] = x;
	pt[0] = y;
}

int main(int argc, char **argv)
{
#if 0
	for (int i = 0; i <= 16; ++i)
		printf("%04x\n", ROR(0x1000, i));
		//printf("%04x\n", (0x1000 >> i) | ((0x1000 << (16 - i)) & 0xFFFF));
#endif
#if 0
	for (int i = 0; i <= 16; ++i)
		printf("%04x\n", ROL(0x1000, i));
#endif

	TK k;
	k[3] = 0x1918;
	k[2] = 0x1110;
	k[1] = 0x0908;
	k[0] = 0x0100;

	TT pt;

	pt[1] = 0x6574;
	pt[0] = 0x694c;

	TT ct;
//ct[1] = 0xa868;
//ct[0] = 0x42f2;

	printf("\n");
	printf("key---------------\n");
	printf("k:  0x%04x 0x%04x 0x%04x 0x%04x\n", k[3], k[2], k[1], k[0]);

	printf("\n");
	printf("encrypt-----------\n");
	printf("pt: 0x%04x 0x%04x\n", pt[1], pt[0]);
	encrypt(pt, ct, k);
	printf("ct: 0x%04x 0x%04x\n", ct[1], ct[0]);

	printf("\n");
	printf("decrypt-----------\n");
	printf("ct: 0x%04x 0x%04x\n", ct[1], ct[0]);
	decrypt(pt, ct, k);
	printf("pt: 0x%04x 0x%04x\n", pt[1], pt[0]);
	printf("\n");

	return 0;
}

