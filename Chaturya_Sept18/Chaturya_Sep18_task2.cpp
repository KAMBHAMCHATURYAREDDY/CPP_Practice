#include <iostream>
#include <iomanip>

class EgoVehicleData {
private:
    int num_lanes;
    float* lane_positions;
    float* sensor_confidence;

public:
    EgoVehicleData(int lanes, const float* laneData, const float* confidenceData)
        : num_lanes(lanes) 
    {
        lane_positions = new float[num_lanes];
        sensor_confidence = new float[num_lanes];
        for (int i = 0; i < num_lanes; i++) {
            lane_positions[i] = laneData[i];
            sensor_confidence[i] = confidenceData[i];
        }
    }

    ~EgoVehicleData() {
        delete[] lane_positions;
        delete[] sensor_confidence;
    }

    float getAverageLanePosition() const {
        float sum = 0.0f;
        for (int i = 0; i < num_lanes; i++) {
            sum += lane_positions[i];
        }
        return (num_lanes > 0) ? (sum / num_lanes) : 0.0f;
    }

    void updateSensorConfidence(float factor) {
        for (int i = 0; i < num_lanes; i++) {
            sensor_confidence[i] *= factor;
        }
    }

    float totalConfidence() const {
        float sum = 0.0f;
        for (int i = 0; i < num_lanes; i++) {
            sum += sensor_confidence[i];
        }
        return sum;
    }

    friend void printEgoVehicleData(const EgoVehicleData& data);
    friend void findHighestConfidenceVehicle(EgoVehicleData* array, int size, const EgoVehicleData*& highest);
};

void printEgoVehicleData(const EgoVehicleData& data) {
    std::cout << "Lane Positions: ";
    for (int i = 0; i < data.num_lanes; i++) {
        std::cout << std::fixed << std::setprecision(2) << data.lane_positions[i] << " ";
    }
    std::cout << std::endl;
    std::cout << "Sensor Confidence: ";
    for (int j = 0; j < data.num_lanes; j++) {
        std::cout << std::fixed << std::setprecision(2) << data.sensor_confidence[j] << " ";
    }
    std::cout << std::endl;
}

void findHighestConfidenceVehicle(EgoVehicleData* array, int size, const EgoVehicleData*& highest) {
    if (size <= 0) {
        highest = nullptr;
        return;
    }
    highest = &array[0];
    for (int i = 1; i < size; i++) {
        if (array[i].totalConfidence() > highest->totalConfidence()) {
            highest = &array[i];
        }
    }
}

void printHighestConfidenceVehicle(const EgoVehicleData* vehicle) {
    if (vehicle == nullptr) {
        std::cout << "No valid vehicle found." << std::endl;
    } else {
        std::cout << "Highest Confidence Vehicle:" << std::endl;
        printEgoVehicleData(*vehicle);
    }
}

int main() {
    float lanes1[] = {3.2f, 3.0f, 3.4f};
    float conf1[] = {0.95f, 0.97f, 0.93f};
    float lanes2[] = {2.9f, 2.8f, 3.1f};
    float conf2[] = {0.92f, 0.90f, 0.88f};
    float lanes3[] = {3.4f, 3.5f, 3.6f};
    float conf3[] = {0.99f, 0.98f, 0.97f};

    EgoVehicleData* vehicles = new EgoVehicleData[3]{
        EgoVehicleData(3, lanes1, conf1),
        EgoVehicleData(3, lanes2, conf2),
        EgoVehicleData(3, lanes3, conf3)
    };

    vehicles[0].updateSensorConfidence(1.05f);
    vehicles[1].updateSensorConfidence(0.95f);
    vehicles[2].updateSensorConfidence(1.10f);

    const EgoVehicleData* highest = nullptr;
    findHighestConfidenceVehicle(vehicles, 3, highest);
    printHighestConfidenceVehicle(highest);

    delete[] vehicles;
    return 0;
}