#include <Utils/random.hpp>

#include <Time/TimeUtils.hpp>

// Call this once at startup (e.g., in your main function before the while loop)
static void init_random() {
    // Use the RTC or another variable source for a better seed than 0
    // If you don't have an RTC, the sequence will be the same every power-up
    static bool inited = false;
    if (!inited)
    {
        srand(TimeUtils::GetCurrentMks());
        inited = true;
    }
}

// Call this function to get a random number [0.0 to 1.0]
float RandomFloat() {
    init_random();
    return (float) rand() / (float)RAND_MAX;
}
