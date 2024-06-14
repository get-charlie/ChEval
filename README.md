# ChEval
A simple stb-style header only library providing an evaluation fuction.

## Features
This function takes an expression as a string and returns the result as a double.
It suppors addition, substraction, multiplication and division.
Expressions can include parenthesis and they will be evaluated appropriately.
If an invalid expression is provided or an illegal opperation is found the function
will return NAN.
## How to use it in your project?
To use this library define `IMPLEMENT_CHEVAL` and then include `cheval.h`.
Then you can use the function at your convenience.
A simple terminal calculator example is included for reference.


