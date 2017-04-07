import serial
import csv

# Configure port and baudrate
device = "COM3"
baudrate = 9600

try:
    with serial.Serial(device, baudrate=baudrate, parity=serial.PARITY_NONE, stopbits=serial.STOPBITS_ONE,
                       bytesize=serial.EIGHTBITS, rtscts=False, dsrdtr=False, xonxoff=False,
                       timeout=None) as teensy_serial, open('test1.csv', 'w') as csvfile:

        print "Connected to", teensy_serial.name

        # initialize csv writer
        csvfile.flush()
        csv_writer = csv.writer(csvfile, delimiter=',', quotechar='"')

        while True:

            line = teensy_serial.read()

            # Do Stuff
            print ">", line
            csv_writer.writerow([line])


except serial.SerialException as e:
    print(e)
except KeyboardInterrupt:
    print("Exited")
