#pragma GCC optimize("O3,unroll-loops")
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")

#include<bits/stdc++.h>

using namespace std;

#define double float
#define vi vector<int>
#define vvi vector<vi>
#define vs vector<string>
#define vd vector<double>
#define vvd vector<vd>
#define pb push_back

const int MAX_CLOTHES=2000;
const int ITERATIONS=9;
const int MAX_REP=15;

int n;
vvd graph;
vvd parecido;

vi repeticiones;

vs id;
map<string,int> inv_id;
vs color_especifico;
vs color_general;
vs material;
vs tipo_general;
vs tipo_especifico;

map<string, int> ind_tipo;
vvi prendas_del_tipo;
set<string> tipos_vistos;

vd degree;

void read_clothes(){
    ifstream  data("./../processed_data/clean_product_data_newsep.txt");

    string line;
    getline(data,line); //ignore first line
    int ac=0;
    while(getline(data,line)){
        if (ac==MAX_CLOTHES) break;
        stringstream  lineStream(line);
        string cell;

        int cellind=0;

        while(getline(lineStream,cell,';')){

            if (cellind==0){ //id
                id.pb(cell);
                inv_id[cell]=ac;
            }
            if (cellind==1) color_especifico.pb(cell);
            if (cellind==2) color_general.pb(cell);
            if (cellind==3) material.pb(cell);
            if (cellind==4) tipo_general.pb(cell);
            if (cellind==5) tipo_especifico.pb(cell);
            
            cellind++;
        }
        if (tipo_general[ac]=="\"Accesories, Swim and Intimate\"") tipo_general[ac]=tipo_especifico[ac];
        if (tipos_vistos.count(tipo_especifico[ac])==0){
            ind_tipo[tipo_especifico[ac]]=prendas_del_tipo.size();
            prendas_del_tipo.pb(vi{});
            tipos_vistos.insert(tipo_especifico[ac]);
        }
        prendas_del_tipo[ind_tipo[tipo_especifico[ac]]].pb(ac);
        ac++;
    }

    //cout << id[1] << ' ' << color_especifico[1] << ' ' << color_general[1] << ' ' << material[1] << '\n';

    n=id.size();
    graph=vvd(n,vd(n,0));
    for (int i=0;i<n;i++) graph[i][i]=1;
    repeticiones=vi(n,0);
}

void read_combinations(){
    ifstream data("outfit_data.csv");

    string last="-1";
    vi outfit;

    string line;
    getline(data,line); //ignore first line
    while(getline(data,line)){
        stringstream lineStream(line);
        string cell;

        int cellind=0;

        string outfit_id;
        string cloth_id;
        while(getline(lineStream,cell,',')){

            if (cellind==0) outfit_id=cell;
            if (cellind==1) cloth_id=cell;
            
            cellind++;
        }

        cloth_id.pop_back();
        reverse(cloth_id.begin(),cloth_id.end());
        cloth_id.pop_back();
        reverse(cloth_id.begin(),cloth_id.end());
        
        if (outfit_id!=last) outfit=vi{};
        last=outfit_id;

        int cloth=inv_id[cloth_id];

        if (cloth==0 && cloth_id!=id[0]) continue;
        //if (repeticiones[cloth]>=MAX_REP) continue;
        //if (id[cloth]=="57005976-TN") cerr << "?????\n";
        repeticiones[cloth]++;

        for (int x:outfit){
            graph[x][cloth]=1;
            graph[cloth][x]=1;

        }

        outfit.pb(cloth);
        
    }
}

double parecido_inicial (int i, int j) {
    if (i==j) return 0;
    double pc = 0, pm = 0, pe = 0;
    if (color_general[i]  == color_general[j]) {
        pc = 0.5;
    }
    if (color_especifico[i] == color_especifico[j]) {
        pc = 1;
    }
    if (material[i] == material[j]) {
        pm = 1;
    }
    if (tipo_especifico[i] == tipo_especifico[j]) {
        pe = 1;
    }
    return pe*pow(0.8*pc + 0.2*pm,1.7);
}

void calc_parecido(){
    parecido=vvd(n,vd(n,0));
    //degree=vd(n,0);
    for (int i=0;i<n;i++){
        for (int j=i;j<n;j++){
            parecido[i][j]=parecido[j][i]=parecido_inicial(i,j);
            //degree[i]+=parecido[i][j];
            //degree[j]+=parecido[j][i];
        }
    }
}

double update_pair(int a, int b){
    if (tipo_especifico[a]==tipo_especifico[b]) return graph[a][b];
    vi c1=prendas_del_tipo[ind_tipo[tipo_especifico[a]]];
    vi c2=prendas_del_tipo[ind_tipo[tipo_especifico[b]]];

    double suma1=0;
    double suma2=0;
    for (int x:c1){  //no quito el caso porque parecido[a][a]=0
        suma1+=graph[x][b]*parecido[x][a];
        suma2+=parecido[x][a];
    }
    for (int x:c2){
        suma1+=graph[x][a]*parecido[x][b];
        suma2+=parecido[x][b];
    }
    //if (rand()%10000) cout << graph[a][b]<< ' ' << suma1 << ' ' << suma2 <<  '\n';
    if (suma2==0) return graph[a][b]*graph[a][b];
    return pow(graph[a][b],1.8)+(1-graph[a][b])*(suma1/suma2);
}

void update(){
    //auto beg = std::chrono::high_resolution_clock::now();
    vvd newgraph(n,vd(n,1));
    vd newdegree(n,0);
    int it=0;
    for (int i=0;i<n;i++){
        for (int j=i+1;j<n;j++){
            newgraph[i][j]=newgraph[j][i]=update_pair(i,j);
            newdegree[i]+=newgraph[i][j];
            newdegree[j]+=newgraph[j][i];
            it++;
            if (rand()%10000000==0){ 
                cout<< "Progress: " << 100.0*it/(n*(n-1)/2) << "%\n";
                //auto end = std::chrono::high_resolution_clock::now();
                //auto duration = std::chrono::duration_cast<microseconds>(end - beg);
                //std::cout << "Elapsed Time: " << std::chrono::duration.count() << '\n';
            }
        }

    }
    //degree=newdegree;
    graph=newgraph;
}

int main(){
    read_clothes();
    read_combinations();
    calc_parecido();

    cout << n << '\n';

    for (int it=0;it<ITERATIONS;it++){
        cout << it << " iteraciones completadas\n";
        update();
    }

    for (int i=0;i<n;i++){
        for (int j=0;j<n;j++) 
            if (max(repeticiones[i],repeticiones[j])>=MAX_REP) graph[i][j]*=0.1;
            else if (min(repeticiones[i],repeticiones[j])<=1) graph[i][j]*=0.1;
            else if (min(repeticiones[i],repeticiones[j])<=2) graph[i][j]*=0.5;

    }

    ofstream out("graph.data");

    out << n << '\n';
    for (int i=0;i<n;i++){
        for (int j=0;j<n;j++){
            out << sqrt(graph[i][j]) << ' '; 
        }
        out << '\n';
    }

    for (int i=0;i<n;i++) out << id[i] << ' ';
    out << '\n';
    for (int i=0;i<n;i++) out << tipo_especifico[i] << ';';
    out << '\n';

   // cerr << repeticiones[inv_id["57005976-TN"]] << '\n';
}