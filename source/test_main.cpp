// #include "linear_hash.hpp"
// #include "trie_node.hpp"
// #include "logger.hpp"
// #include <sstream>
// #include <string>
// #include <fstream>
// 
// using namespace std;
// using mstd::logger;
// 
// bool __debug__ = true;
// 
// int main() {
// //     string strs[] = { "hello", "world", "let's", "add", "some", "words", "and", "then", "more", "words", "I", "am", "writing", "a", "small", "story", "because", "i", "need", "as", "many", "words", "as", "iI", "can", "get" };
// //     string strs_bak[] =  { "hello", "world", "let's", "add", "some", "words", "and", "then", "more", "words", "I", "am", "writing", "a", "small", "story", "because", "i", "need", "as", "many", "words", "as", "iI", "can", "get" };
// //     string wrongs[] = { "this", "im", "hoping", "did", "not", "inserted" };
// //     linear_hash lh;
// //     for (string &s : strs) {
// //         lh.insert(s, false);
// //     }
// //     
// //     for (string &s : strs_bak) {
// //         stringstream ss(s);
// //         trie_node *tmp = lh.get(s);
// //         if (tmp == nullptr) {
// //             logger::error("test_main.cpp", s + " was inserted but not found");
// //             exit(-1);
// //         } else {
// //             logger::success("test_main.cpp", s + " was inserted and found");
// //         }
// //     }
// //     
// //     for (string &s : wrongs) {
// //         stringstream ss(s);
// //         trie_node *tmp = lh.get(s);
// //         if (tmp == nullptr) {
// //             logger::success("test_main.cpp", s + " was not inserted and was not found");
// //         } else {
// //             logger::error("test_main.cpp", s + " was not inserted but found");
// //             exit(-1);
// //         }
// //     }
// //     
// // //     lh.print();
// //     
// //     cout << lh.stats() << endl;
//     
//     linear_hash lh;
//     ifstream correct_in("../big_file.correct", ios::in);
//     string s;
//     while (getline(correct_in, s)) {
//         lh.insert(s, false);
//     }
//     
//     correct_in.clear();
//     correct_in.seekg(0, ios::beg);
//     
//     while (getline(correct_in, s)) {
//         trie_node *tmp = lh.get(s);
//         if (tmp == nullptr) {
//             logger::error("test_main.cpp", s + " was inserted but not found");
//         } else {
// //             logger::success("test_main.cpp", s + " was inserted and found");
//         }
//     }
//     
//     cout << lh.stats(false) << endl;
//     
//     
//     return 0;
// }
