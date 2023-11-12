#pragma once

#define NS_PER_SECOND (1000000000)
#define NS_PER_MS (1000000)

class DeltaTime
{
    public:
        DeltaTime(float time = 0.0f) : m_Time(time)
        {

        }

        operator float() const { return m_Time; }

        float GetSeconds() const { return m_Time; }
        float GetMilliseconds() const { return m_Time * 1000.0f;}
    private:
        float m_Time;
};