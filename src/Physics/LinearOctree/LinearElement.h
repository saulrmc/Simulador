//
// Created by saul on 1/05/26.
//

#ifndef SIMULADORGRAVITACIONAL_LINEARELEMENT_H
#define SIMULADORGRAVITACIONAL_LINEARELEMENT_H

template<typename T>
class LinearElement {
    public:
    LinearElement();
    virtual ~LinearElement();
    double get_object_posX();
    double get_object_posY();
    double get_object_posZ();
    void set_object_posX(const double object_posX);
    void set_object_posY(const double object_posY);
    void set_object_posZ(const double object_posZ);
    unsigned long long int get_morton_code() const;
    void set_morton_code(const unsigned long long int morton_code);
    private:
    T *object;

    //aprovechando
    //los 64 bits del dato tendría 21
    //bits para cada coordenada, lo que me daría un total de
    //2 097 152 celdas en cada eje como máximo.
    unsigned long long int mortonCode;


};



#endif //SIMULADORGRAVITACIONAL_LINEARELEMENT_H
