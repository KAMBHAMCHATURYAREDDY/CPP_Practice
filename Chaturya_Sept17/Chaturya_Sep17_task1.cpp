#include <iostream>
#include <iomanip>
#include <cstdint>

typedef uint32_t uint32;

class Pedestrian
{
public:
    uint32 pedestrian_id;
    float distance_from_vehicle;

    // Constructor
    Pedestrian(uint32 id = 0u, float distance = 0.0f)
        : pedestrian_id(id), distance_from_vehicle(distance)
    {
    }

    
    void updateDistance(float newDistance)
    {
        distance_from_vehicle = newDistance;
    }

    
    float getDistance() const
    {
        return distance_from_vehicle;
    }
};

// Function to find closest pedestrian
void findClosestPedestrian(Pedestrian* array, uint32 size, const Pedestrian*& closest)
{
    closest = nullptr;
    if (array == nullptr || size == 0u)
        return;

    const Pedestrian* currentClosest = &array[0];
    for (uint32 i = 1u; i < size; ++i)
    {
        if (array[i].getDistance() < currentClosest->getDistance())
            currentClosest = &array[i];
    }
    closest = currentClosest;
}


void printPedestrianInfo(const Pedestrian& ped)
{
    std::cout << "Pedestrian ID: " << ped.pedestrian_id
              << " | Distance: " << std::fixed << std::setprecision(2)
              << ped.distance_from_vehicle << " m\n";
}

void printClosestPedestrian(const Pedestrian* closest)
{
    if (closest == nullptr)
    {
        std::cout << "No closest pedestrian found.\n";
        return;
    }
    std::cout << "*** Closest Pedestrian ***\n";
    printPedestrianInfo(*closest);
    std::cout << "**************************\n";
}

int main()
{
    uint32 sizePedestrians = 3u;
    Pedestrian* pedestrians = new Pedestrian[sizePedestrians]
    {
        Pedestrian(101u, 12.4f),
        Pedestrian(102u, 8.7f),
        Pedestrian(103u, 15.1f)
    };

    for (uint32 i = 0u; i < sizePedestrians; ++i)
        printPedestrianInfo(pedestrians[i]);

    // Find closest pedestrian
    const Pedestrian* closestPed = nullptr;
    findClosestPedestrian(pedestrians, sizePedestrians, closestPed);
    printClosestPedestrian(closestPed);

    delete[] pedestrians;
    return 0;
}
