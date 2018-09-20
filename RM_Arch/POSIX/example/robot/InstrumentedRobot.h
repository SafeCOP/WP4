#ifndef ROBOT_INSTRUMENTEDROBOT_H
#define ROBOT_INSTRUMENTEDROBOT_H

#include "../../monitoring.h"
#include "Robot.h"

class InstrumentedRobot : public Robot {
private:
    EventBuffer<bool, 64> clean_event_buffer;
    EventBuffer<bool, 64> paint_event_buffer;
    EventBuffer<bool, 64> gloss_event_buffer;
    EventBuffer<int, 64> temp_event_buffer;

    EventWriter<bool> clean_event_writer;
    EventWriter<bool> paint_event_writer;
    EventWriter<bool> gloss_event_writer;
    EventWriter<int> temp_event_writer;

    void clean() {
        Robot::clean();
        const bool value = true;
        clean_event_writer.push(value);
    }

    void paint() {
        Robot::paint();
        const bool value = true;
        paint_event_writer.push(value);
    }

    void gloss() {
        Robot::gloss();
        const bool value = true;
        gloss_event_writer.push(value);
    }

public:
    InstrumentedRobot() {
        clean_event_buffer.configWriter(clean_event_writer);
        paint_event_buffer.configWriter(paint_event_writer);
        gloss_event_buffer.configWriter(gloss_event_writer);
        temp_event_buffer.configWriter(temp_event_writer);
    }

    void paintCar() {
        Robot::paintCar();
        temp_event_writer.push(this->temperature.load());
    }


    const IEventBuffer<bool> &getClean_event_buffer() const {
        return clean_event_buffer;
    }

    const IEventBuffer<bool> &getPaint_event_buffer() const {
        return paint_event_buffer;
    }

    const IEventBuffer<bool> &getGloss_event_buffer() const {
        return gloss_event_buffer;
    }

    const IEventBuffer<int> &getTemp_event_buffer() const {
        return temp_event_buffer;
    }
};


#endif //ROBOT_INSTRUMENTEDROBOT_H
