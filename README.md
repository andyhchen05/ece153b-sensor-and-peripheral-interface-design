# ECE153B: Sensor and Peripheral Interface Design

## Course Description
Hardware description languages; field-programmable logic and ASIC design techniques. Mixed-signal techniques: A/D and D/A converter interfaces; video and audio signal acquisition, processing and generation, communication and network interfaces.

## Lecture Topics:
- Processors: Microprocessors, Microcontrollers, DSP
- General Purpose I/O (GPIO) pins
- Timers
- Interrupts
- Motors
- Serial Interfaces: UART, I2C, SPI, USB
- Analog Input and Output: Op-Amps, Schmidt Trigger, ADC, DAC
- Parallel Interfaces
- The Memory Subsystem
- Sampling and Digital Filtering

## Lab Descriptions:
- Lab 1: Set-up development environment and use GPIO pins for capturing input and controlling output.
- Lab 2: Use interrupts triggered through GPIO pins. Rotate a stepper motor using GPIO pins and different stepping methods. Configure the real time clock and trigger alarms based on current time.
- Lab 3: Create a periodic interrupt through the use of SysTick. Generate PWM outputs using timers to control a servo-motor and trigger an ultrasonic sensor, then use a timer to capture the sensor’s output.
- Lab 4: Use serial protocols USART, I2C, and SPI for reading/writing data to and from a bluetooth module, thermal sensor, and accelerometer respectively. Configure the peripherals according to desired specifications found within their datasheets.
- Lab 5: Use the the onboard ADC for reading in an analog input and displaying output onto the board’s LED. Use the onboard DAC to generate analog outputs.
- Lab 6: Check for data errors through cyclic redundancy check (CRC) in both software and hardware. Use direct memory access (DMA) for sending data to peripheral.
- Lab 7 (Project) - Create an automated garage door system through the use of an accelerometer, stepper motor, thermal sensor, and bluetooth module. Have the garage door open automatically under certain conditions and control it through software.

## Course Goals:
- Learn about the architecture of microcontrollers, different types of memory (SRAM, DRAM, Flash memory, etc.) as well as standard microcontroller features including but not limited to timers and real time clocks.
- Learn how to use ADCs and DACs to interface with external circuits, as well as digital signal processing concepts such as sampling and digital filters.
- Learn about serial and peripheral interfaces including GPIO, UART, I2C, SPI, and USB and communication interfaces including Wi-Fi, Bluetooth, and GPS.
- Learn about sensors including infrared, ultrasonic, and optical sensors as well as accelerometers and gyroscopes.
- Design micro-controller-based circuits within an integrated development environment (IDE) that interface with external peripherals.
- Understand the basics of sampling and digital filtering in the context of embedded systems.

## ECE Program Outcomes: 
This class contributes to the following ECE program outcomes:
- An ability to identify, formulate, and solve complex engineering problems by applying the principles of engineering, science, and mathematics.
- An ability to apply engineering design to produce solutions that meet specified needs with consideration of public health, safety, and welfare, as well as global, cultural, social, environmental, and economic factors.
- An ability to function effectively on a team whose members together provide leadership, create a collaborative and inclusive environment, establish goals, plan tasks, and meet objectives.
- An ability to acquire and apply new knowledge as needed, using appropriate learning strategies.
