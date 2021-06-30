#include "ExamDetails.h"
using std::cout;

namespace mtm
{
    ExamDetails::ExamDetails(int course_num, int exam_month, int exam_day, double exam_hour, int exam_len, \
    string zoom_link) : course_num(course_num), exam_month(exam_month), exam_day(exam_day), exam_hour(exam_hour), \
    exam_len(exam_len), zoom_link(zoom_link)
    {  
        if (exam_month < first_month || exam_month > last_month || exam_day <= 0 || exam_day > days_in_month)
        {
            throw InvalidDateException();
        }
        
        int exam_time_round_hour = (int) exam_hour;
        float exam_time_half_hour = exam_hour - exam_time_round_hour;
        if (!(exam_time_half_hour > (-1 * epsilon) && exam_time_half_hour < epsilon) && \
        !(exam_time_half_hour > half - epsilon && exam_time_half_hour < half + epsilon))
        {
            throw InvalidTimeException();
        }
        if (exam_len < 0)
        {
            throw InvalidArgsException();
        }

    }

    string ExamDetails::getLink() const
    {
        return zoom_link;
    }

    void ExamDetails::setLink(const string& new_link)
    {
        string copy_link = new_link; //should we copy?
        zoom_link = copy_link;
    }


    int ExamDetails::operator-(const ExamDetails& exam) const 
    {
        int diff_in_days = 0;
        diff_in_days += exam_day - exam.exam_day;
        diff_in_days += days_in_month*(exam_month - exam.exam_month) ;
        return diff_in_days;
    }

    bool ExamDetails::operator<(const ExamDetails& exam) const
    {
        if(exam_month<exam.exam_month)
        {
            return true;
        }
        else if(exam_month==exam.exam_month)
        {
            if(exam_day<exam.exam_day)
            {
                return true;
            }
            else if(exam_day==exam.exam_day)
            {
                if((exam_hour-exam.exam_hour)<epsilon) 
                {
                    return false;
                }
                else
                {
                    return false;
                }
            }
            else
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }

    std::ostream& operator<<(std::ostream& os, const ExamDetails& exam) 
    {
        int exam_time_round_hour = (int) exam.exam_hour;
        float exam_time_half_hour = exam.exam_hour - exam_time_round_hour;
        int exam_time_half_hour_to_print=0;
        if(exam_time_half_hour>ExamDetails::epsilon) 
        {
            exam_time_half_hour_to_print=ExamDetails::half_hour;
        }
        return os <<"Course Number: "<<exam.course_num<< std::endl<<"Time: "<<exam.exam_day<<"."<<exam.exam_month<<" "<<
        exam_time_round_hour<<":"<<exam_time_half_hour_to_print<<"0"<<std::endl
        <<"Duration: "<<exam.exam_len<<":00"<<std::endl<<"Zoom Link: "<<exam.zoom_link  ;
    }

    ExamDetails ExamDetails::makeMatamExam()
    {
        ExamDetails mtm(234124, 7, 28, 13, 3, "https://tinyurl.com/59hzps6m");
        return mtm;
    }
}