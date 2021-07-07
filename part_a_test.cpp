//
// Created by ittay on 3/9/2021.
//
#include <iostream>
#include "sortedList.h"
#include "examDetails.h"

using std::cout;
using std::endl;
using std::string;
using namespace mtm;

#define TEST(num) cout << endl << "TEST " << (num) << endl;

string getLen(string str)
{
    return std::to_string(str.length());
}

bool isTrollLink(const ExamDetails& exam) {
    return (exam.getLink().find("tinyurl") != string::npos);
}

template<class T>
void printList(SortedList<T> list) {
    
    for (auto it = list.begin(); !(it == list.end()); ++it) {
        cout << *it << endl;
    }
    cout << endl;
}

int main()
{
    
    TEST("1.1")
    ExamDetails exam1 = ExamDetails::makeMatamExam();
    ExamDetails exam2(104032, 7, 11, 9.5, 3);
    ExamDetails exam3 = exam1; 
    cout << "the difference between MATAM and infi 2m is " << (exam1-exam2) << " days";

    TEST("1.2")
    ExamDetails& closest = (exam1 < exam2) ? exam1 : exam2;
    cout << "your closest exam is:" << endl << closest << endl;

    TEST("1.3")
    closest.setLink("https://tinyurl.com/ym8wf46t");
    cout << closest << endl;

    TEST("1.4")
    try {
        ExamDetails exam4(236506, 42, 0, 13, 3, "https://tinyurl.com/ym8wf46t");
        cout << exam4 << endl;
    }
    catch (ExamDetails::InvalidDateException& e) {
        cout << "invalid date" << endl;
    }
    
   
    TEST("1.5")
    SortedList<string> lst1 = SortedList<string>();
    lst1.insert("Charlie");
    lst1.insert("Bob");
    lst1.insert("Alice");
    lst1.insert("Donald");
    
    printList(lst1);
   
    TEST("1.6")
    SortedList<ExamDetails> lst2;
    lst2.insert(exam1);
    lst2.insert(exam2);

    printList(lst2);

    TEST("1.7")
    SortedList<string> lst3 = lst1;
    printList(lst3);

    TEST("1.8")
    lst3 = lst3.apply(getLen);
    printList(lst3);


    TEST("1.9")
    lst3.remove(lst3.begin());
    printList(lst3);
    
    TEST("1.10")
    SortedList<ExamDetails> lst4 = lst2.filter(isTrollLink);
    printList(lst2);
    cout << "----------" << endl;
    printList(lst4); 
    return 0;
}
