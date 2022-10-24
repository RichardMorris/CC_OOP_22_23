#include <iostream>
#include <cmath>
#include <memory>
#include <stdlib.h>
#include <math.h>
using std::cout;
using std::endl;
using std::unique_ptr;
using std::make_unique;
using std::shared_ptr;

#include "vec3d.h"
#include "pbyte.h"

int main(int argc, char* argv[]) {
	double r = 1.0;
    int n_th = 20;
    int n_phi = 20;
    if(argc==4) {
        r = atof(argv[1]);
        n_th = atoi(argv[2]);
        n_phi = atoi(argv[3]);
    }
	cout.setf(std::ios_base::fixed);
	cout.precision(3);

    Vec3D u {1,2,3};
    Vec3D v {4,5,6};
    Vec3D w = u + v;
    Vec3D a = w - v;
    Vec3D b = v;
    b += a;
    if( u == a) {
        cout << "equals ";
    } else {
        cout << "not equals ";
    }
    cout << a[0] << ", " << a[1] << ", " << a[2] << endl;
    
    a[0]= 1.5;

    
    cout << a << endl;
    
    try {
        a[3] = 4;
    }
    catch (Vec3D::Bad_index &bi) {
        cout << "Error bad index " << bi.index << endl;
    }
}
