#ifndef ROBOT_ROBOTOPERATIONORDERMONITOR_H
#define ROBOT_ROBOTOPERATIONORDERMONITOR_H

#include "../../monitoring.h"
#include "Robot.h"

class RobotOperationOrderMonitor : public Monitor {
private:
    Robot *robot;

    enum state {
        CLEAN,
        PAINT,
        GLOSS
    };
    state monitor_state;

    Event<bool> clean_event;
    Event<bool> paint_event;
    Event<bool> gloss_event;

    EventReader<bool> clean_event_reader;
    EventReader<bool> paint_event_reader;
    EventReader<bool> gloss_event_reader;
protected:
    void run() {
        bool gap;
        bool popped;

        switch (monitor_state) {
            case CLEAN:
                popped = clean_event_reader.pop(clean_event, gap);
                if (popped) {
                    if (gap) {
                        cout << "RobotOperationOrderMonitor: clean event reader gap occurred!" << endl;
                    }
                } else return;

                monitor_state = PAINT;

            case PAINT:
                popped = paint_event_reader.pop(paint_event, gap);
                if (popped) {
                    if (gap) {
                        cout << "RobotOperationOrderMonitor: paint event reader gap occurred!" << endl;
                    }
                } else return;

                monitor_state = GLOSS;

            case GLOSS:
                popped = gloss_event_reader.pop(gloss_event, gap);
                if (popped) {
                    if (gap) {
                        cout << "RobotOperationOrderMonitor: gloss event reader gap occurred!" << endl;
                    }
                } else return;

                monitor_state = CLEAN;
        }

        bool reset = false;
        if (clean_event > paint_event) {
            cout << "RobotOperationOrderMonitor: Robot operation order error, robot did not clean before painting!" <<
            endl;
            reset = true;
        }

        if (paint_event > gloss_event) {
            cout << "RobotOperationOrderMonitor: Robot operation order error, robot did not paint before glossing!" <<
            endl;
            reset = true;
        }

        if (reset) robot->reset();
    }

public:
    RobotOperationOrderMonitor(Robot *robot,
                               const IEventBuffer<bool> &clean_event_buffer,
                               const IEventBuffer<bool> &paint_event_buffer,
                               const IEventBuffer<bool> &gloss_event_buffer) : Monitor(1, 0), robot(robot) {
        clean_event_buffer.configReader(clean_event_reader);
        paint_event_buffer.configReader(paint_event_reader);
        gloss_event_buffer.configReader(gloss_event_reader);
        monitor_state = CLEAN;
    }
};


#endif //ROBOT_ROBOTOPERATIONORDERMONITOR_H
