#include <iostream>
#include <iomanip>
#include <cstdint>

typedef uint32_t uint32;

class DetectedObject
{
public:
    uint32 object_id;
    float rel_speed;
    float distance;

    DetectedObject(uint32 id = 0u, float spd = 0.0f, float dist = 0.0f)
        : object_id(id), rel_speed(spd), distance(dist) {}

    bool isHigherRisk(const DetectedObject& other) const
    {
        return (this->distance < other.distance) && (this->rel_speed > other.rel_speed);
    }

    void updateValuesByValue(uint32 id, float spd, float dist)
    {
        this->object_id = id;
        this->rel_speed = spd;
        this->distance = dist;
    }

    void updateValuesByReference(uint32 id, float spd, float dist)
    {
        this->object_id = id;
        this->rel_speed = spd;
        this->distance = dist;
    }
};

void updateObjectValuesByValueGlobal(DetectedObject obj, uint32 id, float spd, float dist)
{
    obj.updateValuesByValue(id, spd, dist);
}

void updateObjectValuesByReferenceGlobal(DetectedObject& obj, uint32 id, float spd, float dist)
{
    obj.updateValuesByReference(id, spd, dist);
}

void findHighestRiskObject(DetectedObject* arr, uint32 size, const DetectedObject*& riskObj)
{
    riskObj = nullptr;
    if (arr == nullptr || size == 0u) return;
    const DetectedObject* currentRisk = &arr[0];
    for (uint32 i = 1u; i < size; ++i)
    {
        if (arr[i].isHigherRisk(*currentRisk))
            currentRisk = &arr[i];
    }
    riskObj = currentRisk;
}

void printDetectedObject(const DetectedObject& obj)
{
    std::cout << "Object ID: " << obj.object_id
              << " | Rel Speed: " << std::fixed << std::setprecision(2) << obj.rel_speed << " m/s"
              << " | Distance: " << std::fixed << std::setprecision(2) << obj.distance << " m\n";
}

void printHighestRiskObject(const DetectedObject* obj)
{
    if (obj == nullptr)
    {
        std::cout << "No highest risk object found.\n";
        return;
    }
    std::cout << "*** Highest Risk Object ***\n";
    printDetectedObject(*obj);
    std::cout << "***************************\n";
}

int main()
{
    uint32 sizeObjects = 3u;
    DetectedObject* objects = new DetectedObject[sizeObjects]
    {
        DetectedObject(201u, 10.0f, 25.0f),
        DetectedObject(202u, 15.0f, 20.0f),
        DetectedObject(203u, 12.0f, 30.0f)
    };

    std::cout << "All Detected Objects:\n";
    for (uint32 i = 0u; i < sizeObjects; ++i)
        printDetectedObject(objects[i]);

    std::cout << "\nUpdating object 1 by value (should not persist changes):\n";
    updateObjectValuesByValueGlobal(objects[0], 301u, 20.0f, 50.0f);
    printDetectedObject(objects[0]);

    std::cout << "\nUpdating object 1 by reference (should persist changes):\n";
    updateObjectValuesByReferenceGlobal(objects[0], 301u, 20.0f, 50.0f);
    printDetectedObject(objects[0]);

    const DetectedObject* riskObj = nullptr;
    findHighestRiskObject(objects, sizeObjects, riskObj);
    printHighestRiskObject(riskObj);

    delete[] objects;
    return 0;
}
