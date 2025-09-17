#include <iostream>

class CameraConfig {
private:
    int num_modes;
    int* resolution_width;
    int* resolution_height;
public:
    CameraConfig(int modes, const int* widths, const int* heights) {
        num_modes = modes;
        resolution_width = new int[num_modes];
        resolution_height = new int[num_modes];
        for(int i=0; i<num_modes; i++) {
            resolution_width[i] = widths[i];
            resolution_height[i] = heights[i];
        }
    }
    CameraConfig(const CameraConfig& other) {
        num_modes = other.num_modes;
        resolution_width = new int[num_modes];
        resolution_height = new int[num_modes];
        for(int i=0; i<num_modes; i++) {
            resolution_width[i] = other.resolution_width[i];
            resolution_height[i] = other.resolution_height[i];
        }
    }
    ~CameraConfig() {
        delete[] resolution_width;
        delete[] resolution_height;
    }
    void printConfig() const {
        for(int i=0; i<num_modes; i++) {
            std::cout << "Mode " << i << ": " << resolution_width[i] << "x" << resolution_height[i] << std::endl;
        }
    }
    bool isHigherResolution(int mode1, int mode2) const {
        int res1 = this->resolution_width[mode1] * this->resolution_height[mode1];
        int res2 = this->resolution_width[mode2] * this->resolution_height[mode2];
        return res1 > res2;
    }
};

void printCameraConfig(const CameraConfig& config) {
    config.printConfig();
}

bool globalCompareResolution(const CameraConfig& config, int m1, int m2) {
    return config.isHigherResolution(m1, m2);
}

int main() {
    int widths[3] = {1920, 1280, 3840};
    int heights[3] = {1080, 2160, 2160};
    CameraConfig config(3, widths, heights);
    printCameraConfig(config);
    std::cout << "Compare Mode 0 and Mode 2 (member function): " 
              << (config.isHigherResolution(0, 2) ? "Mode 0 higher" : "Mode 2 higher or equal") << std::endl;
    std::cout << "Compare Mode 0 and Mode 2 (global function): " 
              << (globalCompareResolution(config, 0, 2) ? "Mode 0 higher" : "Mode 2 higher or equal") << std::endl;
    return 0;
}
