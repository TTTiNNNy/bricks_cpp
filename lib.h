template <typename T> class BrickCallbackImpl{
public:
    virtual void call(T = T()) = 0;
};

class BrickPollImpl{
public:
    virtual void poll() = 0;
};

template <typename T> class InterruptSplitter
{
    protected:
        int arr_size;
        BrickCallbackImpl<T> ** splitter_ouput_interrupt;

    public:
        void set_splitter(BrickCallbackImpl<T> ** splitter, int splitter_size)
        {
            splitter_ouput_interrupt = splitter;
            arr_size = splitter_size;
        };
        void call_all()
        {
            for (int i = 0; i < arr_size; i++){
                (*splitter_ouput_interrupt[i]).call();
            }
        };
};
