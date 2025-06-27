//inputs: month, date, year. Follow the Gregorian calendar. Works from the year 0 and onwards.

//structure: main writes a shell to gather info and validate, then pass it to the calendar function.

#include <chrono>
#include <ctime>
#include <iostream>
#include <sstream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

using namespace std;

bool is_leap_year(const unsigned int);
unsigned int calendar(const unsigned int, const unsigned int, const unsigned int);
unsigned int num_leap_years(const unsigned int end, const unsigned int start = 0);

const unsigned int jan_1_0000 = 6;

const vector<string> day_map = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
const vector<string> month_map = {"January", "February", "March", "April", "May", "June", "July", "August", 
    "September", "October", "November", "December"};

const unordered_map<string, int> month_day_map = {{"January", 0}, {"February", 1}, {"March", 2}, {"April", 3}, {"May", 4},
    {"June", 5}, {"July", 6}, {"August", 7}, {"September", 8}, {"October", 9}, {"November", 10}, {"December", 11}};

const vector<int> month_days = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

std::chrono::duration<double> diff1 = std::chrono::duration<double>::zero();
std::chrono::duration<double> diff2 = diff1;
std::chrono::duration<double> diff3 = diff1;

int main() {
    unsigned int year, month, date;
    string year_str, month_str, date_str;

    while (true) {
        //get year
        cout << "Year: ";
        cin >> year_str;
        try {
            year = stoi(year_str);
        } catch (...) {
            cout << "Invalid year. Starting over." << endl;
            continue;
        }


        //get month
        cout << "Month: ";
        cin >> month_str;
        //validate month
        if (month_day_map.find(month_str) == month_day_map.end()) {
            cout << "Unrecognized month. Starting over." << endl;
            continue;
        }
        //convert month
        month = month_day_map.at(month_str);

        //get date
        cout << "Date: ";
        cin >> date_str;
        try {
            date = stoi(date_str);
        } catch (...) {
            cout << "Invalid date. Starting over." << endl;
            continue;
        }
        //How do we check if the reading is a valid unsigned int?
        //validate date
        if (date > month_days[month] || date < 1) {
            if (month != 1) {
                cout << "Date too high. Starting over." << endl;
                continue;
            } else if (date == 29) {
                if (!is_leap_year(year)) {
                    cout << "It's not a leap year! Starting over." << endl;
                    continue;
                }
            } else {
                cout << "Date too high. Starting over." << endl;
                continue;
            }
        }

        //output their date for their validation
        cout << "Day of the week on " << month_map[month] << " " << date << ", " << year;

        time_t timestamp;
        time(&timestamp);
        tm cd = *localtime(&timestamp);

        tuple<int,int,int> current = make_tuple(cd.tm_year + 1900, cd.tm_mon, cd.tm_mday);
        tuple<int,int,int> user_date = make_tuple(year, month, date);
        if (user_date < current) cout << " was ";
        else if (user_date == current) cout << " is ";
        else cout << " will be ";
        cout << day_map[calendar(year, month, date)] << "." << endl;
        for (int i = 0; i < 1000000; ++i) {
            calendar(year, month, date);
        }
    
        auto millis1 = std::chrono::duration_cast<std::chrono::milliseconds>(diff1).count();
        auto millis2 = std::chrono::duration_cast<std::chrono::milliseconds>(diff2).count();
        auto millis3 = std::chrono::duration_cast<std::chrono::milliseconds>(diff3).count();
        cout << millis1 << " " << millis2 << " " << millis3 << endl << endl;
        diff1 = std::chrono::duration<double>::zero();
        diff2 = diff1;
        diff3 = diff1;
    }

}

unsigned int calendar(const unsigned int year, const unsigned int month, const unsigned int date) {

    
//get jan 1 in the desired year
int num_days_added = 365 * year; //does this fit inside int?
// the number of days we have to add is limited by the size of the year datatype
// so we might have to go step by step. check if we can fit 365 * year inside the largest datatype we have,
// and if not, we go step by step. In this case, pre-calculating values lets us more quickly
// calculate way later years.

int num_leap_year_days = num_leap_years(year); //num_leap_years goes from 0 inclusive to year exclusive.
// we can start from some other year if we want, if we're using some pre-calculated value. check the signature if
// you forget what I mean. if you include start, the function will calculate including start and excluding end.
// num_leap_years counts number of leap days from jan1 of start to jan1 of end. if start == end, then
// num leap days is 0.


num_days_added += num_leap_year_days;

    

int jan_1_year = (jan_1_0000 + num_days_added) % 7;

//now for months.
num_days_added = 0;
for (int i = 0; i < month; i++) {
    num_days_added += month_days[i];
}

num_days_added += (month > 1 && is_leap_year(year)); // if it's past february and we're in a leap year, add a day.

int month_1_year = (jan_1_year + num_days_added) % 7;

    

//now for days.

num_days_added = date - 1;

int month_date_year = (month_1_year + num_days_added) % 7;



return month_date_year;

}


bool is_leap_year(unsigned int year) {

//if not div by 4, definitely not a leap year

//if div by 4 but not by 100, definitely a leap year

//if div by 100 but not 400, def not a leap year

//if div by 400, def a leap year

    if (year % 4) return false;
    if (year % 100) return true;
    if (year % 400) return false;
    return true;

}

unsigned int num_leap_years(const unsigned int end, const unsigned int start) {
const auto point1 = std::chrono::steady_clock::now();
    //start inclusive to end exclusive
    if (start > end) return 0;
    const auto point2 = std::chrono::steady_clock::now();
    int num_leap_years = 0;

    //find first year divisible by 4 at or past the starting year
    int y = start;
    while (y < end && y % 4/*!is_leap_year(y)*/) {
        y++;
    }
    //now we check every 4 years instead of every year
    for (; y < end; y += 4) {
        num_leap_years += is_leap_year(y);
    }
    const auto point3 = std::chrono::steady_clock::now();
    //one improvement is to find the first leap year >= start, then loop by y += 4 instead of ++y
    //another improvement is to sum how many div4 there are, then subtract how many div100,
    // then add how many div400
    diff1 += point2-point1;
    diff2 += point3-point2;

    return num_leap_years;
}