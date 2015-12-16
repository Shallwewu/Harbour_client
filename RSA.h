/*RSA ԭ��
-----------------------------------------------------------------------------------------------
  1. ����ѡ�������������p��q��p������q������N=pq
  2. ����ŷ��������������N����N���ʵ���������Ϊ(p-1)(q-1)
  3. ѡ��һ������e��(p-1)(q-1)���ʣ�����eС��(p-1)(q-1)
  4. �����������ʽ����d��d��e �� 1(mod (p-1)(q-1))
  5. ��p��q�ļ�¼���١�

   
  (N,e)�ǹ�Կ��(N,d)��˽Կ�� C�����ģ�M�����ģ��ӽ��ܹ��̣�
		
			C^e �� M ( mod N )

  ���ܹ��̣�
		
			M^d �� C ( mod N )

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