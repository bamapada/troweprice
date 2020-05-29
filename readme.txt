Problem Statement
     Create one word processor, which could reply below queries based on 
     given input, one or multiple sentences without line feed. 
     1. Longest word in the sentence
     2. Shortest word in the sentence
     3. Total number of sentences

Assumption
     1. Sentence, collection of words will end up with one period char or dot, 
        it may have one or more white space before or after and to start another
     2. New line feed will be treated as end of input.
     3. Any input will treated as invalid when it does constains nothing or 
        contains only one or multiple whitespaces and one dot
     4. A word will be tread as sequence os [A-za-z0-9!@#$%^&*()_-+=]{n} plus zero or more 
        whitespaces where n >= 1

Author: Bamapada Chakrabortty

Language: C++
Platfor/Compiler: Linux/g++
C++ Standard: -std=C++14 
Compile in command line: g++ -std=C++14 -o wrdproc wrdproc.cpp


Below test class test the WordProc unctionality
Working procedure
How to provide input in console?
1. Insert {sentence_1, sentence_2, ... sentence_n} without line feed
2. Enter single integer, represent number of input test cases
3. <Query Type> <nth-Sentence> <expected result>
   <Query Type> <nth-Sentence> <expected result>
   .
   .
   .

   Available query Types are: Longest word = 1, Shortest Word = 2, Sentence count = 3
   nth-Sentence: 1,2,3 ... n, in case more than one sentence available
   Expected result: integer
   Note: For query type sentence count (3) , we just provide 1 as we only deals with single paragraph.
Sample Input test case:

The cow jumped over the moon.
3
1 1 6
2 1 3
3 1 1

I have added one sample test data file and can execute like below
cat input01.dat | ./wordproc

Below is the sample result of the above test case
===================================================================
Longest Word: Sentence:1, Expected/Actual:6/6, Status: passed
Shortest Word: Sentence:1, Expected/Actual: 3/3, Status:passed
Sentence Count: Expected/Actual:1/1, Status: passed

Summary: 3 test cases executed, pass=3, failed=0
===================================================================
