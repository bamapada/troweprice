// Problem Statement
//      Create one word processor, which could reply below queries based on 
//      given input, one or multiple sentences without line feed. 
//      1. Longest word in the sentence
//      2. Shortest word in the sentence
//      3. Total number of sentences
//
// Assumption
//      1. Sentence, collection of words will end up with one period char or dot, 
//         it may have one or more white space before or after and to start another
//      2. New line feed will be treated as end of input.
//      3. Any input will treated as invalid when it does constains nothing or 
//         contains only one or multiple whitespaces and one dot
//      4. A word will be tread as sequence os [A-za-z0-9!@#$%^&*()_-+=]{n} plus zero or more 
//         whitespaces where n >= 1
// 
// Author: Bamapada Chakrabortty
//
// Language: C++
// Platfor/Compiler: Linux/g++
// C++ Standard: -std=C++14 
// Compile in command line: g++ -std=C++14 -o wrdproc wrdproc.cpp
// 
#include <iostream>
#include <cstring>
#include <vector>
#include <sstream>
#include <string>

using namespace std;

class WordProc;
class sent_t;

// Construct a word as per given input specification and 
// holds the word property like its length.
class word_t {
    word_t(const char* str, size_t len) : wrd(nullptr), next(nullptr), wlen(len) {
        if(len > 0) {
            wrd = new char[len+1]();
            strncpy(wrd, str, len);
        }
    }
    char *wrd;
    size_t wlen;
    word_t *next;
    friend class sent_t;
    friend class WordProc;
};

// Construct one empty sentence to hold the collection of words
// and also the sentence property like total number of words in the
// sentence of first min/max length of the words in the sentence
class sent_t {
    sent_t()
        : lpHead(nullptr), lpTail(nullptr), nWords(0), next(nullptr)
        {}
    // Add new word in the current sentence
    void AddWord(const char* str, size_t len) {
        if(nullptr != str && len > 0) {
            word_t *lpWord = new word_t(str, len);
            if(nullptr == this->lpHead) {
                this->lpHead = this->lpTail = lpWord;
                this->lpMinWord = this->lpMaxWord = lpWord;
            }
            else {
                this->lpTail->next = lpWord;
                this->lpTail = this->lpTail->next;
                if(lpMinWord->wlen > len)
                    lpMinWord = this->lpTail;
                if(lpMaxWord->wlen < len)
                    lpMaxWord = this->lpTail;
            }
        }
    }
    // Data
    word_t *lpHead;
    word_t *lpTail;
    word_t *lpMinWord;
    word_t *lpMaxWord;
    int nWords;
    sent_t *next;
    friend class WordProc;
};

class WordProc {
    public:
        // d-ctor
        WordProc() 
        : lpParagraph(nullptr), lpTail(nullptr), nSents(0)
        {}
        // d-dtor
        ~WordProc() {

        }

        // Parse one given input and create the paragraph for further processing
        void Parse(char* text) {
            const char nulch = '\0';
            const char lf = '\n';       // Line feed for end of paragraph if exist
            const char ws = ' ';        // Whitespace
            const char st = '.';        // Sentence terminator
            //
            // Handle corner cases first
            // 1. Should not be null as input
            // 2. Enevn though not null should not start with 
            //    a) string termination char.
            //    b) Not even newline which would be treated as start of paragraph as we 
            //       decided to proceed with only one paragraph.
            if((nullptr == text) || (nullptr != text && (nulch == *text || lf == *text)))
                return;
            // 
            // Remove all whitespaces if any at the begining of the first sentence of the paragraph.
            char *start, *ptrch = text;
            size_t len;
            while(ws == *ptrch) ++ptrch;
            // we are ready and safe to parse the text and before that create the paragraph object
            sent_t* lpCurSent = this->AddSentence();
            start = ptrch;
            while(nulch != *ptrch) {
                if(ws == *ptrch) {
                    lpCurSent->AddWord(start, (size_t)(ptrch - start));
                    // If there exists multiple spaces between words, remove them first
                    while(ws == *ptrch) ++ptrch;
                    start = ptrch;
                }
                else if(st == *ptrch) {
                    // Add the trailing word first
                    lpCurSent->AddWord(start, (size_t)(ptrch - start));
                    // Should have one or more whitespaces between previous and next sentence
                    while(st == *ptrch || ws == *ptrch) ++ptrch;
                    if(nulch != *ptrch && lf != *ptrch)
                        lpCurSent = this->AddSentence();
                    start = ptrch;
                }
                else if(lf == *ptrch) {
                    while(lf == *ptrch || ws == *ptrch) 
                        ++ptrch;
                    start = ptrch;
                }
                else {
                    ++ptrch;
                }
            }
        }
        //
        // Returns the longest word of a nth sentence in the paragraph
        // Param: nthSent : int [accept natural number 1,2,3, ... n]
        // 
        int LongestWord(int nthSent) {
            int length = 0;
            if(nthSent > 0 && nthSent <= this->nSents) {
                int pos = 0;
                for(sent_t *lpSent = this->lpParagraph; nullptr != lpSent; lpSent = lpSent->next, ++pos) {
                    if(pos == nthSent - 1 && nullptr != lpSent->lpMaxWord) {
                        length = lpSent->lpMaxWord->wlen;
                        break;
                    }
                }
            }
            return length;
        }
        //
        // Returns the shortest word of a nth sentence in the paragraph
        // Param: nthSent : int [accept natural number 1,2,3, ... n]
        // 
        int ShortestWord(int nthSent) {
            int length = 0;
            if(nthSent > 0 && nthSent <= this->nSents) {
                int pos = 0;
                for(sent_t *lpSent = this->lpParagraph; nullptr != lpSent; lpSent = lpSent->next, ++pos) {
                    if(pos == nthSent - 1 && nullptr != lpSent->lpMinWord) {
                        length = lpSent->lpMinWord->wlen;
                        break;
                    }
                }
            }
            return length;
        }
        //
        // Returns total number of sentence in the paragraph
        // 
        int GetSentenceCount() {
            return this->nSents;
        }

    private:
        // Add new sentences to create paragraph
        sent_t* AddSentence() {
            sent_t *lpSent =  new sent_t();
            if(nullptr == this->lpParagraph) {
                this->lpParagraph = this->lpTail = lpSent;
            } else {
                this->lpTail->next = lpSent;
                this->lpTail = this->lpTail->next;
            }
            ++(this->nSents);
            return lpSent;
        }

    private:
        sent_t *lpParagraph;
        sent_t *lpTail;
        int nSents;
};

// Below test class test the WordProc unctionality
// Working procedure
// How to provide input in console?
// 1. Insert {sentence_1, sentence_2, ... sentence_n} without line feed
// 2. Enter single integer, represent number of input test cases
// 3. <Query Type> <nth-Sentence> <expected result>
//    <Query Type> <nth-Sentence> <expected result>
//    .
//    .
//    .
//
//    Available query Types are: Longest word = 1, Shortest Word = 2, Sentence count = 3
//    nth-Sentence: 1,2,3 ... n, in case more than one sentence available
//    Expected result: integer
//    Note: For query type sentence count (3) , we just provide 1 as we only deals with single paragraph.
// Sample Input test case:
//
// The cow jumped over the moon.
// 3
// 1 1 6
// 2 1 3
// 3 1 1
//
typedef struct {
    string strTestDesc;
    int type;
    vector<int> query;
    bool bPass;
} WrdPrcResult;

class WordProcTest {
    public:
        WordProcTest() : pass(0), failed(0) {}

        void ReadInput() {
            int max_lnq;
            string line;

            // Accept input from the console
            std::getline(std::cin, line);
            wp.Parse(const_cast<char*>(line.c_str()));
            std::cin >> max_lnq;
            int a, b;
            while(max_lnq--) {
                WrdPrcResult wpr;
                std::cin >> wpr.type >> a >> b;
                wpr.query.push_back(a);
                wpr.query.push_back(b);
                test_cases.push_back(wpr);
            }
        }

        void Execute() {
            int rv;
            stringstream ss;
            for(auto& wpr : test_cases) {
                switch(wpr.type) {
                    case 1:                        
                        rv = wp.LongestWord(wpr.query[0]);
                        wpr.bPass = (rv == wpr.query[1])? true : false;
                        ss = std::stringstream();
                        ss << "Longest Word: Sentence:" << wpr.query[0] 
                        << ", Expected/Actual:" << wpr.query[1] << "/" << rv 
                        << ", Status: " << ((wpr.bPass)? "passed" : "failed");
                        wpr.strTestDesc = ss.str();
                        (wpr.bPass)? ++pass : ++ failed;
                        break;
                    case 2:
                        rv = wp.ShortestWord(wpr.query[0]);
                        wpr.bPass = (rv == wpr.query[1])? true : false;
                        ss = std::stringstream();
                        ss << "Shortest Word: Sentence:" << wpr.query[0] 
                        << ", Expected/Actual: " << wpr.query[1] << "/" << rv 
                        << ", Status:" << ((wpr.bPass)? "passed" : "failed");
                        wpr.strTestDesc = ss.str(); 
                        (wpr.bPass)? ++pass : ++ failed;
                        break;
                    case 3:
                        rv = wp.GetSentenceCount();
                        wpr.bPass = (rv == wpr.query[1])? true : false;
                        ss = std::stringstream();
                        ss << "Sentence Count: Expected/Actual:" << wpr.query[1] << "/" << rv 
                        << ", Status: " << ((wpr.bPass)? "passed" : "failed");
                        wpr.strTestDesc = ss.str();
                        (wpr.bPass)? ++pass : ++ failed;
                        break;
                    default:
                    break;
                }
            }
        }

        void Summary() {
            for(auto& wpr : test_cases)
                std::cout << wpr.strTestDesc << std::endl;
            std::cout << '\n' << "Summary: " 
            << test_cases.size() << " test cases executed, pass=" 
            << pass << ", failed=" << failed << std::endl;            
        }

    private:
    WordProc wp;
    int pass, failed;
    vector<WrdPrcResult> test_cases;
};

int main() {
    WordProcTest wpt;
    wpt.ReadInput();
    wpt.Execute();
    wpt.Summary();
    return 0;
}