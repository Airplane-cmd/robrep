#include <iostream>
#include "libusb.h"

constexpr auto VID = 0x04B4;
constexpr auto PID = 0x00F0;
constexpr auto OUT_ENDPOINT_ID = 1;
constexpr auto IN_ENDPOINT_ID = 1;

using namespace std;

struct libusb_device_descriptor DeviceDescriptor;
libusb_context* context = NULL; //a libusb session
libusb_device_handle* DeviceHandle = NULL; //a device handle


int main() {
    libusb_init(&context); //initialize a library session
    libusb_set_debug(context, 3); //set verbosity level to 3, as suggested in the documentation
    DeviceHandle = libusb_open_device_with_vid_pid(context, VID, PID); //these are vendorID and productID for Cypress FX3 specific device

    unsigned char* DataOut = new unsigned char[4]; //data to write
    unsigned char* DataIn = new unsigned char[512]; //data to read
    DataOut[0] = 'a'; DataOut[1] = 'b'; DataOut[2] = 'c'; DataOut[3] = 'd'; //some dummy values
    int BytesWritten, BytesRead, counter = 0; //used to find out how many bytes were written/read

    if (libusb_kernel_driver_active(DeviceHandle, 0) == 1) { //find out if kernel driver is attached
        libusb_detach_kernel_driver(DeviceHandle, 0); //detach it
    }
    libusb_claim_interface(DeviceHandle, 0); //claim interface 0 (the first) of device (desired device FX3 has only 1)

    libusb_bulk_transfer(DeviceHandle, (OUT_ENDPOINT_ID | LIBUSB_ENDPOINT_OUT), DataOut, sizeof(DataOut), &BytesWritten, 0); // write data
    while (libusb_bulk_transfer(DeviceHandle, (IN_ENDPOINT_ID | LIBUSB_ENDPOINT_IN), DataIn, sizeof(DataIn), &BytesRead, 0) == 0 && counter++ < 5) {

        cout << "Reading Successful! Data is: " << endl;
        for (int i = 0; i < BytesRead; i++) {
            cout << DataIn[i];
        }
        cout << endl;

        if (counter < 3) {
            *DataOut = static_cast<char>(rand()); // generate random input
            libusb_bulk_transfer(DeviceHandle, (OUT_ENDPOINT_ID | LIBUSB_ENDPOINT_OUT), DataOut, sizeof(DataOut), &BytesWritten, 0); // write to device
        }
        
    }

    libusb_release_interface(DeviceHandle, 0); //release the claimed interface
    delete[] DataOut; //delete the allocated memory for data
    delete[] DataIn; //delete the allocated memory for data
    libusb_close(DeviceHandle); //close the device we opened
    libusb_exit(context); //needs to be called at the end 

    return 0;
}
