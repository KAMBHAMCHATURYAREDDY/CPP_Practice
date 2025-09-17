#include <iostream>
#include <string>

class ParcelAnalyzer
{
public:
    std::string classifyWeight(int grams) const
    {
        if (grams < 0) return std::string("Invalid");
        if (grams < 500) return std::string("Light");
        if (grams <= 2000) return std::string("Medium");
        return std::string("Heavy");
    }
    std::string classifyWeight(float kilograms) const
    {
        if (kilograms < 0.0f) return std::string("Invalid");
        if (kilograms < 0.5f) return std::string("Light");
        if (kilograms <= 2.0f) return std::string("Medium");
        return std::string("Heavy");
    }
    std::string classifyWeight(double pounds) const
    {
        if (pounds < 0.0) return std::string("Invalid");
        if (pounds < 1.1) return std::string("Light");
        if (pounds <= 4.4) return std::string("Medium");
        return std::string("Heavy");
    }
};

void printClassification(const std::string& unit, double value, const std::string& label)
{
    std::cout << unit << " " << value << " -> " << label << std::endl;
}

int main()
{
    ParcelAnalyzer analyzer;
    int g1 = 450, g2 = 1500, g3 = 2500;
    float k1 = 0.3f, k2 = 1.5f, k3 = 3.0f;
    double p1 = 0.9, p2 = 2.5, p3 = 5.0;

    printClassification("Grams", g1, analyzer.classifyWeight(g1));
    printClassification("Grams", g2, analyzer.classifyWeight(g2));
    printClassification("Grams", g3, analyzer.classifyWeight(g3));

    printClassification("Kilograms", k1, analyzer.classifyWeight(k1));
    printClassification("Kilograms", k2, analyzer.classifyWeight(k2));
    printClassification("Kilograms", k3, analyzer.classifyWeight(k3));

    printClassification("Pounds", p1, analyzer.classifyWeight(p1));
    printClassification("Pounds", p2, analyzer.classifyWeight(p2));
    printClassification("Pounds", p3, analyzer.classifyWeight(p3));

    return 0;
}
