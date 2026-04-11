
#include <iostream>
#include <algorithm>

using namespace std;

class route {
    int *town;
    int number;

public:

    route(int num = 0);
    route(const route &oth); 
    route &operator=(const route &oth); 
    ~route() {
        delete[] town;
    }

    int routePrice(int**);
    bool nextRoute();
    friend ostream &operator << (ostream&out, const route&way);
};

  
    route::route(int num){
        number = num;
        town = new int[number];
        for (int i = 0; i < number; i++) {
            town[i] = i;
        }
    }

 
    route::route(const route &oth) {
        number=oth.number;
        town = new int[number];
        for (int i = 0; i < number; i++) {
            town[i] = oth.town[i];
        }
    }


    route &route::operator=(const route&oth) {
        if (this == &oth) { 
            return *this;
        }
        number = oth.number;
        delete[] town;
        town = new int[number];
        for (int i = 0; i < number; i++) {
            town[i] = oth.town[i];
        }
        return *this;
    }

 
        int route::routePrice(int **arr) {
            int sum = 0;
            for (int i = 0; i < number; i++) {
                sum += arr[town[i]] [town[(i + 1) % number]];
            }
            return sum;
        }

  
    bool route::nextRoute(){
   
       int i = -1;
       int j;
       int k;
  
       for ( k = number - 2; k >= 1; k--) {     
          if (town[k] < town[k + 1]) {
              i = k;
              break;
           }
       }

      
       if (i < 0) {
           return false;  
       }

      for ( k = number - 1; k >= i + 1; k--) { 
            if (town[i] < town[k]) { 
                j = k;
                int tmp = town[j];
                town[j] = town[i];
                town[i] = tmp;
                break; 
            }
        } 



        for ( j = i + 1, k = (number - 1); j < k; j++, k--){
            int tmp = town[j];
            town[j] = town[k];  
            town[k] = tmp;
        }
        return true;  
    }

ostream& operator << (ostream &out, const route &way) {
    for (int i = 0; i < way.number; i++) {      
        out << way.town[i] << " ";
    }
    return out;
}
int main() {

    int numer;
    cout << "enter the numer of cities: ";
    cin >> numer;
    cout << "enter cost array:\n";


    route perm1(numer);

    int **cost = new int *[numer];

    for (int i = 0; i < numer; i++) {
        cost[i] = new int[numer];
        for (int j = 0; j < numer; j++) {
            if (i == j) {
                cost[i][j] = 0;
            } else {
                cout << i << "->" << j << ":";
                cin >> cost[i][j];
            }
        }
        cout  << endl;
    }

    route perm2 = perm1;

    int price1 = perm1.routePrice(cost);
    cout << "Route: " << perm1 << "Price: " << price1 << endl;

  
    while (perm2.nextRoute()) {
        int price2 = perm2.routePrice(cost);
        cout << "Route: " << perm2 << "Price: " << price2 << endl;

        if (price2 <= price1) {
            perm1 = perm2;
            price1 = price2;
        }
    }

    cout << "Optimal price: " << price1 << endl << "Optimal route: " << perm1;

    for (int i = 0; i < numer; i++) {
        delete[] cost[i];
    }

    delete[] cost;

    return 0;
}
