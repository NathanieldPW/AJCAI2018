#pragma once

class Counter
{
    public:
        Counter(int initialValue, bool infinite = true, int limit = 0,
                int extension = 0);
        
        int get_limit();
        int get_value();

        bool is_infinite();
        
        void inc();
        
        bool limit_reached();
        void extend_limit();
    private:
        int value;
        int limit;
        int extension;
        bool infinite;
};
