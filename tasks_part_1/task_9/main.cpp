#include <iostream>
#include <ctime>
#include <cmath>

int age_in_days()
{
    time_t now = time(0);
    std::tm *pers_timeinfo = std::localtime(&now);

    size_t day, month, year;
    std::cout << "print day: ";
    std::cin >> day;
    std::cout << "print month: ";
    std::cin >> month;
    std::cout << "print year: ";
    std::cin >> year;

    pers_timeinfo->tm_mday = day;
    pers_timeinfo->tm_mon = month - 1;
    pers_timeinfo->tm_year = year - 1900;

    std::time_t pers_time_secs = std::mktime(pers_timeinfo);

    double diff = std::difftime(now, pers_time_secs);
    diff = static_cast<int>(std::floor(diff / 86400));
    std::cout << "age in days: " << diff << std::endl;
    return diff;
}

int count_days()
{
    time_t now = time(0);
    std::tm *timeinfo = std::localtime(&now);
    timeinfo->tm_mday = 1;
    timeinfo->tm_mon = 0;
    timeinfo->tm_year = 1900;
    size_t day_counter = 0;

    for (size_t i = 1970; i < 2000; ++i)
    {
        timeinfo->tm_year = i - 1900;
        std::mktime(timeinfo);
        if (timeinfo->tm_wday == 0)
        {
            day_counter++;
        }
    }

    std::cout << "Sundays in 20th century: " << day_counter << std::endl;
}

int main()
{
    /*
    task 9: age of person in days on current date
    Works for people >= 1970
    */
    age_in_days();

    /*
    task 10: count of years in 20th century when 01.01 is Sunday
    !!! Don't work, because mktime need year >= 1970 !!!
    */
    count_days();
    return 0;
}