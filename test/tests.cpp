#include <tut/tut.hpp>
#include <tut/tut_reporter.hpp>
#include <iostream>
#include <cstdlib>

using std::exception;
using std::string;
using std::cout;
using std::cerr;
using std::endl;

using tut::reporter;
using tut::groupnames;

namespace tut
{
test_runner_singleton runner;
}

int main(int argc, const char* argv[])
{
    reporter visi;

    if (argc < 2 || argc > 3)
    {
        cout << "Usage: tests [regression] | [list] | [ group] [test]" << endl;
        cout << "       List all groups: tests list" << endl;
        cout << "       Run all tests: tests regression" << endl;
        cout << "       Run one group: tests std::auto_ptr" << endl;
        cout << "       Run one test: tests std::auto_ptr 3" << endl;
    }


    tut::runner.get().set_callback(&visi);

    try
    {
        if (argc == 1 || (argc == 2 && string(argv[1]) == "regression"))
        {
            tut::runner.get().run_tests();
        }
        else if (argc == 2 && string(argv[1]) == "list")
        {
            cout << "registered test groups:" << endl;
            groupnames gl = tut::runner.get().list_groups();
            groupnames::const_iterator i = gl.begin();
            groupnames::const_iterator e = gl.end();
            while(i != e)
            {
                cout << "  " << *i << endl;
                ++i;
            }
        }
        else if (argc == 2 && string(argv[1]) != "regression")
        {
            tut::runner.get().run_tests(argv[1]);
        }
        else if (argc == 3)
        {
            tut::runner.get().run_test(argv[1],::atoi(argv[2]));
        }
    }
    catch (const exception& ex)
    {
        cerr << "tut raised ex: " << ex.what() << endl;
    }

    return 0;
}
