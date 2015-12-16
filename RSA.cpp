#include "RSA.h"

int rsa_rng(unsigned char *dst, int len, void *dat)
{
	int x;
	srand( GetTickCount() );
	for (x = 0; x < len; x++)   dst[x] = rand() & 0xFF;
	return len;
}

int rsa_preme_random( mp_int *a, int bits )
{
	int err = mp_prime_random_ex( a, 8, bits, LTM_PRIME_2MSB_ON, rsa_rng, NULL );	//LTM_PRIME_SAFE
	if (err != MP_OKAY) {
		return -1;
	}
	return 0;
}

int rsa_gen_key( rsa_key *key, int bits )
{
	mp_int p, q;
	mp_int sp, sq;
	mp_int n, m;
	mp_int e, d;
	mp_int t;

	//init mp_ints
	mp_init( &p );	mp_init( &q );	mp_init( &sp );	mp_init( &sq );
	mp_init( &n );	mp_init( &m );	mp_init( &e );	mp_init( &d ); 	mp_init( &t );

/*
	//genarate p & q
	rsa_preme_random( &p, bits/2 );
	rsa_preme_random( &q, bits/2 );

	//make n & m
	mp_sub_d( &p, 1, &sp );
	mp_sub_d( &q, 1, &sq );
	mp_mul( &p, &q, &n );
	mp_mul( &sp, &sq, &m );
	
	//make e & d 
	mp_set( &e, 3 );
	
	while(1){
		mp_gcd( &e, &m, &t );
		if( ( mp_cmp_d(&t, 1) ) > 0 ){
			mp_add_d( &e, 2, &e );
			continue;
		}
		else break;
	}
	*/
	

	//make e
	mp_set( &e, 65537 );
	while(1){
		//genarate p & q
		rsa_preme_random( &p, bits/2 );
		rsa_preme_random( &q, bits/2 );
		
		//make m
		mp_sub_d( &p, 1, &sp );
		mp_sub_d( &q, 1, &sq );
		mp_mul( &sp, &sq, &m );

		mp_gcd( &e, &m, &t );
		if( ( mp_cmp_d(&t, 1) ) == 0 ){
			break;
		}
	}
	mp_mul( &p, &q, &n );

	mp_invmod( &e, &m, &d );
	

	//copy n d e to key struct
	mp_init( &key->n );
	mp_init( &key->d );
	mp_init( &key->e );
	key->bits = bits;
	mp_copy( &n, &key->n );
	mp_copy( &d, &key->d );
	mp_copy( &e, &key->e );
	
	mp_clear( &p );	mp_clear( &q );	mp_clear( &sp );mp_clear( &sq );
	mp_clear( &n );	mp_clear( &m );	mp_clear( &e );	mp_clear( &d );	mp_clear( &t );

	return 0;
}

/*set rsa key by string */
int rsa_set_key( rsa_key *key, char *sn, char *se, char *sd, int bits, int radix )
{
	key->bits = bits;
	mp_init( &key->n );
	mp_init( &key->d );
	mp_init( &key->e );
	if( sn )	mp_read_radix( &key->n, sn, radix );
	if( se )	mp_read_radix( &key->e, se, radix );
	if( sd )	mp_read_radix( &key->d, sd, radix );
	
	return 0;
}

/*encrypt/decrypt by PUBLIC key*/
int rsa_enc_public( rsa_key *key, char * src, char * dst )
{
	mp_int mp_src, mp_dst;
	mp_init( &mp_src );	
	mp_init( &mp_dst );

	mp_read_radix( &mp_src, src, 16 );
	mp_exptmod( &mp_src, &key->e, &key->n, &mp_dst );
	mp_toradix( &mp_dst, dst, 16 );

	mp_init( &mp_src );	mp_init( &mp_dst );

	return 0;
}

/*encrypt/decrypt by PRIVATE key*/
int rsa_enc_private( rsa_key *key, char * src, char * dst )
{
	mp_int mp_src, mp_dst;
	mp_init( &mp_src );	
	mp_init( &mp_dst );
	
	mp_read_radix( &mp_src, src, 16 );
	mp_exptmod( &mp_src, &key->d, &key->n, &mp_dst );
	mp_toradix( &mp_dst, dst, 16 );

	mp_init( &mp_src );	mp_init( &mp_dst );
	return 0;
}

string rsa_enc(string mac_s, string time_s)
{
	string sc_s = mac_s + time_s;
	char sc[2048];
	memset( sc, 0, sizeof sc );
	int i;
	for (i = 0; i < sc_s.length(); i++)
		sc[i] = sc_s[i];
	sc[i] = '\0';

	char sm[2048] = "";
	string sm_s;

	rsa_key key;
	char sn[] = "F08D035BB07A47CCFFE0D0CB65B69E9DBDFBC1D22248610ABB9FFF9608F9B7A8C8374A008605C7F25C920620C811655D1DFB878A1FA746E37041241EB75B22F7";
	char se[] = "BBE574A3028D32FF6CC64BDBBF22C9A9F818D25DABD52F1205D57884C3124743";
	char sd[] = "18AA5C3A538525EB44E0B4213A016FDB0ABACA879661B5C6C13AC6684FAB5DEE10D5C5AF720C6BFD6C6B4AF7FCB9C6BF9283BAB84861738C1DA4A92565F6DADB";

	//设置密钥对 公钥(sn,se)， 私钥(sn,sd)
	rsa_set_key( &key, sn, se, sd, 512, 16 );

	//公钥加密
	memset( sm, 0, sizeof sm );
	rsa_enc_public( &key, sc, sm );
	printf( "密文：%s\n", sm );

	//私钥解密
	memset( sc, 0, sizeof sc );
	rsa_enc_private( &key, sm, sc );
	printf( "明文：%s\n", sc );

	sm_s = sm;

	return sm_s;
}

string rsa_dec(char regInfo[])
{
	rsa_key key;
	char sn[] = "F08D035BB07A47CCFFE0D0CB65B69E9DBDFBC1D22248610ABB9FFF9608F9B7A8C8374A008605C7F25C920620C811655D1DFB878A1FA746E37041241EB75B22F7";
	char se[] = "BBE574A3028D32FF6CC64BDBBF22C9A9F818D25DABD52F1205D57884C3124743";
	char sd[] = "18AA5C3A538525EB44E0B4213A016FDB0ABACA879661B5C6C13AC6684FAB5DEE10D5C5AF720C6BFD6C6B4AF7FCB9C6BF9283BAB84861738C1DA4A92565F6DADB";
	
	char sc[2048] = "FDB0A91811C7FD03B0EC";
	char sm[2048] = "";
	
	//设置密钥对 公钥(sn,se)， 私钥(sn,sd)
	rsa_set_key( &key, sn, se, sd, 512, 16 );
	
	//私钥解密
	memset( sc, 0, sizeof sc );
	rsa_enc_private( &key, regInfo, sc );
	//printf( "明文：%s\n", sc );
	string regInfo_s = sc;
	return regInfo_s;
}

//int rsa_test()
//{
//	rsa_key key;
//	//char sn[] = "8910F5430F2A1D79E4E1FFB88A594986958CCBA88812B0E300ECC090B81CB25AFC18BEF90911B0C5007BBA5A576EEA3FFD9995A222B33C92E9BA76B6A423F6702B97667D49D3627B01536D42EAC93188110732C229E54977C041B2B7BB07496CFBED3BC0C7B68A4DC7ADB6146BACF5EB53841E4A9637A17B9E2EC572A623E0F9";
//	//char se[] = "010001";
//	//char sd[] = "206ACE5709E3200D9040934686120FFDB0A91811C7FD03B0ECAB11F0285B41239BB40E96BFBA5B607C78C6DE4F07808799E71528BFA43C93528C333F871AAA520CA04D596604E04FC4314EBF27FE96D1B369E1735984787158FA59D7E725FD9F5493D89B5C792D447188B0AD109FBDAE8811ABE25A40BB1473397EEA7CD279B1";
//	char sn[] = "F08D035BB07A47CCFFE0D0CB65B69E9DBDFBC1D22248610ABB9FFF9608F9B7A8C8374A008605C7F25C920620C811655D1DFB878A1FA746E37041241EB75B22F7";
//	char se[] = "BBE574A3028D32FF6CC64BDBBF22C9A9F818D25DABD52F1205D57884C3124743";
//	char sd[] = "18AA5C3A538525EB44E0B4213A016FDB0ABACA879661B5C6C13AC6684FAB5DEE10D5C5AF720C6BFD6C6B4AF7FCB9C6BF9283BAB84861738C1DA4A92565F6DADB";
//
//	char sc[2048] = "FDB0A91811C7FD03B0EC";
//	char sm[2048] = "";
//
//	//设置密钥对 公钥(sn,se)， 私钥(sn,sd)
//	rsa_set_key( &key, sn, se, sd, 512, 16 );
//
//	//rsa_gen_key( &key, 1024 );
//	mp_toradix( &key.n, sm, 16 );
//	printf( "key.n = %s\n", sm );
//	mp_toradix( &key.e, sm, 16 );
//	printf( "key.e = %s\n", sm );
//	mp_toradix( &key.d, sm, 16 );
//	printf( "key.d = %s\n", sm );
//	
//
//	//公钥加密
//	memset( sm, 0, sizeof sm );
//	rsa_enc_public( &key, sc, sm );
//	printf( "密文：%s\n", sm );
//
//	//私钥解密
//	memset( sc, 0, sizeof sc );
//	rsa_enc_private( &key, sm, sc );
//	printf( "明文：%s\n", sc );
//
//	return 0;
//}


//int main()
//{
//	rsa_test(); 
//	system("pause");
//	return 0;
//}


