#ifndef ROBOT_ROBOT_H
#define ROBOT_ROBOT_H

#include <atomic>
#include <iostream>

using namespace std;

class Robot {
protected:
    std::atomic<int> temperature;
    unsigned int error;

    virtual void clean() {
        cout << "Cleaning";
    }

    virtual void paint() {
        cout << " -> Painting";
    }

    virtual void gloss() {
        cout << " -> Glossing";
    }

public:
    Robot() : temperature(0), error(0) { }

    virtual void paintCar() {
        cout << "Robot: painting another car: ";
        clean();
        if (++error % 5 == 0) {
            gloss();
            paint();
        } else {
            paint();
            gloss();
        }
        temperature++;

        cout << endl;
    }

    void coolDown() {
        temperature.store(0);
    }

    void reset() {
        cout << "Robot: reset" << endl;
    }
};


#endif //ROBOT_ROBOT_H
