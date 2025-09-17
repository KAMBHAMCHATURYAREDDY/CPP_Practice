#include <cstdint>
#include <iostream>
#include <iomanip>
#include <limits>
#include <new>

enum class SensorType
{
    LIDAR = 0,
    RADAR = 1,
    CAMERA = 2
};

const char* sensorTypeToString(SensorType t);

class SensorArray
{
public:
    int sensor_id;
    SensorType type;
    double* temperature_readings;
    int num_readings;
    static double global_max_temperature;
    SensorArray(int init_sensor_id, SensorType init_type, const double* readings, int reading_count)
        : sensor_id(init_sensor_id), type(init_type), temperature_readings(nullptr), num_readings(reading_count)
    {
        if (num_readings > 0)
        {
            temperature_readings = new double[num_readings];
            for (int i = 0; i < num_readings; ++i)
            {
                temperature_readings[i] = readings[i];
                if (temperature_readings[i] > SensorArray::global_max_temperature)
                {
                    SensorArray::global_max_temperature = temperature_readings[i];
                }
            }
        }
        else
        {
            temperature_readings = nullptr;
            num_readings = 0;
        }
    }
    ~SensorArray()
    {
        if (temperature_readings != nullptr)
        {
            delete [] temperature_readings;
            temperature_readings = nullptr;
        }
        num_readings = 0;
    }
    double getMaxTemperature() const
    {
        if (temperature_readings == nullptr || num_readings == 0)
        {
            return -std::numeric_limits<double>::infinity();
        }
        double maxVal = temperature_readings[0];
        for (int i = 1; i < num_readings; ++i)
        {
            if (temperature_readings[i] > maxVal)
            {
                maxVal = temperature_readings[i];
            }
        }
        return maxVal;
    }
    void printSensorInfo() const
    {
        double maxTemp = getMaxTemperature();
        std::cout << "Sensor ID: " << sensor_id << ", Type: " << sensorTypeToString(type) << ", MaxTemp: ";
        if (maxTemp == -std::numeric_limits<double>::infinity())
        {
            std::cout << "N/A";
        }
        else
        {
            std::cout << std::fixed << std::setprecision(1) << maxTemp;
        }
        std::cout << std::endl;
    }
    static double getGlobalMaxTemperature()
    {
        return global_max_temperature;
    }
    SensorArray(const SensorArray&) = delete;
    SensorArray& operator=(const SensorArray&) = delete;
    friend const char* sensorTypeToString(SensorType t);
    friend void printSensor(const SensorArray& s);
    friend void printAllSensors(const SensorArray* arr, int size);
};

double SensorArray::global_max_temperature = -std::numeric_limits<double>::infinity();

const char* sensorTypeToString(SensorType t)
{
    switch (t)
    {
        case SensorType::LIDAR: return "LIDAR";
        case SensorType::RADAR: return "RADAR";
        case SensorType::CAMERA: return "CAMERA";
        default: return "UNKNOWN";
    }
}

void printSensor(const SensorArray& s)
{
    s.printSensorInfo();
}

void printAllSensors(const SensorArray* arr, int size)
{
    if (arr == nullptr || size <= 0)
    {
        std::cout << "No sensors to print." << std::endl;
        return;
    }
    for (int i = 0; i < size; ++i)
    {
        printSensor(arr[i]);
    }
}

int main()
{
    const int sensorCount = 3;
    const double readings801[] = {35.5, 36.1, 34.9};
    const double readings802[] = {39.0, 38.7, 39.3};
    const double readings803[] = {30.2, 31.0, 30.5};
    void* raw = operator new[](sizeof(SensorArray) * static_cast<std::size_t>(sensorCount));
    SensorArray* sensorArr = static_cast<SensorArray*>(raw);
    new (&sensorArr[0]) SensorArray(801, SensorType::LIDAR, readings801, 3);
    new (&sensorArr[1]) SensorArray(802, SensorType::RADAR, readings802, 3);
    new (&sensorArr[2]) SensorArray(803, SensorType::CAMERA, readings803, 3);
    for (int i = 0; i < sensorCount; ++i)
    {
        printSensor(sensorArr[i]);
    }
    double globalMax = SensorArray::getGlobalMaxTemperature();
    if (globalMax == -std::numeric_limits<double>::infinity())
    {
        std::cout << "Global Max Temperature: N/A" << std::endl;
    }
    else
    {
        std::cout << "Global Max Temperature: " << std::fixed << std::setprecision(1) << globalMax << std::endl;
    }
    for (int i = 0; i < sensorCount; ++i)
    {
        sensorArr[i].~SensorArray();
    }
    operator delete[](raw);
    raw = nullptr;
    sensorArr = nullptr;
    return 0;
}
