#pragma GCC optimize("O3,unroll-loops")
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")

#include<bits/stdc++.h>

using namespace std;

//#define double float
#define vi vector<int>
#define vvi vector<vi>
#define vs vector<string>
#define vd vector<double>
#define vvd vector<vd>
#define pb push_back

#define pii pair<int,int>
#define pdii pair<double,pii>
#define fi first
#define se second
#define vdii vector<pdii>

void medir(vdii w){
    for (int i=0;i<33;i++){
        cout << "Interval (" << 0.03*i << ','<< 0.03*(i+1) << "): ";
        int count=0;
        for (pdii x:w) count+=(x.fi>0.03*i && x.fi<0.03*(i+1));
        cout << 2*count << '\n';
    }
    cout << "----------\n";
}

double sigmoid(double x){
    return 1.0/(1.0+pow(3.0,8*(0.5-x)));
}

signed main(){
    ifstream in("graph.data");
    int n;
    in >> n;
    vvd v(n,vd(n));
    vdii w;
    double gran=0,peq=1;
    for (int i=0;i<n;i++){
        for (int j=0;j<n;j++){
            in >> v[i][j];
            if (j<=i) continue;
            if (v[i][j]<0.0001 || v[i][j]>0.9999) continue;
            //cerr << v[i][j] << '\n';
            w.pb(pdii{v[i][j],pii{i,j}});
            gran=max(gran,v[i][j]);
            peq=min(peq,v[i][j]);
        }
    }
    sort(w.begin(),w.end());
    medir(w);

    double alpha=1/(gran-peq);
    double m=-alpha*peq/(1-alpha);


    for (int i=0;i<w.size();i++) w[i].fi=m+alpha*(w[i].fi-m);

    medir(w);


    for (int i=0;i<w.size();i++) w[i].fi=pow(w[i].fi,0.33);
    //for (int i=0;i<w.size();i++) w[i].fi=sigmoid(w[i].fi);
    
    medir(w);

    //cerr << peq << ' ' << gran << ' ' << m << ' ' << alpha << '\n';

    //cout <<"sizes: "<< n*n << ' ' << 2*w.size() << '\n';

    //for (int i=0;i<10;i++) cerr << w[i].fi << ' ';
    
    for (int i=0;i<n;i++){
        for (int j=0;j<n;j++) if (v[i][j]>0.9999) v[i][j]=0;
    }


    for (pdii x:w) v[x.se.fi][x.se.se]=v[x.se.se][x.se.fi]=x.fi;

    // vd deg(n,0);
    // for (int i=0;i<n;i++){
    //     for (int j=0;j<n;j++) deg[i]+=v[i][j];
    //     //cout << deg << '\n';
    // }

    // for (int i=0;i<n;i++){
    //     for (int j=0;j<n;j++) v[i][j]=pow(v[i][j],(deg[i]+deg[j])/1000.0);
    // }


    // deg=vd(n,0);
    // for (int i=0;i<n;i++){
    //     for (int j=0;j<n;j++) deg[i]+=v[i][j];
    //     cout << deg[i] << '\n';
    // }
    
    
    ofstream out ("clean_graph_final.data");

    out << n << '\n';
    for (int i=0;i<n;i++){
        for (int j=0;j<n;j++) out << v[i][j] << ' ';
        out << '\n';
    }

    string s;
    getline(in,s);
    getline(in,s);
    out << s << '\n';
    //out << "AAA\n";
    getline(in,s);
    out << s << '\n';
}