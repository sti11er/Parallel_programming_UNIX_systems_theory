#include <stdio.h>

int
time_hours(int seconds)
{
    return (
        seconds / 3600
    );
}

int
time_minutes(int seconds)
{
    return (
        (seconds % 3600) / 60
    );
}

int
time_seconds(int seconds)
{
    return (
        seconds - ( ((seconds / 3600)*3600) + ((seconds % 3600)/60)*60 )
    );
}

int
main(void)
{
    static int tests[][4] = {
        { 0, 0, 0, 0 },
        { 3723, 1, 2, 3 },
        { 86399, 23, 59, 59 },
        { 36000, 10, 0, 0 }
    };

    for (size_t i = 0; i < sizeof tests / sizeof *tests; ++i) {
        int hour = time_hours(tests[i][0]);
        if (hour != tests[i][1]) {
            printf("WRONG HOUR for SECONDS = %d: expected %d, gotten %d\n",
                    tests[i][0], tests[i][1], hour);
            return 0;
        }

        int minute = time_minutes(tests[i][0]);
        if (minute != tests[i][2]) {
            printf("WRONG MINUTE for SECONDS = %d: expected %d, gotten %d\n",
                    tests[i][0], tests[i][2], minute);
            return 0;
        }

        int second = time_seconds(tests[i][0]);
        if (second != tests[i][3]) {
            printf("WRONG SECOND for SECONDS = %d: expected %d, gotten %d\n",
                    tests[i][0], tests[i][3], second);
            return 0;
        }
    }

    printf("OK\n");
}
