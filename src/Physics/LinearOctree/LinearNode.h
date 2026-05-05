//
// Created by saul on 3/05/26.
//

#ifndef SIMULADORGRAVITACIONAL_LINEARNODE_H
#define SIMULADORGRAVITACIONAL_LINEARNODE_H


template < typename T >
class LinearNode {
    public:
    LinearNode();
    virtual ~LinearNode();
    double get_mass() const;
    void set_mass(const double mass);
    double get_centerX() const;
    void set_centerX(const double centerX);
    double get_centerY() const;
    void set_centerY(const double centerY);
    double get_centerZ() const;
    void set_centerZ(const double centerZ);
    double get_centerOfMassX() const;
    void set_centerOfMassX(const double centerOfMassX);
    double get_centerOfMassY() const;
    void set_centerOfMassY(const double centerOfMassY);
    double get_centerOfMassZ() const;
    void set_centerOfMassZ(const double centerOfMassZ);
    unsigned int get_minRange() const;
    void set_minRange(const unsigned int minRange);
    unsigned int get_maxRange() const;
    void set_maxRange(const unsigned int maxRange);


    private:
    unsigned int minRange;
    unsigned int maxRange;
    // index 0: centerOfMassX
    // index 1: centerOfMassY
    // index 2: centerOfMassZ
    double centerOfMass[3]{};
    // double centerOfMassX; //centro de masa del cubo
    // double centerOfMassY;
    // double centerOfMassZ;
    double mass; //masa total que está contenida en el espacio cúbico

    // index 0: centerX
    // index 1: centerY
    // index 2: centerZ
    double center[3]{};
    // double centerX;  // Centro del cubo
    // double centerY;
    // double centerZ;

    double size;     // Tamaño de un lado del cubo
};

#endif //SIMULADORGRAVITACIONAL_LINEARNODE_H
