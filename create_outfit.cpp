#pragma GCC optimize("O3,unroll-loops")
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")


#include<bits/stdc++.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

//#define double float
#define vi vector<int>
#define vvi vector<vi>
#define vs vector<string>
#define vvs vector<vs>
#define vvvs vector<vvs>
#define vd vector<double>
#define vvd vector<vd>
#define pb push_back

#define pdi pair<double,int>
#define vdi vector<pdi>
#define fi first
#define se second

void debug(vs v){
    for (string s:v) cerr << s << ' ';
    cerr << '\n';
}
void debug(vi v){
    for (int s:v) cerr << s << ' ';
    cerr << '\n';
}

const int RECURSION_SIZE=8;
const int RANDOMIZER=30;

int n;
vvd graph;

vs id;
map<string,int> inv_id;
vs tipo;

map<string, int> ind_tipo;
vvi prendas_del_tipo;
set<string> tipos_vistos;

void read_data(){
    ifstream data("./model/clean_graph_final.data");

    data >> n;
    graph=vvd(n,vd(0));
        string l;
    getline(data,l);
    for (int i=0;i<n;i++){
        string line;
        getline(data,line);

        string ac;
        for (char c:line){
            if (c!=' ') ac.pb(c);
            else{
                //if (i==5736) cerr <<"AAA:\n"<< ac << endl;
                if (ac=="-nan") ac="0";
                graph[i].pb(stod(ac));
                ac="";
            }
        }
        //        cerr <<"BBB:\n"<< ac << endl;
        //graph[i].pb(stod(ac));
        //cout << i << ' ' << graph[i][0] << '\n';
    }

    id=vs(n);
    for (int i=0;i<n;i++) data >> id[i];
    //for (int i=0;i<10;i++) cout << id[i] << '\n';
    for (int i=0;i<n;i++) inv_id[id[i]]=i;

    tipo=vs(n);

    string line;
    getline(data,line); //ignore already read line
    getline(data,line);
    stringstream  lineStream(line);
    string cell;

    int cellind=0;
    while(getline(lineStream,cell,';')){

        tipo[cellind]=cell;
        cellind++;
    }

    for (int ac=0;ac<n;ac++){
        if (tipos_vistos.count(tipo[ac])==0){
            ind_tipo[tipo[ac]]=prendas_del_tipo.size();
            prendas_del_tipo.pb(vi{});
            tipos_vistos.insert(tipo[ac]);
        }
        prendas_del_tipo[ind_tipo[tipo[ac]]].pb(ac);
    }

}

double rate_outfit(vi &outfit){
    int k=outfit.size();
    double prod=1;
    for (int i=0;i<k;i++){
        for (int j=i+1;j<k;j++){
            prod*=graph[outfit[i]][outfit[j]];
            if (i+j<=3) prod*=graph[outfit[i]][outfit[j]];
            if (i+j<=2) prod*=graph[outfit[i]][outfit[j]];
            if (i==0) prod*=graph[outfit[i]][outfit[j]];
            //cerr << i << ',' << j << ':' << graph[outfit[i]][outfit[j]] << '\n';
        }
    }
    //cerr << "p: " << prod << '\n';
    return pow(prod,1.0/(k*(k-1)/2+9+(k-1)));
}

vi best_outfit(vvs &tipes, vi &chosen){
    if (chosen.size()==tipes.size()) return chosen;
    //if (chosen.size()>=oblig) if (rand()%10==0) return chosen;

    int AC_REC_SIZE=RECURSION_SIZE;
    if (chosen.size()==1) AC_REC_SIZE*=2;
    if (chosen.size()==2) AC_REC_SIZE*=1.5;
    if (chosen.size()==3) AC_REC_SIZE*=1.25;

    vi type;
    for (string s:tipes[chosen.size()]) type.pb(ind_tipo[s]);
    //ind_tipo[tipes[chosen.size()]];
    
    vdi results;
    for (int s:type){
        vdi partial_results;
        for (int nuevo:prendas_del_tipo[s]){
            if ((nuevo==prendas_del_tipo[s][prendas_del_tipo[s].size()-1] || partial_results.size()!=0)
                 && rand()%RANDOMIZER!=0) continue;
            chosen.pb(nuevo);
            partial_results.pb(pdi{rate_outfit(chosen),nuevo});
            chosen.pop_back();
        }
        sort(partial_results.begin(),partial_results.end());
        reverse(partial_results.begin(),partial_results.end());
        for (int i=0;i<min(1.5*AC_REC_SIZE/type.size(),double(partial_results.size()));i++) results.pb(partial_results[i]);
    }
    sort(results.begin(),results.end());
    reverse(results.begin(),results.end());

    vvi outfits;
    vi mej;
    double valuemej=-1;
    int ac=0;
    for (int i=0;i<min(AC_REC_SIZE,int(results.size()));i++){
        //if (rand()%2) continue;
        chosen.pb(results[i].se);
        outfits.pb(best_outfit(tipes,chosen));
        chosen.pop_back();

        double x=rate_outfit(outfits[ac]);
        if (x>valuemej){
            valuemej=x;
            mej=outfits[ac];
        }
        ac++;
    }
    //debug(mej);
    //for (vi v:outfits) cerr << rate_outfit(v) << '\n';
    //cerr << "------\n";

    //cerr << mej << '\n'

    return mej;
}

int runCommand(const std::string& command) {
    // Execute the command using system
    int result = system(command.c_str());

    // Check the result
    if (result == 0) {
        std::cout << "Command executed successfully." << std::endl;
    } else {
        std::cerr << "Command failed to execute." << std::endl;
    }

    return result;
}


map<string, vector<string> > metatipos;
map<int, string> id_to_type;
map<string, int> type_to_id;
map<int, string> id_to_metatype;
map<string, int> metatype_to_id;
vector<vvs> outfit_schemes;

void start_metatypes(){
    metatipos["Arriba"] = {"Shirt", "T-shirt", "Tops"};
    metatipos["Abajo"] = {"Trousers", "Jeans", "Skirts", "Shorts"};
    metatipos["Completo"] = {"Dresses", "Jumpsuit"};
    metatipos["Accesorios"] = {"Jewellery", "Bags", "Glasses", "Wallets & cases", "Belts and Ties", "Hats, scarves and gloves"};
    metatipos["Invierno"] = {"Coats", "Puffer coats", "Jackets", "Blazer"};
    metatipos["Sobrecapa"] = {"Sweater", "Cardigans"};
    metatipos["Zapatos"] = {"Footwear"};
    metatipos["Intimo"] = {"Intimate"};
}

/*

void initialize_type_ids(){
    int pos = 0;
    int mt_pos = 0;

    for (auto p : metatipos){
        string metatipo = p.first;
        metatype_to_id[metatipo] = mt_pos;
        id_to_metatype[mt_pos] = metatipo;
        mt_pos++;

        for (auto tipo : metatipos[metatipo]){
            id_to_type[pos] = tipo;
            type_to_id[tipo] = pos;
            pos++;
        }
    }
}

*/


// Estructura de un outfit_scheme: 
// {{elementos obligatorios}, {elementos opcionales}}

/*
void initialize_outfit_schemes(){
    outfit_schemes.push_back({{"Arriba", "Abajo", "Zapatos"}, {"Accesorios", "Invierno", "Sobrecapa"}});
    outfit_schemes.push_back({{"Completo", "Zapatos"}, {"Accesorios", "Invierno"}});
    outfit_schemes.push_back({{"Sobrecapa", "Abajo", "Zapatos"}, {"Accesorios", "Invierno"}});
    outfit_schemes.push_back({{"Invierno", "Abajo", "Zapatos", "Arriba"}, {"Accesorios"}});
}
*/

vvs combinations;

void initialize_combinations(){
    combinations.pb(vs{"Arriba", "Abajo", "Zapatos","Accesorios","Accesorios"});
    combinations.pb(vs{"Arriba", "Abajo", "Zapatos","Invierno","Accesorios"});
    combinations.pb(vs{"Arriba", "Abajo", "Zapatos","Sobrecapa","Accesorios"});
    combinations.pb(vs{"Arriba", "Abajo", "Zapatos","Invierno", "Sobrecapa"});
    combinations.pb(vs{"Arriba", "Abajo", "Zapatos","Invierno", "Sobrecapa","Accesorios"});
    combinations.pb(vs{"Arriba", "Abajo", "Zapatos","Invierno", "Sobrecapa","Accesorios","Accesorios"});

    combinations.pb(vs{"Completo", "Zapatos", "Invierno","Accesorios","Accesorios"});
    combinations.pb(vs{"Completo", "Zapatos", "Invierno","Accesorios"});
    combinations.pb(vs{"Completo", "Zapatos", "Accesorios","Accesorios"});

    combinations.pb(vs{"Sobrecapa", "Abajo", "Zapatos","Invierno","Accesorios","Accesorios"});
    combinations.pb(vs{"Sobrecapa", "Abajo", "Zapatos","Invierno","Accesorios"});
    combinations.pb(vs{"Sobrecapa", "Abajo", "Zapatos","Accesorios","Accesorios"});

    combinations.pb(vs{"Invierno", "Abajo", "Zapatos", "Arriba","Accesorios"});
    combinations.pb(vs{"Invierno", "Abajo", "Zapatos", "Arriba","Accesorios","Accesorios"});
}




vector<vvs> get_outfit_schemes(string type){
    vector<vvs> schemes;
    for (auto scheme : outfit_schemes){
        bool found = false;
        for (auto metatipo : scheme[0]){
            for (auto tipopo : metatipos[metatipo]){
                if (tipopo == type)
                    found = true;
            }
        }

        if (found == true)
            schemes.push_back(scheme);
    }

    return schemes;
}

int main(){
    srand(time(0));
    read_data();

    start_metatypes();
    initialize_combinations();

    ifstream inp("request.txt");
    ofstream out("answer.txt");

    string s;
    inp >> s;

    int x=inv_id[s];
    //cerr << s << ' ' << x << '\n';

    string familia="Accesorios";
    for (pair<string,vector<string>> p:metatipos){
        //debug(p.se);
        for (string t:p.se) if (t==tipo[x]) familia=p.fi;
    }

    vvs combos;
    for (vs v:combinations){
        for (int j=v.size()-1;j>=1;j--) if (v[j]==familia) swap(v[j],v[j-1]);
        if (v[0]==familia) combos.pb(v);
    }

    //cerr << "combos: \n";
    //for (vs ves:combos) debug(ves);

    random_shuffle(combos.begin(),combos.end());


    vvvs realcombos(combos.size());
    for (int i=0;i<combos.size();i++){
        realcombos[i]=vvs(combos[i].size());
        for (int j=0;j<combos[i].size();j++) 
            realcombos[i][j]=metatipos[combos[i][j]];
    }

    //cerr << "realcombos0: \n";
    //for (vs ves:realcombos[0]) debug(ves);
    //cerr << "realcombos1: \n";
    //for (vs ves:realcombos[1]) debug(ves);

    vi vx=vi{x};

    vi outfit1=best_outfit(realcombos[0],vx);
    vi outfit2=best_outfit(realcombos[1],vx);

    if (rate_outfit(outfit1)<rate_outfit(outfit2)) swap(outfit1,outfit2);

    ///
    vi best=outfit1;

    for (int y:best) out << id[y] << '\n';
    /*
    for (int y:best){
        string st=string("xdg-open ./data/datathon/datathon/images/")+ string(id[y])+string(".jpg");
        st[st.size()-7]='_';
        runCommand(st);
    }
    */
    cout << rate_outfit(best) << '\n';
    
}
