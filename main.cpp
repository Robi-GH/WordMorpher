#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <cstdlib>
#include <map>
#include <queue>
#include <ctime>
#include <random>

using namespace std;

void createDictionary(vector<string> &dictionary){
    ifstream inputFile("dictionary.txt");
    string word;
    while (getline(inputFile, word)){
        dictionary.push_back(word);
    }
}

void makeWordRange(vector<pair<int,int>> &wordRange, vector<string> &dictionary){
    wordRange.push_back(make_pair(-1,-1));
    wordRange.push_back(make_pair(-1,-1));
    wordRange.push_back(make_pair(-1,-1));
    int startingPoint = 0;
    int endingPoint = 0;
    int sizeOfWords = 3;
    for(auto i: dictionary){
        if(i.length() >sizeOfWords){
            wordRange.push_back(make_pair(startingPoint, endingPoint));
            startingPoint = endingPoint;
            sizeOfWords++;
        }
        endingPoint++;
    }
    wordRange.push_back(make_pair(startingPoint, endingPoint-1));
}


int createRandomNumber(int a, int b, mt19937& gen){
    uniform_int_distribution<> dis(a, b);
    return dis(gen);
}

bool isThereThisWord(string temporary, vector<string> &dictionary, int a, int b){
    return binary_search(dictionary.begin() + a, dictionary.begin() + b, temporary);
}


void findPairsOfWords(vector<pair<int,int>> &wordRange, vector<string> &dictionary){
    mt19937 gen(static_cast<unsigned int>(time(nullptr)));
    for(int i = 0; i < 100; i++){
        while(true){
            int n1 = createRandomNumber(0, dictionary.size()-1, gen);
            string firstWord = dictionary[n1];
            string secondWord;
            map<string, string> path;
            vector<string> words;
            map<string, int> condition;
            condition[firstWord] = 0;
            map<string, int> visited;
            visited[firstWord] = 1;
            path[firstWord] = "0";
            queue<pair<string, int>> listOfUpcomingWords;
            listOfUpcomingWords.push({firstWord, 0});
            int maxDepth = 0;
            while(!listOfUpcomingWords.empty()){
                string word = listOfUpcomingWords.front().first;
                int currentDepth = listOfUpcomingWords.front().second;
                if (currentDepth > maxDepth) maxDepth = currentDepth;
                listOfUpcomingWords.pop();
                if (condition[word] == 1) continue;
                condition[word] = 1;
                if (word != firstWord) words.push_back(word);
                if (currentDepth > 5) break;

                //dodavanje slova na kraj
                //dodavanje slova na pocetak
                string temporary = word;
                if(temporary.length()!=dictionary[dictionary.size()-1].length()){
                    for(int i = 97; i<= 122; i++){
                        temporary = word;
                        temporary = temporary + char(i);
                        if(isThereThisWord(temporary, dictionary, wordRange[temporary.length()].first, wordRange[temporary.length()].second)){
                            if(visited[temporary] == 1){
                                continue;
                            }
                            listOfUpcomingWords.push({temporary, currentDepth + 1});
                            path[temporary] = word;
                            visited[temporary] = 1;
                        }
                        temporary = word;
                        temporary = char(i) + temporary;
                        if(isThereThisWord(temporary, dictionary, wordRange[temporary.length()].first, wordRange[temporary.length()].second)){
                            if(visited[temporary] == 1){
                                continue;
                            }
                            listOfUpcomingWords.push({temporary, currentDepth + 1});
                            path[temporary] = word;
                            visited[temporary] = 1;
                        }
                    }
                }
                //brisanje svakog slova pojedincno
                if(word.length() > 3){
                    for(int i = 0; i< word.length(); i++){
                        temporary = "";
                        for(int j = 0; j<word.length(); j++){
                            if(i != j){
                                temporary += word[j];
                            }
                        }
                        if(isThereThisWord(temporary, dictionary, wordRange[temporary.length()].first, wordRange[temporary.length()].second)){
                            if(visited[temporary] == 1){
                                continue;
                            }
                            listOfUpcomingWords.push({temporary, currentDepth + 1});
                            path[temporary] = word;
                            visited[temporary] = 1;
                        }
                    }
                }
                //promjena svakog slova u neko drugo pojedinacno
                for(int i = 0; i< word.length(); i++){
                    temporary = word;
                    for(int j = 97; j<= 122; j++){
                        if(temporary[i] == char(j)) continue;
                        temporary[i] = char(j);
                        if(isThereThisWord(temporary, dictionary, wordRange[temporary.length()].first, wordRange[temporary.length()].second)){
                            if(visited[temporary] == 1){
                                continue;
                            }
                            listOfUpcomingWords.push({temporary, currentDepth + 1});
                            path[temporary] = word;
                            visited[temporary] = 1;
                        }
                    }
                }
            }
            if(words.size() == 0 || maxDepth < 2){
                continue;
            }
            secondWord = words[createRandomNumber(0, words.size()-1, gen)];

            cout << i+1 << ": ";
            string parent = secondWord;
            vector<string> listOfPaths;
            while(true){
                if(parent == "0") break;
                listOfPaths.push_back(path[parent]);
                parent = path[parent];
            }
            for(int i = listOfPaths.size()-2; i>=0; i--){
                cout << listOfPaths[i] << " ---> ";
            }
            cout << secondWord << endl;
            break;
        }
    }
}

int main(){
    vector<string> dictionary;
    createDictionary(dictionary);
    vector<pair<int,int>> wordRange;
    makeWordRange(wordRange, dictionary);
    findPairsOfWords(wordRange, dictionary);
    return 0;
}
