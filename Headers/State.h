#ifndef STATE_H
#define STATE_H

class State {
private:

public:
    State();
    ~State();

    virtual void update() = 0;
    virtual void render() = 0;
};

#endif
