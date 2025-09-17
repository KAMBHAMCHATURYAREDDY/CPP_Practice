#include <iostream>

class ObjectDistance {
private:
    float radar_distance;
    float camera_distance;
public:
    ObjectDistance(float radar, float camera) : radar_distance(radar), camera_distance(camera) {}
    void adjustDistancesByValue(ObjectDistance obj) {
        radar_distance = obj.radar_distance + 5.0f;
        camera_distance = obj.camera_distance + 5.0f;
    }
    void adjustDistancesByReference(ObjectDistance& obj) {
        radar_distance = obj.radar_distance + 5.0f;
        camera_distance = obj.camera_distance + 5.0f;
    }
    float getRadar() const { return radar_distance; }
    float getCamera() const { return camera_distance; }
};

void printObjectDistance(const ObjectDistance& obj) {
    std::cout << "Radar Distance: " << obj.getRadar() << ", Camera Distance: " << obj.getCamera() << std::endl;
}

ObjectDistance* createObjectDistanceOnHeap(float radar, float camera) {
    return new ObjectDistance(radar, camera);
}

int main() {
    ObjectDistance* obj = createObjectDistanceOnHeap(35.5f, 34.8f);
    std::cout << "Before Adjustment:" << std::endl;
    printObjectDistance(*obj);
    obj->adjustDistancesByValue(*obj);
    std::cout << "After adjustDistancesByValue:" << std::endl;
    printObjectDistance(*obj);
    obj->adjustDistancesByReference(*obj);
    std::cout << "After adjustDistancesByReference:" << std::endl;
    printObjectDistance(*obj);
    delete obj;
    return 0;
}
