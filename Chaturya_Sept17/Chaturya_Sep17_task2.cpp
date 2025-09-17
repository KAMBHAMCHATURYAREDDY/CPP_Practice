#include <iostream>
#include <iomanip>
#include <cstdint>
#include <limits>
#include <cmath>

typedef uint32_t uint32;

class TrackedVehicle
{
public:
    uint32 vehicle_id;
    float speed;
    float distance;

    static float min_recorded_distance;
    static int vehicle_count;

    TrackedVehicle(uint32 id = 0u, float spd = 0.0f, float dist = 0.0f)
        : vehicle_id(id), speed(spd), distance(dist)
    {
        ++TrackedVehicle::vehicle_count;
        if (std::isfinite(distance) && distance < TrackedVehicle::min_recorded_distance)
        {
            TrackedVehicle::min_recorded_distance = distance;
        }
    }

    void display() const
    {
        std::cout << "Vehicle ID: " << vehicle_id
                  << " | Speed: " << std::fixed << std::setprecision(2) << speed << " km/h"
                  << " | Distance: " << std::fixed << std::setprecision(2) << distance << " m\n";
    }

    const TrackedVehicle* compareDistance(const TrackedVehicle& other) const
    {
        return (distance <= other.distance) ? this : &other;
    }

    static float getMinRecordedDistance()
    {
        return min_recorded_distance;
    }

    static int getVehicleCount()
    {
        return vehicle_count;
    }
};

float TrackedVehicle::min_recorded_distance = std::numeric_limits<float>::infinity();
int TrackedVehicle::vehicle_count = 0;

void findLeadVehicle(TrackedVehicle* arr, uint32 size, const TrackedVehicle*& lead)
{
    lead = nullptr;
    if (arr == nullptr || size == 0u)
        return;

    const TrackedVehicle* currentLead = &arr[0];
    for (uint32 i = 1u; i < size; ++i)
    {
        if (arr[i].distance < currentLead->distance)
            currentLead = &arr[i];
    }
    lead = currentLead;
}

void printTrackedVehicle(const TrackedVehicle& vehicle)
{
    vehicle.display();
}

void printLeadVehicle(const TrackedVehicle* lead)
{
    if (lead == nullptr)
    {
        std::cout << "Lead Vehicle: None found\n";
    }
    else
    {
        std::cout << "*** Lead Vehicle ***\n";
        printTrackedVehicle(*lead);
        std::cout << "********************\n";
    }
}

int main()
{
    uint32 sizeVehicles = 4u;
    TrackedVehicle* vehicles = new TrackedVehicle[sizeVehicles]
    {
        TrackedVehicle(501u, 80.0f, 60.0f),
        TrackedVehicle(502u, 78.0f, 45.0f),
        TrackedVehicle(503u, 85.0f, 100.0f),
        TrackedVehicle(504u, 76.0f, 40.0f)
    };

    for (uint32 i = 0u; i < sizeVehicles; ++i)
        printTrackedVehicle(vehicles[i]);

    // Find lead vehicle
    const TrackedVehicle* leadVehicle = nullptr;
    findLeadVehicle(vehicles, sizeVehicles, leadVehicle);
    printLeadVehicle(leadVehicle);

    std::cout << "Min recorded distance across all vehicles: "
              << std::fixed << std::setprecision(2)
              << TrackedVehicle::getMinRecordedDistance() << " m\n";
    std::cout << "Total number of tracked vehicles created: "
              << TrackedVehicle::getVehicleCount() << "\n";

    delete[] vehicles;
    return 0;
}