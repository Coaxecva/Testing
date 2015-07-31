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

    //cout << "context: " << *context << endl;

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

    long ptxtSpace = power_long(101,2);

    FHESecKey* secretKey1 = new FHESecKey(*context);
    FHESecKey* secretKey2 = new FHESecKey(*context);
    secretKey1->GenSecKey(64, ptxtSpace);
    secretKey2->GenSecKey(64, ptxtSpace);
    addSome1DMatrices(*secretKey1);

    EncryptedArray* eas = new EncryptedArray(*context);
    //cout << "--secretKey1-" << *secretKey1 << endl;
    //cout << "--secretKey2-" << *secretKey2 << endl;
    cout << (*secretKey1 == *secretKey2) << endl;
    cout << "ea "<< eas->size() << endl;

    vector<ZZX> b;
    vector<ZZX> ptxts;
    long p2r = eas->getContext().alMod.getPPowR();
    ZZX poly = RandPoly(0,to_ZZ(p2r)); // choose a random constant polynomial
    eas->decode(ptxts, poly);

    cout << "p2r " << p2r << endl;
    cout << "poly " << poly << endl;
    cout << "ptxts " << ptxts << endl;

    Ctxt* ctxts = new Ctxt(*publicKey);
    cout << "ctxts " << *ctxts << endl;
    eas->encrypt(*ctxts, *publicKey, ptxts);
    eas->decrypt(*ctxts, *secretKey1, b);
    cout << "ptxts " << ptxts[1] << endl;
    cout << "b " << b.size() << endl;
    assert(ptxts[1] == b[1]);

    return 0;
}
