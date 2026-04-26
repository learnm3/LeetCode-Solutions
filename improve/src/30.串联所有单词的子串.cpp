/*
 * @lc app=leetcode.cn id=30 lang=cpp
 *
 * [30] 串联所有单词的子串
 */

// @lc code=start
class Solution {
public:
    vector<int> findSubstring(string s, vector<string>& words) {
        vector<int> result;
        if (words.empty() || s.empty()) return result;

        int wordLength = words[0].size();
        int wordCount = words.size();
        int totalLength = wordLength * wordCount;
        int sLen = s.size();

        if (sLen < totalLength) return result;

        unordered_map<string, int> wordMap;
        for (const string& word : words) {
            wordMap[word]++;
        }

        // Only need to check offsets from 0 to wordLength-1
        for (int offset = 0; offset < wordLength; ++offset) {
            unordered_map<string, int> seenWords;
            int left = offset;
            int count = 0;

            for (int i = offset; i <= sLen - wordLength; i += wordLength) {
                string currentWord = s.substr(i, wordLength);

                if (wordMap.find(currentWord) != wordMap.end()) {
                    seenWords[currentWord]++;
                    count++;

                    // Check if current word exceeds required count
                    if (seenWords[currentWord] > wordMap[currentWord]) {
                        // Shrink window from left until we remove the excess word
                        while (seenWords[currentWord] > wordMap[currentWord]) {
                            string leftWord = s.substr(left, wordLength);
                            seenWords[leftWord]--;
                            count--;
                            left += wordLength;
                        }
                    }

                    // Check if we found a valid concatenation
                    if (count == wordCount) {
                        result.push_back(left);
                        // Slide window by one word
                        string leftWord = s.substr(left, wordLength);
                        seenWords[leftWord]--;
                        count--;
                        left += wordLength;
                    }
                } else {
                    // Current word not in wordMap, reset window
                    seenWords.clear();
                    count = 0;
                    left = i + wordLength;
                }
            }
        }

        return result;
    }
};
// @lc code=end

