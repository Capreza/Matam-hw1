#include <iostream>
using std::string;

class ExamDetails
{

private:
    int course_num;
    int exam_month;
    int exam_day;
    double exam_hour;
    int exam_len;
    string zoom_link;
    static const int days_in_month = 30;
    static const int half_hour = 3;
    static constexpr double epsilon = 0.000001;
    static const int first_month = 1;
    static const int last_month = 12;
    static constexpr double half = 0.5;

public:
    ExamDetails(int course_num, int exam_month, int exam_day, double exam_hour, int exam_len, string zoom_link = "");
    ExamDetails(const ExamDetails& examDetails) = default;
    ~ExamDetails() = default;
    string getLink() const;
    void setLink(const string& new_link);
    
    bool operator<(const ExamDetails& exam) const;
    int operator-(const ExamDetails& exam) const;
    ExamDetails& operator=(const ExamDetails& exam) = default;

    class InvalidDateException {};
    class InvalidTimeException {};
    class InvalidArgsException {};

    static ExamDetails makeMatamExam(); //needs to be a reference?

    friend std::ostream& operator<<(std::ostream&, const ExamDetails&);
};