#include "FHE.h"
#include "EncryptedArray.h"
#include <NTL/lzz_pXFactoring.h>
#include <fstream>
#include <sstream>
#include <sys/time.h>

#include <iostream>
#include <stack>
#include <string>
#include <stdlib.h>

using namespace std;

#define DUMP (cout << "test" << endl)

//stack<Ctxt> theStack;
FHEcontext* context;
FHESecKey* secretKey;
FHEPubKey* publicKey;
ZZX G;

void setupHELib()
{
    long p=101;
    long r=1;
    long L=8;
    long c=2;
    long k=80;
    long s=0;
    long d=0;
    long w=64;
    long m = FindM(k, L, c, p, d, s, 0);
    
    context = new FHEcontext(m,p,r);
    buildModChain(*context, L, c);
    G = context->alMod.getFactorsOverZZ()[0];

    secretKey = new FHESecKey(*context);
    publicKey = secretKey;
/*
    srand48(time(0));
    ZZ zzSeed;
    zzSeed = conv(time(0));
    SetSeed(zzSeed);
*/
    secretKey->GenSecKey(w); 
    addSome1DMatrices(*secretKey); // compute key-switching matrices that we need
}


int main(int argc, char **argv)
{
    setupHELib();

    EncryptedArray ea(*context, G);     
    PlaintextArray p(ea);     
    Ctxt& c1 = *(new Ctxt(*publicKey));    
    p.encode(1);    
    ea.encrypt(c1, *publicKey, p); 
    Ctxt& c2 = *(new Ctxt(*publicKey));
    p.encode(2);
    ea.encrypt(c2, *publicKey, p); 
    
    Ctxt t = c1;    
    t += c2;
    
    // TODO: decrypt before printing
    //PlaintextArray p(ea);
    ea.decrypt(t, *secretKey, p);
    //cout << "The answer is: " ;
    //p.print(cout); cout << endl;
    vector<long> res;
    p.decode(res);
    cout << res[0] << endl;    

    return 0;
}
