#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
#include <math.h>
#include <bitset>
#include <tuple>
#include <fstream>
#include <sstream>
#include <chrono>

using namespace std;

vector<tuple<string, int, int>> generateSubset(vector<tuple<string, int, int>> v, int n){
    
    vector<tuple<string, int, int>> ret;
    
    string s = bitset<8>(n).to_string();
    reverse(s.begin(), s.end());
    
    for(int i = 0; i < s.length(); i++){
        if(s[i] == '1'){
            ret.push_back(v[i]);
        }
    }
    
    return ret;
}

pair<int, vector<tuple<string, int, int>>> ComputeMaxProfit(vector<tuple<string, int, int>> I, int W){
    
    int maxProfit = 0;
    vector<tuple<string, int, int>> S;
    vector<tuple<string, int, int>> M;
    
    int sumOfI = 0;
    for(int i = 0; i < I.size(); i++){
        sumOfI += get<1>(I[i]);
    }
    
    //if the sum of the weights in I <= W
    if(sumOfI < W){
        //return the profit of the items in I, and I;
        pair<int, vector<tuple<string, int, int>>> p;
        
        int sumOfIProfit = 0;
        for(int i = 0; i < I.size(); i++){
            sumOfIProfit += get<2>(I[i]);
        }
        
        p.first = sumOfIProfit;
        p.second = I;
        return p;
    }
    
    int count = 0;
    //while there are more subsets to generate
    while(count < pow(2, I.size())){
        
        S = generateSubset(I, count);
        
        int sumOfS = 0;
        for(int i = 0; i < S.size(); i++){
            sumOfS += get<1>(S[i]);
        }
        
        //if the sum of the weights in S <= W
        if(sumOfS <= W){
            //if the profit of the items in S > maxProfit 
            
            int sumOfSProfit = 0;
            for(int i = 0; i < S.size(); i++){
                sumOfSProfit += get<2>(S[i]);
            }
            
            if(sumOfSProfit > maxProfit){               
                //update maxProfit
                maxProfit = sumOfSProfit;
                //copy S to M   
                M = S;
            }
        }
        //generate the next subset S   
        count++;
    }
    
    //return maxProfit and M
    pair<int, vector<tuple<string, int, int>>> p;
    p.first = maxProfit;
    p.second = M;
    return p;
}

int main(int argc, char *argv[]) {
    
    vector<tuple<string, int, int>> input;
    vector<pair<string, int>> profitPrices;
    
    pair<int, vector<tuple<string, int, int>>> result;
    
    ifstream infile1(argv[2]);
    ifstream infile2(argv[4]);
    
    string card_name;
    int card_profit;
    
    string line;   
    string line2;
    
    int count = 0;
    int weight;
    
    ofstream output("output.txt");
    cout.rdbuf(output.rdbuf());
    
    while(getline(infile1, line)){
        stringstream s(line);
        
        if(isdigit(line[0])){
            continue;
        }
        else{
            s >> card_name >> card_profit;
            profitPrices.push_back(make_pair(card_name, card_profit));
        }
    }
    
    string s1;
    string s2;
    
    
    while(getline(infile2, line2)){
        stringstream s(line2);
        
        s >> s1 >> s2;
        
        if(count == 0){
            if(input.size() > 0){
                
                auto start = chrono::high_resolution_clock::now();
                result = ComputeMaxProfit(input, weight);
                auto end = chrono::high_resolution_clock::now();
                chrono::duration<double> time_needed = end - start;
                
                cout << input.size() << " " << result.first << " " << result.second.size() << " " << time_needed.count() << endl;
            }
            
            count = stoi(s1);
            weight = stoi(s2);
            input.clear();
        }
        else{
            int profit;
            for(int i = 0; i < profitPrices.size(); i++){
                if(profitPrices[i].first == s1){
                    profit = profitPrices[i].second;
                    break;
                }
            }
            
            input.push_back(make_tuple(s1, stoi(s2), profit));
            count--;
        }  
    }
    
    auto start = chrono::high_resolution_clock::now();
    result = ComputeMaxProfit(input, weight);
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> time_needed = end - start;
    
    cout << input.size() << " " << result.first << " " << result.second.size() << " " << time_needed.count() << endl;
}