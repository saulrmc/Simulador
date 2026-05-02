//
// Created by saul on 1/05/26.
//

#ifndef SIMULADORGRAVITACIONAL_LINEARNODE_H
#define SIMULADORGRAVITACIONAL_LINEARNODE_H



class LinearNode {
    public:
    LinearNode();
    virtual ~LinearNode();
    private:
    unsigned int mortonCode;

public:
    unsigned int get_morton_code() const;

    void set_morton_code(const unsigned int morton_code);
};



#endif //SIMULADORGRAVITACIONAL_LINEARNODE_H
