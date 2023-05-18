import serial
import re
import time
import json

ser = serial.Serial('/dev/ttyUSB0', 115200, timeout=10)

def read_p1_data():
    data = ''
    while True:
        line = ser.readline().decode().strip()
        if line.startswith('!'):
            data += line
            break
        data += line + '\n'
    return data

def parse_p1_data(data):
    data_dict = {}
    lines = data.split('\n')
    for line in lines:
        if line.startswith('1-0:1.8.1'):
            data_dict['Rate 1 (day) - total consumption'] = float(re.findall(r'\(([\d\.]+)\*kWh\)', line)[0])
        elif line.startswith('1-0:1.8.2'):
            data_dict['Rate 2 (night) - total consumption'] = float(re.findall(r'\(([\d\.]+)\*kWh\)', line)[0])
        elif line.startswith('1-0:2.8.1'):
            data_dict['Rate 1 (day) - total production'] = float(re.findall(r'\(([\d\.]+)\*kWh\)', line)[0])
        elif line.startswith('1-0:2.8.2'):
            data_dict['Rate 2 (night) - total production'] = float(re.findall(r'\(([\d\.]+)\*kWh\)', line)[0])
        elif line.startswith('0-0:96.14.0'): 
            data_dict['Current_rate_(1_is_day_en_2_is_night)'] = float(re.findall(r'\((\d+)\)', line)[0])
        elif line.startswith('1-0:1.7.0'):
            data_dict['All phases consumption'] = float(re.findall(r'\(([\d\.]+)\*kW\)', line)[0])
        elif line.startswith('1-0:21.7.0'):
            data_dict['L1 consumption'] = float(re.findall(r'\(([\d\.]+)\*kW\)', line)[0])
        elif line.startswith('1-0:41.7.0'):
            data_dict['L2 consumption'] = float(re.findall(r'\(([\d\.]+)\*kW\)', line)[0])
        elif line.startswith('1-0:61.7.0'):
            data_dict['L3 consumption'] = float(re.findall(r'\(([\d\.]+)\*kW\)', line)[0])
        elif line.startswith('1-0:1.7.0'):
            data_dict['All phases consumption'] = float(re.findall(r'\(([\d\.]+)\*kW\)', line)[0])
        elif line.startswith('1-0:22.7.0'):
            data_dict['L1 production'] = float(re.findall(r'\(([\d\.]+)\*kW\)', line)[0])
        elif line.startswith('1-0:42.7.0'):
            data_dict['L2 production'] = float(re.findall(r'\(([\d\.]+)\*kW\)', line)[0])
        elif line.startswith('1-0:62.7.0'):
            data_dict['L3 production'] = float(re.findall(r'\(([\d\.]+)\*kW\)', line)[0])
        elif line.startswith('1-0:2.7.0'):
            data_dict['All phases production'] = float(re.findall(r'\(([\d\.]+)\*kW\)', line)[0])
        elif line.startswith('1-0:32.7.0'):
            data_dict['L1 voltage'] = float(re.findall(r'\(([\d\.]+)\*V\)', line)[0])
        elif line.startswith('1-0:52.7.0'):
            data_dict['L2 voltage'] = float(re.findall(r'\(([\d\.]+)\*V\)', line)[0])
        elif line.startswith('1-0:72.7.0'):
            data_dict['L3 voltage'] = float(re.findall(r'\(([\d\.]+)\*V\)', line)[0])
        elif line.startswith('1-0:31.7.0'):
            data_dict['L1 current'] = float(re.findall(r'\(([\d\.]+)\*A\)', line)[0])
        elif line.startswith('1-0:51.7.0'):
            data_dict['L2 current'] = float(re.findall(r'\(([\d\.]+)\*A\)', line)[0])
        elif line.startswith('1-0:71.7.0'):
            data_dict['L3 current'] = float(re.findall(r'\(([\d\.]+)\*A\)', line)[0])
        elif line.startswith('0-1:24.2.1'):
            data_dict['Water consumption'] = float(re.findall(r'\(([\d\.]+)\*m3\)', line)[0])
        elif line.startswith('0-1:24.2.3'):
            data_dict['Gas consumption'] = float(re.findall(r'\(([\d\.]+)\*m3\)', line)[0])    
    return data_dict

while True:
    data = read_p1_data()
    parsed_data = parse_p1_data(data)
    print(json.dumps(parsed_data))
    quit()
