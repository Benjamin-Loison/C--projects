#include <iostream>
#define MSG_MAX_LEN 22
#include <gmpxx.h>
#include <random>
using namespace std;

static const char chars[] = "123456789";

string genNb(unsigned long size)
{
	string generated = "";
	for(unsigned long index = 0; index < size; index++)
		generated += chars[rand() % (sizeof(chars) - 1)];
	cout << generated << endl;
	return generated;
}

int main(int argc, char *argv[])
{
	//random_device rd;
	//mt19937 rng(rd());
	unsigned long min = 400;
	//uniform_int_distribution<unsigned long> uni(min, 2 * min);
	//unsigned long preP = uni(rng), preQ = uni(rng), preC = uni(rng); // take different rnge for future
	//TODO: function generate string number by adding random 0 to 9 digit to the string
	mpz_class preP(genNb(min)), preQ(genNb(min)), preC(genNb(min)); // TODO: random size
	cout << "preP: " << preP << endl << "preQ: " << preQ << endl;

	mpz_class P, Q, f = preP, g = preQ;
	mpz_nextprime(P.get_mpz_t(), f.get_mpz_t());
	mpz_nextprime(Q.get_mpz_t(), g.get_mpz_t());
	cout << "P: " << P << endl << "Q: " << Q << endl;

	mpz_class N = P * Q, d, M = (P - 1) * (Q - 1), C = preC, h; // public : N, C
	mpz_gcd(h.get_mpz_t(), M.get_mpz_t(), C.get_mpz_t());
	while(h != 1)
	{
		C++;
		mpz_gcd(h.get_mpz_t(), M.get_mpz_t(), C.get_mpz_t());
	}
	cout << "N: " << N << endl << "M: " << M << endl << "C: " << C << endl;
	string message = "012345 111.111.111.111";
	message = "Bonjour !";
	mpz_class ascii[MSG_MAX_LEN], toSend[MSG_MAX_LEN];
	for(unsigned short messageIndex = 0; messageIndex < message.length(); messageIndex++)
	{
		ascii[messageIndex] = (unsigned short)message[messageIndex];
		mpz_powm(d.get_mpz_t(), ascii[messageIndex].get_mpz_t(), C.get_mpz_t(), N.get_mpz_t());
        	cout << "Encryption: " << d << endl;
		toSend[messageIndex] = d;
	}
	
	mpz_class a = C, b = M, U = 1, v = 0, x = 0, y = 1, c, i, q, r, tmp_b = b;
	while (b > 0)
	{
		r = a % b;
		q = (a - r) / b;
		c = U;
		i = v;
		U = x;
		v = y;
		x = c - q * x;
		y = i - q * y;
		a = b;
		b = r;
	}
	if(U < 2)
	{
		mpz_class k = 2 - U;
		tmp_b *= -1;
		mpz_fdiv_q(k.get_mpz_t(), k.get_mpz_t(), tmp_b.get_mpz_t());
		tmp_b *= -1;
		U -= k * tmp_b;
	}
	if(U > tmp_b)
	{
		mpz_class k = tmp_b - U;
		tmp_b *= -1;
		mpz_fdiv_q(k.get_mpz_t(), k.get_mpz_t(), tmp_b.get_mpz_t());
		tmp_b *= -1;
		U -= k * tmp_b;
	}
	cout << "U: " << U << endl;
	
	for(unsigned short messageIndex = 0; messageIndex < message.length();  messageIndex++)
	{
		mpz_powm(d.get_mpz_t(), toSend[messageIndex].get_mpz_t(), U.get_mpz_t(), N.get_mpz_t());
		cout << "Decryption: " << d.get_ui() << endl;
		cout << "Decryption: " << (char)d.get_ui() << endl;
		cout << "Decryption: " << (char)(unsigned short)d.get_ui() << endl;
	}

	return 0;
}
