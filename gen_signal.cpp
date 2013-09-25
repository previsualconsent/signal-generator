#include <iostream>
#include <stdio.h>
#include <stdint.h>

using namespace std;

class Packet
{
    public:
        Packet() 
        {
            for(int i=0; i<12;i++)
                data[i]=0;

            //set bc0
            data[1] = 1;
        }
        ~Packet() {} 
    private:
        uint8_t data[12];

        void set_bits(int byte_num, int start, int low_bit, int hi_bit, int value)
        {
            uint8_t in_mask = make_mask(low_bit,hi_bit);
            uint8_t out_mask = ~make_mask(start,start+(hi_bit-low_bit +1));
            data[byte_num] = (((in_mask & value) << start) >> low_bit) + (out_mask & data[byte_num]);
        }

        void output_char(uint8_t value)
        {
            for(int j=7; j>=0;j--)
                cout << (bool)(value & (1 << j));
            cout << endl;
        }

        uint8_t make_mask(int low_bit, int hi_bit)
        {
            uint8_t mask=0;
            for(int i = low_bit; i <= hi_bit;i++)
                mask+=(1 << i);
            return mask;
        }

    public:
        void set_adc(int qie,uint8_t value)
        {
            data[qie+2]=value;
        }
        void set_tdc(int qie, uint8_t value)
        {
            if(value < 64)
            {
                switch(qie)
                {
                    case 0:
                        set_bits(1, 4, 0, 3, value);
                        set_bits(8, 0, 4, 5, value);
                        break;
                    case 1:
                        set_bits(8, 2, 0, 5, value);
                        break;
                    case 2:
                        set_bits(9, 0, 0, 5, value);
                        break;
                    case 3:
                        set_bits(9, 6, 0, 1, value);
                        set_bits(10, 0, 2, 5, value);
                        break;
                    case 4:
                        set_bits(10, 4, 0, 3, value);
                        set_bits(11, 0, 4, 5, value);
                        break;
                    case 5:
                        set_bits(11, 2, 0, 5, value);
                        break;
                }
            }
            else cout << "tdc value, " << (int)value << ", to high\n";
        }

        void print_data()
        {
            for(int i=0; i<12;i++)
            {
                cout << i << ": ";
                output_char(data[i]);
            }
        }
        void write_data()
        {
            for(int i=0; i<12;i++)
            {
                output_char(data[i]);
            }
        }

};

int main()
{
    Packet mypacket;
    
    mypacket.set_adc(0,255);
    mypacket.set_adc(1,255);
    mypacket.set_adc(2,255);
    mypacket.set_adc(3,255);
    mypacket.set_adc(4,255);
    mypacket.set_adc(5,255);
    mypacket.set_tdc(0,63);
    mypacket.set_tdc(1,63);
    mypacket.set_tdc(2,63);
    mypacket.set_tdc(3,63);
    mypacket.set_tdc(4,63);
    mypacket.set_tdc(5,63);


    mypacket.print_data();
}

