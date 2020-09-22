#include<math.h>
#include<string>

using namespace std;

class DotDir {
  private:
    float c[4];
  public:
     friend DotDir operator-(const DotDir& dd1, const DotDir& dd2);
     friend DotDir operator+(const DotDir& dd1, const DotDir& dd2);

     DotDir(float x, float y, float z, float w){
         c[0] = x;
         c[1] = y;
         c[2] = z;
         c[3] = w;
     }

     int mod() {
         return sqrt(x * x + y * y + z * z);
     }

     string ToString(){
       return to_string(x) + ", " + to_string(y) + ", " + to_string(z) + ", " + to_string(w); 
     }
};

DotDir operator+(const DotDir& dd1, const DotDir& dd2){
  // Dirección + Dirección = Dirección OK, w = 0
  // Punto + Punto = NO HACER NO OK, w = 0
  // Dirección + Punto = Punto OK, w = 1
  // Punto + Dirección = Punto OK, w = 1
  return DotDir(dd1.x + dd2.x, dd1.y + dd2.y, dd1.z + dd2.z, dd1.w + dd2.w);
}

DotDir operator-(const DotDir& dd1, const DotDir& dd2){
  // Dirección - Dirección = Dirección OK, w = 0
  // Punto - Punto = Dirección OK, w = 0
  // Dirección - Punto = NO HACER NO OK, w = - 1
  // Punto - Dirección = Punto OK, w = 1
  return DotDir(dd1.x - dd2.x, dd1.y - dd2.y, dd1.z - dd2.z, dd1.w - dd2.w);
}
