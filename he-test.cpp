// Example program
#include <iostream>
#include <string>
#include <vector>
#include <random>
using namespace std;

#define q 12
#define plain_text 8

long mul(vector<int> a, vector<int> b)
{
    long s = 0;
    for(unsigned i=0; i<a.size(); i++)
    {        
        s = s + a[i] * b[i];
    }
    return s;
}

vector<int> add(vector<int> a, vector<int> b)
{
    vector<int> s;
    for(unsigned i=0; i<a.size(); i++)
    {        
        s.push_back(a[i]+b[i]);
    }

    return s;
}

int main()
{    
    cout << "This is distributed HELib !!!" << endl;
    
    int my_a[5] = { 16, 2, 77, 40, 12 };
    vector<int> a (my_a, my_a + sizeof(my_a) / sizeof(int) );
    
    int my_s1[5] = { 230, 110, 400, 150, 260 };
    vector<int> s1 (my_s1, my_s1 + sizeof(my_s1) / sizeof(int) );
    
    int my_s2[5] = { 200, 100, 700, 300, 500 };
    vector<int> s2 (my_s2, my_s2 + sizeof(my_s2) / sizeof(int) );
            
    default_random_engine generator;
    normal_distribution<double> distribution(3.0,2.0);    
    int e;
    for (int i=0; i<100; i++) 
    { e = distribution(generator); }
       
    long Enc = (mul(a, add(s1, s2)) + q*e + plain_text) % q;
    cout << "Encrypted text: " << Enc << endl;
    
    
    /* ************************************ */
    for (int i=0; i<100; i++) 
        e = distribution(generator); 
    long p1 = mul(a, s1) + q*e;
    
    for (int i=0; i<100; i++) 
        e = distribution(generator); 
    long p2 = mul(a, s2) + q*e;
    /* ************************************ */
    long Dec = (Enc - p1 - p2) % q + q;
    cout << "Decrypted text: " << Dec%q << endl;
    
    return 0;
}
