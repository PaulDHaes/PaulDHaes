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
            data_dict['Rate_1_(day)-total_consumption'] = float(re.findall(r'\(([\d\.]+)\*kWh\)', line)[0])
        elif line.startswith('1-0:1.8.2'):
            data_dict['Rate_2_(night)-total_consumption'] = float(re.findall(r'\(([\d\.]+)\*kWh\)', line)[0])
        elif line.startswith('1-0:2.8.1'):
            data_dict['Rate_1_(day)-total_production'] = float(re.findall(r'\(([\d\.]+)\*kWh\)', line)[0])
        elif line.startswith('1-0:2.8.2'):
            data_dict['Rate_2_(night)-total_production'] = float(re.findall(r'\(([\d\.]+)\*kWh\)', line)[0])
        elif line.startswith('0-0:96.14.0'): 
            data_dict['Current_rate_(1_is_day_en_2_is_night)'] = float(re.findall(r'\((\d+)\)', line)[0])
        elif line.startswith('1-0:1.7.0'):
            data_dict['All_phases_consumption'] = float(re.findall(r'\(([\d\.]+)\*kW\)', line)[0])
        elif line.startswith('1-0:21.7.0'):
            data_dict['L1_consumption'] = float(re.findall(r'\(([\d\.]+)\*kW\)', line)[0])
        elif line.startswith('1-0:41.7.0'):
            data_dict['L2_consumption'] = float(re.findall(r'\(([\d\.]+)\*kW\)', line)[0])
        elif line.startswith('1-0:61.7.0'):
            data_dict['L3_consumption'] = float(re.findall(r'\(([\d\.]+)\*kW\)', line)[0])
        elif line.startswith('0-0:17.0.0'):
            data_dict['Switch_electricity'] = float(re.findall(r'\(([\d\.]+)\*kW\)', line)[0])
        elif line.startswith('0-1:24.2.1'):
            data_dict['water_consumption'] = float(re.findall(r'\(([\d\.]+)\*m3\)', line)[0])
        elif line.startswith('0-1:24.2.3'):
            data_dict['gas_consumption'] = float(re.findall(r'\(([\d\.]+)\*m3\)', line)[0])    
    return data_dict

while True:
    data = read_p1_data()
    parsed_data = parse_p1_data(data)
    print(json.dumps(parsed_data))
    quit()
