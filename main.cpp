#include <iostream>
#include <memory>

template<typename T>
struct ControlBlock
{
    T* data_pointer{};
    size_t  reference_counter{};

};

template<typename T>
class shared_ptr
{
public:
    shared_ptr()
    {}
    explicit shared_ptr(T* data) : data_pointer{ data }, control_block{ new ControlBlock<T> }
    {
        control_block->data_pointer = data_pointer;
        ++control_block->reference_counter;
    }
    shared_ptr(const shared_ptr<T>& other) noexcept
    {
        data_pointer = other.data_pointer;
        delete control_block;
        control_block = other.control_block;
        ++control_block->reference_counter;
    }
    shared_ptr<T>& operator=(const shared_ptr<T>& other)
    {
        if (this == &other)
            return *this;
        deref();
        data_pointer = other.data_pointer;
        --control_block; ++other.control_block->reference_counter;
        control_block = other.control_block;

        return *this;
    }

    shared_ptr(shared_ptr<T>&& other) noexcept
    {
        std::swap(data_pointer, other.data_pointer);
        delete control_block;
        std::swap(control_block, other.control_block);
    }

    shared_ptr<T>& operator=(shared_ptr<T>&& other) noexcept
    {
        if (this == &other)
            return *this;
        deref();
        std::swap(data_pointer, other.data_pointer);
        control_block = other.control_block;
        other.control_block = nullptr;

        return *this;
    }

    T& operator*() const { return *data_pointer; }
    T* operator->()  const { return  data_pointer; }

    ~shared_ptr()
    {
        deref();
    }

public:
    size_t use_count() { return control_block->reference_counter; }
private:
    void deref()
    {
        if (control_block)
        {
            if (control_block->reference_counter == 1) {
                delete data_pointer;
                delete control_block;
            }
            else {
                --control_block->reference_counter;
                data_pointer = nullptr;
                control_block = nullptr;
            }
        }
    }
    T* data_pointer{};
    ControlBlock<T>* control_block{new ControlBlock<T>()};
};

class A
{
public:
    int a, b, c, d, f, g, qmzm, zxc, asd, dfhg, bcv, wer, yuk, ghjm, oiy;
};

void test()
{
    shared_ptr<A> t1{ new A };
    shared_ptr<A> t2{ new A };
    shared_ptr<A> t3{ new A };

    t1 = t3;
    t2 = std::move(t3);

    //shared_ptr<A>tt (t3);
}

int main() {
    shared_ptr<int> sh{ new int(5) };
    shared_ptr<int> s(sh);
    shared_ptr<int> sd(std::move(s));
    //test();
    return 0;
}
