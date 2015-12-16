/*RSA 原理
-----------------------------------------------------------------------------------------------
  1. 随意选择两个大的质数p和q，p不等于q，计算N=pq
  2. 根据欧拉函数，不大于N且与N互质的整数个数为(p-1)(q-1)
  3. 选择一个整数e与(p-1)(q-1)互质，并且e小于(p-1)(q-1)
  4. 用以下这个公式计算d：d×e ≡ 1(mod (p-1)(q-1))
  5. 将p和q的记录销毁。

   
  (N,e)是公钥，(N,d)是私钥。 C是明文，M是密文，加解密过程：
		
			C^e ≡ M ( mod N )

  解密过程：
		
			M^d ≡ C ( mod N )

*/
#include "math/tommath.h"
#include "windows.h"
#include <iostream>
#include <string>
using namespace std;

typedef struct {
	int bits;			/* bits in key */
	mp_int n;			/* modulus */
	mp_int e;			/* public exponent */
	mp_int d;			/* private exponent */
}rsa_key;

int rsa_rng(unsigned char *dst, int len, void *dat);
int rsa_preme_random( mp_int *a, int bits );
int rsa_gen_key( rsa_key *key, int bits );
int rsa_set_key( rsa_key *key, char *sn, char *se, char *sd, int bits, int radix );
int rsa_enc_public( rsa_key *key, char * src, char * dst );
int rsa_enc_private( rsa_key *key, char * src, char * dst );
string rsa_enc(string mac_s, string time_s);
string rsa_dec(char regInfo[]);

//rsa_dec