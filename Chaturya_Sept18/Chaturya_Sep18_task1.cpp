#include <iostream>
#include <iomanip>
#include <string>

class VehicleStatus {
private:
    int num_wheels;
    float* wheel_speed;
    int num_temps;
    float* engine_temp;

public:
    VehicleStatus(int wheels, const float* wheelData, int temps, const float* tempData)
        : num_wheels(wheels), num_temps(temps) 
    {
        wheel_speed = new float[num_wheels];
        engine_temp = new float[num_temps];
        for (int i = 0; i < num_wheels; i++) {
            wheel_speed[i] = wheelData[i];
        }
        for (int j = 0; j < num_temps; j++) {
            engine_temp[j] = tempData[j];
        }
    }

    ~VehicleStatus() {
        delete[] wheel_speed;
        delete[] engine_temp;
    }

    float averageWheelSpeed() const {
        float sum = 0.0f;
        for (int i = 0; i < num_wheels; i++) {
            sum += wheel_speed[i];
        }
        return (num_wheels > 0) ? (sum / num_wheels) : 0.0f;
    }

    float maxEngineTemp() const {
        if (num_temps == 0) return 0.0f;
        float maxVal = engine_temp[0];
        for (int i = 1; i < num_temps; i++) {
            if (engine_temp[i] > maxVal) {
                maxVal = engine_temp[i];
            }
        }
        return maxVal;
    }

    bool isWheelSpeedHigher(const VehicleStatus& other) const {
        float maxThis = wheel_speed[0];
        for (int i = 1; i < num_wheels; i++) {
            if (wheel_speed[i] > maxThis) {
                maxThis = wheel_speed[i];
            }
        }
        float maxOther = other.wheel_speed[0];
        for (int j = 1; j < other.num_wheels; j++) {
            if (other.wheel_speed[j] > maxOther) {
                maxOther = other.wheel_speed[j];
            }
        }
        return maxThis > maxOther;
    }

    friend void printVehicleStatus(const VehicleStatus& vs);
    friend bool compareWheelSpeedGlobal(const VehicleStatus& vs1, const VehicleStatus& vs2);
};

void printVehicleStatus(const VehicleStatus& vs) {
    std::cout << "Vehicle Status:" << std::endl;
    std::cout << "  Wheel Speeds: ";
    for (int i = 0; i < vs.num_wheels; i++) {
        std::cout << std::fixed << std::setprecision(2) << vs.wheel_speed[i] << " ";
    }
    std::cout << std::endl;
    std::cout << "  Engine Temps: ";
    for (int j = 0; j < vs.num_temps; j++) {
        std::cout << std::fixed << std::setprecision(2) << vs.engine_temp[j] << " ";
    }
    std::cout << std::endl;
}

bool compareWheelSpeedGlobal(const VehicleStatus& vs1, const VehicleStatus& vs2) {
    return vs1.isWheelSpeedHigher(vs2);
}

int main() {
    float wheel1[] = {55.5f, 56.6f, 57.2f, 55.9f};
    float temp1[] = {90.5f, 88.9f};
    float wheel2[] = {50.0f, 51.2f, 49.8f, 50.4f};
    float temp2[] = {92.0f, 89.5f};

    VehicleStatus vehicle1(4, wheel1, 2, temp1);
    VehicleStatus vehicle2(4, wheel2, 2, temp2);

    std::cout << "=== Vehicle 1 ===" << std::endl;
    printVehicleStatus(vehicle1);

    std::cout << "=== Vehicle 2 ===" << std::endl;
    printVehicleStatus(vehicle2);

    std::cout << "Comparison using member function: ";
    if (vehicle1.isWheelSpeedHigher(vehicle2)) {
        std::cout << "Vehicle 1 has higher max wheel speed." << std::endl;
    } else {
        std::cout << "Vehicle 2 has higher or equal max wheel speed." << std::endl;
    }

    std::cout << "Comparison using global function: ";
    if (compareWheelSpeedGlobal(vehicle1, vehicle2)) {
        std::cout << "Vehicle 1 has higher max wheel speed." << std::endl;
    } else {
        std::cout << "Vehicle 2 has higher or equal max wheel speed." << std::endl;
    }

    return 0;
}