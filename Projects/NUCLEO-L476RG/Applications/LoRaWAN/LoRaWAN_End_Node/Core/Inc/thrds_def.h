#ifndef THRDS_DEF_H
#define THRDS_DEF_H

void onUSBPlugIn(void);
void onUSBPlugOff(void);
void Idle_Task(void*);

#define USBPlugInSignal	8
#define USBPlugOffSignal 16

void wrap_USB_Init(void *argument);
void wrap_USB_DeInit(void *argument);

#endif /* THRDS_DEF_H */
