#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <istream>

#include <numeric>
#include <iostream>
#include <cstdio>
#include <cmath>

using namespace std;

int main(){
    vector<string> row ={"5.1","6.1","7.1","8.1"};
    // std::cout<<row[1]<<std::endl;
    vector<double> quato {stod(row[0]), stod(row[1]), stod(row[2]), stod(row[3])};
    double inner;
    inner=inner_product(quato.begin(), quato.end(), quato.begin(), 0.0);
    cout<<inner<<endl;
    vector<double> quato1={quato[0]*sqrt(2.0/inner),quato[1]*sqrt(2.0/inner),quato[2]*sqrt(2.0/inner),quato[3]*sqrt(2.0/inner)};
    cout<<quato1[0]<<endl;
    cout<<quato1[1]<<endl;
    cout<<quato1[2]<<endl;
    cout<<quato1[3]<<endl;
    cout<<1.0-quato1[1]*quato1[1]-quato1[2]*quato1[2]<<endl;
    cout<<quato1[0]*quato1[1]-quato1[2]*quato1[3]<<endl;
    cout<<quato1[0]*quato1[2]+quato1[1]*quato1[3]<<endl;
    cout<<quato1[0]*quato1[1]+quato1[2]*quato1[3]<<endl;
    cout<<1.0-quato1[0]*quato1[0]-quato1[2]*quato1[2]<<endl;
    cout<<quato1[1]*quato1[2]-quato1[0]*quato1[3]<<endl;
    cout<<quato1[0]*quato1[2]-quato1[1]*quato1[3]<<endl;
    cout<<quato1[1]*quato1[2]+quato1[0]*quato1[3]<<endl;
    cout<<1.0-quato1[0]*quato1[0]-quato1[1]*quato1[1]<<endl;
    cout<<0.0<<endl;
    cout<<0.0<<endl;
    cout<<0.0<<endl;
    cout<<1.0<<endl;
}