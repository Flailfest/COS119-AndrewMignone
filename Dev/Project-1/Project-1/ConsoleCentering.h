#pragma once

class CenteredStreamBuffer;

// Centers every line written through std::cout for the duration of this object's lifetime.
// Create one instance at the start of main().
class ConsoleCentering
{
public:
    ConsoleCentering();
    ~ConsoleCentering();

    ConsoleCentering(const ConsoleCentering&) = delete;
    ConsoleCentering& operator=(const ConsoleCentering&) = delete;

private:
    class Impl;
    Impl* impl_;
};
