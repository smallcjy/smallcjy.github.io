#include<limits.h>
#include<bits/stdc++.h>
using namespace std;


class Solution {
public:
    /**
     * @param maze: the maze
     * @param start: the start
     * @param destination: the destination
     * @return: the shortest distance for the ball to stop at the destination
     */
    const int dirs[5] = {1, 0, -1, 0, 1};

    struct Point {
        int x, y;
    };

    int ans = INT_MAX;

    vector<string> wordSort(vector<char> &alphabet, vector<string> &words) {
        // Write your code here
        char into[500], outto[500];
        for(int i = 0; i < alphabet.size(); i++) {
            into[alphabet[i]] = 'a' + i;
            outto['a' + i] = alphabet[i];
        }
        
        for(auto& word : words) {
            for(int i = 0; i < word.size(); i++) {
                word[i] = into[word[i]];
            }
        }

        sort(words.begin(), words.end());
        for(auto& word : words) {
            for(int i = 0; i < word.size(); i++) {
                word[i] = outto[word[i]];
            }
        }

        return words;
    }
};