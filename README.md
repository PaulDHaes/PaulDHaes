
# DSMR (Dutch Smart Meter Requirements) electricity dashboard for Raspberry Pi

## Prequsits
- Raspberry pi 3b/ 3b+ /4
- Prefeared 16GB or more sd card for storage
- Raspbian OS 64 bit *if prefeared you can use gui version but is not needed.*
- Special RJ11 to usb cable that has converter build in
  [Buy cable](https://webshop.cedel.nl/Slimme-meter-kabel-P1-naar-USB?gclid=Cj0KCQjwkOqZBhDNARIsAACsbfLt-R-eDRTF_N71Z2UZsGzT_Ve1b7lJw5-FMawz6sVlJQ9zLBQNe-MaAux8EALw_wcB)
- Enable the P1 port on the smart meter this can be done normaly on the distributor website.
  
## What does it do?

Data from Elektricity meter on port P1 this sends data every second -> Rapsberry pi USB with special cable -> Telegraf execute script to recieve data and transforms it to json -> Telegraf sends data to influxdb and put in the right bucket -> Grafana exfiltrate data by the use of a influxdb querry.

## Config_pi.sh
- Print ip and wait 5 sec
- The script enables SSH
- Disables swap memmory -> this is for the degradation prevention of the SD card
- Does a full update
- installs all nessesary programs docker, (python3 pip and libs for the python script) for debug
- checks if wifi and usb is on and give output on what port the USB is on 
- Does some optimazation for docker
- Disables the unnescesery stuff like bluetooth ,vnc and telnet
- Updates the kernel
- Installes openssl and makes a hex32 token for the influxdb bucket
- Changes the UBS permission to let the telegraf container read the USB port
- Does docker-compose up command to pull the container and make a config
- Reboots

## Docker-compose
The docker-compose.yml is here to easly install and configure the right containers.
This includes storage options and the network to let the communicate with each other.
#### influxdb container
Influxdb is a Time Series Data Platform in short time based database. We need this to store the data from telegraf. To manage/debug the influxdb it has a visual webapplication that runs on port 8086. This can be accesed by 
``` web
http://[Raspberrypi_IP]:8086
```
#### grafana container
This is a obsevery platform this means that we can observer all our data easily and clean. To access grafana just go to the raspberry pi following by adding port 3000. The username and passoword are already configured in the docker-compose file but it's recommend to changed it. 
``` web
http://[Raspberrypi_IP]:3000
```
#### telegraf container
This is a server based agent that collects and send the data in this case to influxdb. This like all the other container has no webapplication running instead the port 8125 is used to collect the data. This has also mounts the usb port to that container aswel as the python script needed to fetch the data.
#### portainer container
This is docker container manager this is not nescesry to have this but it easy to mannage the containers and restart them. this has a webapplication on port 9000 and a mangement port on 8000 to conect to other portainers. 
``` web
http://[Raspberrypi_IP]:9000
```
## Elektricity_meter.py
This script reads the data from the P1 port of the smart meter and transform it to readable data that can be parsed. The smart meter spits out obiscodes and these needs to linked to what it actual is after that the script reads the data and transforms it to a json like format.

## Install instructions
### Passwords, usernames and databases are defined in the .env file
### Install Pi_config.sh
This script is here if you want to start form a fresh install of Raspbian OS.
To install launche the terminal if you use the GUI version and do:
``` bash
git clone https://github.com/PaulDHaes/PaulDHaes.git
cd PaulDHaes
chmod +x Pi_config.sh
./Pi_config.sh
```
*If the Pi_config.sh script is not executed please execute the folowing commands*

``` bash
apt-get update
apt-get install docker.io docker-compose openssl
influx_token=$(openssl rand -hex 32)
sed -i "s|supercool_new_token|$influx_token|g" .env
sed -i "s|supercool_new_token|$influx_token|g" telegraf.conf
chmod 777 /dev/ttyUSB0
```

### Edit telegraf.conf file and the Elektricity_meter.py
Edit out Water consumption with Gas if you know you only have Gas
```telegraf.conf
[[inputs.exec]]
  commands = ["python3 /home/Elektricity_meter.py"]
  timeout = "10s"
#  debug=True
  data_format = "json"
  json_string_fields = [
    "All phases consumption",
    "All phases production",
    "Current_rate_(1_is_day_en_2_is_night",
    "L1 consumption",
    "L1 current",
    "L1 production",
    "L1 voltage",
    "L2 consumption",
    "L2 current",
    "L2 production",  "L2 voltage",
    "L3 consumption",
    "L3 current",
    "L3 production",
    "L3 voltage",
    "Rate 1 (day) - total consumption",
    "Rate 1 (day) - total production",
    "Rate 2 (night) - total consumption",
    "Rate 2 (night) - total production",  
    "Water consumption"
    #"Gas consumption"
    ]
```
```Elektricity_meter.py
# Change your serial port here:
serialport = '/dev/ttyUSB0'
```
### Docker compose use
To run the docker container use the following command and wait till done.
``` bash
docker-compose up -d
```

### Grafana setup
Setup grafana so it can querry data from influxdb. This can be done by simply adding a new database and fill in the fields for influxdb2.
Login -> left botom cornor click on the gear icon and select datasource -> add influxdb database -> change querry language to flux -> in http -> url change it to http://172.19.0.3:8086 
for AUTH chose only basic auth -> set Basic Auth Details -> change this to the influxdb username and password form the .env file
for InfluxDB Details -> Organization, token and default bucket are in the .env file -> save and test

#### Import basic dashboard
Go to Left 4 squars and click import dahsboard -> copy and past the json dashboard data in.
If the data is not loaded click the title name of the dashboard panel like Fase1,2 & 3 -> click edit -> click on the eye icon at the botom twice.
Normally this should fix it or you just need to wait 1-5 minutes.

## Home Assistant
Copy the provide homeassistant file in your home assistant configuration.conf file, add your values and restart home assistant

## Make it your own
- Edit the telegraf config so you can have more data from maybe other devices of you network.
- Make your own custom grafana dashboards.
- Edit the .env file to set better passwords.

## TODO

- [x] Update docker container
- [ ] Add more functionality
- [x] Add the USB baudrate in script
- [x] Make own docker container to easly download
- [ ] Secure the communication by using SSL
- [ ] Auto edit the grafana dashboard to set all the value
- [x] Make influxdb api key and put it in telegraf.conf
- [x] Edit docker network
- [ ] Add Integration with Home Assistant
