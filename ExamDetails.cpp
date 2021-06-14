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

public:
    ExamDetails(int course_num, int exam_month, int exam_day, double exam_hour, int exam_len, string zoom_link = "");
    ExamDetails(const ExamDetails& examDetails) = default;
    ~ExamDetails() = default;
    
    ExamDetails& operator=(const ExamDetails& exam) = default;

    static ExamDetails makeMatamExam(); //needs to be a reference?
};


ExamDetails::ExamDetails(int course_num, int exam_month, int exam_day, double exam_hour, int exam_len, \
string zoom_link = "") : course_num(course_num), exam_month(exam_month), exam_day(exam_day), exam_hour(exam_hour), \
exam_len(exam_len), zoom_link(zoom_link)
{  }

/* ExamDetails::examDetails(const ExamDetails& exam) : course_num(exam.course_num), \
exam_month(exam.exam_month), exam_day(exam.exam_day), exam_hour(exam.exam_hour), \
exam_len(exam.exam_len), zoom_link(exam.zoom_link) //do i need to manually copy string?
{  } */

//does this need to be friend? 
/* ExamDetails& ExamDetails::operator=(const ExamDetails& examDetails) //do i need to destroy original zoom link?
{
    if (this == &examDetails)
    {
        return *this;
    }
    course_num = examDetails.course_num;
    exam_month = examDetails.exam_month;
    exam_day = examDetails.exam_day;
    exam_hour = examDetails.exam_hour;
    exam_len = examDetails.exam_len;
    zoom_link = examDetails.zoom_link;
    return *this;
} */

static ExamDetails ExamDetails::makeMatamExam()
{
    ExamDetails mtm(234124, 7, 28, 13, 3, "https://tinyurl.com/59hzps6m");
    return mtm;
}