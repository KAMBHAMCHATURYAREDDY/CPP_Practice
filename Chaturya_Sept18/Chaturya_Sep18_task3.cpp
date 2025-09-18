#include <iostream>
#include <iomanip>
#include <string>

enum SatelliteStatus { OPERATIONAL, MAINTENANCE, DECOMMISSIONED };

class SatelliteData {
private:
    int satellite_id;
    int num_antennas;
    float* signal_strength;
    float orbital_altitude;
    SatelliteStatus status;
    static int active_satellites;

public:
    SatelliteData(int id, int antennas, const float* signals, float altitude, SatelliteStatus st)
        : satellite_id(id), num_antennas(antennas), orbital_altitude(altitude), status(st) 
    {
        signal_strength = new float[num_antennas];
        for (int i = 0; i < num_antennas; i++) {
            signal_strength[i] = signals[i];
        }
        active_satellites++;
    }

    SatelliteData(const SatelliteData& other)
        : satellite_id(other.satellite_id), num_antennas(other.num_antennas),
          orbital_altitude(other.orbital_altitude), status(other.status) 
    {
        signal_strength = new float[num_antennas];
        for (int i = 0; i < num_antennas; i++) {
            signal_strength[i] = other.signal_strength[i];
        }
        active_satellites++;
    }

    ~SatelliteData() {
        delete[] signal_strength;
        active_satellites--;
    }

    float getAverageSignalStrength() const {
        float sum = 0.0f;
        for (int i = 0; i < num_antennas; i++) {
            sum += signal_strength[i];
        }
        return (num_antennas > 0) ? (sum / num_antennas) : 0.0f;
    }

    void boostSignal(float factor) {
        for (int i = 0; i < num_antennas; i++) {
            signal_strength[i] *= factor;
        }
    }

    void boostSignal(float factor, float threshold) {
        for (int i = 0; i < num_antennas; i++) {
            if (signal_strength[i] < threshold) {
                signal_strength[i] *= factor;
            }
        }
    }

    static int getActiveSatelliteCount() {
        return active_satellites;
    }

    void setStatus(SatelliteStatus new_status) {
        status = new_status;
    }

    std::string getStatusString() const {
        switch (status) {
            case OPERATIONAL: return "OPERATIONAL";
            case MAINTENANCE: return "MAINTENANCE";
            case DECOMMISSIONED: return "DECOMMISSIONED";
            default: return "UNKNOWN";
        }
    }

    friend void printSatelliteData(const SatelliteData& sd);
    friend bool compareAltitude(const SatelliteData& s1, const SatelliteData& s2);
    friend void cloneSatellite(const SatelliteData& source, SatelliteData*& target);
    friend void updateStatusIfWeak(SatelliteData& sd, float confidence_threshold);
};

int SatelliteData::active_satellites = 0;

void printSatelliteData(const SatelliteData& sd) {
    std::cout << "Satellite ID: " << sd.satellite_id << std::endl;
    std::cout << "Orbital Altitude: " << std::fixed << std::setprecision(2) << sd.orbital_altitude << std::endl;
    std::cout << "Signal Strengths: ";
    for (int i = 0; i < sd.num_antennas; i++) {
        std::cout << std::fixed << std::setprecision(2) << sd.signal_strength[i] << " ";
    }
    std::cout << std::endl;
    std::cout << "Status: " << sd.getStatusString() << std::endl;
}

bool compareAltitude(const SatelliteData& s1, const SatelliteData& s2) {
    return s1.orbital_altitude > s2.orbital_altitude;
}

void cloneSatellite(const SatelliteData& source, SatelliteData*& target) {
    target = new SatelliteData(source);
}

void printActiveSatelliteCount() {
    std::cout << "Active Satellites: " << SatelliteData::getActiveSatelliteCount() << std::endl;
}

void updateStatusIfWeak(SatelliteData& sd, float confidence_threshold) {
    if (sd.getAverageSignalStrength() < confidence_threshold) {
        sd.setStatus(MAINTENANCE);
    }
}

int main() {
    float signals1[] = {78.5f, 80.2f, 79.0f};
    float signals2[] = {75.0f, 76.5f};

    SatelliteData sat1(101, 3, signals1, 550.0f, OPERATIONAL);
    SatelliteData sat2(102, 2, signals2, 600.0f, MAINTENANCE);

    sat1.boostSignal(1.05f);
    sat2.boostSignal(1.10f, 77.0f);

    std::cout << "Compare Altitude: ";
    if (compareAltitude(sat1, sat2)) {
        std::cout << "Satellite 1 is higher." << std::endl;
    } else {
        std::cout << "Satellite 2 is higher." << std::endl;
    }

    SatelliteData* satClone = nullptr;
    cloneSatellite(sat1, satClone);

    std::cout << "=== Satellite 1 ===" << std::endl;
    printSatelliteData(sat1);
    std::cout << "=== Satellite 2 ===" << std::endl;
    printSatelliteData(sat2);
    std::cout << "=== Cloned Satellite ===" << std::endl;
    printSatelliteData(*satClone);

    updateStatusIfWeak(sat1, 79.5f);
    updateStatusIfWeak(sat2, 79.5f);

    std::cout << "After Status Update:" << std::endl;
    printSatelliteData(sat1);
    printSatelliteData(sat2);

    printActiveSatelliteCount();

    delete satClone;
    return 0;
}