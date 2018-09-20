#ifndef ROBOT_TEMPERATUREMONITOR_H
#define ROBOT_TEMPERATUREMONITOR_H

#include "../../monitoring.h"
#include "Robot.h"

class TemperatureMonitor : public Monitor {
private:
    EventReader<int> temperature_EventReader;
    Robot *robot;

protected:
    void run() {
        Event<int> event;
        bool gap;
        bool pop = temperature_EventReader.pop(event, gap);
        if (pop) {
            int temperature = event.getData();
            if (temperature > 7) {
                cout <<
                "Temperature Monitor: Robot temperature beyond safety threshold level, cooling counter measures deployed" <<
                endl;
                robot->coolDown();
            }
            if (gap) {
                cout << "Temperature Monitor: A temperature event reader gap occurred!" << endl;
            }
        }
    }

public:
    TemperatureMonitor(Robot *robot, const IEventBuffer<int> &temp_event_buffer) : Monitor(1, 0), robot(robot) {
        temp_event_buffer.configReader(temperature_EventReader);
    }
};


#endif //ROBOT_TEMPERATUREMONITOR_H
