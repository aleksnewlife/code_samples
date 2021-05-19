

// URL: https://en.wikipedia.org/wiki/Most_vexing_parse


/* The common ways to force the compiler to consider this as a variable definition are:
  1. To add an extra pair of parentheses:
    TimeKeeper time_keeper((Timer()));
  2. To use copy initialization:[1]
    TimeKeeper time_keeper = TimeKeeper(Timer());
     
    (In C++11 and later.) To use uniform initialization[2][3] with braces:
    TimeKeeper time_keeper{Timer()};
    TimeKeeper time_keeper(Timer{});
    TimeKeeper time_keeper{Timer{}};
*/

class Timer {
 public:
	 Timer()
	 {
	 }
};

class TimeKeeper {
 public:
  TimeKeeper(const Timer& t)
  {

  }

  int get_time()
  {
  	return 0;
  }
};

int main() {
	TimeKeeper time_keeper( Timer() );   // Error: Compiler read this as function declaration 
  // TimeKeeper time_keeper{ Timer() };   // Ok, call Copy Constructor of TimeKeeper
  // TimeKeeper time_keeper( (Timer()) );   // Ok, call Copy Constructor of TimeKeeper

	return time_keeper.get_time(); // Error: no member get_time in time_keeper function
}

// https://en.wikipedia.org/wiki/Most_vexing_parse
// is seemingly ambiguous, since it could be interpreted either as

// a variable definition for variable time_keeper of class TimeKeeper, initialized with an anonymous instance of class Timer or
// a function declaration for a function time_keeper which returns an object of type TimeKeeper and has a single (unnamed) parameter which is a pointer to function returning type Timer (and taking no input). (See Function object#In C and C++)
// Most programmers expect the first, but the C++ standard requires it to be interpreted as the second.