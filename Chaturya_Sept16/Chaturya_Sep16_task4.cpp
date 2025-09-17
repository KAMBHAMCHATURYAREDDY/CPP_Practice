#include <iostream>

class RadarSignal {
private:
    int num_channels;
    float* signal_strength;
public:
    RadarSignal(int num, float* signals) {
        num_channels = num;
        signal_strength = new float[num_channels];
        for(int i=0; i<num_channels; i++) {
            signal_strength[i] = signals[i];
        }
    }
    ~RadarSignal() {
        delete[] signal_strength;
    }
    float averageSignal() const {
        float sum = 0.0f;
        for(int i=0; i<num_channels; i++) {
            sum += signal_strength[i];
        }
        return sum / num_channels;
    }
    void boostSignalByValue(RadarSignal obj) {
        for(int i=0; i<num_channels; i++) {
            signal_strength[i] = obj.signal_strength[i] + 5.0f;
        }
    }
    void boostSignalByReference(RadarSignal& obj) {
        for(int i=0; i<num_channels; i++) {
            signal_strength[i] = obj.signal_strength[i] + 5.0f;
        }
    }
    int getNumChannels() const { return num_channels; }
    float getSignal(int index) const { return signal_strength[index]; }
};

void printRadarSignal(const RadarSignal& radar) {
    for(int i=0; i<radar.getNumChannels(); i++) {
        std::cout << "Channel " << i << " Strength: " << radar.getSignal(i) << std::endl;
    }
}

RadarSignal* createRadarSignalHeap(int num, float* signals) {
    return new RadarSignal(num, signals);
}

int main() {
    std::cout << "Scenario: Radar Signal Strength Management in ADAS" << std::endl;
    float signals[4] = {55.5f, 48.2f, 60.1f, 52.6f};
    RadarSignal* radar = createRadarSignalHeap(4, signals);

    std::cout << "Initial Radar Signal Strengths:" << std::endl;
    printRadarSignal(*radar);

    radar->boostSignalByValue(*radar);
    std::cout << "Radar Signal Strengths After Boosting by Value (temporary copy):" << std::endl;
    printRadarSignal(*radar);

    radar->boostSignalByReference(*radar);
    std::cout << "Radar Signal Strengths After Boosting by Reference (direct update):" << std::endl;
    printRadarSignal(*radar);

    std::cout << "Final Average Signal Strength: " << radar->averageSignal() << std::endl;

    delete radar;
    return 0;
}