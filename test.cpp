#include <iostream>
#include <unistd.h>
#include "lib.h"

class OftenEvent{};
class RarelyEvent{};

class UniqueObject{

public:
    static int i;
    int id;

    UniqueObject(){
        id = i;
        i++;
    };
};

int UniqueObject::i = 0;

class EventListenerOftenEvent: public BrickCallbackImpl<OftenEvent>, UniqueObject{
public:
    EventListenerOftenEvent() : UniqueObject(){};
    void call(OftenEvent) override{
        static int i = 0;
        std::cout<<"BrickCallbackImpl<OftenEvent>. iterator: "<<i<< " obj id: "<<id<<"\n\r";
        i++;
    }
};



class EventListenerRarelyEvent: public BrickCallbackImpl<RarelyEvent>, UniqueObject{
public:
    EventListenerRarelyEvent(): UniqueObject(){};
    void call(RarelyEvent) override{
        static int i = 0;
        std::cout<<"BrickCallbackImpl<RarelyEvent>. iterator: "<<i<<" obj id: "<< id<<"\n\r";
        i++;
    }
};

class EventEmitter: public BrickPollImpl {
public:
    void poll() override{
        static int i = 0;
        switch(i) {
        case 0 ... 2: case  4 ... 6: case 8:
                std::cout << "EventEmitter emmit often event on step  " << i << "\n\r";
                this->often_event->call();
            break;
            case 3: case 7:
                std::cout << "EventEmitter emmit rarely event on step  " << i << "\n\r";
                this->rarely_event->call();
                break;
            case 9:
                std::cout << "EventEmitter emmit splitter event on step  " << i << "\n\r";
                this->splitter.call_all();
            default:
                std::cout << "unimplemented doing on step " << i << "\n\r";
                break;
        }
        std::cout << "\n\r";
        i++;
    }

public:
    BrickCallbackImpl<RarelyEvent>* rarely_event;
    BrickCallbackImpl<OftenEvent>* often_event;
    InterruptSplitter<RarelyEvent> splitter;

    EventEmitter(
            BrickCallbackImpl<RarelyEvent> *rarely_event,
            BrickCallbackImpl<OftenEvent> *often_event,
            InterruptSplitter<RarelyEvent> splitter
            ) : rarely_event(rarely_event), often_event(often_event), splitter(splitter){};


};

int main() {
    EventListenerOftenEvent often_listener_obj_1;
    EventListenerRarelyEvent rarely_listener_obj_1;
    EventListenerRarelyEvent rarely_listener_obj_2;
    EventListenerRarelyEvent rarely_listener_obj_3;

    InterruptSplitter<RarelyEvent> splitter;
    BrickCallbackImpl<RarelyEvent> * rarely_listener_obj_arr[] =
            {
                static_cast<BrickCallbackImpl<RarelyEvent> *>(&rarely_listener_obj_2),
                static_cast<BrickCallbackImpl<RarelyEvent> *>(&rarely_listener_obj_3)
            };
    splitter.set_splitter(rarely_listener_obj_arr, 2);

    EventEmitter emitter(
            static_cast<BrickCallbackImpl<RarelyEvent> *>(&rarely_listener_obj_1),
            static_cast<BrickCallbackImpl<OftenEvent> *>(&often_listener_obj_1),
            splitter);

    for (int i = 0; i < 10; i++) {
        emitter.poll();
        usleep(100000);
    }
    return 1;
}